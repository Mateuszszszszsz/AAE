clear all;

A = [
    0.835 0.667; 
    0.333 0.266
    ];
b = [0.168; 0.067];

[x_jacobi, res_j, sol_j] = jacobi(A, b, 10e6);
[x_gauss_seidel, res_gs, sol_gs] = gauss_seidel(A, b, 10e6);
land=tic;
[x_landweber, res_l, sol_l] = landweber(A, b, 10e6, 0.0001);
land=toc(land)
sor=tic;
[x_sor, res_s, sol_s] = successive_over_relaxation(A, b, 10e6, 0.0001);
sor=toc(sor)
kacz = tic;
[x_kacz, res_kacz, sol_kacz] = kaczmarz(A, b, 10e6, 0.5);
kacz=toc(kacz)
scale =1;

figure(1);
hold on;
plot(sol_j, '.', 'MarkerIndices', [1:scale:length(sol_j)]);
plot(sol_gs, '*', 'MarkerIndices', [1:scale:length(sol_gs)]);
plot(sol_l , 'x', 'MarkerIndices', [1:200*scale:length(sol_l)]);
plot(sol_s, 'v', 'MarkerIndices', [1:200*scale:length(sol_s)]);
plot(sol_kacz, 'o' , 'MarkerIndices', [1:scale:length(sol_kacz)]);
title('Solution errors')
legend( 'Landweber method', 'SOR', 'Kaczmarz')
set(gca, 'YScale', 'log')
hold off;

figure(2);
hold on;
plot(res_j, '.', 'MarkerIndices', [1:scale:length(res_s)]);
plot(res_gs, '*', 'MarkerIndices', [1:scale:length(res_gs)]);
plot(res_l, 'x', 'MarkerIndices', [1:200*scale:length(res_l)]);
plot(res_s, 'v', 'MarkerIndices', [1:200*scale:length(res_s)]);
plot(res_kacz, 'o', 'MarkerIndices', [1:scale:length(res_kacz)] );
title('Residual errors')
legend('Landweber method','SOR', 'Kaczmarz')
set(gca, 'YScale', 'log')

hold off;

