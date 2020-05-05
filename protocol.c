#include <arpa/inet.h>
#include <byteswap.h>
#include <stdint.h>
#include <string.h>

#include "dynamic_vector.h"
#include "protocol.h"
#include "utils.h"

#define NUM_OF_ARGS 4
#define DBUFF_SIZE 32
#define PARAM_DESCR_SIZE 8

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

void _protocol_add_chr(d_buff_t* tmp_msg, char c) {
  d_buff_append(tmp_msg, &c, sizeof(char));
}

void _protocol_add_str(d_buff_t* tmp_msg, char* s, size_t padding) {
  d_buff_append(tmp_msg, s, strlen(s));
  _protocol_add_chr(tmp_msg, '\0');
  for(int i = 0; i < padding; i++) {
    _protocol_add_chr(tmp_msg, '\0');
  }
}

void _protocol_add_int32(d_buff_t* tmp_msg, uint32_t i) {
  uint32_t aux = bswap_32(htonl(i));
  d_buff_append(tmp_msg, (char*)&aux, sizeof(uint32_t));
}

void _protocol_add_parameter(
    d_buff_t* tmp_msg, char type, char* data_type, char* param) {

  _protocol_add_chr(tmp_msg, type);
  _protocol_add_chr(tmp_msg, 0x01);
  _protocol_add_str(tmp_msg, data_type, 0);

  if (type == SIGNATURE_TYPE) {
    _protocol_add_chr(tmp_msg, (int)strlen(param));
  } else {
    _protocol_add_int32(tmp_msg, strlen(param));
  }

  _protocol_add_str(tmp_msg, param, get_8_aligned_padding(strlen(param) + 1));
}

uint32_t _protocol_body_length(vector_t* arguments) {
  uint32_t length = 0;
  for(int i = NUM_OF_ARGS; i < arguments->n_elements; i++) {
    length += sizeof(uint32_t) + strlen((char*)vector_get(arguments, i)) + 1;
  }
  return length;
}

uint32_t _protocol_param_arr_length(vector_t* params) {
  uint32_t length = 0;
  for (int i = 0; i < NUM_OF_ARGS; i++) {
    length += PARAM_DESCR_SIZE + get_8_aligned_size(vector_get(params, i));
  }
  return length;
}

void _protocol_add_params(d_buff_t* tmp_msg, vector_t* args) {
  _protocol_add_parameter(tmp_msg, DESTINY_TYPE, 
    DESTINY_DATA_TYPE, vector_get(args, DESTINY_INDEX));

  _protocol_add_parameter(tmp_msg, PATH_TYPE, 
    PATH_DATA_TYPE, vector_get(args, PATH_INDEX));

  _protocol_add_parameter(tmp_msg, INTERFACE_TYPE, 
    INTERFACE_DATA_TYPE, vector_get(args, INTERFACE_INDEX));

  _protocol_add_parameter(tmp_msg, METHOD_TYPE, 
    METHOD_DATA_TYPE, vector_get(args, METHOD_INDEX));

  bool there_is_signature = (vector_length(args) > 4);
  if (there_is_signature){
    char* signature_str = concat_n_times("s", vector_length(args) - 4);
    _protocol_add_parameter(tmp_msg, SIGNATURE_TYPE, 
      SIGNATURE_DATA_TYPE, signature_str);
    free(signature_str);
  }
}

void _protocol_add_body(d_buff_t* tmp_msg, vector_t* args) {
  for(int i = NUM_OF_ARGS; i < vector_length(args); i++) {
    _protocol_add_int32(tmp_msg, strlen((char*)vector_get(args, i)));
    _protocol_add_str(tmp_msg, (char*)vector_get(args, i), 0);
  }
}

void _protocol_encode_header(d_buff_t* tmp_msg, vector_t* args, uint32_t id) {
  _protocol_add_chr(tmp_msg, 'l');
  _protocol_add_chr(tmp_msg, 0x01);
  _protocol_add_chr(tmp_msg, 0x0);
  _protocol_add_chr(tmp_msg, 0x01);
  _protocol_add_int32(tmp_msg, _protocol_body_length(args));
  _protocol_add_int32(tmp_msg, id);
  _protocol_add_int32(tmp_msg, _protocol_param_arr_length(args));
  _protocol_add_params(tmp_msg, args);
}

char* protocol_encode(char* msg, uint32_t msg_id) {
  vector_t arguments;
  vector_init(&arguments);
  vectorize_msg(&arguments, msg);
  d_buff_t acum_msg;
  d_buff_init(&acum_msg, DBUFF_SIZE);
  _protocol_encode_header(&acum_msg, &arguments, msg_id);
  if (vector_length(&arguments) > 4) { // Si hay body
    _protocol_add_body(&acum_msg, &arguments);
  }
  vector_destroy(&arguments);
  d_buff_destroy(&acum_msg);
  return d_buff_generate_str(&acum_msg);
}