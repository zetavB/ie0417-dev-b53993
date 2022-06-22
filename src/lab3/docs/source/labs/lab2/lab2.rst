*************
Laboratorio 2
*************
Package: http://test.pypi.org/project/eieManager-zetavB/
Git Repo: http://github.com/zetavB/ie0417-dev-b53993-b83362-b42629

Decision de Endpoints
=====================
Para el laboratorio se decide utilizar 3 endpoints posibles.

/
---
   * GET: La raiz, esto simplemente mostrara algun mensaje.

/devices/
----------
   * GET: Aqui se desplegaran todos los dispositivos disponibles, junto con su tipo.
   * POST: Se recibe un JSON con el nombre de dispositivo y el tipo para crear un nuevo dispositivo usando DeviceManager.

/devices/{devId}
----------------
   * GET: Aqui se mostraran todos los detalles de el dispositivo con el nombre correspondiente a devId.
   * POST: Aqui se envia el comando a ejecutar. El tipo de dispositivo determina los comandos disponibles. Se ejecuta el Command con un CommandRunner y se actualiza el valor interno.
   * PUT: Aqui se envia un JSON con el nombre, tipo, y direccion IP deseada para cambiar las caracteristicas de un dispositivo particular.
   * DELETE: Se elimina el dispositivo especificado.
