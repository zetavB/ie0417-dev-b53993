from random import randint

from ..sensor import Sensor


class LevelSensor(Sensor):
    def __init__(self, name: str) -> None:
        temp = randint(0, 254)
        conexion = "192.168.2.{}".format(temp)
        super().__init__(name, "level", conexion, ["ReadLevel"])

    def read(self) -> float:
        return randint(1, 50)

    def update(self, changes: list):
        self._name = changes[0]
        self._type = changes[1]
        self._conexion = changes[2]
