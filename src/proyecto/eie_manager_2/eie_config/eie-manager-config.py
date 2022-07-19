import requests
import paho.mqtt.client as mqtt
import json
import time
import struct

url_policy = "http://localhost:8080/api/2/policies/my.test:policy"
url_thing = "http://localhost:8080/api/2/things/my.test:octopus"

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


#def connection_config():


def ditto_policy_register(url):
    payload = encode_json("policy.json")
    print(payload)
    r = requests.put(url, data = payload)


def ditto_thing_register(url):
    payload = encode_json("thing.json")
    print(payload)
    r = requests.put(url, data = payload)


ditto_thing_register(url_thing)
time.sleep(5)
ditto_policy_register(url_policy)
