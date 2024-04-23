clear all;
time= [];
A = [
    1 1 1;
    1 1 2;
    1 2 2;
    ];
b = [1;2;1];

[x_kacz, res_kacz, sol_kacz] = kaczmarz(A, b, 10e6, 0.5);
[x_kacz1, res_kacz1, sol_kacz1] = kaczmarz(A, b, 10e6, 0.8);
[x_kacz2, res_kacz2, sol_kacz2] = kaczmarz(A, b, 10e6, 1);
[x_kacz3, res_kacz3, sol_kacz3] = kaczmarz(A, b, 10e6, 1.2);
[x_kacz4, res_kacz4, sol_kacz4] = kaczmarz(A, b, 10e6, 1.5);
scale =5;

figure(1);
hold on;
plot(sol_kacz, 'o' , 'MarkerIndices', [1:scale:length(sol_kacz)]);
plot(sol_kacz1, '*' , 'MarkerIndices', [1:scale:length(sol_kacz)]);
plot(sol_kacz2, '.' , 'MarkerIndices', [1:scale:length(sol_kacz)]);
plot(sol_kacz3, 'v' , 'MarkerIndices', [1:scale:length(sol_kacz)]);
plot(sol_kacz4, '+' , 'MarkerIndices', [1:scale:length(sol_kacz)]);
title('Solution errors for different alpha')
legend( '0.5','0.8','1','1.2','1.5')
set(gca, 'YScale', 'log')
hold off;

figure(2);
hold on;
plot(res_kacz, 'o' , 'MarkerIndices', [1:scale:length(sol_kacz)]);
plot(res_kacz1, '*' , 'MarkerIndices', [1:scale:length(sol_kacz)]);
plot(res_kacz2, '.' , 'MarkerIndices', [1:scale:length(sol_kacz)]);
plot(res_kacz3, 'v' , 'MarkerIndices', [1:scale:length(sol_kacz)]);
plot(res_kacz4, '+' , 'MarkerIndices', [1:scale:length(sol_kacz)]);
title('Residual errors for different alpha')
legend( '0.5','0.8','1','1.2','1.5');

set(gca, 'YScale', 'log')

hold off;

