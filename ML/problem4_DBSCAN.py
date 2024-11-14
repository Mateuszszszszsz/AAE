import cv2
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import DBSCAN

# Load and preprocess the image
image_path = 'image.jpg'
image = cv2.imread(image_path)
image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
pixels = image.reshape((-1, 3))

# Apply DBSCAN
dbscan = DBSCAN(eps=3).fit(pixels) #changing of the eps gives some differences MS
labels = dbscan.labels_
unique_labels = np.unique(labels)
centers = np.array([pixels[labels == label].mean(axis=0) for label in unique_labels if label != -1]).astype(np.uint8)

# Map labels to colors
segmented_image = np.zeros_like(pixels)
for label in unique_labels:
    if label != -1:
        segmented_image[labels == label] = centers[label]
segmented_image = segmented_image.reshape(image.shape)

# Display results
plt.figure()
plt.imshow(segmented_image)
plt.title("DBSCAN Clustering")
plt.axis('off')
plt.show()
