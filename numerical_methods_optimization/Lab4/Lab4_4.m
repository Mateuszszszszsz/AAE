clear all;

A = [2 3 -1 10 21 44 -9 1 -1;
    1 2 2 8 15 35 8 -3 1;
    3 1 1 6 16 53 -7 2 2];

b = [118 77 129]';

x0 = FOCUSS(A, b, 0);
x1 = FOCUSS(A, b, 1);

x_ideal = A\b;

