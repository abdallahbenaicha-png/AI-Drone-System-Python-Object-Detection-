from ultralytics import YOLO
import cv2

model = YOLO("yolov8n.pt")

def analyze(frame):
    results = model(frame)

    objects = []

    for r in results:
        for c in r.boxes.cls:
            objects.append(model.names[int(c)])

    if "person" in objects:
        return "ALERT"
    elif "car" in objects:
        return "TRACK"
    else:
        return "SAFE"
