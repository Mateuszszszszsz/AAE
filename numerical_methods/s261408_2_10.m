clear all;
interval_start = -2;
interval_end = 2;
% Define the system of differential equations
dxdt = @(x, y) -y + x*(1 - sqrt(x^2 + y^2));
dydt = @(x, y) x + y*(1 - sqrt(x^2 + y^2));

% Second-order Runge-Kutta method
rk2 = @(f, x, y, h) y + h/2 * (f(x, y) + f(x + h, y + h * f(x, y)));

% Define the range and step size for plotting
x_range = linspace(interval_start,interval_end , 10);
y_range = linspace(interval_start,interval_end , 10);
[X, Y] = meshgrid(x_range, y_range);

% Calculate slopes at each point using the defined system of equations
U = zeros(size(X));
V = zeros(size(Y));
for i = 1:numel(X)
    U(i) = dxdt(X(i), Y(i));
    V(i) = dydt(X(i), Y(i));
end

% Plot the slope field
quiver(X, Y, U, V);
hold on;
title('Slope Field');
xlabel('x');
ylabel('y');
axis tight equal;

% Optionally, draw approximate solutions using the Runge-Kutta method
% Initial conditions for approximate solutions
initial_conditions = [
 0.5, 0.5];
%-0.5, -0.5;
% 1, 0;
%-1, 0;
 %0, 1;
% 0, -1];

% Iterate through initial conditions and plot approximate solutions
for i = 1:size(initial_conditions, 1)
    x0 = initial_conditions(i, 1);
    y0 = initial_conditions(i, 2);
    h = 0.05; % Step size
    num_steps =  abs(interval_start,interval_end ) / h;
    x = x0;
    y = y0;
    for j = 1:num_steps
        x_next = rk2(dxdt, x, y, h);
        y_next = rk2(dydt, y, x, h);
        plot([x, x_next], [y, y_next], 'r');
        x = x_next;
        y = y_next;
    end
end