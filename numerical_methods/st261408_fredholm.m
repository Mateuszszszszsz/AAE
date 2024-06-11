clear all;
close all;
clc


%data init
interval_start = 0.0;
interval_end = pi;
error = [];
lambda=1;
%function init
exact_solution = @(x) sin(x); %used for calculation
    f = @(x) (x + 1) .* sin(x) + pi;
    k = @(x, s) x + s;

            %solving start
numberOfProbes = [50 100 200 400 800]; %2.^(1:12);
for n = numberOfProbes %iterating throught numers of samples
    lambda = 1;
    x = linspace(interval_start, interval_end, n + 1)';
    h = (interval_end - interval_start) / n;

           
            %method
            A = zeros(n + 1, n + 1);
    b_vector = zeros(n + 1, 1);

    for i = 1:n + 1
        A(i, i) = 1;
        for j = 1:n
            A(i, j) = A(i, j) + 0.5 * lambda * h * k(x(i), x(j));
            A(i, j + 1) = A(i, j + 1) + 0.5 * lambda * h * k(x(i), x(j + 1));
        end
        b_vector(i) = f(x(i));
        b_vector(i) = sin(x(i))+ quad(@(s) (x(i)+s).*sin(s),0,pi);

    end

    y = A \ b_vector;  

            %exact values calculation
     exact_solution_x = exact_solution(x); %vector of exact solutions in time

            %plotting figure
figure(n);
plot(x, y, '.', x ,exact_solution_x, '-');
title("Grid: " + h);
lgd = legend("approximated", "exact","location",'northeast' );
title(lgd, "Solution:");
xlim([interval_start, interval_end]);

            %error calculation
temp_e =0;
temp_e = max(abs(exact_solution_x-y),[], "all");
error = [error , temp_e]; %adding maximum error for the particular n point case

            %index calculation
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



