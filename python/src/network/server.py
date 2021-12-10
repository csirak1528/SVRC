import socket
from sys import exit
import time
import os
import json



def get_ip():
    pass


SEPARATOR = "<SEPARATOR>"
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
addr="localhost"
print(addr)

def binder(queue=5,port=5025):
    try:
        s.bind((addr, port))
        s.listen(queue)
        return True
    except OSError:
        return False


def data_scan(filename, buffer, update=False):
    with open(filename, "wb") as f:
        while True:
            k = clientsocket.recv(buffer)
            if not k:
                break
            f.write(k)
        f.close()
    
def parse_json(bytes):
    return json.loads(bytes)
    
binder()
while True:
    try:
        clientsocket, address = s.accept()
    except KeyboardInterrupt:
        exit()
    cmd = clientsocket.recv(200)
    scmd = cmd.decode("utf-8")
    print(parse_json(cmd))
