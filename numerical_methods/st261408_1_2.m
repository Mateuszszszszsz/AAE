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

%method choice 'explicit', 'implicit', 'trapezoidal', 'midpoint'

method = 'trapezoidal';

%solving start

for n = [10 ,20 , 40, 80, 160, 320, 640, 1280] %iterating throught numers of samples
    h =  abs(interval_start - interval_end) / n; %grid size calculation
    t = linspace(interval_start, interval_end, n + 1); %time vector init

    exact_f_t = zeros(1, round(n)); %place for approximated function
for i = 1:n+1
     exact_f_t(i) = exact_f(t(i)); %vector of exact solutions in time
end

switch method
    case'explicit'
            approx_f = [];
            approx_f(1) = y_start;
            for k = 1:n
                estimated = approx_f(k) + h*f(t(k), approx_f(k));
                approx_f = [approx_f, estimated];
            end
    case'implicit'
    approx_f = [];
            approx_f(1) = y_start;
            for k = 1:n
                estimated = (approx_f(k) + h * 100 * cos(t(k+1)) - h * sin(t(k+1))) / (1 + 100*h);
                approx_f = [approx_f, estimated];
            end
    case 'trapezoidal'
    approx_f = [];
            approx_f(1) = y_start;
            for k = 1:n
                estimated = (approx_f(k)+h/2*(-100*approx_f(k)+100*cos(t(k))-sin(t(k))+100*cos(t(k+1))-sin(t(k+1))))/(1+h/2*100);
                approx_f = [approx_f, estimated];
            end
    case 'midpoint'
    approx_f = [];
            approx_f(1) = y_start;
            approx_f(2) = y_start + h * f(t_start, y_start); 
            for k = 2:n
                estimated = approx_f(k-1) + 2 * h * approx_f(k);
                approx_f = [approx_f, estimated];
            end
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


