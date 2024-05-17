% Define the number of subintervals
n_values = [50, 100, 200,];

% Exact solution
yd = @(x) sin(pi * x);

for n = n_values
    h = 1 / n;
    x = linspace(0, 1, n+1);
    
    % Assemble the stiffness matrix and load vector
    A = zeros(n-1, n-1);
    b = zeros(n-1, 1);
    
    for i = 1:n-1
        for j = 1:n-1
            A(i, j) = integral(@(x) phi_prime(x, i, h) .* phi_prime(x, j, h) + (x + 1) .* phi(x, i, h) .* phi(x, j, h), max((i-1)*h, 0), min((i+1)*h, 1));
        end
        b(i) = integral(@(x) (pi^2 + x + 1) .* sin(pi * x) .* phi(x, i, h), max((i-1)*h, 0), min((i+1)*h, 1));
    end
    
    % Solve the linear system
    coeffs = A \ b;
    
    % Compute the approximate solution
    yp = zeros(size(x));
    for i = 1:n-1
        yp = yp + coeffs(i) * phi(x, i, h);
    end
    
    % Plot the exact and approximate solutions
    figure;
    plot(x, yd(x), 'r', 'DisplayName', 'Exact Solution');
    hold on;
    plot(x, yp, 'b--', 'DisplayName', 'Approximate Solution');
    title(['n = ', num2str(n)]);
    legend show;
    hold off;
    
    % Define a dense set of points for error calculation
    x_dense = linspace(0, 1, 1000);
    yp_dense = zeros(size(x_dense));
    for i = 1:n-1
        yp_dense = yp_dense + coeffs(i) * phi(x_dense, i, h);
    end
    
    % Calculate the error
    error2 = sqrt(integral(@(x) (yd(x) - interp1(x_dense, yp_dense, x)).^2, 0, 1));
    fprintf('n = %d, Error = %.6f\n', n, error2);
end

function val = phi(x, i, h)
    % Hat function
    val = max(1 - abs(x - i * h) / h, 0);
end

function val = phi_prime(x, i, h)
    % Derivative of the hat function
    val = (abs(x - i * h) < h) .* sign(i * h - x) / h;
end
