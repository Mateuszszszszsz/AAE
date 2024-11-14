import cv2
import numpy as np
import matplotlib.pyplot as plt
from sklearn.mixture import GaussianMixture

# Load and preprocess the image
image_path = 'image.jpg'
image = cv2.imread(image_path)
image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
pixels = image.reshape((-1, 3))

# Apply GMM
k = 2
gmm = GaussianMixture(n_components=k, random_state=42)
labels = gmm.fit_predict(pixels)
centers = np.uint8(gmm.means_)
segmented_image = centers[labels].reshape(image.shape)

# Display results
plt.figure()
plt.imshow(segmented_image)
plt.title("Gaussian Mixture Model")
plt.axis('off')
plt.show()
