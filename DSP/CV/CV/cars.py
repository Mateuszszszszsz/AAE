import cv2
import imutils
import numpy as np

CAR_NR = 3

img = cv2.imread(f"car{CAR_NR}.jpg", cv2.IMREAD_COLOR)
cv2.imshow("Original", img)
cv2.waitKey(0)


match CAR_NR:
    case 1 | 3:
        pass
    case 2:
        img = cv2.convertScaleAbs(img, alpha=2.8, beta=0.0)
        cv2.imshow("Contrast changed", img)
        cv2.waitKey(0)
    case 4:
        img = cv2.convertScaleAbs(img, alpha=0.4, beta=0.0)
        cv2.imshow("Contrast changed", img)
        cv2.waitKey(0)


img = cv2.resize(img, (620, 480))
cv2.imshow("Resized", img)
cv2.waitKey(0)


gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  # convert to grey scale
cv2.imshow("Grayscale", gray)
cv2.waitKey(0)


match CAR_NR:
    case 1 | 2:
        gray = cv2.bilateralFilter(gray, 11, 17, 17)  # Blur to reduce noise
    case 3:
        gray = cv2.bilateralFilter(gray, 11, 17, 17)
    case 4:
        gray = cv2.bilateralFilter(gray, 4, 17, 17)

cv2.imshow("Blurred", gray)
cv2.waitKey(0)


match CAR_NR:  # Perform Edge detection
    case 1:
        edged = cv2.Canny(gray, 30, 200)
    case 2:
        edged = cv2.Canny(gray, 300, 200)
    case 3:
        edged = cv2.Canny(gray, 100, 100)
    case 4:
        edged = cv2.Canny(gray, 30, 120)

cv2.imshow("Edges", edged)
cv2.waitKey(0)


if CAR_NR == 3:
    # **Apply morphological operations to close gaps in edges**
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (1, 3))
    edged = cv2.morphologyEx(edged, cv2.MORPH_CLOSE, kernel)
    edged = cv2.dilate(edged, kernel, iterations=2)  # Further strengthen the edges
    cv2.imshow("Dilated", edged)
    cv2.waitKey(0)


# find contours in the edged image, keep only the largest
# ones, and initialize our screen contour
cnts = cv2.findContours(edged.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
cnts = imutils.grab_contours(cnts)
cnts = sorted(cnts, key=cv2.contourArea, reverse=True)[:10]
screenCnt = None


match CAR_NR:
    case 1:
        peri_factor = 0.017
    case 2:
        peri_factor = 0.017
    case 3:
        peri_factor = 0.08
    case 4:
        peri_factor = 0.08

# loop over our contours
for c in cnts:
    # approximate the contour
    peri = cv2.arcLength(c, True)
    approx = cv2.approxPolyDP(c, peri_factor * peri, CAR_NR in [1, 2, 3])

    # if our approximated contour has four points, then
    # we can assume that we have found our screen
    if len(approx) == 4 and cv2.isContourConvex(approx):
        screenCnt = approx
        break

if screenCnt is None:
    detected = 0
    print("No contour detected")
else:
    detected = 1

if detected == 1:
    cv2.drawContours(img, [screenCnt], -1, (0, 255, 0), 3)

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
