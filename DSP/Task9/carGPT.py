import cv2
import imutils
import numpy as np
import pytesseract
from PIL import Image
pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe'

# Read the image
img = cv2.imread('car4.jpg', cv2.IMREAD_COLOR)

# Resize image for better performance
img = cv2.resize(img, (620, 480))

# Convert image to grayscale
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# Apply a bilateral filter to reduce noise while preserving edges
# Adjusted parameters for better noise reduction and edge preservation
gray = cv2.bilateralFilter(gray, d=9, sigmaColor=17, sigmaSpace=17)

# Perform edge detection (Canny)
edged = cv2.Canny(gray, 100, 200)

# Find contours in the edged image, keep only the largest ones, and initialize our screen contour
cnts = cv2.findContours(edged.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
cnts = imutils.grab_contours(cnts)
cnts = sorted(cnts, key=cv2.contourArea, reverse=True)[:10]
screenCnt = None

# Loop through the contours and check for a rectangle (license plate shape)
for c in cnts:
    peri = cv2.arcLength(c, True)
    approx = cv2.approxPolyDP(c, 0.02 * peri, True)  # Increased approximation accuracy
    
    # If the contour has four points, we assume it's the license plate
    if len(approx) == 4:
        screenCnt = approx
        break

# If a contour is found, proceed with masking the plate region
if screenCnt is not None:
    # Draw the contour for visualization (optional)
    cv2.drawContours(img, [screenCnt], -1, (0, 255, 0), 3)

    # Mask the region of the plate
    mask = np.zeros(gray.shape, np.uint8)
    new_image = cv2.drawContours(mask, [screenCnt], 0, 255, -1)
    new_image = cv2.bitwise_and(img, img, mask=mask)

    # Crop the region of interest (license plate)
    (x, y) = np.where(mask == 255)
    (topx, topy) = (np.min(x), np.min(y))
    (bottomx, bottomy) = (np.max(x), np.max(y))
    cropped_plate = gray[topx:bottomx + 1, topy:bottomy + 1]

    # Display cropped image (license plate region)
    cv2.imshow('Cropped Plate', cropped_plate)

    # Use Tesseract OCR to extract text from the cropped license plate region
    config = '--oem 3 --psm 8'  # Improved PSM value for single text line
    text = pytesseract.image_to_string(cropped_plate, config=config)
    print("Detected License Plate Number:", text.strip())

else:
    print("No license plate detected")

# Show the original image with detected contour
cv2.imshow('Detected License Plate', img)
cv2.waitKey(0)
cv2.destroyAllWindows()
