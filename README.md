# Trabajo Practico N°1

Trabajo practico N°1 de Taller de Programación 1(75.42)



*Alumno:* Julián Crespo

*Padron:* 100490

*Repositiorio:* https://github.com/komod0/tp1



---



### Diseño

El programa cuenta con varias entidades y estructuras auxiliares que sirvieron para poder separar responsabilidades, modularizar y obtener una interfaz amigable.



![](img/diag_clases.png)

En principio el cliente y el servidor no tienen contacto directo entre ellos, toda comunicación pasa por mediante de el protocolo para tareas de codificación/decodificación de los mensajes y por los sockets para realizar los correspondientes send y recv.



Como se puede observar tanto el cliente como el servidor cuentan con sockets, en el caso de server cuenta con dos sockets asociados, uno que vendría a cumplir el rol del "aceptador" y el otro que seria el socket mismo por donde ocurre la comunicación.

El cliente es quien se encarga de recibir la entrada por stdint o por mediante un archivo, una vez seleccionado el origen de la entrada se procede a leer y parsear cada una de las llamadas una linea a la vez, para esto se ayuda de un buffer fijo de 32 bytes, por el cual se van leyendo de a una las lineas, esta parte del trabajo fue la que resulto de mayor dificultad ya que para poder leer el archivo de a una linea con el buffer fijo se debió de usar gran cantidad de variables indicadoras y de punteros dado que había una gran cantidad de casos bordes.



Se adjunta un diagrama de secuencia para esquematizar como es que se procesa envia y se recibe la respuesta a una llamada de a una linea a la vez.



![](img/diagrama_secuencia.png)





### Faltantes:


Debido a un mal aprovechamiento del tiempo y una subestimación del tiempo requerido para realizar el trabajo no se pudo completar el mismo, se logro terminar la parte del cliente junto con los sockets y la interfaz del servidor, pero faltaron la parte de procesamiento y respuesta del server a los mensajes, asi mismo falto documentacion y corregir algunas perdidas de memoria en el lado del cliente.
