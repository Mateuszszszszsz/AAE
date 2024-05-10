clear all;
close all;

% Function initialization
a = 0;
b = 1;
y0 = 0;
y1 = 0;
k = 0;
error = zeros(5, 1);
error2 = zeros(5, 2);
her = zeros(5, 1);

for index = 1:length([50, 100, 200, 400, 800])
    n = [50, 100, 200, 400, 800];
    k = k+1;
    x = linspace(a, b, n(index)+1);
    h = (b-a)/n(index);

    % Construction of the main matrix
    M = zeros(n(index)-1);

    v0 = zeros(n(index)-1, 1);
    v1 = zeros(n(index)-1, 1);
    v2 = zeros(n(index)-1, 1);
    for i = 1:(n(index)-1)
        xr = x(i+1);
        v0(i) = 1/h*(p()+p()) + h/3*(q(xr+h/2) + q(xr-h/2));
    end

    for i = 1:(n(index)-2)
        xr = x(i+2);
        v2(i) = -1/h*p()+h/6*q(xr-h/2);
        xr = x(i+1);
        v1(i) = -1/h*p()+h/6*q(xr-h/2);
    end

    D = [-1; 0; 1];
    block = [v1, v0, v2]';
    M = spdiags(block, D, n(index)-1, n(index)-1);

    % Construction of the vector on the right-hand side
    fb = zeros(n(index)-1, 1);
    for i = 1:(n(index)-1)
        xr = x(i+1);
        fb(i) = h/2*(f(xr+h/2) + f(xr-h/2));
    end

    % Approximate solution
    [L,U] = lu(M);
    [P,~,~] = lu(M);

    fb = P * fb;
    z = L \ fb;
    yp = U \ z;

    ypp = [y0; yp; y1];

    % Vector of exact values
    yd = y(x)';

    % Global error
    error(index) = max(abs(ypp-yd));
    her(index) = h;
    [L2err, H1err] = H1error(x, h, ypp, @y, @ydx);
    error2(index, :) = [L2err, H1err];

    % Pointwise errors
    argx = x;
    argy = ypp;
    argy_exact = y(argx);
    arg_error = abs(ypp - yd);

    figure(index)
    plot(argx, argy, '.', 'Color', [0.5 0 0.5], 'DisplayName', 'Estimated solution')
    hold on
    plot(argx, argy_exact, '.', 'Color', [0 0.5 0], 'DisplayName', 'Exact solution')
    hold off
    legend()
    xlabel("t")
    ylabel("y(t)")
    plotName = strcat('Ritz-Galerkin method for n=', num2str(n(index)));
    title(plotName)
    grid on
    saveas(gcf, strcat('Results_4_3/', plotName, '.png'))

    figure(index+6)
    plot(argx, arg_error, '.', 'Color', [0.6 0.3 0], 'DisplayName', 'Error')
    legend()
    xlabel("t")
    ylabel("Error")
    plotName = strcat('Error of solution estimation for n=', num2str(n(index)));
    title(plotName)
    grid on
    saveas(gcf, strcat('Results_4_3/', plotName, '.png'))
end

% Prepare order of the error plot
loger = -log(error2(:,1)');
logher = -log(her);
pr = polyfit(logher, loger, 1);

figure(index+7)
plot(logher, loger, '.', 'Color', [0.8 0 0], 'LineStyle', '-', 'LineWidth', 1)
hold on
plot(logher, pr(1)*logher+pr(2), '.', 'Color', [0.8 0 0], 'LineStyle', '-', 'LineWidth', 1)
hold off
legend()
plotName = 'Order of the error';
title(plotName)
grid on
saveas(gcf, strcat('Results_4_3/', plotName, '.png'))

