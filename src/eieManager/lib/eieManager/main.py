import logging
from random import choice
from fastapi import FastAPI
from typing import Dict, Optional
from pydantic import BaseModel, Json

from . import command
from .args import parse_args
from .DeviceManager.manager import SensorManager
from .DeviceManager.analyzers import avg_thresh_analyzer as avt


log = logging.getLogger(__name__)
app = FastAPI()

args = parse_args()

config_name = args.config
sensor_type_name = args.sensor_type
sensor_cmd_per_period, sensor_period_sec = (100, 5)
alert_cmd_per_period, alert_period_sec = (2, 1)
analyzer_avg_thresh = 10
num_read_commands = 200

# Set up command runners
sensor_mgr = SensorManager(config_name)
sensor_cmd_runner = command.CommandRunner(
    cmd_per_period=sensor_cmd_per_period,
    period_sec=sensor_period_sec)
alert_cmd_runner = command.CommandRunner(
    cmd_per_period=alert_cmd_per_period,
    period_sec=alert_period_sec)
sensor_cmd_runner.start()
alert_cmd_runner.start()

class Data(BaseModel):
    data : list

@app.get("/")
async def root():
    return {"message": "Hello World!"}

@app.post("/devices/")
async def addDevice(changes: Data):
    sensor_mgr.create_new_sensor(changes.data)
    return "Device added"

@app.delete("/devices/{devId}")
async def deleteDevice(devId: str):
    sensor_mgr.delete_sensor(devId)
    return "Device deleted"

@app.put("/devices/{devId}")
async def updateDevice(devId: str, changes: Data):
    tempList = sensor_mgr.update_sensor(devId, changes.data)
    return "Device changed"

@app.get("/devices/")
async def showDevices():
    
    sensor_mgr.get_sensor_names_per_type('temperature')
    sensor_mgr.get_sensor_names_per_type('level')
    temps = ['temperature']*len(sensor_mgr.get_sensor_names_per_type('temperature'))
    levels = ['level']*len(sensor_mgr.get_sensor_names_per_type('level'))
    dictTemp = dict(zip(sensor_mgr.get_sensor_names_per_type('temperature'), temps))
    dictLevel = dict(zip(sensor_mgr.get_sensor_names_per_type('level'), levels))
    return {**dictTemp, **dictLevel}

@app.get("/devices/{devId}")
async def showDevice(devId: str):
    tempList = sensor_mgr.get_sensor_details(devId)
    return tempList

# Teardown command runners
sensor_cmd_runner.stop()
alert_cmd_runner.stop()

def main():
    """
    Sensor commands application main function.
    """
    args = parse_args()

    config_name = args.config
    sensor_type_name = args.sensor_type
    sensor_cmd_per_period, sensor_period_sec = (100, 5)
    alert_cmd_per_period, alert_period_sec = (2, 1)
    analyzer_avg_thresh = 10
    num_read_commands = 200

    # Set up command runners
    sensor_mgr = SensorManager(config_name)
    sensor_cmd_runner = command.CommandRunner(
        cmd_per_period=sensor_cmd_per_period,
        period_sec=sensor_period_sec)
    alert_cmd_runner = command.CommandRunner(
        cmd_per_period=alert_cmd_per_period,
        period_sec=alert_period_sec)
    sensor_cmd_runner.start()
    alert_cmd_runner.start()
    
    # Start API

    # # Set up sensor analyzer with "above average threshold alert" strategy
    # analyzer = avt.SensorAvgThreshAnalyzer(avg_thresh=analyzer_avg_thresh)
    # avt.set_alert_handle_strategy(analyzer, alert_cmd_runner)
    # avt.set_above_compare_strategy(analyzer) 

    # Generate read commands for temp sensors

    # print(sensor_mgr.get_sensor_names())
    # temp_sensor_names = sensor_mgr.get_sensor_names_per_type(sensor_type_name)
    # for _ in range(num_read_commands):
    #     rand_sensor_name = choice(temp_sensor_names)
    #     read_cmd = sensor_mgr.create_sensor_read_cmd(rand_sensor_name,
    #                                                  analyzer)
    #     sensor_cmd_runner.send(read_cmd)

    # Teardown command runners
    #sensor_cmd_runner.stop()
    #alert_cmd_runner.stop()


if __name__ == "__main__":
    main()
