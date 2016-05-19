#!/usr/bin/env python

# Non-standard libraries.
import cv2
import numpy
import serial

# Standard libraries.
import Queue
import time
import threading


# Constants.
CAMERA_ID = 0


def thread_image_capture(shutdown, frames, camera_id):
    print "imgcap\tThread starting..."

    camera = cv2.VideoCapture(camera_id)
    t = time.clock()

    while(not shutdown.is_set()):
        success, frame = camera.read()

        if success:
            fps = 1 / (time.clock() - t)
            t = time.clock()
            cv2.putText(frame, str(int(fps)), (10,30), cv2.FONT_HERSHEY_PLAIN, 1, (255,255,255))
            frames.put_nowait(frame)

    camera.release()

    print "imgcap\tThread stopping..."

if __name__ == "__main__":
    event_shutdown = threading.Event()
    queue_frames = Queue.Queue()

    thread_obj_image_capture = threading.Thread(target = thread_image_capture,
                                                name   = "imgcap",
                                                args   = (event_shutdown, queue_frames, CAMERA_ID))

    thread_obj_image_capture.start()

    time_start = time.clock()
    time_lasttrans = time.clock()

    while(not event_shutdown.is_set()):
        if(not queue_frames.empty()):
            frame = queue_frames.get();
            cv2.imshow('frame', frame)

        if((time.clock() - time_lasttrans) > 0.1):
            print "main\tSerial send: "

        user_input = cv2.waitKey(1)

        if user_input == ord('q'):
            event_shutdown.set();
        elif

    thread_obj_image_capture.join();
    print "main\tImage Capture thread closed."