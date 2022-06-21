# from utils import rand_gen
from pytest import fixture
import pytest
import logging
from sensor_commands.sensor import manager, sensor


class MockSensor(sensor.Sensor):
    def __init__(self, name: str, stype: str, unit: str) -> None:
        self._name = name
        self._type = stype
        self._unit = unit
        self._readed = False

    def read(self) -> bool:
        self._readed = True
        return self._readed

    def assert_read(self) -> int:
        if(self._readed):
            return 1
        else:
            return 0


@fixture
def sensor_mgr():
    file_name = "/home/dev/ws/testing_python/config/sensors_cfg.json"
    logging.info("Instancing a sensor manager")
    sensor_manager = manager.SensorManager(file_name)
    return sensor_manager


# 1
def test_sensor_manager_supported_types(sensor_mgr):
    logging.info("Getting suported types: ")
    print(sensor_mgr.get_supported_sensor_types())


# 2
def test_sensor_manager_single_sensor_create_destroy(sensor_mgr):
    logging.info("TEST CREATE/DESTROY")
    sensor_mgr.create_sensor("level-eie555", "level")
    print("getting sensors info: ", sensor_mgr.get_sensors_info())
    with pytest.raises(AssertionError):
        sensor_mgr.create_sensor("level-eie555", "level")
    sensor_mgr.destroy_sensor("level-eie555")
    with pytest.raises(AssertionError):
        sensor_mgr.destroy_sensor("level-eie555")


# 3
def test_sensor_manager_single_sensor_read_command(sensor_mgr):
    logging.info("TEST SINGLE SENSOR READ CMD func")
    sensor_mgr.create_sensor("temperature-eie333", "temperature")
    sensor_read = sensor_mgr.create_sensor_read_cmd("temperature-eie333")
    # Execute
    print("EXECUTING SENSOR_READ")
    sensor_read.execute()
    # Destroy
    sensor_mgr.destroy_sensor("temperature-eie333")


# 4
def test_sensor_manager_mock_type_register_unregister(sensor_mgr):
    logging.info("TEST MOCK REGISTER-UNREGISTER")
    mock = MockSensor("mock", "mock", "fakes")
    sensor_mgr.register_sensor_type("mock", mock)
    print("Getting supported types:", sensor_mgr.get_supported_sensor_types())
    sensor_mgr.unregister_sensor_type("mock")


# 5
def test_sensor_manager_mock_sensor_create_destroy(sensor_mgr):
    logging.info("TEST MOCK CREATE-DESTROY")
    mock = MockSensor("mock-eietest", "mock", "fakes")
    # Register
    sensor_mgr.register_sensor_type("mock", mock)
    print("Getting supported types:", sensor_mgr.get_supported_sensor_types())
    # Create
    sensor_mgr.create_sensor("mock-eietest", "mock")
    print("Getting sensors info with mock sensor", sensor_mgr.get_sensors_info())
    # Destroy
    sensor_mgr.destroy_sensor("mock-eie-test")
    # UNREGISTER
    sensor_mgr.unregister_sensor_type("mock")


# 6
def test_sensor_manager_mock_sensor_read_command(sensor_mgr):
    logging.info("TEST MOCK READ CMD")
    mock = MockSensor("mock-eietest", "mock", "fakes")
    sensor_mgr.register_sensor_type("mock", mock)
    sensor_read = sensor_mgr.create_sensor_read_cmd("mock-eietest")
    # Execute
    print("EXECUTING SENSOR_READ")
    sensor_read.execute()
    # Destroy
    sensor_mgr.destroy_sensor("mock-eietest")
    # Assert read
    assert mock.assert_read() == 1, "Not read"
    # Destroy
    sensor_mgr.destroy_sensor("mock-eie-test")
    # UNREGISTER
    sensor_mgr.unregister_sensor_type("mock")
