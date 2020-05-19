#include <arpa/inet.h>
#include <byteswap.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "common_str_vector.h"
#include "common_protocol.h"
#include "common_utils.h"

#define NUM_OF_ARGS 4
#define DBUFF_SIZE 64
#define PARAM_DESCR_SIZE 8
#define SIGNATURE_DESC_LEN 5
#define ARR_LENGTH_OFFSET 12
#define BODY_LENGTH_OFFSET 4
#define ID_OFFSET 8
#define PARAM_ARR_OFFSET 16
#define PARAM_LEN_OFFSET 4
#define PARAM_OFFSET 8

#define DESTINY_TYPE 6
#define PATH_TYPE 1
#define INTERFACE_TYPE 2
#define METHOD_TYPE 3
#define SIGNATURE_TYPE 8

#define DESTINY_DATA_TYPE "s"
#define PATH_DATA_TYPE "o"
#define INTERFACE_DATA_TYPE "s"
#define METHOD_DATA_TYPE "s"
#define SIGNATURE_DATA_TYPE "g"

enum pos_args{DESTINY_INDEX, PATH_INDEX, INTERFACE_INDEX, METHOD_INDEX};

void _protocol_add_chr(vector_t* tmp_msg, char c) {
  str_vector_append(tmp_msg, strndup(&c, sizeof(char)), sizeof(char));
}

void _protocol_add_str(vector_t* tmp_msg, char* s, size_t padding) {
  str_vector_append(tmp_msg, strndup(s, strlen(s)), strlen(s));
  _protocol_add_chr(tmp_msg, '\0');
  for ( int i = 0; i < padding; i++ ) {
    _protocol_add_chr(tmp_msg, '\0');
  }
}

void _protocol_add_int32(vector_t* tmp_msg, uint32_t i) {
  uint32_t aux = bswap_32(htonl(i));
  str_vector_append(tmp_msg, strndup((char*)&aux,
                    sizeof(uint32_t)), sizeof(uint32_t));
}

void _protocol_add_parameter(
    vector_t* tmp_msg, char type, char* data_type, char* param) {

  _protocol_add_chr(tmp_msg, type);
  _protocol_add_chr(tmp_msg, 0x01);
  _protocol_add_str(tmp_msg, data_type, 0);

  if (type == SIGNATURE_TYPE) {
    _protocol_add_chr(tmp_msg, (int)strlen(param));
    _protocol_add_str(tmp_msg,param,
                get_8_aligned_padding(strlen(param) + 1 + SIGNATURE_DESC_LEN));
  } else {
    _protocol_add_int32(tmp_msg, strlen(param));
    _protocol_add_str(tmp_msg, param, get_8_aligned_padding(strlen(param) + 1));
  }
}

uint32_t _protocol_body_length(vector_t* arguments) {
  uint32_t length = 0;
  for ( int i = NUM_OF_ARGS; i < arguments->n_elements; i++ ) {
    length += sizeof(uint32_t) + strlen(str_vector_get(arguments, i)) + 1;
  }
  return length;
}

uint32_t _protocol_param_arr_length(vector_t* params) {
  uint32_t length = 0;
  for (int i = 0; i < NUM_OF_ARGS; i++) {
    length += PARAM_DESCR_SIZE + get_8_aligned_len(str_vector_get(params, i));
  }
  bool there_is_signature = (str_vector_len(params) > 4);
  if ( there_is_signature ) {
    length += (SIGNATURE_DESC_LEN + str_vector_len(params) - NUM_OF_ARGS + 1);
  } else {
    length -= get_8_aligned_padding(
      strlen((char*)str_vector_get(params, NUM_OF_ARGS - 1)));
  }
  // No se cuenta el padding del ultimo elemento
  return length;
}

void _protocol_add_params(vector_t* tmp_msg, vector_t* args) {
  _protocol_add_parameter(tmp_msg, DESTINY_TYPE, 
    DESTINY_DATA_TYPE, str_vector_get(args, DESTINY_INDEX));

  _protocol_add_parameter(tmp_msg, PATH_TYPE, 
    PATH_DATA_TYPE, str_vector_get(args, PATH_INDEX));

  _protocol_add_parameter(tmp_msg, INTERFACE_TYPE, 
    INTERFACE_DATA_TYPE, str_vector_get(args, INTERFACE_INDEX));

  _protocol_add_parameter(tmp_msg, METHOD_TYPE, 
    METHOD_DATA_TYPE, str_vector_get(args, METHOD_INDEX));

  bool there_is_signature = (str_vector_len(args) > 4);
  if (there_is_signature){
    char* signature_str = concat_n_times("s", str_vector_len(args) - 4);
    _protocol_add_parameter(tmp_msg, SIGNATURE_TYPE, 
      SIGNATURE_DATA_TYPE, signature_str);
    free(signature_str);
  }
}

void _protocol_add_body(vector_t* tmp_msg, vector_t* args) {
  for ( int i = NUM_OF_ARGS; i < str_vector_len(args); i++ ) {
    _protocol_add_int32(tmp_msg, strlen(str_vector_get(args, i)));
    _protocol_add_str(tmp_msg, str_vector_get(args, i), 0);
  }
}

void _protocol_encode_header(vector_t* tmp_msg, vector_t* args, uint32_t id) {
  _protocol_add_chr(tmp_msg, 'l');
  _protocol_add_chr(tmp_msg, 0x01);
  _protocol_add_chr(tmp_msg, 0x0);
  _protocol_add_chr(tmp_msg, 0x01);
  _protocol_add_int32(tmp_msg, _protocol_body_length(args));
  _protocol_add_int32(tmp_msg, id);
  _protocol_add_int32(tmp_msg, _protocol_param_arr_length(args));
  _protocol_add_params(tmp_msg, args);
}

void _protocol_print_parameter_type(char c) {
  switch (c) {
  case 1:
    printf("* Ruta: ");
    break;
  case 2:
    printf("* Interfaz: ");
    break;
  case 3:
    printf("* Metodo: ");
    break;
  case 6:
    printf("* Destino: ");
    break;
  case 9:
    printf("* Parametros:\n");
  default:
    break;
  }
}

char* protocol_encode(protocol_t* protocol, 
    char* msg, uint32_t msg_id, size_t* len) {
  vector_t arguments;
  str_vector_init(&arguments);
  vectorize_msg(&arguments, msg);
  _protocol_encode_header(&protocol->vector , &arguments, msg_id);
  if (str_vector_len(&arguments) > 4) { // Si hay body
    _protocol_add_body(&protocol->vector, &arguments);
  }
  free_vector_elems(&arguments);
  str_vector_destroy(&arguments);
  *len = str_vec_total_size(&protocol->vector);
  char* formatted_str = str_vector_join(&protocol->vector);
  free_vector_elems(&protocol->vector);
  str_vector_clear(&protocol->vector);
  return formatted_str;
}

void protocol_destroy(protocol_t* protocol) {
    str_vector_destroy(&protocol->vector);
}

void protocol_init(protocol_t* protocol) {
  str_vector_init(&protocol->vector);
}

uint32_t _get_uint_at(const char* msg, size_t offset) {
  uint32_t* aux = (uint32_t*)(msg + offset);
  // de little a big y luego al host
  return ntohl(bswap_32(*aux));
}

uint32_t protocol_decode_arr_len(const char* msg) {
  return _get_uint_at(msg, ARR_LENGTH_OFFSET);
}

uint32_t protocol_decode_body_len(const char* msg) {
  return _get_uint_at(msg, BODY_LENGTH_OFFSET);
}

uint32_t protocol_decode_id(const char* msg) {
  return _get_uint_at(msg, ID_OFFSET);
}

uint32_t protocol_decode_param_len(const char* msg) {
  return _get_uint_at(msg, PARAM_LEN_OFFSET);
}

void protocol_decode_and_print(protocol_t* protocol) {
  size_t msg_len = str_vec_total_size(&protocol->vector);
  char* msg = str_vector_join(&protocol->vector);

  uint32_t arr_len = protocol_decode_arr_len(msg);
  uint32_t body_len =  protocol_decode_body_len(msg);
  uint32_t param_len;
  uint32_t id = protocol_decode_id(msg);
  bool there_is_body = (body_len != 0);
  printf("* Id: 0x%08x\n", id);
  for ( int i = PARAM_ARR_OFFSET; i < arr_len; ) {
    char param_t = msg[i];
    if ( param_t == 9 ) break;
    _protocol_print_parameter_type(param_t);
    param_len = protocol_decode_param_len(msg + i);
    printf("%s\n", msg + i + PARAM_OFFSET);
    i += get_8_aligned_size(param_len + 1) + PARAM_DESCR_SIZE;
  }
  int body_offset = msg_len - body_len;
  if ( there_is_body ) {
    for ( int j = body_offset; j < msg_len; ) {
      param_len = _get_uint_at(msg, j);
      printf("    * %s\n", msg + j + 4);
      j += 4 + param_len + 1;
    }
  }
  printf("\n");
  free(msg);
  free_vector_elems(&protocol->vector);
  str_vector_clear(&protocol->vector);
}



