import cv2
import imutils
import numpy as np
import pytesseract
from PIL import Image

img = cv2.imread("car4.jpg", cv2.IMREAD_COLOR)
img_copy = img


img = cv2.convertScaleAbs(img, alpha=0.4, beta=0.1)

img = cv2.resize(img, (620, 480))

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # convert to grey scale

img = cv2.resize(img_copy, (620, 480))

gray = cv2.bilateralFilter(gray, 4, 17, 17)  # Blur to reduce noise

edged = cv2.Canny(gray, 30, 120)  # Perform Edge detection

cv2.imshow("debug_gray.jpg", gray)
cv2.imshow("debug_edge", edged)
cv2.waitKey(0)
cv2.destroyAllWindows()

# find contours in the edged image, keep only the largest
# ones, and initialize our screen contour
cnts = cv2.findContours(edged.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
cnts = imutils.grab_contours(cnts)
cnts = sorted(cnts, key=cv2.contourArea, reverse=True)[:10]
screenCnt = None

# loop over our contours
for c in cnts:
    # approximate the contour
    peri = cv2.arcLength(c, True)
    print(c)
    approx = cv2.approxPolyDP(c, 0.08 * peri, False)


    # if our approximated contour has four points, then
    # we can assume that we have found our screen
    if len(approx) == 4 and cv2.isContourConvex(approx):
        print(approx)
        screenCnt = approx
        break
if screenCnt is None:
    detected = 0
    print("No contour detected")
else:
    detected = 1

if detected == 1:
    cv2.drawContours(img, [screenCnt], -1, (0, 255, 0), 3)
    cv2.drawContours(edged, [screenCnt], -1, (0, 255, 0), 3)
    cv2.imshow("debug_edge", edged)


# Masking the part other than the number plate
mask = np.zeros(gray.shape, np.uint8)
new_image = cv2.drawContours(
    mask,
    [screenCnt],
    0,
    255,
    -1,
)
new_image = cv2.bitwise_and(img, img, mask=mask)

# Now crop
(x, y) = np.where(mask == 255)
(topx, topy) = (np.min(x), np.min(y))
(bottomx, bottomy) = (np.max(x), np.max(y))
Cropped = gray[topx : bottomx + 1, topy : bottomy + 1]

cv2.imshow("image", img)
cv2.imshow("Cropped", Cropped)

cv2.waitKey(0)
cv2.destroyAllWindows()