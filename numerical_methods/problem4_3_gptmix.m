clear all;
close all;
% Parameters
a = 0; b = 1; % x domain
c = 0; d = 1; % t domain
lambda = 1;
n_values = [50, 100, 200]; % Grid sizes to test
errors = zeros(size(n_values));

% Exact solution
exact_solution = @(x, t) x.^2 .* t;

for idx = 1:length(n_values)
    n = n_values(idx);
    h = (b - a) / n;
    k = lambda * h;
    m = round((d - c) / k);
    
    x = linspace(a, b, n+1);
    t = linspace(c, d, m+1);
    
    % Initialize solution matrix
    U = zeros(n+1, m+1);
    
    % Boundary and initial conditions
    U(:, 1) = 0; % Initial condition u(x,0) = 0
    U(1, :) = 0; % Boundary condition u(0,t) = 0
    U(end, :) = 1; % Boundary condition u(1,t) = 1
    
    % Crank-Nicolson method
    A = zeros(n-1, n-1);
    B = zeros(n-1, 1);
    
    for j = 1:m
        % Construct the matrix A and vector B
        for i = 2:n
            A(i-1, i-1) = 1 + lambda;
            if i > 2
                A(i-1, i-2) = -lambda / 2;
            end
            if i < n
                A(i-1, i) = -lambda /2;
            end
            
            B(i-1) = (1 - lambda) * U(i, j) + ...
                     (lambda / 2) * (U(i, j) + U(i-1, j)) + ...
                     k * (x(i)^2 + 4*x(i)*t(j) + x(i)^2 + 4*x(i)*t(j+1))/2;
        end
        
        % Update B for the current time step
        U(2:n, j+1) = A \ B;
    end
    
    % Calculate error
    exact_U = exact_solution(x', t);
    errors(idx) = max(max(abs(U - exact_U)));
end

% Display errors
disp('Errors for different n values:');
disp(errors);

% Plotting results for the largest n
figure;
mesh(x, t, U');
title('Approximate Solution using Crank-Nicolson');
xlabel('x');
ylabel('t');
zlabel('u(x,t)');

figure;
mesh(x, t, exact_U');
title('Exact Solution');
xlabel('x');
ylabel('t');
zlabel('u(x,t)');

% Plot error
figure;
loglog(n_values, errors, '-o');
title('Error vs Grid Size');
xlabel('n');
ylabel('Error');
grid on;
