import cv2

# Load the cascade
face_cascade = cv2.CascadeClassifier(
    cv2.data.haarcascades + "haarcascade_frontalface_default.xml"
)

# To capture video from webcam.
# cap = cv2.VideoCapture(0)
# To use a video file as input
# cap = cv2.VideoCapture('filename.mp4')
cap = cv2.VideoCapture("testAI.jpg")

while True:
    # Read the frame
    _, img = cap.read()
    # Convert to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # Detect the faces
    faces = face_cascade.detectMultiScale(gray, 1.1, 4)
    # Draw the rectangle around each face
    for x, y, w, h in faces:
        cv2.putText(img, f"{x}, {y}", (x, y - 2), fontFace=1, fontScale=1, color=(0, 0, 255))
        # cv2.addText(img, f"({x}, {y}", (x, y - 2), nameFont="Arial")
        cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)
    # Display
    cv2.imshow("img", img)

    # Stop if escape key is pressed
    k = cv2.waitKey(30) & 0xFF
    if k == 27:
        break

    # Uncomment to stop for still image
    while True:
      pass
# Release the VideoCapture object
cap.release()
