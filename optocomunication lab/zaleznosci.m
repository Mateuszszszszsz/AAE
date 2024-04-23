clear all;

y = [
-15.045
-14.9525
-15.2925
-14.9225
-14.91
-14.93
-15.015
]; % output power
x = [
1 0.07	0.71    2.44	0.42;
1 -0.2	0.7     1.71	0.87;
1 -0.06	0.7	    1.47	0.4;
1 -0.05	0.74	0.58	1.31;
1 -0.04	0.79	0.46	0.99;
1 -0.12	0.65	0.75	1.86;
1 0.07	0.57	0.63	1.93;
]; %data matrix
a = x\y %coefficient matrix
y1 = x*a %estimated output power with coeficients
res = y1-y %residuals of Output power
