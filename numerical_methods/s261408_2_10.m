clear all;
interval_start = -2;
interval_end = 2;
% Define the system of differential equations
dxdt = @(x, y) -y + x*(1 - sqrt(x^2 + y^2));
dydt = @(x, y) x + y*(1 - sqrt(x^2 + y^2));

% Second-order Runge-Kutta method
rk2 = @(f, x, y, h) y + h/2 * (f(x, y) + f(x + h, y + h * f(x, y)));

% Define the range and step size for plotting
x_range = linspace(interval_start,interval_end , 30);
y_range = linspace(interval_start,interval_end , 30);
[X, Y] = meshgrid(x_range, y_range);

% Calculate slopes at each point using the defined system of equations
U = zeros(size(X));
V = zeros(size(Y));
for i = 1:numel(X)
    U(i) = dxdt(X(i), Y(i));
    V(i) = dydt(X(i), Y(i));
end
N = sqrt(U.^2+V.^2);
U = U./N;
V=V./N;
% Plot the slope field
figure(1)
quiver(X, Y, U, V);
hold on;
title('Slope Field');
xlabel('x');
ylabel('y');
axis tight equal;

% Optionally, draw approximate solutions using the Runge-Kutta method
% Initial conditions for approximate solutions
initial_conditions = [
0.5, 0.0;
1.5, 0.;
];

% Iterate through initial conditions and plot approximate solutions
for i = 1:size(initial_conditions, 1)
    Xt = [initial_conditions(i, 1)];
    Yt = [initial_conditions(i, 2)];
    x0 = initial_conditions(i, 1);
    y0 = initial_conditions(i, 2);
    h = 0.01; % Step size

    num_steps =  abs(interval_start-interval_end ) / h;
    x = x0;
    y = y0;
    x_next = x0;
    y_next = y0;
    Xt = [x0];
    Yt = [y0];
    while ~(y<0 && y_next > 0)
        x =x_next;
        y= y_next;
 k1 = [h * dxdt(x,y); h * dydt(x,y)];
 k2 = [h * dxdt(x+0.5*k1(1),y + 0.5*k1(2)); h * dydt(x+0.5*k1(1),y + 0.5*k1(2))];
 x_next = x + k2(1);
 y_next = y + k2(2);
 Xt = [Xt, x];
 Yt = [Yt, y];
    end
    plot(Xt,Yt,'--rs','LineWidth',3,'MarkerSize',0.001,'Color',[initial_conditions(i,i) 0 0]);

    axis([-2 2 -2 2])
end
    legend("Slope Field","Probe1", "Probe 2")
    Xt = [Xt, x];
    Yt = [Yt, x];


