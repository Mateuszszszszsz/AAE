import cv2

# Load the cascade
face_cascade = cv2.CascadeClassifier(
    cv2.data.haarcascades + "haarcascade_frontalface_default.xml"
)

# To capture video from webcam.
cap = cv2.VideoCapture(0)
# To use a video file as input
# cap = cv2.VideoCapture('filename.mp4')
# cap = cv2.VideoCapture("testAI.jpg")

while True:
    # Read the frame
    _, img = cap.read()
    # Convert to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # Detect the faces
    faces = face_cascade.detectMultiScale(gray, 1.1, 4)
    # Draw the rectangle around each face
    for x, y, w, h in faces:
        cv2.putText(img, f"{x}, {y}", (x, y - 2), fontFace=2, fontScale=1.5, color=(255, 0, 0))
        # cv2.addText(img, f"({x}, {y}", (x, y - 2), nameFont="Arial")
        cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)
    # Display
    cv2.imshow("img", img)

    # Stop if escape key is pressed
    k = cv2.waitKey(30) & 0xFF
    if k == 27:
        break
# Release the VideoCapture object
cap.release()
