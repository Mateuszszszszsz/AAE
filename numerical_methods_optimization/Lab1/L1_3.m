clear all;

A = [0.0001 1 ; 1 1];
b = [1 2]';

B = [A, b];

%without pivoting
for k = 1:2
    for i = k+1:2
        B(i,:) = round(B(i, :) - (B(i,k)/B(k,k)*B(k,:)),3,"significant")
    end
end

for k = 2:-1:1
    i = k - 1;
    B(k,3) = B(k,3) / B(k,k);
    B(k,k) = 1;
    for j = i:-1:1
        B(j, :) = B(j, :) - (B(j, k) * B(k, :));
    end
end

B

%with pivoting
A = [0.0001 1 ; 1 1];
b = [1 2]';

B = [A, b];


for k = 1:2
    [Bmax,inx] = max(abs(B(k:2,k)));
    inx = inx + k - 1;

    temp = B(inx,:);
    B(inx, :) = B(k,:);
    B(k,:) = temp;
    
    for i = k+1:2
        B(i,:) = round(B(i, :) - (B(i,k)/B(k,k)*B(k,:)),3,"significant");
    end
end

for k = 2:-1:1
    i = k - 1;
    B(k,3) = B(k,3) / B(k,k);
    B(k,k) = 1;
    for j = i:-1:1
        B(j, :) = B(j, :) - (B(j, k) * B(k, :));
    end
end
B