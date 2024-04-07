clear all;
xi = [1 2 3 4 5 6 7 8];
yi = [1.5 2 2.8 4.1 4.9 6.3 5.0 11.5];

% Formulating the linear equation y = mx + b
A = [xi' ones(length(xi), 1)];
b = yi';


[U, S, V] = mySVD(A)    % Performing Singular Value Decomposition (SVD)

% Solve for the coefficients using pseudo-inverse
coefficients = V * (S \ (U' * b));

% Extracting slope and intercept from coefficients
m = coefficients(1);
b = coefficients(2);

% Calculate the error estimates for slope and intercept
residuals = A * coefficients - b;
sigma2 = sum(residuals.^2) / (length(xi) - 2);
sigma_m = sqrt(sigma2 * V(1, 1));
sigma_b = sqrt(sigma2 * V(2, 2));

% Display the results
fprintf('Best fit line: y = %.4fx + %.4f\n', m, b);
fprintf('Error estimate for slope (m): %.4f\n', sigma_m);
fprintf('Error estimate for intercept (b): %.4f\n', sigma_b);

function [U, S, V] = mySVD(A)
% mySVD: Singular Value Decomposition (SVD) implementation
% Input:
%   A: m x n matrix
% Output:
%   U: Left singular vectors, m x m orthogonal matrix
%   S: Singular values, diagonal matrix of size m x n
%   V: Right singular vectors, n x n orthogonal matrix



[m, n] = size(A);   % Compute the size of A

ATA = A' * A;   % Perform eigenvalue decomposition of A*A'
[V, eig_vals] = eig(ATA);

[eig_vals_sorted, idx] = sort(diag(eig_vals), 'descend');   % Sort eigenvalues in descending order
V = V(:, idx);

s = sqrt(eig_vals_sorted);  % Compute singular values
S = zeros(m, n); % Construct the singular value matrix
S(1:min(m, n), 1:min(m, n)) = diag(s);

U = A * V / S;  % Compute left singular vectors

end