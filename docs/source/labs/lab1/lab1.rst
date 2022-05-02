*************
Laboratorio 1
*************

Planeamiento
------------
..
    * De manera breve, explique cómo se pueden planear los `releases` de funcionalidad del proyecto para habilitar lo más rápido posible el desarrollo en el equipo del App (externo a `eieLabs`).

    * Utilice conceptos de planeamiento a largo plazo con metodologías Agile (Quiz 2).

* Para tomar en cuenta el equipo de desarrollo de la aplicación móvil, es importante definir milestones claros en relación a ciertos elementos del programa. Debemos preguntarnos: "Que componentes utilizaría la aplicación para comunicarse con el proyecto?". La respuesta a esto es evidentemente el API. Entonces teniendo esto en cuenta se vuelve crucial implementar la capa del API lo más pronto posible. Tomando en cuenta la metodología Agile, debemos de optimizar los elementos mas importantes como parte de los milestones para sacar un producto a mercado en un tiempo fijo. Probablemente el API no es el primer componente que tengamos listo, ya que hay que tomar en cuenta que elementos del eieManager para poder entender como se realizara la interconexión. Aun asi, si se pueden claramente definir los requerimientos funcionales del eieManager seria posible tener un release del API listo rápidamente para que el equipo aparte pueda trabajar en paralelo.

Requerimientos
--------------
* Funcionales:
    * REQ-F01: El eieManager debe poder comunicarse con n cantidad de dispositivos del tipo eieDevice simultáneamente.
    * REQ-F02: El eieManager debe poder enviar comandos a los dispositivos eieDevice a dispositivos específicos y también en `broadcast` usando RPC.
    * REQ-F03: El eieManager debe exponer una API para que clientes externos puedan enviar comandos y recibir respuestas de los dispositivos eieDevice.
    * REQ-F04: Los eieDevice deben de ser capaces de recibir comandos predefinidos y poder dar una respuesta valida a un comando que no reconocen.
    * REQ-F05: El eieManager debe de poder reiniciar ante un fallo que genere el cierre del proceso.
* No Funcionales:
    * REQ-NF01: El API expuesto debe ser fácilmente ingerible tal que un equipo de desarrollo pueda preparar una aplicación móvil en paralelo.
    * REQ-NF02: El eieManager debe soportar una amplia variedad de dispositivos, y que ademas estos se puedan agregar fácilmente sin cambios en el código.
    * REQ-NF03: El eieManager debe ser capaz de enviar una variedad de comandos en diferentes protocolos de comunicación, y que estos ademas se puedan expander fácilmente sin cambios en el código.
    * REQ-NF04: Se debe de priorizar el desarrollo del API para que el equipo externo que desarrolla la aplicación móvil pueda trabajar en paralelo.
    * REQ-NF05: Los eieDevice deben de ser capaces de funcionar con diferentes protocolos de comunicacion sin afectar el funcionamiento del API.