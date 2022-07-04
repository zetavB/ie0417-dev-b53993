******************************
Proyecto Final: EieManager 2.0
******************************

.. image:: img/factory_iot.png
   :align: center

Justificación
++++++++++++++

Debido a las carencias existente a nivel de requerimientos funcionales y atributos de calidad del
el ``eie_manager`` actual, se ha decidido realizar algunas modificaciones al diseño para
implementar un prototipo actualizado al que se le ha llamado ``eieManager2.0``.

Se decidió implementar el concepto de `digital twin <https://www.ibm.com/topics/what-is-a-digital-twin>`_,
utilizando el framework de código abierto
`Eclipse Ditto <https://www.eclipse.org/ditto/intro-overview.html>`_, el cual utiliza un tipo
de licencia `EPL 2.0 <https://www.eclipse.org/legal/epl-2.0/>`_.  La implementacion de ``Ditto``
permite crear modelos virtuales que reflejan un objeto real, de modo que el ``client`` no tenga que interactuar
directamente con los devices, sino que interactúe con el digital twin para acceder un device real, de esta
forma se logra implementar un patron de diseño ``Proxy`` al prototipo ``eieManager2.0``.
El uso de ``Ditto`` resulta atractivo debido a que se logra implementar atributos de calidad requeridos en el nuevo
``eieManager2.0``, por ejemplo:

* La implementación de este componente coincide con los objetivos de negocio de aumentar el rendimiento y reducir los tiempos de latencia en momentos donde existan demasiados devices funcionando al mismo tiempo. La `arquitectura basada en microservicios <https://www.eclipse.org/ditto/architecture-overview.html>`_ de ``Eclipse Ditto`` consigue descentralizar el sistema, distribuyéndolo en diferentes procesos y por lo tanto, mejorando la ``escalabilidad`` del sistema.

* Implementa un `REST API <https://www.eclipse.org/ditto/httpapi-overview.html>`_ y un protocolo genérico (`Ditto protocol <https://www.eclipse.org/ditto/protocol-overview.html>`_) para administrar los dispositivos y abstraer el hardware mediante el concepto de `Thing <https://www.eclipse.org/ditto/basic-thing.html>`_.

* Mejora la ``seguiridad`` del sistema estableciendo politicas para el acceso a los dispositivos, de modo que solo sistemas autorizados puedan interactuar con las características de los devices

* Se puede integrar con una amplia variedad de protocolos de transporte tales como ``MQTT``, ``AMQP``, ``HTTP``, ``Apache Kafka``


Ya que ``Ditto`` no es una plataforma completa de IoT es necesario que los desarrolladores implementen un ``REST API`` que configure
los dispositivos y politícas de acceso, además de la logica para el procesamiento de mensajes y sincronizacion de estado entre el dispositivo real y su digital twin

Además, se propuso utilizar `Eclipse Mosquitto <https://mosquitto.org/>`_ para cubrir la necesidad de comunicar eventos de
forma asincrónica mediante el patrón de `MQTT <https://mqtt.org/>`_  ``publish/subscribe``, al ser un protocolo liviano, es facilmente
adaptable en dispositivos de bajos recursos y con poca memoria. La implementación de este componente
permite el desacoplamiento de los ``eie_devices``, permitiéndoles interactuar con otros componentes de forma
remota, por lo tanto este componente incorpora el patron de diseño ``Broker``. Al igual que ``Ditto``,
``Mosquitto`` utiliza `EPL 2.0 <https://www.eclipse.org/legal/epl-2.0/>`_

Según los requerimientos y objetivos de negocio, para la nueva arquitectura de ``eieManager 2.0``, se propone integrar ``Eclipse Ditto`` con ``Eclipse Mosquitto`` para la comunicación con los dispositivos mediante ``MQTT``. El siguiente diagrama muesta esta arquitectura.

.. image:: img/eie_manager_2.0.png
   :align: center


Requerimientos
++++++++++++++

- ``eie-device``
   - El ``eie-device`` debe ser implementado como una estructura ``Thing`` segun el protocolo ``Ditto``.
   - El ``eie-device`` debe soportar la recepcion de mensajes de ``Ditto`` a traves de ``MQTT`` para actualizarse a si mismo cuando su digital ``Twin`` llama a actualizarlo utilizando un callback tras modificar su propiedad ``configration`` de parte de un ``API`` externo.
   - El ``eie-device`` debe soportar el envio de mensajes a ``Ditto`` para actualizar su digital ``Twin`` mediante ``MQTT`` utilizando la publicacion de comandos que actualizan el ``Status`` en un feautre del ``Twin``.
   - El ``eie-device`` debe implementar la funcionalidad de ``Device Discovery`` donde el ``eie-device`` se subscibe a un topico especifico y publica un mensaje con el ``JSON`` correspondiente a su configuracion.

- ``eie-manager-config``
   - El ``eie-manager-config`` debe configurar las poiliticas de acceso y conexiones entre source target al broker de ``MQTT`` utilizando los archivos ``JSON`` de configuracion y luego resitrando esta informacion con el ``REST API`` de ``Ditto``.
   - El ``eie-manager-config`` debe implementar la funcionalidad de ``Device Discovery`` donde el ``eie-manager-config`` debe subscirbirse a un topic de ``request``, unicamente al iniciar el servicio, donde puede recibir solicitudes de los dispositivos.
   - El ``eie-manager-config`` debe implementar la funcionalidad de ``Device Discovery`` donde el ``eie-manager-config`` debe publicar a un topic de ``response`` una vez que se reciba y registre una solicitud de un dispositvo y haya registrado el nuevo ``Thing`` usando el ``REST API`` de Ditto.

Diseño de API
+++++++++++++
``device_create``
  Esta funcion se encarga de crear el ``eie_device``. Esto implica la subscripcion y publicacion de un mensaje en el topic ``request`` utilizando ``Device Discovery`` para la creacion del digital ``Twin`` en el ``eie_manager 2.0``.

``device_destroy``
  Esta funcion se encarga de destruir el ``eie_device``. Esto implica la publicacion de un mensaje indicando que se va a remover el device, y la des-subscripcion del topic ``request``.

``recieve_request``
  Esta funcion se encarga de actualizar su configuracion cuando se recibe un mensaje de ``Ditto`` a traves de ``MQTT``, enviado por parte del digital ``Twin`` al modificar su propiedad ``configration``.

``send_response``
  Esta funcion se encarga de mandar mensajes de ``Ditto`` de vuelta el ``eie_manager 2.0`` utilizando ``MQTT`` para actualizar la propiedad ``status`` de su digital ``Twin``.

Diagramas
+++++++++
1. El ``client`` modifica la propiedad ``configuration`` en un feature del `twin` de un dispositivo. Recuerde que esto genera un evento que ``Ditto`` notifica al dispositivo real mediante ``MQTT`` para que se actualice el estado del hardware. Debe mostrar toda la secuencia de comunicación desde el ``client`` hasta el ``eie-device`` (pasando por ``Ditto`` y el ``MQTT broker``).

.. uml::

  @startuml
  participant Client
  participant Ditto
  participant "MQTT Broker"
  participant "eie-device"
  Client->Ditto: Config Modify Request via REST
  Ditto->Ditto: Modify feature property
  Ditto-> "MQTT Broker": Ditto Event
  "MQTT Broker"-> "eie-device": Send JSon with Thing structure after applying changes
  "eie-device"-> "eie-device": Parse JSon
  "eie-device"-> "eie-device": Execute user feature property callback
  @enduml

2. El ``eie-device`` actualiza la propiedad ``status`` en un feature de su `twin` correspondiente publicando a un topic de ``MQTT``. Debe mostrar toda la secuencia de comunicación desde ``eie-device`` hasta ``Ditto`` (pasando por el ``MQTT broker``).

.. uml::

  @startuml
  participant Ditto
  participant "MQTT Broker"
  participant "eie-device"
  "eie-device"->"eie-device":  Feature propery (status) update
  "eie-device"->"MQTT Broker": Publish update
  "MQTT Broker"->"MQTT Broker": eie-device topic update
  "MQTT Broker"-> Ditto: Subscribers update
  "Ditto"-> Ditto: Feature update
  @enduml

3. El ``eie-device`` publica su configuración inicial y es registrado por ``eie-manager-config`` en ``Ditto``. Debe mostrar toda las secuencia de comunicación desde ``eie-device`` hasta ``Ditto`` (pasando por ``eie-manager-config``).

.. uml::

  @startuml
  participant Ditto
  participant "eie-manager-config"
  participant "MQTT Broker"
  participant "eie-device"
  "eie-device"->"MQTT Broker": Publish initial config of the Thing structure
  "MQTT Broker"-> "MQTT Broker": Update general topic members
  "MQTT Broker"-> "eie-manager-config": Send JSon File with the thing initial config
  "eie-manager-config" -> "eie-manager-config": Configure access politics
  "eie-manager-config" -> "MQTT Broker": Configure source/target conections
  "eie-manager-config"-> Ditto: Register device through REST API
  @enduml
