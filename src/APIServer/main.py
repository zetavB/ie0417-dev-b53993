from fastapi import FastAPI
from typing import Optional
from pydantic import BaseModel

app = FastAPI()

class Device(BaseModel):
    name: str
    type: str
    commands: list
    conexion: str

list = ["tester1", "tester2"]

deviceList = {
	name: Device(
		name = name,
		type = "temp",
		conexion = "192.168.1.1:1337",
		commands = ["stand", "sit", "play", "sleep"]
	)for name in list
}

@app.post("/devices/")
async def addDevice(deviceId, type, location, commands):
    device : Device(
        name = deviceId,
        type = type,
		conexion = location,
		commands = commands
    )
    return device

@app.get("/devices/")
async def showDevices():
	return deviceList

@app.get("/devices/{deviceId}")
async def showDevice(deviceId):
	device = deviceList[deviceId]
	return device

@app.delete("/devices/{deviceId}")
async def deleteDevice(deviceId):
	del deviceList[deviceId]

@app.get("/")
async def root():
	return {"message": "Hello World!"}