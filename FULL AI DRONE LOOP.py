import cv2
from ultralytics import YOLO
import paho.mqtt.client as mqtt

model = YOLO("yolov8n.pt")

client = mqtt.Client()
client.connect("192.168.1.10", 1883)

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    results = model(frame)

    classes = []
    for r in results:
        for c in r.boxes.cls:
            classes.append(model.names[int(c)])

    print("Detected:", classes)

    # 🚨 DECISION LOGIC
    if "person" in classes:
        print("ALERT → sending STOP")
        client.publish("drone/control", "STOP")

    elif "car" in classes:
        print("TRACK → moving forward")
        client.publish("drone/control", "FORWARD")

    else:
        client.publish("drone/control", "HOVER")

    cv2.imshow("Drone AI", results[0].plot())

    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
