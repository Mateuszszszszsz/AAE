import cv2
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans

# Load and preprocess the image
image_path = 'image.jpg'  # Replace with your image path
image = cv2.imread(image_path)
image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
pixels = image.reshape((-1, 3)).astype(np.float32)

# Apply K-Means
k = 2
kmeans = KMeans(n_clusters=k, random_state=42).fit(pixels)
labels = kmeans.labels_
centers = np.uint8(kmeans.cluster_centers_)
segmented_image = centers[labels.flatten()].reshape(image.shape)

# Display results
plt.figure()
plt.imshow(segmented_image)
plt.title("K-Means Clustering")
plt.axis('off')
plt.show()
