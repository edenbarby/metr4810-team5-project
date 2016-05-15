#!/usr/bin/env python

import cv2
import numpy
# import pyserial
import time

video_capture_device_id = 0
video_capture_device = None


if __name__ == "__main__":
    font = cv2.FONT_HERSHEY_PLAIN
    
    video_capture_device = cv2.VideoCapture(video_capture_device_id)

    while(True):
        time_start = time.clock()

        ret, frame = video_capture_device.read()

        time_stop = time.clock()
        ms = (time_stop - time_start)*1000
        cv2.putText(frame, str(int(ms)), (10,30), font, 1, (255,255,255))

        cv2.imshow('frame', frame)

        # print cv2.waitKey(1)
        if cv2.waitKey(1) == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
