clear all
A = [
    1 1 1 1;
    1 3 1 1;
    1 -1 3 1;
    1 1 1 3;
    1 1 1 -1
    ];


b = [2 ; 1 ; 6 ; 3 ; 1];



[U, S, V] = svd(A);    % Performing Singular Value Decomposition (SVD)
x = V * (S \ (U' * b))

%x1 = pinv(A)*b



