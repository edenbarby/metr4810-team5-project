#!/usr/bin/env python

import ctypes
import cv2
import numpy
import Queue
import serial
import struct
import time
import threading


TRACKBAR_VALUE_SPEED = 100;
TRACKBAR_VALUE_CLAWOPEN = 0;
TRACKBAR_VALUE_CLAWCLOSED = 0;

TRACKBAR_MAX_SPEED = 100;
TRACKBAR_MAX_CLAWOPEN = 100;
TRACKBAR_MAX_CLAWCLOSED = 100;


# Constants.
SERIAL_START_BYTE  = 0x12
SERIAL_STOP_BYTE   = 0x21
SERIAL_BAUD        = 9600
SERIAL_BYTESIZE    = serial.EIGHTBITS
SERIAL_PARITY      = serial.PARITY_NONE
SERIAL_STOPBITS    = serial.STOPBITS_ONE

# FTDI serial chip ID
SERIAL_PORT = "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AL0227SV-if00-port0"

CAMERA_ID = 0


def trackbar_callback_speed(trackbar_value):
    TRACKBAR_VALUE_SPEED = trackbar_value
def trackbar_callback_clawopen(trackbar_value):
    TRACKBAR_VALUE_CLAWOPEN = trackbar_value
def trackbar_callback_clawclosed(trackbar_value):
    TRACKBAR_VALUE_CLAWCLOSED = trackbar_value


# Thread dedicated to capturing video frames. This ensures that the main thread
# does not have to wait ~50ms for each new image.
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
    state_motor_left = 100;
    state_motor_right = 100;
    state_servo_angle = 0;

    # Instantiate and open serial port.
    serial_obj = serial.Serial(port     = SERIAL_PORT,
                               baudrate = SERIAL_BAUD,
                               bytesize = SERIAL_BYTESIZE,
                               parity   = SERIAL_PARITY,
                               stopbits = SERIAL_STOPBITS,
                               timeout  = 2)

    # Setup time tracking variables.
    time_start = time.time()
    time_lasttrans = time.time()

    cv2.namedWindow("Display")
    # cv2.createTrackbar("Motor Speed", "Display", TRACKBAR1_VALUE, TRACKBAR1_VALUE_MAX, trackbar_callback1);
    # cv2.createTrackbar("Servo Pos", "Display", TRACKBAR2_VALUE, TRACKBAR2_VALUE_MAX, trackbar_callback2);

    # Setup shutdown event and image frame queue for handling the image capture thread.
    event_shutdown = threading.Event()
    queue_frames = Queue.Queue()

    # Instantiate and start the image capture thread.
    thread_obj_image_capture = threading.Thread(target = thread_image_capture,
                                                name   = "imgcap",
                                                args   = (event_shutdown, queue_frames, CAMERA_ID))

    thread_obj_image_capture.start()

    # Main processing loop.
    while(not event_shutdown.is_set()):
        # If the image capture thread has added a frame to the queue, display it.
        if(not queue_frames.empty()):
            frame = queue_frames.get();
            cv2.imshow("Display", frame)

        # If it's time to transmit another serial packet, assemble said pack and
        # and transmite it.
        # if((time.time() - time_lasttrans) > 0.1):
        #     time_lasttrans = time.time()

        #     packet = struct.pack("BBBBB", SERIAL_START_BYTE, motor_state_left,
        #                          motor_state_right, servo_state,
        #                          SERIAL_STOP_BYTE)

        #     # packet = struct.pack("BBBBB", SERIAL_STOP_BYTE, servo_state,
        #     #                      motor_state_right, motor_state_left,
        #     #                      SERIAL_START_BYTE)

        #     serial_obj.write(packet)

        #     # print "main\tPacket: ", motor_state_left, " ", motor_state_right, " ", servo_state

        # Get user input.
        user_input = (cv2.waitKey(1) & 0xFF)

        # Handle user input.
        # if user_input == ord('w'):
        #     # Forward
        #     motor_state_left = 100 + TRACKBAR1_VALUE;
        #     motor_state_right = 100 + TRACKBAR1_VALUE;
        # if user_input == ord('a'):
        #     # Turn Left
        #     motor_state_left = 100 - TRACKBAR1_VALUE;
        #     motor_state_right = 100 + TRACKBAR1_VALUE;
        # if user_input == ord('s'):
        #     # Backwards
        #     motor_state_left = 100 - TRACKBAR1_VALUE;
        #     motor_state_right = 100 - TRACKBAR1_VALUE;
        # if user_input == ord('d'):
        #     # Turn Right
        #     motor_state_left = 100 + TRACKBAR1_VALUE;
        #     motor_state_right = 100 - TRACKBAR1_VALUE;
        if user_input == ord(' '):
            # Stop
            motor_state_left = 100;
            motor_state_right = 100;
        if user_input == ord('q'):
            event_shutdown.set();

    # Wait for the image capture thread to end.
    thread_obj_image_capture.join();
    print "main\tImage Capture thread closed."


