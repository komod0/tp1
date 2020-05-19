#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>

#include "common_str_vector.h"

typedef struct protocol {
  vector_t vector;
}protocol_t;

void protocol_init(protocol_t* protocol);

char* protocol_encode(protocol_t* protocol,
   char* msg, uint32_t msg_id, size_t* len);

void protocol_decode_and_print(protocol_t* protocol);

uint32_t protocol_decode_arr_len(const char* msg);

uint32_t protocol_decode_body_len(const char* msg);

uint32_t protocol_decode_id(const char* msg);

void protocol_destroy(protocol_t* protocol);

#endif // PROTOCOL_H
