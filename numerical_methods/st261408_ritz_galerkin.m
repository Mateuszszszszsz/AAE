clear all;
close all;

%data init
interval_start = 0.0;
interval_end = 1.0;
error = [];

%function init
exact_solution = @(x) sin(pi*x); %used for calculation



fx = @(x) (pi^2+x+1).*sin(pi*x);  
p = @(x) 1; 
q = @(x) (x+1);

%solving start
numberOfProbes = [50 100]; %2.^(1:12);
for n = numberOfProbes %iterating throught numers of samples
    h =  abs(interval_start - interval_end) / n; %grid size calculation
    x = linspace(interval_start, interval_end, n+1);
    exact_solution_x = zeros(1, round(n)); %place for approximated function

    %exact values calculation
for i = 1:n+1
     exact_solution_x(i) = exact_solution(x(i)); %vector of exact solutions in time
end
   % method
       A = zeros(n-1, n-1);
f = zeros(n-1,1);

for i = 1:n-2
         A(i, i) = (p(h * i - (h/2)) + p(h * i + (h/2)))/h + (q(h * i - h/2) + q(h * i + h/2)) * (h/3);
         A(i, i + 1) = -p(h * i + h/2) / h + q(h * i + h/2) * (h/6);
         A(i + 1, i) = -p(h * i + h/2) / h + q(h * i + h/2) * (h/6);
end

A(n-1, n-1) = (p(h * (n+1) - (h/2)) + p(h * (n+1) + (h/2)))/h + (q(h * (n+1) - h/2) + q(h * (n+1) + h/2)) * (h/3);


for i = 1:n-1
   %f(i) = quad(fx,x(i),x(i+1)); %matlab functtion
    f(i) = (fx(x(i)) + 4* fx((x(i)+x(i+1))/2) +   fx(x(i+1)))*h/6; %myself quad

            
end

c = A \ f;

    y = zeros(size(x));
    for i = 1:n-1
        y = y + c(i) * phi(x, i, h);
    end

%plotting figure

figure(n);
plot(x, y, '.', x ,exact_solution_x, '-');
title("Grid: " + h);
lgd = legend("approximated", "exact","location",'northeast' );
title(lgd, "Solution:");
xlim([0, 1]);

%error calculation
temp_e =0;
for i = 1:n
first = exact_solution(x(i)) - y(i);
second = exact_solution((x(i)+x(i+1))/2)-((y(i)+y(i+1))/2);
third = exact_solution(x(i+1))-y(i+1);
temp_e = temp_e + (first^2+4*second^2+third^2)*h/6;
end

temp_e = sqrt(temp_e);
error = [error , temp_e ]; %adding maximum error for the particular n point case

index = log2(error(1:length(error)-1)./error(2:length(error)));
if(n>numberOfProbes(1))
fprintf(1, "For n: %f" , n)
fprintf(1, " points case index is: %f" , index(length(index)))
fprintf(1, " with maximum error: %f \n", temp_e)
end
end


figure(2)
plot( numberOfProbes, error)
set(gca, 'YScale', 'log')
title("Error values:");
figure(3)
plot(numberOfProbes(2: end), index)
title("Index values:")


function val = phi(x, i, h)
    % Hat function
    val = max(1 - abs(x - i * h) / h, 0);
end
