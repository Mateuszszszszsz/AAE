clear all;

%data init

interval_start = 1;
interval_end = 2;
t_start = 1;
y_start = -1;
error = [];
target_points = [1.04, 1.55, 1.97];
linear_error = zeros(1,3);
interpolated_value = zeros(1,3);
h1= 0.05;
desired_accuracy = 0.001;


%function init

f = @(t,y) 1/t^2-y/t-y^2;
exact_f =  @(t) -1/t;

%method choice 'explicit', 'implicit'

method = 'explicit';

%solving start


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
                a2 = (h1*(t(k+1))^2);
                a1 = (t(k+1))^2+h1*t(k+1);
                a0 = (-approx_f(k)*(t(k+1))^2-h1);
                estimated = (-a1+sqrt(a1^2-4*a2*a0))/(2*a2);
                approx_f = [approx_f, estimated];
            end
   
       end


%plotting figure

figure(1);
plot(t, approx_f, '.', t ,exact_f_t, '-');
title("Grid: " + h1);
lgd = legend("approximated", "exact","location",'southeast' );
title(lgd, "Solution:");
xlim([1, 2]);

%error calculation
temp_e = max(abs(exact_f_t-approx_f), [], "all");
error = [error , temp_e ]; %adding maximum error for the particular n point case

fprintf(1, "For n: %f" , n)
fprintf(1, " maximum error is : %f \n", temp_e)


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

% grid width for desired accuracy
aqquired_error = inf;
h2= 1;
while aqquired_error > desired_accuracy
    h2 = h2 / 2;
    n = abs(interval_start - interval_end)/h2;
t2 = linspace(interval_start, interval_end, n+1);

exact_f_t = zeros(1, round(n));
 for k = 1:n+1
 exact_f_t(k) = exact_f(t2(k));
    end

    switch method
        case'explicit'
                approx_f = [];
                approx_f(1) = y_start;
                for k = 1:n
                    estimated = approx_f(k) + h2*f(t2(k), approx_f(k));
                    approx_f = [approx_f, estimated];
                end
        case'implicit'
        approx_f = [];
                approx_f(1) = y_start;
                for k = 1:n
                    a2 = (h2*(t2(k+1))^2);
                    a1 = (t2(k+1))^2+h2*t2(k+1);
                    a0 = (-approx_f(k)*(t2(k+1))^2-h2);
                    estimated = (-a1+sqrt(a1^2-4*a2*a0))/(2*a2);
                    approx_f = [approx_f, estimated];
                end
    end
    aqquired_error = max(abs(approx_f - exact_f_t)); 
end

fprintf(1, "To get accuracy: %f gird needs to be set: %f and the maximum error is: %f \n", desired_accuracy, h2, aqquired_error)
figure(2);
plot(t2, approx_f, '.', t2 ,exact_f_t, '-');
title("Desired accuracy: " + desired_accuracy);
subtitle("Grid: " + h2);
lgd = legend("approximated", "exact","location",'southeast' );
title(lgd, "Solution:");
xlim([1, 2]);

    %function definition

function value = lin_int(x0,y0,x1,y1,x)
delta = (y1-y0)*(x-x0)/(x1-x0);
value = y0 +delta;
end