import os
import sys
import socket
from collections import namedtuple
import re
import stat
import subprocess

def main():
    sock = socket.socket(socket.AF_INET)
    sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,True)
    sock.bind(("127.0.0.1",9000))
    sock.listen(0)
    while True:
        conn_sock, addr_info = sock.accept()
        c = Connection(conn_sock)
        c.read_request()
        c.respond("this is the response")
        break

class Connection:
    status_map = {
            200 : 'OK',
            404 : 'Not Found',
            }
    def __init__(self, c):
        self.conn_socket = c
        self.buffer = ""
        self.headers = {}
        self.method = ""
        self.path = ""
        self.version = ""

    def _read_line(self):
        return self._read_until("\r\n")

    def _read_until(self, string):
        while string not in self.buffer:
            self.buffer += self.conn_socket.recv(7).decode('utf-8')
        tmp = self.buffer.split(string, maxsplit=1)
        result = tmp[0]
        self.buffer = tmp[1]
        return result

    def read_request(self):
        line = self._read_line()
        line = line.split(" ", maxsplit=2)
        self.method = line[0]
        self.path = line[1]
        self.version = line[2]
        self.get_headers()

    def get_headers(self):
        # returns a map of keys to values, setting if necessary
        if len(self.headers) == 0:
            while True:
                line = self._read_line()
                if line == "":
                    break
                # split the header into key and value.
                tmp = re.split(":\s*",line)
                key, value = tmp[0], tmp[1]
                self.headers[key] = value
        return self.headers;

    def get_body(self):
        """return the body of the request, as a string"""
        pass

    def respond(self, content):
        filename = os.getcwd() + self.path
        if os.path.exists(filename):
            status_code = 200
            content = open(filename, 'r').read()
            if stat.S_IXUSR & os.stat(filename)[stat.ST_MODE]:
                content = subprocess.check_output(filename, stderr=subprocess.STDOUT, shell=True).decode('utf-8')# shell means yeah go ahead and execute it.
        else:
            status_code = 404
            content = "File not found yo"
        status_text = self.status_map[status_code]
        # this should be broken up into seperate methods
        self.conn_socket.send("{} {} {} \r\n {} \r\n {}".
                              format(self.version, status_code, status_text,
                                     "Content-Length: {}".format(len(content)), content).encode())

    def close(self):
        self.conn_socket.close()


if __name__ == "__main__":
    main()
