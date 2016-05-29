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
CAMERA_ID = 1

SERIAL_START_BYTE = 0x12
SERIAL_STOP_BYTE  = 0x21
SERIAL_BAUD       = 9600
SERIAL_BYTESIZE   = serial.EIGHTBITS
SERIAL_PARITY     = serial.PARITY_NONE
SERIAL_STOPBITS   = serial.STOPBITS_ONE
SERIAL_PORT = "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AL0227SV-if00-port0"

# Servo angle constants.
STATE_SERVO_ANGLE_DELTA  = 5
STATE_SERVO_ANGLE_OPEN   = 10
STATE_SERVO_ANGLE_CLOSED = 90

# Trackbar constants.
TRACKBAR_DEFAULT_SPEED = 80
TRACKBAR_MAX_SPEED     = 100

def foo(a):
    pass

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
    state_servo_angle = STATE_SERVO_ANGLE_OPEN;

    # Instantiate and open serial port.
    # serial_obj = serial.Serial(port     = SERIAL_PORT,
    #                            baudrate = SERIAL_BAUD,
    #                            bytesize = SERIAL_BYTESIZE,
    #                            parity   = SERIAL_PARITY,
    #                            stopbits = SERIAL_STOPBITS,
    #                            timeout  = 2)

    # Setup the trackbar that controls speed.
    cv2.namedWindow("Display")
    cv2.createTrackbar("Speed", "Display", TRACKBAR_DEFAULT_SPEED, TRACKBAR_MAX_SPEED, foo);

    # Setup shutdown event and image frame queue for handling the image capture thread.
    event_shutdown = threading.Event()
    queue_frames = Queue.Queue()

    # Instantiate and start the image capture thread.
    # thread_obj_image_capture = threading.Thread(target = thread_image_capture,
    #                                             name   = "imgcap",
    #                                             args   = (event_shutdown, queue_frames, CAMERA_ID))

    # thread_obj_image_capture.start()

    # Flag used to signal another serial transmission.
    serial_packet_ready = 0

    # Main processing loop.
    while(not event_shutdown.is_set()):
        # If the image capture thread has added a frame to the queue, display it.
        if(not queue_frames.empty()):
            frame = queue_frames.get()
            cv2.imshow("Display", frame)

        # Get user input.
        user_input = (cv2.waitKey(1) & 0xFF)

        # Handle user input.
        if user_input == ord('w'):
            # Forward
            state_motor_left = 100 + cv2.getTrackbarPos("Speed", "Display")
            state_motor_right = 100 + cv2.getTrackbarPos("Speed", "Display")
            serial_packet_ready = 1
        if user_input == ord('a'):
            # Turn Left
            state_motor_left = 100 - cv2.getTrackbarPos("Speed", "Display")
            state_motor_right = 100 + cv2.getTrackbarPos("Speed", "Display")
            serial_packet_ready = 1
        if user_input == ord('s'):
            # Backwards
            state_motor_left = 100 - cv2.getTrackbarPos("Speed", "Display")
            state_motor_right = 100 - cv2.getTrackbarPos("Speed", "Display")
            serial_packet_ready = 1
        if user_input == ord('d'):
            # Turn Right
            state_motor_left = 100 + cv2.getTrackbarPos("Speed", "Display")
            state_motor_right = 100 - cv2.getTrackbarPos("Speed", "Display")
            serial_packet_ready = 1
        if user_input == ord('o'):
            # Open Servo
            state_servo_angle -= STATE_SERVO_ANGLE_DELTA

            if(state_servo_angle < STATE_SERVO_ANGLE_OPEN):
                state_servo_angle = STATE_SERVO_ANGLE_OPEN

            serial_packet_ready = 1
        if user_input == ord('l'):
            # Close Servo
            state_servo_angle += STATE_SERVO_ANGLE_DELTA

            if(state_servo_angle > STATE_SERVO_ANGLE_CLOSED):
                state_servo_angle = STATE_SERVO_ANGLE_CLOSED

            serial_packet_ready = 1
        if user_input == ord(' '):
            # Stop
            state_motor_left = 100
            state_motor_right = 100
            serial_packet_ready = 1
        if user_input == ord('q'):
            event_shutdown.set()

        # If it's time to transmit another serial packet, assemble said pack and
        # and transmite it.
        if(serial_packet_ready):
            serial_packet_ready = 0

            packet = struct.pack("BBBBB", SERIAL_START_BYTE, state_motor_left,
                                 state_motor_right, state_servo_angle,
                                 SERIAL_STOP_BYTE)

            # serial_obj.write(packet)

            print "main\tPacket: ", state_motor_left, " ", state_motor_right, " ", state_servo_angle

    # Wait for the image capture thread to end.
    thread_obj_image_capture.join();
    print "main\tImage Capture thread closed."


