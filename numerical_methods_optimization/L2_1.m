clear all
clc
% algorithm 1?

%a

% A = [3,-1;1,2;2,1];
% b = [4;0;1];

A = [1 1 -1 ; 2 -1 6 ; -1 4 1 ; 3 2 -1];
b = [5 1 0 6]';
xa = inv(A'*A)*A'*b
t1 = tic
xa = A\b
t1 = toc(t1)
%b

A = [3 1 1; 2 3 -1 ; 2 -1 1 ; 3 -3 3];
b = [6 1 0 8]';
% xb = inv(A'*A)*A'*b
% 
% %c
A = [1 1 -1 ; 2 -1 6 ; -1 4 1 ; 3 2 -1];
b = [5 1 0 6]';
% 
% xc = inv(A'*A)*A'*b
% t1 = toc(t1)