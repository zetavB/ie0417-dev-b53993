"""
PyZMQ REQ socket client example module.

This is the client script for the zmq_server_rep_pthread program (@ examples/c/zmq_demo).
"""

import time
import zmq
import struct
import json
def convertTuple(tup):
        # initialize an empty string
    str = ''
    for item in tup:
        str = str + item
    return str

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

def main():
    commands = ["message","ping_pong"]
    payload = encode_json('../config/device_cfg.json')
    payload_size = len(payload)
    #print(payload, payload_size)

    port = 5555;
    context = zmq.Context()
    print("Connecting to server...")
    client = context.socket(zmq.REQ)
    with client.connect(f"tcp://localhost:{port}"):
        #for command in commands:
        for command in commands:
            # Send request
            # Assuming little-endian in C side
            command = bytes(command, 'utf-8')
            header = struct.pack('< 100s i', command, payload_size)
            print(" \n*************** Sending... ************************ \n")
            print("command: ",command.decode("utf-8"), "\npayload_size: ", int(payload_size), "\npayload: ", payload)
            client.send(header+payload)

            # Receive response
            rep = client.recv()
            #offset = len(header)
            rep_payload= struct.unpack(str(len(rep))+'s', rep)
            print(" \n************ Receiving response... ****************\n")
            print(f"Received [payload response: {str(rep_payload)}]")

if __name__ == "__main__":
    main()
