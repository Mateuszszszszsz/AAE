clear all;

%data init

interval_start = 0.0;
interval_end = pi;
error = [];
error_h1 = [];
%function init
exact_solution = @(x) sin(x);
fx = @(x) sin(x);
fp = @(x) cos(x);


%solving start
numberOfProbes = [50 100 200 400 800]; %2.^(1:12);
for n = numberOfProbes %iterating throught numers of samples
    h =  abs(interval_start - interval_end) / n; %grid size calculation
    x = linspace(interval_start, interval_end, n+1);
    
    exact_solution_x = zeros(1, round(n)); %place for approximated function
for i = 1:n+1
     exact_solution_x(i) = exact_solution(x(i)); %vector of exact solutions in time
end
   % method
       A = zeros(n+1, n+1);
f = zeros(n+1, 1);

for i = 1:n
    A(i, i+1) = h/6;
    A(i, i) = 2*h/3;
    A(i+1, i) = h/6;
end

A(1, 1) = 2*h/3;
A(n+1, n+1) = 2*h/3;
f(1) = 0;
f(n+1) = 0;

for i = 2:n
    f(i) = (fx( (x(i)+x(i-1))/2 )+fx(   (x(i)+x(i+1))/2   ))/2;             
end

c = A \ f;
y = A*c;

yp = [];
%div calc
for i = 1:n
yp(i) = (y(i+1)-y(i))/h;
end



%plotting figure

figure(n);
plot(x, y, '.', x ,exact_solution_x, '-');
title("Grid: " + h);
lgd = legend("approximated", "exact","location",'north' );
title(lgd, "Solution:");
xlim([0, pi]);

%error calculation
temp_e =0;

for i = 1:n
first = fx(x(i)) - y(i);
second = fx((x(i)+x(i+1))/2)-((y(i)+y(i+1))/2);
third = fx(x(i+1))-y(i+1);
temp_e = temp_e + (first^2+4*second^2+third^2)*h/6;
end
temp_e = sqrt(temp_e);
error = [error , temp_e ]; %adding maximum error for the particular n point case



%calculating h1 error
temp_e_h1 = 0;
for i = 1:n
first = fx(x(i)) - y(i);
second = fx((x(i)+x(i+1))/2)-((y(i)+y(i+1))/2);
third = fx(x(i+1))-y(i+1);
temp_e_h1 = temp_e_h1 + (first^2+4*second^2+third^2)*h/6;
end

temp_e_h2 = 0;
for i = 1:n-1
first = fp(x(i)) - yp(i);
second = fp((x(i)+x(i+1))/2)-((yp(i)+yp(i+1))/2);
third = fp(x(i+1))-yp(i+1);
temp_e_h2 = temp_e_h2 + (first^2+4*second^2+third^2)*h/6;
end
temp_e_h = sqrt(temp_e_h1) + sqrt(temp_e_h2);
error_h1 = [error_h1 , temp_e_h ];



index = log2(error(1:length(error)-1)./error(2:length(error)));
index_h = log2(error_h1(1:length(error_h1)-1)./error_h1(2:length(error_h1)));
if(n>numberOfProbes(1))
fprintf(1, "For n: %f" , n)
fprintf(1, " points case index is: %f" , index(length(index)))
fprintf(1, " with maximum error: %f ", temp_e)
fprintf(1, " H index is: %f" , index_h(length(index_h)))
fprintf(1, " H error: %f \n", temp_e_h)
end
end
figure(2)
plot( numberOfProbes, error)
set(gca, 'YScale', 'log')
title("Error values:");
figure(3)
plot(numberOfProbes(2: end), index)
%set(gca, 'YScale', 'log')
title("Index values:")
figure(4)
plot( numberOfProbes, error_h1)
set(gca, 'YScale', 'log')
title("H Error values:");
figure(5)
plot(numberOfProbes(2: end), index_h)
%set(gca, 'YScale', 'log')
title("H Index values:")

