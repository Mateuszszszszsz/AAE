function [x] = gaussian_elimination(A, b)
    B = [A, b];
    [m, n] = size(A);

    for k = 1:n - 1
        [Bmax,inx] = max(abs(B(k:n,k)));
        inx = inx + k - 1;

        temp = B(inx,:);
        B(inx, :) = B(k,:);
        B(k,:) = temp;
    
        for i = k+1:n
            B(i, :) = B(i, :) - (B(i,k)/B(k,k)*B(k,:));
        end
    end

    for k = n:-1:1
        i = k - 1;
        B(k, n + 1) = B(k, n + 1) / B(k, k);
        B(k, k) = 1;
        for j = i:-1:1
            B(j, :) = B(j, :) - (B(j, k) * B(k, :));
        end
    end

    x = B(:, n + 1);
end