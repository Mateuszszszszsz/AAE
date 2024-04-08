% Define the constants
a = 1;
b = 2;
y_a = 2;
y_b = 2.5;
h = 0.001;

% Define the differential equation
dydx = @(x, y) [y(2); 2 * y(1)^3 - 6 * y(1) - 2 * x^3];

% Solve the differential equation using ode45
[x_grid, y_approx] = ode45(dydx, [a b], [y_a; 0]); 

% Plot the approximated solution
plot(x_grid, y_approx(:, 1));
hold on;

% Compute the exact solution
y_exact = x_grid + 1 ./ x_grid;

% Plot the exact solution
plot(x_grid, y_exact);
hold off;

xlabel('x');
ylabel('y');
legend('Approximated', 'Exact Solution');

% Shooting method function
function p_final = shooting_method(a, b, y_a, y_b, h)
    initial_p = 0.8;
    initial_q = -0.9;
    tolerance = 0.001;

    % Define the differential equation
    dydx = @(x, y, p) [y(2); 2 * y(1)^3 - 6 * y(1) - 2 * x^3 + p];

    % Shooting method loop
    while true
        % Integrate the differential equation
        [x_grid, y_approx] = ode45(@(x, y) dydx(x, y, initial_p), [a b], [y_a; 0]);

        % Check the final value of y
        y_final = y_approx(end, 1);

        % If the final value is close enough to y_b, break the loop
        if abs(y_final - y_b) < tolerance
            break;
        end

        % Update initial_p based on the final value of y
        if y_final > y_b
            initial_p = initial_p - 0.1;
        else
            initial_p = initial_p + 0.1;
        end
    end

    % Set the final value of p
    p_final = initial_p;
end
