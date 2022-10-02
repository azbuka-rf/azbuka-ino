# # print("opening serial port...")
import serial
ser = serial.Serial()
ser.baudrate = 115200
ser.port = '/dev/ttyUSB6'
print(ser)
ser.dtr = None
ser.rts = None
print(ser)
ser.open()


# # while(1):
#   zamay = str(ser.read_all())[2:-1]
#   zamay = zamay.replace("\\r\\n", '\n')
#   print(zamay, end="")
# # ser.close()

# # import sys
# # while(1):
# #   print(sys.stdin.read())

def reader():
  while(1):
    zamay = str(ser.read_all())[2:-1]
    zamay = zamay.replace("\\n", '\n')
    zamay = zamay.replace("\\r", "")
    print(zamay, end="")

def writer():
  while(1):
    msg = input() + "\n"
    ser.write(msg.encode())

from threading import Thread

IN = Thread(target=reader); IN.start()
OUT = Thread(target=writer); OUT.start()

