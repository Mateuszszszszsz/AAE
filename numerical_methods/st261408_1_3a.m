clear all;

%data init

interval_start = 0.0;
interval_end = pi;
t_start = 0;
y_start = 1.0;
error = [];

%function init

f = @(t,y) -100*y + 100*cos(t) - sin(t);
exact_f = @(t) cos(t);


%solving start
numberOfProbes = 2.^(1:12);
for n = numberOfProbes %iterating throught numers of samples
    h =  abs(interval_start - interval_end) / n; %grid size calculation
    t = linspace(interval_start, interval_end, n + 1); %time vector init

    exact_f_t = zeros(1, round(n)); %place for approximated function
for i = 1:n+1
     exact_f_t(i) = exact_f(t(i)); %vector of exact solutions in time
end
   % Range-Kutta's method
            approx_f = [];
            approx_f(1) = y_start;
            for k = 1:n
                k1 = h*f(t(k),approx_f(k));
                k2 = h*f(t(k)+0.5*h,approx_f(k)+0.5*k1);
                estimated = approx_f(k)+k2;
                approx_f = [approx_f, estimated];
            end
    
%plotting figure

figure(n);
plot(t, approx_f, '.', t ,exact_f_t, '-');
title("Grid: " + h);
lgd = legend("approximated", "exact","location",'north' );
title(lgd, "Solution:");
xlim([0, pi]);

%error calculation
temp_e = max(abs(exact_f_t-approx_f), [], "all");
error = [error , temp_e ]; %adding maximum error for the particular n point case

index = log2(error(1:length(error)-1)./error(2:length(error)));
if(n>10)
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
set(gca, 'YScale', 'log')
title("Index values:")

