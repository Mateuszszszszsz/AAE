A = [0.0001, 1; 1, 1];
b = [1; 2];

x_without_pivoting = gaussianElimination(A, b, false);

x_with_pivoting = gaussianElimination(A, b, true);

x_without_pivoting
x_with_pivoting

function x = gaussianElimination(A, b, usePivoting)
    n = size(A, 1);
    Ab = [A, b];
    
    for i = 1:n-1
        if usePivoting
            [~, maxIndex] = max(abs(Ab(i:n, i)));
            maxIndex = maxIndex + i - 1;
            temp = Ab(i, :);
            Ab(i, :) = Ab(maxIndex, :);
            Ab(maxIndex, :) = temp;
        end
        
        for j = i+1:n
            factor = Ab(j, i) / Ab(i, i);
            factor = round(factor, 3, 'significant');
            Ab(j, i:n+1) = Ab(j, i:n+1) - factor * Ab(i, i:n+1);
            Ab(j, i:n+1) = round(Ab(j, i:n+1), 3, 'significant'); 
        end
    end
    
    x = zeros(n, 1);
    for i = n:-1:1
        x(i) = (Ab(i, n+1) - Ab(i, 1:n) * x) / Ab(i, i);
    end
end

