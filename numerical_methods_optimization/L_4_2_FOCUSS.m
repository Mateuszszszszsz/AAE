clear all;
A = [
    1 2 2 3 1 ;
    2 4 4 6 2 ;
    3 6 6 9 6 ;
    1 2 4 5 3
    ];

x = [1 0 1 1 0]';

b = A*x;

t1= tic;
x1 = FOCUSS(A, b,4)
t1=toc(t1)
res = x -x1 

function [x] = FOCUSS(A, b, p)
[m, n] = size(A);
l = 1e-8;
x = rand(n, 1);

running = true;
th = 0.001; % threshold
while running
    prev_x = x;

    W = diag(abs(x).^(1 - p/2));
    tmp_a = A * W;
    q = tmp_a' * inv(tmp_a*tmp_a' + l*eye(m)) * b;
    x = W * q;
    if (norm(x - prev_x) < th)
        running = false;
    end
end
end
