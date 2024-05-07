% Define the coefficients of the objective function
c = [1; 1];  % Coefficients of x and y

% Define the coefficients matrix for the constraints
A = [
    1, 2;
    4, 2;
    -1, 1
    ];

% Define the right-hand side of the constraints
b = [4; 12; 1];

% Define the lower bounds for variables
lb = [0; 0];

% Use linprog to solve the linear programming problem
t1 = tic;
[Z,x] = simplex(A,b,c)
%[x, Z] = linprog(-c, A, b, [], [], lb);
t1=toc(t1)


% Display results
disp("Optimal Solution:")
disp("x = " + x(1))
disp("y = " + x(2))
disp("Max x + y = " + Z)
