import cv2

# Load the cascade
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades +  'haarcascade_frontalface_default.xml')

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
    text_y_position = 25  # Starting Y position for the coordinates
    for (x, y, w, h) in faces:
        cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2)
        coord_text = f"({x}, {y}) to ({x+w}, {y+h})"
        cv2.putText(img, coord_text, (10, text_y_position), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
        text_y_position += 25
    # Display
    cv2.imshow('img', img)
    cv2.imwrite("detected_faces_output.jpg", img)
    # Stop if escape key is pressed
    k = cv2.waitKey(30) & 0xff
    if k==27:
        break
# Release the VideoCapture object
cap.release()