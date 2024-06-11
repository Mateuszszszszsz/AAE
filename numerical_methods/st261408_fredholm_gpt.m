function fredholm_integral_equation()
    % Define the exact solution and functions f and k
    exact_solution = @(x) sin(x);
    f = @(x) (x + 1) .* sin(x) + pi;
    k = @(x, s) x + s;

    % Define the range of n values to be tested
    n_values = [50, 100, 200, 400, 800];
    errors = zeros(size(n_values));
    
    % Loop over each n value and solve the integral equation
    for idx = 1:length(n_values)
        n = n_values(idx);
        [x, u_approx] = solve_fredholm(n, f, k);
        u_exact = exact_solution(x);
        errors(idx) = max(abs(u_exact - u_approx));
        
        % Plot exact and approximate solutions
        figure;
        plot(x, u_exact, 'r-', 'DisplayName', 'Exact Solution');
        hold on;
        plot(x, u_approx, 'b--', 'DisplayName', sprintf('Approximate Solution (n=%d)', n));
        xlabel('x');
        ylabel('u(x)');
        title(sprintf('Solution Comparison for n=%d', n));
        legend;
        hold off;
    end

    % Display the errors for each n value
    for idx = 1:length(n_values)
        fprintf('Error for n=%d: %f\n', n_values(idx), errors(idx));
    end

    % Plot the error vs. number of subintervals
    figure;
    loglog(n_values, errors, 'o-');
    xlabel('Number of Subintervals (n)');
    ylabel('Error');
    title('Error vs Number of Subintervals');
    grid on;
end

function [x, u_approx] = solve_fredholm(n, f, k)
    a = 0;
    b = pi;
    lambda = 1;
    x = linspace(a, b, n + 1)';
    h = (b - a) / n;

    A = zeros(n + 1, n + 1);
    b_vector = zeros(n + 1, 1);

    for i = 1:n + 1
        A(i, i) = 1;
        for j = 1:n
            A(i, j) = A(i, j) + 0.5 * lambda * h * k(x(i), x(j));
            A(i, j + 1) = A(i, j + 1) + 0.5 * lambda * h * k(x(i), x(j + 1));
        end
        b_vector(i) = f(x(i));
        b_vector(i) = sin(x(i))+ quad(@(s) (x(i)+s).*sin(s),0,pi);

    end

    u_approx = A \ b_vector;
end
