import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans

img = cv.imread('ORL/subset/1.pgm')

img = cv.cvtColor(img, cv.COLOR_BGR2RGB)

img = img.reshape(-1, 3)

kmeans = KMeans()
kmeans.fit(img)

