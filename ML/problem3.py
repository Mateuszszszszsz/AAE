from PIL import Image
import os
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans

images =[]

for filename in os.listdir("ORL/subset"):
  img_path = os.path.join("ORL/subset", filename)
  image = Image.open(img_path)
  images.append(np.array(image).flatten())


images = np.array(images)

n_clusters = 5
kmeans = KMeans(n_clusters=n_clusters)
kmeans.fit(images)

print(kmeans.labels_)
n_images = 10
fig, axes = plt.subplots(1, n_images, figsize=(15, 5))

for ax, img, label in zip(axes, images, kmeans.labels_):
    ax.imshow(img.reshape(112, 92), cmap='gray')  # Adjust shape as necessary
    ax.set_title(f'{label}')
    ax.axis('off')

plt.show()