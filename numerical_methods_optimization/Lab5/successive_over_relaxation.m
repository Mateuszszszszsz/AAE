function [x, residual_error, solution_error] = successive_over_relaxation(A, b, max_iterations, omega)
    [~, n] = size(A);
    x = zeros(n, 1);
    
    exact = A\b; 
    % compute errors:
    %   solution error: norm(current_x - exact)/norm(exact)
    %   residual error: 
    residual_error = [];
    solution_error = [];

    threshold = 0.0001;

    D = diag(diag(A));
    L = tril(A) - D;
    U = triu(A) - D;

    S = L + (1 / omega) * D;
    T = -(U + ((1 - omega)/omega) * D);
    G = inv(S) * T;
    sr = max(abs(eig(G)));

    i = 0;

    if sr > 1
        disp('Spectral radius > 1, sor method will not converge');
    else 
        while (i < max_iterations)
            i = i + 1;
            x_prev = x;
            
            for i=1:n
                    sigma=0;
                    for j=1:i-1
                        sigma=sigma + A(i, j) * x(j);
                    end
        
                    for j=i+1:n
                        sigma = sigma + A(i, j) * x_prev(j);
                    end
                    x(i)=(1/A(i,i))*(b(i)-sigma);
                end

            solution_error = [solution_error, norm(x - exact)/norm(exact)];
            residual_error = [residual_error, norm(A * x - b)/n];

            if (norm(x_prev - x) < threshold)
               break; 
            end
        end
    end
end