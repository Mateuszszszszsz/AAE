clear all;

A = [
    0.835 0.667; 
    0.333 0.266
    ];
y = [0.168; 0.067];


[x_jacobi,res_jacobi,sol_jacobi] = jacobi(A,y,10^6);
[x_gaus,res_gaus,sol_gaus] = gauss_seidel(A,y,10^6);
[x_land,res_land,sol_land] = landweber(A,y,10^6,0.001);
[x_suc,res_suc,sol_suc] = successive_over_relaxation(A,y,10e6,0.001);

figure(1)
hold on
plot(res_jacobi)
plot(res_land)
plot(res_gaus)
plot(res_suc)
hold off

figure(2)
hold on
plot(sol_jacobi)
plot(sol_land)
plot(sol_gaus)
plot(sol_suc)
hold off
