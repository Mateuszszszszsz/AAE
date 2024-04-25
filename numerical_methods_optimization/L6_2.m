% Define the coefficients of the objective function
c = [30; 20];  % Coefficients of x and y

% Define the coefficients matrix for the constraints
A = [
    -20 -10;
    -20 -30;
    ];
A = A./1000;
% Define the right-hand side of the constraints
b = [-0.06 ; -0.12];

% Define the lower bounds for variables
lb = [0; 0];

% Use linprog to solve the linear programming problem
[x, Z] = linprog(c, A, b, [], [],lb);

% The maximum value of x + y
max_x_plus_y = -Z;

% Display results
disp("Optimal Solution:")
disp("x = " + x(1))
disp("y = " + x(2))
disp("Max x + y = " + max_x_plus_y)
