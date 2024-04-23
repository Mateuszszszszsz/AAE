A = [1 2 3 4;
    -1 1 2 1;
    0 2 1 3;
    0 0 1 1];

b = [1 1 1 1]';

[x_jacobi, res_j, sol_j] = jacobi(A, b, 10e6);
[x_gauss_seidel, res_gs, sol_gs] = gauss_seidel(A, b, 10e6);
[x_landweber, res_l, sol_l] = landweber(A, b, 10e6, 0.01);
[x_sor, res_s, sol_s] = successive_over_relaxation(A, b, 10e6,0.0000000000001);

figure(1);
hold on;
plot(sol_j, '.');
plot(sol_gs, '*');
plot(sol_l , 'x');
plot(sol_s, 'v');
title('Solution errors')
legend('Landweber method')
set(gca, 'YScale', 'log')
hold off;

figure(2);
hold on;
plot(res_j, '.');
plot(res_gs, '*');
plot(res_l, 'x');
plot(res_s, 'v');
title('Residual errors')
legend('Landweber method')
set(gca, 'YScale', 'log')

hold off;