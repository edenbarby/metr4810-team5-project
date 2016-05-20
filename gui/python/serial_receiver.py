#!/usr/bin/env python

import serial
import time


SERIAL_PORT = "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AL0227SV-if00-port0"
SERIAL_BAUD        = 9600
SERIAL_BYTESIZE    = serial.EIGHTBITS
SERIAL_PARITY      = serial.PARITY_ODD
SERIAL_STOPBITS    = serial.STOPBITS_TWO

SERIAL_START_BYTE  = 0x7E
SERIAL_STOP_BYTE   = 0x7F


if __name__ == "__main__":
    serial_obj = serial.Serial(port     = SERIAL_PORT,
                               baudrate = SERIAL_BAUD,
                               bytesize = SERIAL_BYTESIZE,
                               parity   = SERIAL_PARITY,
                               stopbits = SERIAL_STOPBITS,
                               timeout  = 1)

    while(True):
        serial_obj.write('a')
        time.sleep(0.01)
