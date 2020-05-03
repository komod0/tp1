#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "buffer.h"
#include "dyn_buffer.h"
#include "reader.h"


#define BUFFER_SIZE 32

int client_run(FILE* input) {
  reader_t reader;
  if (!reader_init(&reader, input)) return false;


    bool is_there_eol = false;


    do {
      reader_readline(&reader);
      first_ocurrence = strchr(&buffer, '\n');
      is_there_eol = (first_ocurrence != NULL);

      if (is_there_eol) {
        read_until = first_ocurrence - &buffer;
      }
      memcpy(buff_dyn, &buffer, read_until);

    } while (!reader_is_at_eof(&reader));
    
    

}