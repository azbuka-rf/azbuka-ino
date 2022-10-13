'''
ochen jalko
russia ura
'''

# import wave, random,struct,math
import socket
sampleRate = 8000.0
durations = 1
frequency = 440.0

HOST = "192.168.43.185"
PORT = 1234
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)
print('Socket created')

s.bind((HOST, PORT))

s.listen(5)
print ('Socket awaiting messages')
(conn, addr) = s.accept()
print ('Connected')

def reader():
  while(1):
    data = str(conn.recv(1024))[2:-1]
    print(data)

def writer():
  while(1):
    msg = input() + "\n"
    conn.send(msg.encode())

from threading import Thread

IN = Thread(target=reader); IN.start()
OUT = Thread(target=writer); OUT.start()
