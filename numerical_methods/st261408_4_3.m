close all
clear all

%data init
x0 = 0;
xk = 1;
t0 = 0;
tk = 1;
a_xt = -2;
u_x0 = 0;
u_0t = 0;
numberOfProbes = [50, 100, 200, 400];

% lambda
lamb = 0.2;

%function init
error = [];
f = @(x, t) x.^2 + 4 * x * t;
f_ex = @(x, t) (x.^2) * t;

for m = numberOfProbes
    n = floor(m / lamb);
    h = (xk - x0) / m;
    k = lamb * h;

    x = linspace(x0, xk, m + 1);
    t = linspace(t0, tk, n + 1);

    U = zeros(m, 1);
    for idx = 1:m
        U(idx) = u_x0;
    end

    A = zeros(m, m);
    B = zeros(m, m);

    for idx = 1:m-1
        A(idx, idx) = 1 - 0.5 * lamb * a_xt;
        B(idx, idx) = 1 + 0.5 * lamb * a_xt;
        A(idx + 1, idx) = 0.5 * lamb * a_xt;
        B(idx + 1, idx) = -0.5 * lamb * a_xt;
    end

    A(m, m) = 1 - 0.5 * lamb * a_xt;
    B(m, m) = 1 + 0.5 * lamb * a_xt;

    for i = 1:floor(tk / k)
        W = zeros(m, 1);
        W(1) = u_0t;

        F = zeros(m, 1);
        for idx = 1:m
            F(idx) = f(x(idx + 1), t(i) + k / 2);
        end

        right_side = B * U - lamb * a_xt * W + k * F;

        U = A \ right_side;
    end

    x = linspace(x0, xk, m);
    f_exact = zeros(m, 1);
    for i = 1:m
        f_exact(i) = f_ex(x(i), 1);
    end
    
    figure(m);
    plot(x, U, x, f_exact);
    legend('Numerical Solution', 'Exact Solution');
    xlabel('x');
    ylabel('u');
    title(['Solution for m = ', num2str(m)]);
    grid on;

    % Calculate error
    temp_e = max(max(abs(U-f_exact)));
    error = [error,temp_e];

    %index calculation
index = log2(error(1:length(error)-1)./error(2:length(error)));
if(m>numberOfProbes(1))
fprintf(1, "For n: %f " , n)
fprintf(1, "and m: %f" , m)
fprintf(1, " points case index is: %f" , index(length(index)))
fprintf(1, " with maximum error: %f \n", temp_e)
end
end

figure(1)
plot(numberOfProbes, error);
title("Error values:");
figure(2)
plot(numberOfProbes(2: end), index)
title("Index values:")
