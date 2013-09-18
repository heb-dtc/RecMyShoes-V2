#!/usr/bin/env python

import serial
from threading import Thread
import cv2
import time
import os
import datetime
import shutil

#some var.
record = None
videoName = None
localVideoPath = None

#grab tge serial port to which the Arduino is talking to
ser = serial.Serial('/dev/ttyACM0', 9600)

def getDriveName():
    return os.listdir('/media')[0]

def InitialiseCamera():
    camera = cv2.VideoCapture(0)
    if camera is None:
        print('Warning: unable to access camera')
    else:
        print('initialized camera')
    return camera

def InitialiseWriter():
    global videoName
    global localVideoPath

    fps = 7
    size = (640, 480)
    videoName = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S") 
    localVideoPath = '/home/pi/Dev/out/' + videoName + '.avi'

    codec = cv2.cv.CV_FOURCC('M','J','P','G')

    video  = cv2.VideoWriter(localVideoPath, codec, fps, size, True);
    if video is None:
        print('Warning: unable to create video writer')
    else:
    	print('initialized writer')
    return video

def StartListening():
    global record
    while 1:
       	#read value on the serial port
    	val = ser.readline()
    	print 'arduino sent command'
	
	if record:
	    record = False
        else:
            record = True
            Thread(target=RecordTask).start()

def Record():
    cam = InitialiseCamera()
    writer = InitialiseWriter()
    print('Start recording.....')
    while record:
        f,img = cam.read()
        try:
            writer.write(img)
        except:
            print "Unexpected error: ", sys.exec_info()[0]
    print('complete')
    cam.release()

def RecordTask():
    print('New record task')
    Record()
    print('Record done')
    CopyVideoToUSB()
    print('Copy done')

def CopyVideoToUSB():
    print('Copy video to USB')
    global videoName
    global localVideoPath
    
    usbDiskName = getDriveName()
    #destinationFile = '/media/1B0D-0A18__/'+ videoName + '.avi'
    destinationFile = '/media/'+ usbDiskName + '/' + videoName + '.avi'
    print localVideoPath
    print destinationFile
    shutil.move(localVideoPath, destinationFile)

if __name__ == '__main__':
    StartListening()

