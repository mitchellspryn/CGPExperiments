# A quick script to label the bounding boxes for the card dataset

import numpy as np
import cv2

import sys
import os

last_clicked_point = None
start_x = 0
start_y = 0
pixel_step = 5

def process_click(event, x, y, flags, param):
    global last_clicked_point
    if (event == cv2.EVENT_LBUTTONDOWN):
        last_clicked_point = (x, y)

def wait_for_click():
    global start_x
    global start_y
    global pixel_step
    global last_clicked_point
    while True:
        key = cv2.waitKey(10)
        if (last_clicked_point is not None):
            return True
        elif (key == ord('r')):
            return False
        elif (key == ord('w')):
            start_y = max(0, start_y - pixel_step)
            return False
        elif (key == ord('s')):
            start_y = min(3500, start_y + pixel_step)
            return False
        elif (key == ord('a')):
            start_x = max(0, start_x - pixel_step)
            return False
        elif (key == ord('d')):
            start_x = min(2500, start_x + pixel_step)
            return False

def main():
    global start_x
    global start_y
    global last_clicked_point
    image_number = int(sys.argv[1])
    image = cv2.imread('{0}.jpg'.format(image_number))
    cv2.namedWindow('image')

    cv2.setMouseCallback('image', process_click)

    done = False
    out_width = 500
    out_height = 500
    while (not done):
        min_x = min(start_x, image.shape[1] - out_width)
        max_x = min_x + out_width
        min_y = min(start_y, image.shape[0] - out_height)
        max_y = min_y + out_height

        crop_image = image[min_y:max_y, min_x:max_x]
        cv2.imshow('image', crop_image)

        rect_points = []
        should_restart = False
        for i in range(0, 4, 1):
            last_clicked_point = None
            if not wait_for_click():
                should_restart = True
                break

            rect_points.append(last_clicked_point)
            draw_image = crop_image.copy()
            for rect_point in rect_points:
                draw_image = cv2.circle(draw_image, rect_point, 3, (0, 255, 0), -1)

            cv2.imshow('image', draw_image)

        if (should_restart):
            continue

        labels = np.zeros((out_height, out_width), dtype=np.uint8)
        points_array = np.array([[p[0], p[1]] for p in rect_points], dtype=np.int32)
        labels = cv2.fillConvexPoly(labels, points_array, (255, 255, 255))

        out_image_path = 'processed/{0}.png'.format(image_number)
        out_label_path = 'processed/{0}_label.png'.format(image_number)

        cv2.imwrite(out_image_path, crop_image)
        cv2.imwrite(out_label_path, labels)
        
        done = True

if __name__ == '__main__':
    main()





