import numpy as np
import matplotlib.pyplot as plt
import sklearn.cluster as cluster
from sklearn_extra.cluster import KMedoids
import skfuzzy as fuzzy

mean_1 = [2, 3]
cov_1 = [[1, 1.5], [1.5, 3]]

mean_2 = [-5, 10]
cov_2 = [[2, -1], [-1, 1]]

mean_3 = [-8, 2]
cov_3 = [[1, 0], [0, 3]]

x_1, y_1 = np.random.multivariate_normal(mean_1, cov_1, 1000).T
x_2, y_2 = np.random.multivariate_normal(mean_2, cov_2, 1000).T
x_3, y_3 = np.random.multivariate_normal(mean_3, cov_3, 1000).T

plt.plot(x_1, y_1, 'x')
plt.plot(x_2, y_2, 'x')
plt.plot(x_3, y_3, 'x')
plt.show()

x_mixed = np.concatenate([x_1, x_2, x_3])
y_mixed = np.concatenate([y_1, y_2, y_3])

mixed = np.array(list(zip(x_mixed, y_mixed)))

# Clustering
fig, ax = plt.subplots(3, 2)

# K-Means clustering
kmeans = cluster.KMeans(n_clusters=3)
kmeans.fit(mixed)

ax[0][0].set_title("K-Means clustering")
ax[0][0].scatter(x_mixed, y_mixed, c=kmeans.labels_)


# K-Medoids clustering
kmedoids = KMedoids(n_clusters=3)
kmedoids.fit(mixed)

ax[0][1].set_title("K-Medoids clustering")
ax[0][1].scatter(x_mixed, y_mixed, c=kmedoids.labels_)


# Fuzzy C-Means clustering
cntr, u, _, _, _, _, _ = fuzzy.cmeans(mixed.T, 3, m=2, error=0.005, maxiter=1000)
cmeans_labels = np.argmax(u, axis=0)

ax[1][0].set_title("Fuzzy C-Means clustering")
ax[1][0].scatter(x_mixed, y_mixed, c=cmeans_labels, cmap='viridis')


# Spectral
spectral_clustering = cluster.SpectralClustering(n_clusters=3)
spectral_clustering.fit(mixed)

ax[1][1].set_title("Spectral clustering")
ax[1][1].scatter(x_mixed, y_mixed, c=spectral_clustering.labels_)


# DBSCAN
dbscan = cluster.DBSCAN()
dbscan.fit(mixed)

ax[2][0].set_title("DBSCAN clustering")
ax[2][0].scatter(x_mixed, y_mixed, c=dbscan.labels_)

#

# Display plots
plt.show()