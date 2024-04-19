clear all;

%data init

interval_start = 0.0;
interval_end = 1;
error = [];

%function init






exact_solution = @(x) (x - 1) .* exp(x);
g = @(x) -(x.^2 + 3) .* exp(x); 
p = @(x) -(x+1);
q = 2;


%solving start
numberOfProbes = [50 100 200 400 800 ]; %2.^(1:12);
for n = numberOfProbes %iterating throught numers of samples
    h =  abs(interval_start - interval_end) / n; %grid size calculation
    x = linspace(interval_start, interval_end, n+1);
    
    exact_solution_x = zeros(1, round(n)); %place for approximated function
for i = 1:n+1
     exact_solution_x(i) = exact_solution(x(i)); %vector of exact solutions in time
end
   % method
       A = zeros(n+1, n+1);
b = zeros(n+1, 1);

for i = 2:n
    A(i, i+1) = -(1- p(x(i)) * h / 2);
    A(i, i) = 2 + q * h^2;
    A(i, i-1) = -(1+ p(x(i)) * h / 2);
end

A(1, 1) = 1;
A(n+1, n+1) = 1;
b(1) = -1;
b(n+1) = 0;

for i = 2:n
    b(i) = h^2 * g(x(i));
end

y = A \ b;

         
    
%plotting figure

figure(n);
plot(x, y, '.', x ,exact_solution_x, '-');
title("Grid: " + h);
lgd = legend("approximated", "exact","location",'north' );
title(lgd, "Solution:");
xlim([0, 1]);

%error calculation
temp_e = max(abs(exact_solution_x-y'), [], "all");
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
title("Maximum error values:");
figure(3)
plot(numberOfProbes(2: end), index)
%set(gca, 'YScale', 'log')
title("Index values:")

