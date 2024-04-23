clear all;

%data init

interval_start = 1;
interval_end = 2;
t_start = 1;
y_start = -1/3;
error = [];
ratio = [];
target_points = [1.03, 1.55, 1.97];
linear_error = zeros(1,3);
interpolated_value = zeros(1,3);
h1= 0.02;


%function init

f = @(t,y) 2/((2*t+1)^2) - y/(2*t+1) - y^2;
exact_f =  @(t) -1/(2*t+1);

%method choice 'explicit', 'implicit'

method = 'explicit';

%solving start

for j=1 : 10
n =  abs(interval_start - interval_end) / h1; %number of samples calculation
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
                estimated = approx_f(k) + h1*f(t(k), approx_f(k));
                approx_f = [approx_f, estimated];
            end
    case'implicit'
    approx_f = [];
            approx_f(1) = y_start;
            for k = 1:n
                a2 = (h1);
                a1 = 1+h1/(2*t(k+1)+1);
                a0 = -approx_f(k)-2*h1/(2*t(k+1)+1)^2;
                estimated = (-a1+sqrt(a1^2-4*a2*a0))/(2*a2);
                approx_f = [approx_f, estimated];
            end
   
   end


%plotting figure

figure(n);
plot(t, approx_f, '.', t ,exact_f_t, '-');
title("Grid: " + h1);
lgd = legend("approximated", "exact","location",'southeast' );
title(lgd, "Solution:");
xlim([1, 2]);

%error calculation
temp_e = max(abs(exact_f_t-approx_f), [], "all");
error = [error , temp_e ]; %adding maximum error for the particular n point case



if(j>1)
    ratio = [ratio, error(j-1)/error(j)]; %log2(error(1:length(error)-1)./error(2:length(error)));
fprintf(1, "For n: %f" , n)
fprintf(1, " points ratio of error is: %f" , ratio(length(ratio)))
fprintf(1, " with maximum error: %f \n", temp_e)
end


%second part (b) linear interpolation

    for i = 1:length(target_points)
        for j = 1:length(t)-1
            if t(j) <= target_points(i) && target_points(i) <= t(j+1) %check if the point is between our discrete points
                interpolated_value(i) = lin_int(t(j),approx_f(j),t(j+1),approx_f(j+1), target_points(i));
                linear_error(i) = abs((-1./target_points(i))-interpolated_value(i));
                break
            end
        end
    end

fprintf(1, "Errors due to linear interpolation: %f %f %f \n" , linear_error)
h1 = h1/2;
end

figure(2)
plot(error)
set(gca, 'YScale', 'log')
title("Maximum error values:");
figure(3)
plot(ratio)
%set(gca, 'YScale', 'log')
title("Ratio values:")


    %function definition

function value = lin_int(x0,y0,x1,y1,x)
delta = (y1-y0)*(x-x0)/(x1-x0);
value = y0 +delta;
end