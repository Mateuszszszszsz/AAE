clear all
A = [2 1 2; 1 2 3; 4 1 2];
B = invertMat(A)

function A_inv = invertMat(A)
    [n, m] = size(A);
    
    AI = [A eye(n)];
    
    for i = 1:n
        AI(i, :) = AI(i, :) / AI(i, i);
        
        for j = 1:n
            if i ~= j
                AI(j, :) = AI(j, :) - AI(i, :) * AI(j, i);
            end
        end
    end
    
    A_inv = AI(:, n+1:end);
end