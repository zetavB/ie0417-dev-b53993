# ie0417-dev
Ambiente base para proyectos de IE0417 - Diseño de Software para Ingeniería
# ie0417-dev-lab3
## Integrantes:
* Dualock Fonseca Monge
* Jordi Louzao Von Breyman
## Pasos de construcción
Desde el contenedor de desarrollo instale las bibliotecas `libzmq` y `czmq` utilizando CMake:

```console
$ git clone https://github.com/zeromq/libzmq.git
$ cd libzmq
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
$ sudo ldconfig
```

```console
$ git clone https://github.com/zeromq/czmq.git
$ cd czmq
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
$ sudo ldconfig
```
Instale la dependencia de `cJSON`, ejecutando las siguientes instrucciones:

```console
$ git clone https://github.com/DaveGamble/cJSON.git
$ cd cJSON
$ mkdir build
$ cd build
$ cmake ..
$ make install
```

Desde el contenedor de desarrollo en el directorio base del ejemplo, ejecute los siguiente comandos:

```console
$ mkdir build
$ cd build
$ cmake ..
$ make

Para compilar y correr el codigo vaya al directorio base ejecute los siguiente comandos:

```console
$ mkdir build
$ cd build
$ cmake ..
$ make
$./eie_device_server
```
En otra terminal vaya a la carpeta src/python y ejecute los siguientes commandos
```console
$ python3 -m venv .venv
$ source .venv/bin/activate
$ pip3 install pyzmq
$ python3 zmq_client_req.py
```
