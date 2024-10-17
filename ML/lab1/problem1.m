clear;
close all;
clc;

mean_1 = [2, 3];
cov_1 = [1, 1.5; 1.5, 3];

mean_2 = [-5, 10];
cov_2 = [2, -1; -1, 1];

mean_3 = [-8, 2];
cov_3 = [1, 0; 0, 3];

rng('default');
data_1 = mvnrnd(mean_1, cov_1, 1000);
data_2 = mvnrnd(mean_2, cov_2, 1000);
data_3 = mvnrnd(mean_3, cov_3, 1000);

mixed_data = [data_1; data_2; data_3];

true_labels = [ones(1000, 1); 2 * ones(1000, 1); 3 * ones(1000, 1)];

nmi_values = zeros(5, 1);
ss_values = zeros(5, 1);
ri_values = zeros(5, 1);
figure;

subplot(3, 2, 1);
[idx_kmeans, C_kmeans] = kmeans(mixed_data, 3);
gscatter(mixed_data(:,1), mixed_data(:,2), idx_kmeans);
title('K-Means Clustering');
ss_kmeans = silhouette(mixed_data, idx_kmeans);
title('Silhouette Plot - K-Means');

nmi_values(1) = normalized_mutual_info(true_labels, idx_kmeans);
ss_values(1) = mean(ss_kmeans);
ri_values(1) = rand_index(true_labels, idx_kmeans);

subplot(3, 2, 2);
[idx_kmedoids, C_kmedoids] = kmedoids(mixed_data, 3);
gscatter(mixed_data(:,1), mixed_data(:,2), idx_kmedoids);
title('K-Medoids Clustering');
ss_kmedoids = silhouette(mixed_data, idx_kmedoids);
title('Silhouette Plot - K-Medoids');

nmi_values(2) = normalized_mutual_info(true_labels, idx_kmedoids);
ss_values(2) = mean(ss_kmedoids);
ri_values(2) = rand_index(true_labels, idx_kmedoids);

subplot(3, 2, 3);
[centers, U, ~] = fcm(mixed_data, 3);
[~, idx_fuzzy] = max(U);
gscatter(mixed_data(:,1), mixed_data(:,2), idx_fuzzy);
title('Fuzzy C-Means Clustering');
ss_fuzzy = silhouette(mixed_data, idx_fuzzy);
title('Silhouette Plot - Fuzzy C-Means');

nmi_values(3) = normalized_mutual_info(true_labels, idx_fuzzy);
ss_values(3) = mean(ss_fuzzy);
ri_values(3) = rand_index(true_labels, idx_fuzzy);

subplot(3, 2, 4);
epsilon = 0.5;
MinPts = 5;
idx_dbscan = dbscan(mixed_data, epsilon, MinPts);
gscatter(mixed_data(:,1), mixed_data(:,2), idx_dbscan);
title('DBSCAN Clustering');
ss_dbscan = silhouette(mixed_data, idx_dbscan);
title('Silhouette Plot - DBSCAN');

nmi_values(4) = normalized_mutual_info(true_labels, idx_dbscan);
ss_values(4) = mean(ss_dbscan);
ri_values(4) = rand_index(true_labels, idx_dbscan);

subplot(3, 2, 5);
gmm_model = fitgmdist(mixed_data, 3);
idx_gmm = cluster(gmm_model, mixed_data);
gscatter(mixed_data(:,1), mixed_data(:,2), idx_gmm);
title('GMM Clustering');
ss_gmm = silhouette(mixed_data, idx_gmm);
title('Silhouette Plot - GMM');

nmi_values(5) = normalized_mutual_info(true_labels, idx_gmm);
ss_values(5) = mean(ss_gmm);
ri_values(5) = rand_index(true_labels, idx_gmm);

figure;
subplot(1, 3, 1);
bar(ss_values);
title('Average Silhouette Score');
xlabel('Clustering Method');
ylabel('Silhouette Score');
set(gca, 'XTickLabel', {'K-Means', 'K-Medoids', 'Fuzzy C-Means', 'DBSCAN', 'GMM'});

subplot(1, 3, 2);
bar(nmi_values);
title('Normalized Mutual Information (NMI)');
xlabel('Clustering Method');
ylabel('NMI');
set(gca, 'XTickLabel', {'K-Means', 'K-Medoids', 'Fuzzy C-Means', 'DBSCAN', 'GMM'});

subplot(1, 3, 3);
bar(ri_values);
title('Rand Index (RI)');
xlabel('Clustering Method');
ylabel('Rand Index');
set(gca, 'XTickLabel', {'K-Means', 'K-Medoids', 'Fuzzy C-Means', 'DBSCAN', 'GMM'});


function nmi_value = normalized_mutual_info(true_labels, predicted_labels)
    contingency_table = crosstab(true_labels, predicted_labels);
    mutual_info = sum(sum(contingency_table .* log2((contingency_table+eps) ./ ...
                  (sum(contingency_table, 2) * sum(contingency_table, 1)))));
    entropy_true = -sum(sum(contingency_table, 2) .* log2(sum(contingency_table, 2) / sum(contingency_table(:))));
    entropy_pred = -sum(sum(contingency_table, 1) .* log2(sum(contingency_table, 1) / sum(contingency_table(:))));
    nmi_value = mutual_info / sqrt(entropy_true * entropy_pred);
end

function ri_value = rand_index(true_labels, predicted_labels)
    n = length(true_labels);
    a = 0; b = 0;
    for i = 1:n
        for j = i+1:n
            if (true_labels(i) == true_labels(j)) && (predicted_labels(i) == predicted_labels(j))
                a = a + 1;
            elseif (true_labels(i) ~= true_labels(j)) && (predicted_labels(i) ~= predicted_labels(j))
                b = b + 1;
            end
        end
    end
    ri_value = (a + b) / (n * (n - 1) / 2);
end

