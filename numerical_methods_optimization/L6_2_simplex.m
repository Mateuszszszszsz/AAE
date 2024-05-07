% Define the coefficients of the objective function
c = [30; 20];  % Coefficients of x and y

% Define the coefficients matrix for the constraints
A = [
    20 10;
    20 30;
    ];
A = A./1000;
% Define the right-hand side of the constraints
b = [0.06 ; 0.12];

% Define the lower bounds for variables
lb = [0; 0];

% Use linprog to solve the linear programming problem
t1=tic;
%[x, Z] = linprog(c, A, b, [], [],lb);
[Z,x] = simplex(A,b,c)
t1=toc(t1)
% The maximum value of x + y
max_x_plus_y = Z;

% Display results
disp("Optimal Solution:")
disp("cheese= " + x(1))
disp("bread= " + x(2))
disp("Price = " + max_x_plus_y)
