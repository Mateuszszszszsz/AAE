import cv2
import numpy as np
import matplotlib.pyplot as plt
from pyclustering.cluster.kmedoids import kmedoids
from pyclustering.utils import calculate_distance_matrix

# Load and preprocess the image
image_path = 'image.jpg'  # Replace with your image path
image = cv2.imread(image_path)
image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
pixels = image.reshape((-1, 3))

# Normalize the data to avoid numerical overflow
pixels = pixels / 255.0  # Normalize to the range [0, 1]

# Compute distance matrix
distance_matrix = calculate_distance_matrix(pixels)

# Apply K-Medoids
k = 2
initial_medoids = [0, len(pixels)//3, 2*len(pixels)//3]  # Choose initial medoids
kmedoids_instance = kmedoids(distance_matrix, initial_medoids)
kmedoids_instance.process()
clusters = kmedoids_instance.get_clusters()

# Map cluster labels to pixel colors
labels = np.zeros(len(pixels))
for cluster_idx, cluster in enumerate(clusters):
    for pixel_idx in cluster:
        labels[pixel_idx] = cluster_idx
centers = np.array([pixels[cluster].mean(axis=0) for cluster in clusters]) * 255  # Denormalize
centers = np.uint8(centers)
segmented_image = centers[labels.astype(int)].reshape(image.shape)

# Display results
plt.figure()
plt.imshow(segmented_image)
plt.title("K-Medoids Clustering (Normalized)")
plt.axis('off')
plt.show()
