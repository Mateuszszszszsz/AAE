clear all;
y = [28 ; 30 ; 21 ; 23 ; 23]; % yield of field
x = [
    1 50 18 10 ; 
    1 40 20 16 ; 
    1 35 14 10 ;  
    1 40 12 12 ; 
    1 30 16 14];

a = inv(x'*x)*x'*y

yEst = [];
yEst = x*a

S = [length(y) sum(x(:,2)) sum(x(:,3)) sum(x(:,4)); 
    sum(x(:,2)) sum(x(:,2).^2) sum(x(:,3).^2) sum(x(:,4).^2) ];
Y  = sum()



yEst2 = inv(S)*Y
