import cv2
import imutils
import numpy as np
from pytesseract import image_to_string
from PIL import Image

# Load the image
img = cv2.imread('car3.jpg', cv2.IMREAD_COLOR)
img = cv2.convertScaleAbs(img, alpha=1.5, beta=50.0)  # Adjust contrast and brightness
img = cv2.resize(img, (620, 480))

# Convert to grayscale
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# Apply bilateral filter to reduce noise while preserving edges
gray = cv2.bilateralFilter(gray, 11, 17, 17)

# Canny edge detection
median_val = np.median(gray)
lower = int(max(0, 0.67 * median_val))
upper = int(min(255, 1.33 * median_val))
edged = cv2.Canny(gray, lower, upper)

# **Apply morphological operations to close gaps in edges**
kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))
edged = cv2.morphologyEx(edged, cv2.MORPH_CLOSE, kernel)
edged = cv2.dilate(edged, kernel, iterations=2)  # Further strengthen the edges

# Debug: Show the edge-detected image
cv2.imshow("Debug - Edges", edged)
cv2.waitKey(0)
cv2.destroyAllWindows()

# Find contours
cnts = cv2.findContours(edged.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
cnts = imutils.grab_contours(cnts)
cnts = sorted(cnts, key=cv2.contourArea, reverse=True)[:10]

screenCnt = None

# Loop over the contours to find a quadrilateral
for c in cnts:
    peri = cv2.arcLength(c, True)
    approx = cv2.approxPolyDP(c, 0.02 * peri, True)  # Adjusted approximation factor
    if len(approx) == 4:
        screenCnt = approx
        break

if screenCnt is None:
    detected = 0
    print("No contour detected")
else:
    detected = 1
    cv2.drawContours(img, [screenCnt], -1, (0, 255, 0), 3)

# Masking the part other than the number plate
if detected == 1:
    mask = np.zeros(gray.shape, np.uint8)
    new_image = cv2.drawContours(mask, [screenCnt], 0, 255, -1)
    new_image = cv2.bitwise_and(img, img, mask=mask)

    # Crop the region containing the plate
    (x, y) = np.where(mask == 255)
    (topx, topy) = (np.min(x), np.min(y))
    (bottomx, bottomy) = (np.max(x), np.max(y))
    Cropped = gray[topx:bottomx+1, topy:bottomy+1]

    # Apply thresholding for better OCR
    _, Cropped = cv2.threshold(Cropped, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)

    # Display the result
    cv2.imshow('Cropped', Cropped)
    cv2.waitKey(0)

cv2.imshow('Detected Plate', img)
cv2.waitKey(0)
cv2.destroyAllWindows()
