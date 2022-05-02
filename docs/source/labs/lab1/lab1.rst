*************
Laboratorio 1
*************

Planeamiento
============
..
    * De manera breve, explique cómo se pueden planear los `releases` de funcionalidad del proyecto para habilitar lo más rápido posible el desarrollo en el equipo del App (externo a `eieLabs`).

    * Utilice conceptos de planeamiento a largo plazo con metodologías Agile (Quiz 2).

* Para tomar en cuenta el equipo de desarrollo de la aplicación móvil, es importante definir milestones claros en relación a ciertos elementos del programa. Debemos preguntarnos: "Que componentes utilizaría la aplicación para comunicarse con el proyecto?". La respuesta a esto es evidentemente el API. Entonces teniendo esto en cuenta se vuelve crucial implementar la capa del API lo más pronto posible. Tomando en cuenta la metodología Agile, debemos de optimizar los elementos mas importantes como parte de los milestones para sacar un producto a mercado en un tiempo fijo. Probablemente el API no es el primer componente que tengamos listo, ya que hay que tomar en cuenta que elementos del eieManager para poder entender como se realizara la interconexión. Aun asi, si se pueden claramente definir los requerimientos funcionales del eieManager seria posible tener un release del API listo rápidamente para que el equipo aparte pueda trabajar en paralelo.

Requerimientos
==============
* Funcionales:
    * REQ-F01: El eieManager debe poder comunicarse con n cantidad de dispositivos del tipo eieDevice simultáneamente.
    * REQ-F02: El eieManager debe poder enviar comandos a los dispositivos eieDevice a dispositivos específicos y también en `broadcast`.
    * REQ-F03: El eieManager debe exponer una API para que clientes externos puedan enviar comandos y recibir respuestas de los dispositivos eieDevice.
    * REQ-F04: Los eieDevice deben de ser capaces de recibir comandos predefinidos y poder dar una respuesta valida a cualquier comando.
    * REQ-F05: El eieManager debe de poder reiniciar ante un fallo que genere el cierre del proceso.
* No Funcionales:
    * REQ-NF01: El API expuesto debe ser fácilmente ingerible tal que un equipo de desarrollo pueda preparar una aplicación móvil en paralelo.
    * REQ-NF02: El eieManager debe soportar una amplia variedad de dispositivos, y que ademas estos se puedan agregar fácilmente sin cambios en el código.
    * REQ-NF03: El eieManager debe ser capaz de enviar una variedad de comandos en diferentes protocolos de comunicación, y que estos ademas se puedan expander fácilmente sin cambios en el código. Inicialmente se utilizara el protocolo RPC.
    * REQ-NF04: Se debe de priorizar el desarrollo del API para que el equipo externo que desarrolla la aplicación móvil pueda trabajar en paralelo.
    * REQ-NF05: Los eieDevice deben de ser capaces de funcionar con diferentes protocolos de comunicación sin afectar el funcionamiento del API.

Attribute-driven Design
=======================
El primer paso es definir si los requerimientos obtenidos son suficientes y si están priorizados. En este caso podemos decir que si, y si. Pasando al segundo punto, es necesario separar el sistema en elementos. Escogeremos el eieManager y eieDevice como los dos elementos a descomponer para seguir el proceso.

eieManager
----------
Primero procedemos a tomar los requerimientos y clasificarlos conforme a su importancia del punto de vista de los `stakeholders` y a un nivel de arquitectura:

* REQ-F01 (H,M)
* REQ-F02 (H,M)
* REQ-F03 (H,H)
* REQ-F05 (M,L)
* REQ-NF01 (H,H)
* REQ-NF02 (H,H)
* REQ-NF03 (H,H)
* REQ-NF04 (H,H)

De estos elementos elegimos los siguientes como los `drivers` de la arquitectura a diseñar: REQ-F01, REQ-F02, REQ-F03, y REQ-NF03.
El siguiente paso del proceso es escoger un concepto de diseño para cumplir los elementos elegidos. Tomando en cuenta estos requerimientos podemos decir que la mayoría de ellos se centran en la modificabilidad y flexibilidad en su uso. Esto implica que del punto de vista de los `stakeholders` lo principal es asegurarse que el sistema se pueda expander fácilmente. Tomando esto en cuenta se proponen dos patrones de diseño que pueden cumplir esta necesidad. Primero se tiene el adaptador, en este patron el eieManager funciona como un intermediario entre el API y los eieDevice. Esto con el fin de lidiar con los diferentes protocolos de ambos lados y garantizar la comunicación. El segundo patron que se propone es el mediador. Bajo este patron el eieManager funciona como el mediador entre el API y los dispositivos. SU funcionamiento es similar al adaptador excepto que provee mas libertad en los diferentes dispositivos que lo pueden llamar, y reduce la necesidad de modificar el eieManager para poder lidiar con diferentes tipos de dispositivo, ya que el adaptador adapta entre dos protocolos, no varios necesariamente.

+------------------------+-------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+------------------------------------------------------------------------+
|                        | Patron Adaptador                                                                                      | Patron Adaptador                                                                                                               | Patron Mediador                                                                                  | Patron Mediador                                                        |
+========================+=======================================================================================================+================================================================================================================================+==================================================================================================+========================================================================+
| Driver Arquitectónico  | Pros                                                                                                  | Cons                                                                                                                           | Pros                                                                                             | Cons                                                                   |
+------------------------+-------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+------------------------------------------------------------------------+
| REQ-F01                | Al ser un adaptador se puede fácilmente comunicar con los eieDevice fácilmente.                       | Podría requerir la instanciacion de varios eieManagers para lidiar con los diferentes dispositivos                             | Puede lidiar con los distintos dispositivos simultáneamente.                                     | Requiere mas complejidad para poder lidiar con todos simultáneamente.  |
+------------------------+-------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+------------------------------------------------------------------------+
| REQ-F02                | El adaptador puede enviar comandos en el protocolo esperado.                                          | Podría complicarse el broadcast dependiendo de como este diseñado el adaptador y si hay multiples protocolos de comunicación.  | Puede fácilmente enviar comandos en broadcast a multiples dispositivos.                          | N/A                                                                    |
+------------------------+-------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+------------------------------------------------------------------------+
| REQ-F03                | Se puede hacer la conexión entre el API y las respuestas y protocolos de transmisión de los módulos.  | Es posible que se requieran multiples adaptadores dependiendo de la implementación                                             | Puede hacer la traducción entre el API y los dispositivos.                                       | N/A                                                                    |
+------------------------+-------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+------------------------------------------------------------------------+
| REQ-NF03               | Puede hacer la comunicación en varios protocolos.                                                     | Podría requerir multiples instancias para lidiar con los diferentes parámetros.                                                | Se puede diseñar para lidiar con multiples protocolos que se pueden expander sin mayor problema. | N/A                                                                    |
+------------------------+-------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------+------------------------------------------------------------------------+

Teniendo estos detalles en cuenta, es posible determinar que una implementación usando el patron de diseño de Mediador seria lo ideal para este componente. 
Para el siguiente paso se deben de instanciar algunos componentes para la funcionalidad del elemento. Se eligen los siguientes componentes:

* ConfigHandler: Esto se encarga de agregar y remover dispositivos soportados y proveer la information de los mismos al eieManager.
* APIServer: Este componente se encarga de recibir las solicitudes del cliente y enviarle las respuestas de los dispositivos.
* CommandRegistry: Esto se encarga de llevar un registro de los comandos soportados y como lidiar con ellos.
* DeviceManager: Esto maneja los dispositivos asociados y lleva control de su ciclo de vida.
* GroupManager: Esto se encarga de manejar las listas de distribución para grupos `broadcast`.
* CommandInvoker: Esto controla la ejecución de los comandos enviados por el cliente.
* TransportClient: Este abstrae el protocolo de comunicación para los dispositivos de tal manera que se pueda usar cualquier protocolo mientras pase por un proceso estándar.
* DatabaseHandler: Este elemento funcionara para manejar una base de datos que contiene los distintos elementos a interactuar con.

Pasando al siguiente paso, podemos notar que los requerimientos funcionales se alinean con nuestra solución propuesta bajo el patron de diseño elegido. Finalmente procedemos al ultimo paso que es repetir este proceso para eieDevice.

eieDevice
---------
Primero procedemos a tomar los requerimientos y clasificarlos conforme a su importancia del punto de vista de los `stakeholders` y a un nivel de arquitectura:

* REQ-F04 (H,M)
* REQ-NF05 (H,H)

Escogemos estos dos elementos como `drivers` para la arquitectura a diseñar. Ahora procedemos a elegir patrones de diseño para satisfacer estos requerimientos. En particular se eligen los patrones de fachada y fabrica. Este primero funciona para abstraer la interacción con el subsistema del dispositivo y enviar datos con un formato estándar. Este segundo funciona para crear una amplia variedad de objetos usando una sola clase fabricante. Se evalúan sus pros y contras a continuación:

+------------------------+-----------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------------------------------------------+-----------------------------------------------------------------------------------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------------------+
|                        | Patron Fachada                                                                                                                    | Patron Fachada                                                                                       | Patron Fabrica                                                                                                        | Patron Fabrica                                                                                                                          |
+========================+===================================================================================================================================+======================================================================================================+=======================================================================================================================+=========================================================================================================================================+
| Driver Arquitectonico  | Pros                                                                                                                              | Cons                                                                                                 | Pros                                                                                                                  | Cons                                                                                                                                    |
+------------------------+-----------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------------------------------------------+-----------------------------------------------------------------------------------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------------------+
| REQ-F04                | Provee una capa de abstraccion para que el eieDevice reciba procese comandos estandar.                                            | Puede verse limitado en el caso de que no se defina el comportamiento ante comandos inesperados.     | La fabrica puede generar una variedad de eieDevices tal que todos puedan responder a comandos estandar particulares.  | La interaccion con ell device no tiene una capa de abstraccion necesariamente, lo cual puede llevar a dificultades.                     |
+------------------------+-----------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------------------------------------------+-----------------------------------------------------------------------------------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------------------+
| REQ-NF05               | La fachada permite una abstraccion ocmpleta del funcionamiento interno del dispositivo facilitando el procesamiento de comandos.  | Esta implementacion puede ser laboriosa para que soporte una variedad de protoclos de comunicacion.  | La fabrica permite generar eieDevices tal que se trabaje con diferentes metodos de transmision de datos.              | Se debera sobrecargar los metodoos de recibo y envio de datos de las subclases de la fabrica para lidiar con comunicacion alternativa.  |
+------------------------+-----------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------------------------------------------------------+-----------------------------------------------------------------------------------------------------------------------+-----------------------------------------------------------------------------------------------------------------------------------------+

Tomando en cuenta estos factores, se elije utilizar el patron de Fachada, esto con el fin de abstraer las comunicaciones lo mas posible a nivel de dispositivo. Esto permite gran flexibilidad ya que al agregar un nuevo dispositivo de cualquier tipo se puede facilmente agregar una capa de traduccion al protocolo de comunicacion estandar, permitiendo agregar dispositivos sin modificar el codigo de eieManager.
Ahora debemos discutir las instancias de elementos del eieDevice. Se elijen las siguientes:

* TransportServer: Esta es la capa de abstraccion que se encarga de comunicarse con el TransportClient de eieManager. Esto es lo que vendria siendo la Fachada elegida como patron de diseño.
* CommandManager: Esto lleva el registro de comandos soportados por el dispositivo, llama a su ejecuccion, y lidia con comandos desconocidos. Lo ideal en este elemento seria devolver una respuesta estandarizada que corresponda a `comannd not supported` tal que la capa de abstraccion pueda comunicar esta respuesta.
* Command: Este elemento implementa la funcionalidad especifica del comando. Esto puede ser algo desde leer una señal de temperatura de un termistor, hasta accionar un motor que inicie la mezcla en un proceso quimico. Este command debe retornar algun mensaje indicando el estado o otro dato deseado, ademas de realizar la accion.

Teneiendo estas consideraciones, pasamos al siguiente paso. Podemos determinar que estas funciones directamente soportan los requerimientos planteados en el proceso para eieDevice. Con esto damos por concluido el proceso de ADD.

Patrones de Diseño
==================
..
  * Explique cómo se puede aplicar el patrón de diseño `Proxy <https://en.wikipedia.org/wiki/Proxy_pattern>`_ para abstraer la interacción y comunicación con los dispositivos desde ``eieManager``.

    * Dentro de los componentes sugeridos en la introducción, a cuáles se les puede relacionar con este patrón?

  * Explique cómo se puede aplicar el patrón de diseño `Command <https://en.wikipedia.org/wiki/Command_pattern>`_ para desacoplar los procesos de:

    * Encapsular la información requerida para ejecutar comandos en dispositivos específicos.
    * Ejecutar los comandos y esperar la respuesta correspondiente.

Proxy
-----


Command
-------
