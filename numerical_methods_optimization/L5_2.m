clear all;
A = [
    1 1 1;
    1 1 2;
    1 2 2;
    ];
b = [1;2;1];

[x_jacobi, res_j, sol_j] = jacobi(A, b, 10e6);
[x_gauss_seidel, res_gs, sol_gs] = gauss_seidel(A, b, 10e6);

t1 = tic;
[x_landweber, res_l, sol_l] = landweber(A, b, 10e6, 0.11);
t1=toc(t1)
[x_sor, res_s, sol_s] = successive_over_relaxation(A, b, 10e6, 1);
t2=tic;
[x_kacz, res_kacz, sol_kacz] = kaczmarz(A, b, 10e6, 1);
t2=toc(t2)
scale =5;

figure(1);
hold on;
plot(sol_j, '.', 'MarkerIndices', [1:scale:length(sol_j)]);
plot(sol_gs, '*', 'MarkerIndices', [1:scale:length(sol_gs)]);
plot(sol_l , 'x', 'MarkerIndices', [1:scale:length(sol_l)]);
plot(sol_s, 'v', 'MarkerIndices', [1:scale:length(sol_s)]);
plot(sol_kacz, 'o' , 'MarkerIndices', [1:scale:length(sol_kacz)]);
title('Solution errors')
legend( 'Landweber method', 'Kaczmarz')
set(gca, 'YScale', 'log')
hold off;

figure(2);
hold on;
plot(res_j, '.', 'MarkerIndices', [1:scale:length(res_s)]);
plot(res_gs, '*', 'MarkerIndices', [1:scale:length(res_gs)]);
plot(res_l, 'x', 'MarkerIndices', [1:scale:length(res_l)]);
plot(res_s, 'v', 'MarkerIndices', [1:scale:length(res_s)]);
plot(res_kacz, 'o', 'MarkerIndices', [1:scale:length(res_kacz)] );
title('Residual errors')
legend('Landweber method', 'Kaczmarz')
set(gca, 'YScale', 'log')

hold off;

