#!/usr/bin/env python

import serial
import struct
import time


SERIAL_PORT = "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AL0227SV-if00-port0"
SERIAL_BAUD        = 9600
SERIAL_BYTESIZE    = serial.EIGHTBITS
SERIAL_PARITY      = serial.PARITY_NONE
SERIAL_STOPBITS    = serial.STOPBITS_TWO

SERIAL_START_BYTE  = 0x00
SERIAL_STOP_BYTE   = 0x01


if __name__ == "__main__":
    serial_obj = serial.Serial(port     = SERIAL_PORT,
                               baudrate = SERIAL_BAUD,
                               bytesize = SERIAL_BYTESIZE,
                               parity   = SERIAL_PARITY,
                               stopbits = SERIAL_STOPBITS,
                               timeout  = 0.1)

    while(True):
        for i in range(0, 10):
            packet = struct.pack("B", i)
            serial_obj.write(packet)
        # trans = 'a'
        
        # recv = serial_obj.read(1)
        # print "Transmitted ", trans, " Received ", recv
        time.sleep(1)
