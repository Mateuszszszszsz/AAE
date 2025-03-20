clear all;

A = [2 -1 0 0; -1 2 -1 0; 0 -1 2 -1; 0 0 -1 2];
b = [0 0 0 5]';

B = [A, b];

for k = 1:3
    for i = k+1:4
        B(i,:) = B(i, :) - (B(i,k)/B(k,k)*B(k,:));
    end
end

for k = 4:-1:1
    i = k - 1;
    B(k,5) = B(k,5) / B(k,k);
    B(k,k) = 1;
    for j = i:-1:1
        B(j, :) = B(j, :) - (B(j, k) * B(k, :));
    end
end
B