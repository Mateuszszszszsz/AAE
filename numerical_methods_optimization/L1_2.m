clear all;

%A = [2 -1 0 0; -1 2 -1 0; 0 -1 2 -1; 0 0 -1 2];
%b = [0 0 0 5]';

A = [1 1 1; 1 1 2; 1 2 2];
b = [1 2 1]';

B = [A, b];

for k = 1:2
    [Bmax,inx] = max(abs(B(k:3,k)));
    inx = inx + k - 1;

    temp = B(inx,:);
    B(inx, :) = B(k,:);
    B(k,:) = temp;
    
    for i = k+1:3
        B(i,:) = B(i, :) - (B(i,k)/B(k,k)*B(k,:));
    end
end

for k = 3:-1:1
    i = k - 1;
    B(k,4) = B(k,4) / B(k,k);
    B(k,k) = 1;
    for j = i:-1:1
        B(j, :) = B(j, :) - (B(j, k) * B(k, :));
    end
end
B