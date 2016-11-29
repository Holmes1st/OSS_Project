#!/usr/bin/python
import socket as s

HOST = 'localhost'
PORT = 31337
ADDR = (HOST, PORT)

try:
    link = s.socket(s.AF_INET, s.SOCK_STREAM) # make socket
except Exception as e:
    print e;
    exit(1);

try:
    link.connect(ADDR) # connect to echo server
except Exception as e:
    print e;
    exit(1);

print "[ACCESS] Success"
print "[READING] ..."

while(1):
    data = link.recv(1024)
    if data:
        print data
    else:
        break;

