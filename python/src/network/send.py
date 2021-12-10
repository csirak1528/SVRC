import json
import socket

# import os
# import sys
# import time
# import random




s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = "localhost"


def connect(addr=host, test=False,port=5025):
    global s
    try:
        s.connect((addr, port))
    except:
        return False
    if test:
        s.close()
    return True

def json_to_bytes(data):
    return json.dumps(data).encode('utf-8')

def sendbytes(bytes, buffer):
    s.sendall(json_to_bytes(bytes))
    s.close()



if __name__ == "__main__":
    if connect():
        sendbytes( {"dict":'hi'},1)
