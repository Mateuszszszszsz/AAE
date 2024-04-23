clear all;

A = [2 -1 0 0;
    -1 2 -1 0;
    0 -1 2 -1;
    0 0 -1 2];

b = [0 0 0 5]';

[x_jacobi, res_j, sol_j] = jacobi(A, b, 10e6);
[x_gauss_seidel, res_gs, sol_gs] = gauss_seidel(A, b, 10e6);
[x_landweber, res_l, sol_l] = landweber(A, b, 10e6, 0.15);
[x_sor, res_s, sol_s] = successive_over_relaxation(A, b, 10e6, 0.5);

figure(1);
hold on;
plot(sol_j, '.');
plot(sol_gs, '*');
plot(sol_l , 'x');
plot(sol_s, 'v');
title('Solution errors')
legend('Jacobi method', 'Gauss-Seidel method', 'Landweber method', 'Successive over-relaxation method')
set(gca, 'YScale', 'log')
hold off;

figure(2);
hold on;
plot(res_j, '.');
plot(res_gs, '*');
plot(res_l, 'x');
plot(res_s, 'v');
title('Residual errors')
legend('Jacobi method', 'Gauss-Seidel method', 'Landweber method', 'Successive over-relaxation method')
set(gca, 'YScale', 'log')

hold off;