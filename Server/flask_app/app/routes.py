from flask import request
from werkzeug.datastructures import FileStorage

from app import app
import numpy as np
import cv2
import os
import uuid


def save_img(img):
    img_dir = "esp32_images"
    if not os.path.isdir(img_dir):
        os.mkdir(img_dir)
    cv2.imwrite(os.path.join(img_dir, "img_" + str(uuid.uuid1()) + ".jpg"), img)


@app.route('/')
@app.route('/index', methods=['GET'])
def index():
    return "Hello From Spring ESP32 Image Server", 200


@app.route('/upload', methods=['POST', 'GET'])
def upload():
    received = request
    if received.files:
        print(received.files['image'])
        # convert string of image data to uint8
        file: FileStorage = received.files['image']
        np_arr = np.fromstring(file.read(), np.uint8)
        # decode image
        img = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)
        save_img(img)

        return "[SUCCESS] Image Received", 201
    else:
        return "[FAILED] Image Not Received", 404
