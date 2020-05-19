#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef struct socket { 
  int fd;
} socket_t;

// Intenta contectar al socket al host y el servicio especificado
// devuelve 0 en exito, 1 en otro caso
int socket_connect(socket_t* self, const char* host, const char* service);

// Inicializa el file descriptor del socket para que valga -1
void socket_init(socket_t* self);

// Enlaza al socket especificado con el servicio serv
// devuelve 0 en exito, 1 en otro caso
int socket_bind(socket_t* self, const char* serv);

// Hace al socket pasivo para escuchar conexiones
// devuelve 0 en exito, 1 en otro caso
int socket_listen(socket_t* self);

// Acepta una conexion mediante el socket self y guarda el fd en peer
// en caso de exito devuelve 0, 1 en otro caso
int socket_accept(socket_t* self, socket_t* peer);

// Manda length bytes de buffer a traves de self
// En caso de exito devuelve la cantidad de bytes enviados, si
// cerro la conexion devuelve 0, en caso de error -1
int socket_send(socket_t* self, const char* buffer, size_t length);

// Recibe length bytes y los almacena en buffer
// En caso de exito devuelve la cantidad de bytes recibidos, 0 si se
// cerro la conexion y -1 en caso de error
int socket_recv(socket_t* self, char* buffer, size_t length);

// Cierra la conexion y el file descriptor asociado al socket
int socket_destroy(socket_t* socket);

#endif // SOCKET_H
