#!/usr/bin/env python

import ctypes
import cv2
import numpy
import Queue
import serial
import struct
import time
import threading


# Constants.
SERIAL_SEND_RATE   = 2 # (Hz)
SERIAL_START_BYTE  = 0x7E
SERIAL_STOP_BYTE   = 0x7F
SERIAL_BAUD        = 9600
SERIAL_BYTESIZE    = serial.EIGHTBITS
SERIAL_PARITY      = serial.PARITY_NONE
SERIAL_STOPBITS    = serial.STOPBITS_ONE

# The serial port name for Eden's PC (Ubuntu)
# SERIAL_PORT = "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A703TYBJ-if00-port0"
# The serial port name for Eden's laptop
# Possible just different FTDI chips
SERIAL_PORT = "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AL0227SV-if00-port0"

CAMERA_ID = 0


# Thread dedicated to capturing video frames. This ensures that the main thread
# does not have to wait ~70ms for each new image.
def thread_image_capture(shutdown, frames, camera_id):
    print "imgcap\tThread starting..."

    camera = cv2.VideoCapture(camera_id)
    t = time.time()

    while(not shutdown.is_set()):
        success, frame = camera.read()

        if success:
            fps = 1 / (time.time() - t)
            t = time.time()
            cv2.putText(frame, str(int(fps)), (10,30), cv2.FONT_HERSHEY_PLAIN, 1, (255,255,255))
            frames.put_nowait(frame)

    camera.release()

    print "imgcap\tThread stopping..."


if __name__ == "__main__":
    # Setup default craft state.
    motor_state_left = 0;
    motor_state_right = 0;
    servo_state = 100; # 0 - fully closed / 100 - fully open

    # Instantiate and open serial port.
    serial_obj = serial.Serial(port     = SERIAL_PORT,
                               baudrate = SERIAL_BAUD,
                               bytesize = SERIAL_BYTESIZE,
                               parity   = SERIAL_PARITY,
                               stopbits = SERIAL_STOPBITS,
                               timeout  = 2)

    # Setup shutdown event and image frame queue for handling the image capture thread.
    event_shutdown = threading.Event()
    queue_frames = Queue.Queue()

    # Instantiate and start the image capture thread.
    thread_obj_image_capture = threading.Thread(target = thread_image_capture,
                                                name   = "imgcap",
                                                args   = (event_shutdown, queue_frames, CAMERA_ID))

    thread_obj_image_capture.start()

    # Setup time tracking variables.
    time_start = time.time()
    time_lasttrans = time.time()

    # Main processing loop.
    while(not event_shutdown.is_set()):
        # If the image capture thread has added a frame to the queue, display it.
        if(not queue_frames.empty()):
            frame = queue_frames.get();
            cv2.imshow('frame', frame)

        # If it's time to transmit another serial packet, assemble said pack and
        # and transmite it.
        if((time.time() - time_lasttrans) > 0.5):
            time_lasttrans = time.time()

            packet = struct.pack("bbbbb", SERIAL_START_BYTE, motor_state_left,
                                 motor_state_right, servo_state,
                                 SERIAL_STOP_BYTE)

            serial_obj.write(packet)

        # Get user input.
        user_input = (cv2.waitKey(1) & 0xFF)

        # Handle user input.
        if (user_input >= ord('A')) and (user_input <= ord('z')):
            print "main\tInput: ", user_input
        if user_input == ord('q'):
            event_shutdown.set();

    # Wait for the image capture thread to end.
    thread_obj_image_capture.join();
    print "main\tImage Capture thread closed."


