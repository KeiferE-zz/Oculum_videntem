# import packages
from time import sleep
from collections import deque
from matplotlib import pyplot as plt
import numpy as np
import argparse
import cv2


cap = cv2.VideoCapture(0)
cap.set(3,1280)
cap.set(4,1024)
cap.set(15, -8.0)

# define the list of boundaries
boundaries = [
	([10, 10, 1], [36, 70, 255])
]



while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
    imgray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
    r,t = cv2.threshold(imgray,127,255,0)
    im2, contours, hierarchy = cv2.findContours(t,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    sleep(0.1)
    
    for(lower, upper) in boundaries:
        lower = np.array(lower, dtype = "uint8")
        upper = np.array(upper, dtype = "uint8") 

        mask = cv2.inRange(frame, lower, upper)
        output = cv2.bitwise_and(frame, frame, mask = mask)

        ret,thresh = cv2.threshold(mask, 40, 255, 0)
        im2,contours,hierarchy = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        #contours
        if len(contours) != 0:
            # draw in blue the contours that were founded
            cv2.drawContours(output, contours, 3, (0,255,0), 3)

            #find the biggest area
            c = max(contours, key = cv2.contourArea)

            x,y,w,h = cv2.boundingRect(c)
            # draw the book contour (in green)
            cv2.rectangle(output,(x,y),(x+w,y+h),(0,255,0),2)

        #histogram pixel color density
        hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
        hist = cv2.calcHist( [hsv], [0, 1], None, [180, 256], [0, 180, 0, 256] )

            
    # Display the resulting frame
    cv2.imshow('frame', np.hstack([frame, output]))
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()