function [x, residual_error, solution_error] = kaczmarz(A, b, max_iterations, alpha)
    [~, n] = size(A);
    x = zeros(n, 1);
    
    exact = gaussian_elimination(A, b);
    residual_error = [];
    solution_error = [];

    threshold = 10e-6;

    iterations = 0;

    G = inv(A);
    while (iterations < max_iterations)
        x_prev = x;
        iterations = iterations + 1;

        for i = 1:n
            x = x + alpha * ((b(i) - dot(x, A(i, :))) / (norm(A(i, :)) ^ 2)) * A(i, :)';
        end

        solution_error = [solution_error, norm(x - exact)/norm(exact)];
        residual_error = [residual_error, norm(A * x - b)/n];

        if (norm(x_prev - x) < threshold)
               break; 
        end
    end
end