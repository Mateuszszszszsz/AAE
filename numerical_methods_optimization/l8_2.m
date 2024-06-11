close all 
clear all

x = 2.46557;
y = 1.1479;


hessian = zeros(2,2) 
hessian(1,1) = (15*x^4-60*x^3+96*x^2-6*x*(y+14)*4*y+31);
hessian(2,1) = (1-3*x)*(x-1);
hessian(1,2) = hessian(2,1);
hessian(2,2) = 4;
hessian

M1 = hessian(1,1)
M2 = det(hessian)
