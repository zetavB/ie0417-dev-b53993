"""
PyZMQ REQ socket client example module.

This is the client script for the zmq_server_rep_pthread program (@ examples/c/zmq_demo).
"""

import time
import zmq
import struct
import json

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
    commands = ["message", "pingpong"]
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
            header = struct.pack('< 100s i ', command, payload_size)
            print(header, payload)
            client.send(header+payload)

            # Receive response
            rep = client.recv()
            rep_command, rep_payload_size, rep_out_buffer= struct.unpack('<100s i 100s', rep)
            print(f"Received response [command_response: {rep_command}, payload_size:{rep_payload_size}, buff: {rep_out_buffer}]")
if __name__ == "__main__":
    main()
