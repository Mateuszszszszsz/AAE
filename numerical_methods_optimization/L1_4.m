clear all
t1 = tic;
A = [0.835 0.667; 0.333 0.266];
b = [0.168 0.067]';

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
t1 = toc(t1)
B
t2 = tic;
A = [0.835 0.667; 0.333 0.266];
b = [0.168 0.066]';

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
t2 = toc(t2)
B