function [x] = jacobi(A, b, max_iterations)
    [~, n] = size(A);
    x = zeros(n, 1);
    
    threshold = 0.0001;

    s = diag(A);
    S = diag(s);
    T = S - A;
    G = inv(S) * T;
    sr = max(abs(eig(G)));
    
    i = 0;

    if sr > 1
        disp('Spectral radius > 1, jacobi method will not converge');
    else 
        while (i < max_iterations)
            i = i + 1;
            x_prev = x;
            x = (T*x +  b) ./ s;

            if (norm(x_prev - x) < threshold)
               break; 
            end
        end
    end
end