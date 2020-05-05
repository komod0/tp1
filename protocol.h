#ifndef PROTOCOL_H
#define PROTOCOL_H

#define NUM_OF_ARGS 4

#include <stdbool.h>
#include <stdint.h>

#include "dyn_buffer.h"
#include "dynamic_vector.h"

char* protocol_encode(char* msg, uint32_t msg_id, size_t* len);

char* protocol_decode(char* msg);

bool protocol_init(protocol_t* protocol, char* msg);


#endif