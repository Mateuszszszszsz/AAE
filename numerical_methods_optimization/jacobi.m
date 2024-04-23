function [x, residual_error, solution_error] = jacobi(A, b, max_iterations)
    [~, n] = size(A);
    x = zeros(n, 1);
    
    exact = gaussian_elimination(A, b);
    residual_error = [];
    solution_error = [];

    threshold = 10e-6;

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

            solution_error = [solution_error, norm(x - exact)/norm(exact)];
            residual_error = [residual_error, norm(A * x - b)/n];

            if (norm(x_prev - x) < threshold)
               break; 
            end
        end
    end
end