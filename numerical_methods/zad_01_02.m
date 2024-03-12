clear all;
a = 0.0;
b = pi;
t0 = a;
y0 = 1.0;

f = @(t,y) -100*y + 100*cos(t) - sin(t);
yd = @(t) cos(t);   % exact solution

method = 'trapezoidal';
% possible methods: 'explicit', 'implicit', 'trapezoidal', 'midpoint'

er = [];

for n= [10 ,20 , 40, 80, 160, 320, 640, 1280] %h = [0.4, 0.2, 0.1, 0.05, 0.025, 0.0125]
    h =  abs(a - b) / n; %calculate grid size  
    %n = abs(a - b) / h;    % calculate number of steps based on step size and total time
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
                yar = (ya(k) + h * 100 * cos(t(k+1)) - h * sin(t(k+1))) / (1 + 100*h);
                ya = [ya, yar];
            end

        case 'trapezoidal'
            ya = [];
            ya(1) = y0;
            for k = 1:n
                yar = (ya(k)+h/2*(-100*ya(k)+100*cos(t(k))-sin(t(k))+100*cos(t(k+1))-sin(t(k+1))))/(1+h/2*100);
                ya = [ya, yar];
            end

        case 'midpoint'
            ya = [];
            ya(1) = y0;
            ya(2) = y0 + h * f(t0, y0);
            for k = 2:n
                yar = ya(k-1) + 2 * h * ya(k);
                ya = [ya, yar];
            end
    end

    figure(n);
    plot(t, ya, '.', t, ydd, '-');
    title("Grid: " + h);

    e = max(abs(ya-ydd), [], "all");
    er = [er, e];

    wykl = er(1:length(er)-1)./er(2:length(er));
    alpha = log2(wykl);

    for i = 1:length(alpha)
        fprintf(1, "%f \n", (alpha(i)));
    end
end
