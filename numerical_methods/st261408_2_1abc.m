clear all;
a = 1.0;
b = 2.0;
t0 = a;
y0 = -1.0;
des_accuracy = 0.01;

f = @(t,y) 1/t^2-y/t-y^2;
yd = @(t) -1/t;   % exact solution

method = 'explicit';
% possible methods: 'explicit', 'implicit', 'trapezoidal', 'midpoint'

er = [];
target_points = [1.04, 1.55, 1.97];
interpolated = zeros(1, 3);
max_error = inf;
h=0.2;
while max_error > des_accuracy
    h=h/2;
    n = abs(a - b) / h;    % calculate number of steps based on step size and total time
    t = linspace(a, b, n + 1);

    ydd = zeros(1, round(n));   % exact values at times k * h
    for k = 1:n+1
        ydd(k) = yd(t(k));
    end
 
    switch method
        case 'explicit'
            ya = [];
            ya(1) = y0;
            for k = 1:n
                yar = ya(k) + h*f(t(k), ya(k));
                ya = [ya, yar];
            end

        case 'implicit'
            ya = [];
            ya(1) = y0;
            for k = 1:n
                yar =((-(1+h/(t(k+1)+1)))+sqrt((1+2*h/(t(k+1)+1))^2-4*h*((1/(t(k+1)+1))^2-ya(k))))/(2*h*t(k+1)*t(k+1));
                ya = [ya, yar];
            end
    end
%b 
    for i = 1:length(target_points)
        for j = 1:length(t)-1
            if t(j) <= target_points(i) && target_points(i) <= t(j+1)
                interpolated(i) = lin_int(t(j),ya(j),t(j+1),ya(j+1), target_points(i));
                break
            end
        end
    end
    
    ex_val = -1./target_points;
%c
   error_y = abs(ydd-ya);
   max_error = max(error_y)
end
 
 
 
 
    
    
    
    figure(1);
    plot(t, ya, '.', t, ydd, '-');
    title("Grid: " + h);

    e = max(abs(ya-ydd), [], "all");
    er = [er, e];

    wykl = er(1:length(er)-1)./er(2:length(er));
    alpha = log2(wykl);

    for i = 1:length(alpha)
        fprintf(1, "%f \n", (alpha(i)));
    end



function value = lin_int(x0,y0,x1,y1,x)
value = y0 +(y1-y0)*(x-x0)/(x1-x0);
end

