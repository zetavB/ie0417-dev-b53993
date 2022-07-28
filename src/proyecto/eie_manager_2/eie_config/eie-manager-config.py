#!/usr/bin/env python3
import requests
import json
import time
import struct


url_policy = "http://localhost:8080/api/2/policies/my.test:policy"
url_thing = "http://localhost:8080/api/2/things/my.test:octopus"
url_conection  = "http://localhost:8080/devops/piggyback/connectivity?timeout=10000"


def encode_json(file):
    f = open(file)
    #returns JSON object as
    # a dictionary
    payload = json.load(f)
    # Closing file
    f.close()
    payload = json.dumps(payload)
    payload = bytes(payload, "utf-8")
    return payload


def decode_json(payload):
    payload_decoded = payload.decode('utf8').replace("'", '"')
    payload_decoded =   json.loads(payload_decoded)
    payload_decoded = json.dumps(payload_decoded)
    return payload_decoded;


def ditto_policy_create(url):
    payload = encode_json("policy.json")
    try:
        r = requests.put(url, data = payload)
        return 0
    except:
        return -1


def ditto_thing_create(url):
    payload = encode_json("policy.json")
    try:
        r = requests.put(url, data = payload)
        return 0
    except:
        return -1


def create_mqtt_conection(url):
    payload = encode_json("DevOps.json")
    try:
        r = requests.post(url, data = payload)
        return 0
    except:
        return -1


def ditto_policy_return():
    payload = encode_json("policy.json")
    payload_decoded = decode_json(payload)
    output = 'Sending' + payload_decoded
    print(output)
    return output


def ditto_thing_return():
    payload = encode_json("thing.json")
    payload_decoded = decode_json(payload)
    output = 'Sending' + payload_decoded
    print(output)
    return output


def main():
    time.sleep(10)
    temp_return = ditto_thing_create(url_thing)
    if( temp_return== 0):
        print(ditto_thing_return())
    else:
        print('Thing configuration failed')
    time.sleep(10)
    temp_return = ditto_policy_create(url_policy)
    if(temp_return== 0):
        print(ditto_policy_return())
    else:
        print('Policy configuration failed')
    time.sleep(10)
    temp_return = create_mqtt_conection(url_conection)
    if(temp_return== 0):
        print('Conected')
    else:
        print('Conection failed')

main()

#time.sleep(5)
#ditto_policy_create(url_policy)
#print(ditto_policy_create(url_policy))
