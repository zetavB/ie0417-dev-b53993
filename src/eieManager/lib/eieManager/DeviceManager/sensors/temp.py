from random import randint

from ..sensor import Sensor


class TempSensor(Sensor):
    def __init__(self, name: str) -> None:
        temp = randint(0, 254)
        conexion = "192.168.1.{}".format(temp)
        super().__init__(name, "temperature", conexion, ["ReadTemp"])

    def read(self) -> float:
        return randint(0, 100)

    def update(self, changes: list):
        self._name = changes[0]
        self._type = changes[1]
        self._conexion = changes[2]
        pass
