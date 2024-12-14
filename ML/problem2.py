import time
import numpy as np
import matplotlib.pyplot as plt
import sklearn.cluster as cluster
import sklearn.mixture as mixture
import skfuzzy as fuzzy

from sklearn.metrics.cluster import rand_score
from sklearn.metrics.cluster import normalized_mutual_info_score
from sklearn.metrics import silhouette_score

from plot_helpers import plot_silhouette
from latex_helpers import generate_latex_table

mean_1 = [0, 4]
cov_1 = [[0.85, 1], [1, 1.2]]

mean_2 = [-5, 10]
cov_2 = [[6, -0.5], [-0.5, 0.1]]

mean_3 = [-8, 2]
cov_3 = [[1, 0], [0, 2]]

x_1, y_1 = np.random.multivariate_normal(mean_1, cov_1, 100).T
x_2, y_2 = np.random.multivariate_normal(mean_2, cov_2, 1000).T
x_3, y_3 = np.random.multivariate_normal(mean_3, cov_3, 100).T

trueLabels = np.concatenate([[0] * 100, [1] * 1000, [2] * 100])

plt.plot(x_1, y_1, 'x')
plt.plot(x_2, y_2, 'x')
plt.plot(x_3, y_3, 'x')
plt.show()

x_mixed = np.concatenate([x_1, x_2, x_3])
y_mixed = np.concatenate([y_1, y_2, y_3])

mixed = np.array(list(zip(x_mixed, y_mixed)))

# Clustering
fig, ax = plt.subplots(2, 3)
fig_silhouette, ax_silhouette = plt.subplots(2, 3)


ax[0][0].set_title("Original clusters")
ax[0][0].scatter(x_mixed, y_mixed, c=trueLabels, edgecolors="black")
ax_silhouette[0][0].set_title("Original clusters")
plot_silhouette(mixed, trueLabels, 3, ax=ax_silhouette[0][0])

# K-Means clustering
kmeans = cluster.KMeans(n_clusters=3)
start = time.time()
kmeans.fit(mixed)
end = time.time()

kmeans_time = (end - start)

ax[0][1].set_title("K-Means clustering")
ax[0][1].scatter(x_mixed, y_mixed, c=kmeans.labels_, edgecolors="black")
ax_silhouette[0][1].set_title("K-Means clustering")
plot_silhouette(mixed, kmeans.labels_, 3, ax=ax_silhouette[0][1])


# K-Medoids clustering
# kmedoids = KMedoids(n_clusters=3)
# start = time.time()
# kmedoids.fit(mixed)
# end = time.time()
# kmedoids_time = (end - start)

# ax[0][1].set_title("K-Medoids clustering")
# ax[0][1].scatter(x_mixed, y_mixed, c=kmedoids.labels_)


# Fuzzy C-Means clustering
start = time.time()
cntr, u, _, _, _, _, _ = fuzzy.cmeans(mixed.T, 3, m=2, error=0.005, maxiter=1000)
end = time.time()
fuzzy_cmeans_time = (end - start)
cmeans_labels = np.argmax(u, axis=0)

ax[0][2].set_title("Fuzzy C-Means clustering")
ax[0][2].scatter(x_mixed, y_mixed, c=cmeans_labels, cmap='viridis', edgecolors="black")
ax_silhouette[0][2].set_title("Fuzzy C-Means clustering")
plot_silhouette(mixed, cmeans_labels, 3, ax=ax_silhouette[0][2])

# Spectral
spectral_clustering = cluster.SpectralClustering(n_clusters=3)
start = time.time()
spectral_clustering.fit(mixed)
end = time.time()
spectral_clustering_time = (end - start)

ax[1][0].set_title("Spectral clustering")
ax[1][0].scatter(x_mixed, y_mixed, c=spectral_clustering.labels_, edgecolors="black")
ax_silhouette[1][0].set_title("Spectral clustering")
plot_silhouette(mixed, spectral_clustering.labels_, 3, ax=ax_silhouette[1][0])

# DBSCAN
dbscan = cluster.DBSCAN()
start = time.time()
dbscan.fit(mixed)
end = time.time()
dbscan_time = (end - start)

ax[1][1].set_title("DBSCAN clustering")
ax[1][1].scatter(x_mixed, y_mixed, c=dbscan.labels_, edgecolors="black")
ax_silhouette[1][1].set_title("DBSCAN clustering")
plot_silhouette(mixed, dbscan.labels_, 3, ax=ax_silhouette[1][1])

# GMM
gmm = mixture.GaussianMixture(n_components=3)
start = time.time()
gmm.fit(mixed)
end = time.time()
gmm_time = (end - start)

ax[1][2].set_title("GMM clustering")
ax[1][2].scatter(x_mixed, y_mixed, c=gmm.predict(mixed), edgecolors="black")
ax_silhouette[1][2].set_title("GMM clustering")
plot_silhouette(mixed, gmm.predict(mixed), 3, ax=ax_silhouette[1][2])

print(f"K-Means time: {kmeans_time}")
# print(f"K-Medoids time: {kmedoids_time}")
print(f"Fuzzy c-means time: {fuzzy_cmeans_time}")
print(f"Spectral clustering: {spectral_clustering_time}")
print(f"DBSCAN time: {dbscan_time}")
print(f"GMM time: {gmm_time}")



results = {
    "K-Means": {
        "time": kmeans_time,
        "RI": rand_score(trueLabels, kmeans.labels_),
        "NMI": normalized_mutual_info_score(trueLabels, kmeans.labels_),
        "SS": silhouette_score(mixed, kmeans.labels_),
    },
    "Fuzzy C-Means": {
        "time": fuzzy_cmeans_time,
        "RI": rand_score(trueLabels, cmeans_labels),
        "NMI": normalized_mutual_info_score(trueLabels, cmeans_labels),
        "SS": silhouette_score(mixed, cmeans_labels),
    },
    "Spectral Clustering": {
        "time": spectral_clustering_time,
        "RI": rand_score(trueLabels, spectral_clustering.labels_),
        "NMI": normalized_mutual_info_score(trueLabels, spectral_clustering.labels_),
        "SS": silhouette_score(mixed, spectral_clustering.labels_),
    },
    "DBSCAN": {
        "time": dbscan_time,
        "RI": rand_score(trueLabels, dbscan.labels_),
        "NMI": normalized_mutual_info_score(trueLabels, dbscan.labels_),
        "SS": silhouette_score(mixed, dbscan.labels_),
    },
    "GMM": {
        "time": gmm_time,
        "RI": rand_score(trueLabels, gmm.predict(mixed)),
        "NMI": normalized_mutual_info_score(trueLabels, gmm.predict(mixed)),
        "SS": silhouette_score(mixed, gmm.predict(mixed)),
    },
}


# Display plots
plt.show()

metrics = ["time", "RI", "NMI", "SS"]
algorithms = ["K-Means", "Fuzzy C-Means", "Spectral Clustering", "DBSCAN", "GMM"]
print(generate_latex_table(results, metrics, algorithms))



# Display plots
plt.show()