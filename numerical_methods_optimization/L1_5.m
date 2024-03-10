clear all;

A = [2 1 2; 1 2 3; 4 1 2];
I = [1 0 0; 0 1 0; 0 0 1];

B = [A, I];

for k = 1:2
    for i = k+1:3
        B(i,:) = B(i, :) - (B(i,k)/B(k,k)*B(k,:));
    end
end

for k = 3:-1:1
    i = k - 1;
    B(k,4:6) = B(k,4:6) / B(k,k);
    B(k,k) = 1;
    for j = i:-1:1
        B(j, :) = B(j, :) - (B(j, k) * B(k, :));
    end
end

A_1 = B(:, 4:6);

A*A_1