#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>

#include "common_str_vector.h"

typedef struct protocol {
  vector_t vector;
}protocol_t;

// Inicializa los valores de protocol
void protocol_init(protocol_t* protocol);

// Recibe un mensaje y su id, y lo encodea al formato deseado, 
// devuelve su longitud en len
char* protocol_encode(protocol_t* protocol,
   char* msg, uint32_t msg_id, size_t* len);

// Recibe un protocolo y decodifica el mensaje que se encuentra en el
// vector asociado, ademas imprime por pantalla los valores obtenidos
void protocol_decode_and_print(protocol_t* protocol);

// Recibe un mensaje con el formato debido y devuelve la longitud
// asociada con el arreglo de parametros
uint32_t protocol_decode_arr_len(const char* msg);

// Recibe un mensaje con el formato debido y devuelve la longitud
// asociada con el cuerpo del mensaje
uint32_t protocol_decode_body_len(const char* msg);

// Recibe un mensaje con el formato debido y devuelve el id asociado
uint32_t protocol_decode_id(const char* msg);

// Destruye protocol y las estructuras subyacientes, cualquier valor 
// almacenado en ellos debe ser liberado por el usuario si hace falta
void protocol_destroy(protocol_t* protocol);

#endif // PROTOCOL_H
