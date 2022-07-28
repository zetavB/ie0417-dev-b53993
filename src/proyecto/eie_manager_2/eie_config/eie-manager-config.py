#!/usr/bin/env python3
from flask import Flask, render_template, request
import requests
import json
import time
import struct


url_policy = "http://localhost:8080/api/2/policies/my.test:policy"
url_thing = "http://localhost:8080/api/2/things/my.test:octopus"
app = Flask(__name__)


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


def ditto_policy_register(url):
    payload = encode_json("policy.json")
    payload_decoded = decode_json(payload)
    print("Unserialized payload\n", payload_decoded)
    print('Serialized payload', payload)
    try:
        #r = requests.put(url, data = payload)
        output = 'Sending' + payload_decoded
        print(output)
        return output
    except:
        return 'Port not available'


def ditto_thing_register(url):
    payload = encode_json("thing.json")
    payload_decoded = decode_json(payload)
    print("Unserialized payload\n", payload_decoded)
    print('Serialized payload', payload)
    try:
        #r = requests.put(url, data = payload)
        output = 'Sending' + payload_decoded
        return output
    except:
        return 'Port not available'


@app.route('/')
def hello():
    return render_template('index.html')


@app.route('/request', methods = ['POST', 'GET'])
def send_request():
    if request.form.get('Policy')== 'Policy':
        return ditto_policy_register(url_policy)
        time.sleep(2)
    elif request.form.get('Thing') == 'Thing':
        return ditto_thing_register(url_thing)
        time.sleep(2)
    #data = ditto_policy_register(url_policy)
    #return render_template('index.html', data = data)


if __name__ == '__main__':
    app.run()

#time.sleep(5)
#ditto_policy_register(url_policy)
#print(ditto_policy_register(url_policy))
