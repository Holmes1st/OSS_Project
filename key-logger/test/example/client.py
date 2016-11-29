#!/usr/bin/python
import socket as s
import sys


def how_2_use():
    print "USE : %s SERVER_IP SERVER_PORT" % sys.argv[0]
    exit(1);

def main():
    if len(sys.argv) !=3:
        how_2_use()

    HOST = sys.argv[1]
    PORT = int(sys.argv[2])
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

if __name__ == '__main__':
    main()
