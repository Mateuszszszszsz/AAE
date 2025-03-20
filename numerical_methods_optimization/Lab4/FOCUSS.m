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