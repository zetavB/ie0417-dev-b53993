import requests
import paho.mqtt.client as mqtt
import json
import time
import struct

url = "http://localhost:8080/api/2/policies/my.test:policy"


def encode_json(file):
    f = open(file)
    # returns JSON object as
    # a dictionary
    payload = json.load(f)
    # Closing file
    f.close()
    payload = json.dumps(payload)
    payload = bytes(payload, "utf-8")
    return payload


def connection_config():


def ditto_register():
    payload = encode_json("payload.json")
    print(payload)
    r = requests.put(url, data = payload)


ditto_register()
