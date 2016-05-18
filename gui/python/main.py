#!/usr/bin/env python

import cv2
import numpy
import serial
import time
from threading import Thread

video_capture_device_id = 0
video_capture_device = None


def thread_display():
    while(True):
        pass
    # video_capture_device = cv2.VideoCapture(video_capture_device_id)

    # while(True):
    #     ret, frame = video_capture_device.read()
    #     cv2.imshow('frame', frame)


def thread_input_user():
    while(True):
        user_input= cv2.waitKey(1);

        if(user_input >= ord('a')):
            print "User input: ", user_input, "\n"

def thread_serial(interval):
    while(True):
        print str(interval), " sec"
        time.sleep(interval)

if __name__ == "__main__":
    thread_display_obj    = Thread(target = thread_display, args = ())
    thread_input_user_obj = Thread(target = thread_input_user, args = ())
    thread_serial_obj     = Thread(target = thread_serial, args = (1, ))

    thread_display_obj.start();
    thread_input_user_obj.start();
    thread_serial_obj.start();

    # serial_thread.join();

    font = cv2.FONT_HERSHEY_PLAIN
    
    video_capture_device = cv2.VideoCapture(video_capture_device_id)

    time_start = time.clock()
    while(cv2.waitKey(1) != ord('q')):
        ret, frame = video_capture_device.read()
        
        ms = (time.clock() - time_start)*1000
        time_start = time.clock();

        cv2.putText(frame, str(int(ms)), (10,30), font, 1, (255,255,255))
        cv2.imshow('frame', frame)

    cap.release()
    cv2.destroyAllWindows()
