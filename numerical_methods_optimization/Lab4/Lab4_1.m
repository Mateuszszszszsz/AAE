clear all;

A = [1 3 1;
    -1 -2 1;
    3 7 -1];

b = [10 -5 20]';

x_a = FOCUSS(A, b, 1);

b = [1 1 -1]';

x_b = FOCUSS(A, b, 1);

b = [1 0 1]';

x_c = FOCUSS(A, b, 1);