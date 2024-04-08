
clear all;


% Initial and boundary conditions
y_a = 2;
y_b = 5/2;
initial_slope_guess = -0.5; % Initial guess for y'(1)
step_size = 0.01; % Step size for RK method

% Solve the BVP using the shooting method
slope_guess = shooting_method(@differential_equation, [1, 2], y_a, y_b, initial_slope_guess, step_size);

% Solve the IVP one final time with the converged slope guess
y_final = rk2(@differential_equation, [1, 2], [y_a, slope_guess], step_size);

% Exact solution for comparison (even though it doesn't solve the given differential equation)
exact_solution = @(x) x+(1/x);

% Plotting
x_final = 1:step_size:2;
plot(x_final, y_final(1, :), 'LineWidth', 2);
hold on;
fplot(exact_solution, [1, 2], 'r--', 'LineWidth', 2);
hold off;
xlabel('x');
ylabel('y');
legend('Numerical Solution', 'Exact Solution (for comparison)');
title('Solution of the Differential Equation');

% Define the differential equation as a system of first order ODEs

function dydx = differential_equation(x, y)
    dydx = [y(2); 2*y(1)^3-6*y(1)-2*x^3]; % y(1) = y, y(2) = y'
end

% Shooting method to adjust the initial slope guess
function slope_guess = shooting_method(ode_func, x_range, y_a, y_b, initial_slope, step_size)
    tolerance = 1e-5;
    max_iterations = 100;
    slope_guess = initial_slope;
    for iteration = 1:max_iterations
        y = rk2(ode_func, x_range, [y_a; slope_guess], step_size);
        if abs(y(1,end) - y_b) < tolerance
            fprintf('Converged after %d iterations.\n', iteration);
            return;
        elseif y(1,end) > y_b
            slope_guess = slope_guess - 0.01; % Adjust the slope guess
        else
            slope_guess = slope_guess + 0.01;
        end
    end
    fprintf('Maximum iterations reached without convergence.\n');
end

% Runge-Kutta 4th order method for solving ODEs
function y = rk2(ode_func, x_range, initial_conditions, step_size)
    x = x_range(1):step_size:x_range(2);
    n = length(x);
    y = zeros(length(initial_conditions), n);
    y(:,1) = initial_conditions;

    for i = 1:n-1
        k1 = ode_func(x(i), y(:,i));
        k2 = ode_func(x(i) + 0.5*step_size, y(:,i) + 0.5*step_size*k1);
        y(:,i+1) = y(:,i) + step_size*k2;
    end
end
