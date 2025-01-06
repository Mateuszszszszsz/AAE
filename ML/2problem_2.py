import os
import numpy as np
from sklearn.decomposition import PCA, KernelPCA
from skimage.io import imread
from skimage.transform import resize

# Path to the ORL database
orl_path = 'ORL'

# Parameters
selected_subjects = [f"s{i}" for i in range(1, 11)]  # Select 10 subjects
image_shape = (112, 92)  # Assuming ORL images are 112x92 pixels
j_values = [9, 25, 64]  # Dimensionality to reduce to

# Load the images
def load_orl_images(base_path, subjects):
    images = []
    labels = []
    for label, subject in enumerate(subjects):
        subject_path = os.path.join(base_path, subject)
        for filename in os.listdir(subject_path):
            if filename.endswith('.pgm'):
                image_path = os.path.join(subject_path, filename)
                image = imread(image_path, as_gray=True)
                image = resize(image, image_shape, anti_aliasing=True).flatten()
                images.append(image)
                labels.append(label)
    return np.array(images), np.array(labels)

images, labels = load_orl_images(orl_path, selected_subjects)

# Function to apply PCA and Kernel PCA
def apply_dimensionality_reduction(images, method="PCA", kernel=None):
    results = {}
    for j in j_values:
        if method == "PCA":
            model = PCA(n_components=j)
        elif method == "KPCA":
            model = KernelPCA(n_components=j, kernel=kernel)
        else:
            raise ValueError("Unknown method")
        reduced_images = model.fit_transform(images)
        results[j] = reduced_images
    return results

# Apply PCA
pca_results = apply_dimensionality_reduction(images, method="PCA")

# Optionally, apply Kernel PCA with RBF kernel
kpca_results = apply_dimensionality_reduction(images, method="KPCA", kernel="rbf")

# Display results
for j in j_values:
    print(f"PCA with J={j}: Shape={pca_results[j].shape}")
    print(f"Kernel PCA with J={j}: Shape={kpca_results[j].shape}")
