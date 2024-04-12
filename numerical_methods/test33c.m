clear all;
close all;
clc;
1;
n = 50;
h = 1 / n;
a = 0; b = 1;

x = linspace(a, b, n+1);
exact_solution = (x - 1) .* exp(x);

rhs = @(x) -(x.^2 + 3) .* exp(x);

A = zeros(n+1, n+1);
b = zeros(n+1, 1);

for i = 2:n
    A(i, i+1) = -1+ (x(i)+1) * h / 2;
    A(i, i) = 2 + 2 * h^2;
    A(i, i-1) = -1 - (x(i+1)+1) * h / 2;
end

A(1, 1) = 1;
A(n+1, n+1) = 1;
b(1) = -1;
b(n+1) = 0;

for i = 2:n
    b(i) = h^2 * rhs(x(i));
end

y = A \ b;

figure;
plot(x, y, 'o-');
hold on;

plot(x, exact_solution, 'r--');
xlabel('x');
ylabel('y');
legend('Numerical Approximation', 'Exact Solution', 'Location', 'Best');
grid on;