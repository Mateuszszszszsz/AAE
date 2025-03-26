import numpy as np
import matplotlib.pyplot as plt
from helpers import filter_images_and_labels, read_idx1, read_idx3
from latex_helpers import generate_latex_table
from PCA_algorithm import PCA
from sklearn.decomposition import KernelPCA
from sklearn.manifold import TSNE, LocallyLinearEmbedding
from sklearn.mixture import GaussianMixture
import time
from sklearn.metrics.cluster import rand_score

images = read_idx3("t10k-images-idx3-ubyte/t10k-images-idx3-ubyte")
labels = read_idx1("t10k-labels-idx1-ubyte/t10k-labels-idx1-ubyte")

desired_labels = [0, 1, 4]
filtered_images, filtered_labels = filter_images_and_labels(
    images, labels, desired_labels
)


def print_results(data, method, ax):
    symbols = ['.', 'x', '+']
    colors = ["#000", "#555", "#aaa"]
    for i, digit in enumerate([0, 1, 4]):
        mask = filtered_labels == digit
        ax.scatter(data[mask, 0], data[mask, 1], label=f"Digit {digit}", marker=symbols[i], c=colors[i])
    ax.set_title(f"2D PCA Projection of MNIST ({method})")
    ax.set_xlabel("PC1")
    ax.set_ylabel("PC2")
    ax.legend()
    ax.grid(True)


fig, ax = plt.subplots(2, 3)

prep_data = np.reshape(
    filtered_images,
    (len(filtered_images), filtered_images[0].shape[0] * filtered_images[0].shape[1]),
)


time_start = time.time()
PCA_data = PCA(filtered_images)
PCA_time = time.time() - time_start
print_results(PCA_data, "PCA", ax[0][0])

time_start = time.time()
gauss_data = KernelPCA(n_components=2, kernel="rbf").fit_transform(prep_data)
gauss_time = time.time() - time_start
print_results(gauss_data, "Gauss kernel KPCA", ax[0][1])

time_start = time.time()
poly2_data = KernelPCA(n_components=2, kernel="poly", degree=2).fit_transform(prep_data)
poly2_time = time.time() - time_start
print_results(poly2_data, "2nd degree polynomial kernel KPCA", ax[0][2])

time_start = time.time()
poly3_data = KernelPCA(n_components=2, kernel="poly", degree=3).fit_transform(prep_data)
poly3_time = time.time() - time_start
print_results(poly3_data, "3nd degree polynomial kernel KPCA", ax[1][0])

time_start = time.time()
tsne_data = TSNE(2).fit_transform(prep_data)
tsne_time = time.time() - time_start
print_results(tsne_data, "t-SNE", ax[1][1])

time_start = time.time()
lle_data = LocallyLinearEmbedding(n_components=2).fit_transform(prep_data)
lle_time = time.time() - time_start
print_results(lle_data, "Locally Linear Embedding", ax[1][2])

plt.show()

RUN_CLUSTERING = True

if not RUN_CLUSTERING:
    exit()

clustering = GaussianMixture(n_components=3)
clustering_results = {
  "PCA": clustering.fit_predict(PCA_data),
  "Gauss": clustering.fit_predict(gauss_data),
  "poly2": clustering.fit_predict(poly2_data),
  "poly3": clustering.fit_predict(poly3_data),
  "tsne": clustering.fit_predict(tsne_data),
  "lle": clustering.fit_predict(lle_data),
}

clustering_RI = {
    "PCA":   {"RI": rand_score(filtered_labels, clustering_results["PCA"]), "time": PCA_time},
    "Gauss": {"RI": rand_score(filtered_labels, clustering_results["Gauss"]), "time": gauss_time},
    "poly2": {"RI": rand_score(filtered_labels, clustering_results["poly2"]), "time": poly2_time},
    "poly3": {"RI": rand_score(filtered_labels, clustering_results["poly3"]), "time": poly3_time},
    "tsne":  {"RI": rand_score(filtered_labels, clustering_results["tsne"]), "time": tsne_time},
    "lle":   {"RI": rand_score(filtered_labels, clustering_results["lle"]), "time": lle_time},
}

metrics = ["RI", "time"]
algorithms = ["PCA", "Gauss", "poly2", "poly3", "tsne", "lle"]

print(generate_latex_table(clustering_RI, metrics, algorithms, "RI scores and times of execution of PCA methods", "task1_scores"))



# print(f"PCA: {clustering_RI["PCA"]}")`
# print(f"Gauss: {clustering_RI["Gauss"]}")
# print(f"poly2: {clustering_RI["poly2"]}")
# print(f"poly3: {clustering_RI["poly3"]}")
# print(f"tsne: {clustering_RI["tsne"]}")
# print(f"lle: {clustering_RI["lle"]}")`