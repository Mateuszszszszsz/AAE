import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans

img = cv.imread('ORL/subset/1.pgm')

print(img.shape)
cv.kmeans(img, 5)

# kmeans = KMeans()
# kmeans.fit(img)