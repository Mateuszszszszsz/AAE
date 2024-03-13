clear all;
%solving gauss 
A = [1 2 3 4 ; -1 1 2 1 ; 0 2 1 3 ; 0 0 1 1];
b = [1 1 1 1]';

B = [A, b];

for k = 1:3
    [Bmax,inx] = max(abs(B(k:4,k)));
    inx = inx + k - 1;

    temp = B(inx,:);
    B(inx, :) = B(k,:);
    B(k,:) = temp;
    
    for i = k+1:4
        B(i,:) = B(i, :) - (B(i,k)/B(k,k)*B(k,:));
    end
end

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
