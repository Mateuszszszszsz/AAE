clear all
close all
clc

% Set up parameters
x0 = 0;
xk = 2;
t0 = 0;
tk = 0.5;

Lamb = [0.2, 0.4];
numberOfProbes = [20, 40, 80, 160, 320];

for lamb = Lamb
    figure
    
    er = [];
    
    for idx = 1:numel(numberOfProbes)
        n = numberOfProbes(idx);
        disp(['lambda = ', num2str(lamb), ', n = ', num2str(n)]);
        
        h = (xk - x0) / n;
        k = lamb * h^2;
        m = floor(tk / k);
        
        x = linspace(x0, xk, n + 1);
        t = linspace(t0, tk, m + 1);
        
        % Checking stability conditions
        stable = k / (h^2) <= 1/2;
        
        if ~stable
            disp(['- stability condition: NOT OK - UNSTABLE']);
            subtitle("Lambda: "+ lamb +"Unstable")
        else
            disp(['- stability condition: OK - STABLE']);
            subtitle("Lambda: "+ lamb+ " Stable")
        end
        
        %  store all values of u at all times t
        U = zeros(n + 1, m + 1);
        
        % initials
        U(:, 1) = u(x, 0);
        U(1, 1) = 0;
        
        % A matrix init
        A = diag((1 - 2 * lamb) * ones(n - 1, 1)); 
        A = A + diag(lamb * ones(n - 2, 1), 1);     
        A = A + diag(lamb * ones(n - 2, 1), -1);  
        
        % moments in time
        for j = 1:m
            U(2:n, j + 1) = A * U(2:n, j);
        end
        
        u_appr = U(:, end);
        u_ex = u_exact(x, 0.5);
        
        error = max(abs(u_appr - u_ex),[],"all");
        
        er = [er, error];
        disp(['- max error: ', num2str(error)]);
        
        subplot(3, 2, idx)
        plot(x, u_ex, 'DisplayName', 'Exact Solution');
        hold on
        plot(x, U(:, end), 'DisplayName', 'Approximate Solution');
        hold off
        title(['n = ', num2str(n)]);
        legend
        
    end
    
    wykl = er(1:end-1) ./ er(2:end);
    alpha = log2(wykl);
    
    subplot(3, 2, 6)
    plot(wykl);
    title('Convergence');
end

% Define u at boundary values
function result = u(x, t)
    if t == 0
        result = sin(2 * pi * x);
    elseif x == 0 || x == 2
        result = 0;
    end
end

% Define exact value of u for comparison
function result = u_exact(x, t)
    result = exp(-4 * pi^2 * t) * sin(2 * pi * x);
end
