from typing import Optional
from abc import ABC, abstractmethod

from ..command import Command


class Sensor(ABC):
    """
    Generic sensor representation.

    :param str name: Name of the sensor.
    :param str stype: Type of the sensor.
    :param str unit: Measurement unit of the sensor.
    """
    def __init__(self, name: str, stype: str, conexion: str, commands: list) -> None:
        self._name = name
        self._type = stype
        self._commands = commands
        self._conexion = conexion

    def name(self) -> str:
        """
        Gets the name of the sensor.
        """
        return self._name

    def type(self) -> str:
        """
        Gets the type of the sensor.
        """
        return self._type

    def conexion(self) -> str:
        """
        Gets the conexion of the sensor.
        """
        return self._conexion

    def commands(self) -> list:
        """
        Gets the commands of the sensor.
        """
        return self._commands

    @abstractmethod
    def read(self) -> float:
        """
        Reads the sensor.
        :return: Sensor reading.
        """
        pass


class SensorAnalyzer(ABC):
    """
    Generic sensor analyzer that processes updates from sensor reads.
    """
    @abstractmethod
    def update(self, value: float):
        """
        Updates the analyzer state with a new sensor reading.

        :param float value: Sensor reading value.
        """
        pass


class SensorReadCommand(Command):
    """
    Command to read a Sensor and optionally update a SensorAnalyzer.

    :param sensor: Sensor object.
    :type sensor: :class:`Sensor`
    :param analyzer: SensorAnalyzer object or None.
    :type analyzer: :class:`SensorAnalyzer` or None.
    """
    def __init__(
            self,
            sensor: Sensor,
            analyzer: Optional[SensorAnalyzer] = None
    ) -> None:
        self.sensor = sensor
        self.analyzer = analyzer

    def execute(self) -> None:
        """
        Reads the sensor and optionally send value to analyzer.
        """
        name = self.sensor.name()
        stype = self.sensor.type()
        conexion = self.sensor.conexion()
        commands = self.sensor.commands()
        value = self.sensor.read()
        if self.analyzer is not None:
            self.analyzer.update(value)
        print(f"Device Response: [{stype}] {name} @ {conexion}")
        print("Commands Available:")
        print(*commands, sep = ", ")
        
