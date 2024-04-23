function [x, residual_error, solution_error] = landweber(A, b, max_iterations, alpha)
    [~, n] = size(A);
    x = zeros(n, 1);
    
    exact = gaussian_elimination(A, b);
    residual_error = [];
    solution_error = [];

    threshold = 10e-6;

    S = (1/alpha) * eye(n);
    T = S - A' * A;
    G = T * alpha;
    sr = max(abs(eig(G)));
    
    c = alpha * A' * b;

    i = 0;

    if sr > 1
        disp('Spectral radius > 1, landweber method will not converge');
    else 
        while (i < max_iterations)
            i = i + 1;
            x_prev = x;
            x = x_prev + alpha * A' * (b - A * x_prev);

            solution_error = [solution_error, norm(x - exact)/norm(exact)];
            residual_error = [residual_error, norm(A * x - b)/n];

            if (norm(x_prev - x) < threshold)
               break; 
            end
        end
    end
end