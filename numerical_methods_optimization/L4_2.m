clear all
A = [
    1 2 2 3 1 ;
    0 4 4 6 2 ;
    3 6 6 9 6 ;
    1 2 4 5 3
    ];
x = [1 0 1 1 0]';

b = A*x



[U, S, V] = svd(A)    % Performing Singular Value Decomposition (SVD)
x1 = V * (S \ (U' * b));
%x1 = (inv(A'*A)*A')*b %nie działa
%x1 = pinv(A)*b

b1  =A*x1

res = x -x1 


