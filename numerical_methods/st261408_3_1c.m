clear all


% initial and boundary conditions
a = 1;
b = 2;
y_a = 2;
y_b = 2.5;
p = 0.8; %initial p
q = -0.9; %initial q
h = 0.001;  % step size for Runge-Kutta's method
hs = 0.001; % tolerance of shooting method


final_p = shooting_method(a, b, y_a, y_b, p, q, h,hs); 

x_grid = a:h:b; %linspace
y_final = rk2(a, b, [y_a; final_p], h); %computing final approximated solution
y_exact = x_grid + 1 ./ x_grid; %computing exact solution
plot(x_grid, y_final(1, :)); %plotting aproximated solution
hold on;
plot(x_grid, y_exact); %plotting exact solution
hold off;
xlabel('x');
ylabel('y');
legend('Approxiated', 'Exact Solution');

function y = f(x, y)
    y = [y(2); 2 * y(1)^3 - 6 * y(1) - 2 * x^3];
end

function y = rk2(a, b, initial_conditions, h)
    x = a:h:b;
    n = length(x);
    y = zeros(length(initial_conditions), n);
    y(:, 1) = initial_conditions;

    for i = 1:n - 1
        k1 = h * f(x(i), y(:, i));
        k2 = h * f(x(i) + h, y(:, i) + k1);
        y(:, i + 1) = y(:, i) + 0.5 * (k1 + k2);
    end
end

function final_p = shooting_method(a, b, y_a, y_b, initial_p, initial_q, step_size, tolerance)
    p = initial_p;
    q = initial_q;

    c = (p + q) / 2;
    y = rk2(a, b, [y_a; c], step_size);

    while (abs(y(1, end) - y_b) > tolerance)
        c = (p + q) / 2;
        y = rk2(a, b, [y_a; c], step_size);

        if y(1, end) > y_b
            p = c;
        else
            q = c;
        end
    end
    final_p = p;
end