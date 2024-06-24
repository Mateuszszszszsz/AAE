clear all
close all
clc

% Set up parameters
x0 = 0;
xk = 2;
t0 = 0;
tk = 0.5;

Lamb = [0.2, 0.4, 0.6];
numberOfProbes = [20, 40,80,180,360];



for lamb = Lamb
    figure

    er = [];

    for idx = 1:numel(numberOfProbes)
        n = numberOfProbes(idx);
        disp(['lambda = ', num2str(lamb), ', n = ', num2str(n)]);

        h = (xk - x0) / n;
        k = lamb * h;
        m = floor(tk / k);

        x = linspace(x0, xk, n + 1);
        t = linspace(t0, tk, m + 1);



        % Checking stability conditions
        stable = k / (h) <= 1/2;

        if ~stable
            disp(['- stability condition: NOT OK - UNSTABLE']);
            subtitle("Lambda: "+ lamb +"Unstable")
        else
            disp(['- stability condition: OK - STABLE']);
            subtitle("Lambda: "+ lamb+ " Stable")
        end

        % initialing U
        U = zeros(n + 1, m + 1);
        U(:, 1) = u(x, 0);
        U(1, 1) = 0;
        U(:,2) = u(x, 0);

        % A matrix init
        A = diag(2*(1-lamb^2)  * ones(n - 1, 1)); 
        A = A + diag( lamb^2* ones(n - 2, 1), 1);     
        A = A + diag( lamb^2* ones(n - 2, 1), -1);  

        % moments in time
        for j = 2:m
            U(2:n, j + 1) = A * U(2:n, j)- U(2:n, j-1);
        end

        u_appr = U(:, end);
        %u_ex = u_exact(x, 0.5);
        u_ex = fourier(x,t,n,m);
        error = max(abs(u_appr - u_ex),[],"all");

        er = [er, error];
        disp(['- max error: ', num2str(error)]);

        subplot(3, 2, idx)
        plot(x, u_ex, 'DisplayName', 'Exact Solution');
        hold on
        plot(x, U(:, end), 'DisplayName', 'Approximate Solution');
        hold off
        title(['n = ', num2str(n)]);
        legend

    end

    wykl = er(1:end-1) ./ er(2:end);
    alpha = log2(wykl);

    subplot(3, 2, 6)
    plot(wykl);
    title('Convergence');
end

% Define u at boundary values
function result = u(x, t)
    if t == 0
        result = x.*(2-x);
    elseif x == 0 || x == 2
        result = 0;
    end

end

function result = fourier(x,t,n,m)
ak = zeros(5,1);

for kk=1:5
ak(kk) = integral(@(x) (2-x).*x.*sin(0.5*kk*pi*x),0,2);
end
u=zeros(n,1)
for i=1:length(x)
    Utemp = 0;
    for kk =1 :5
Utemp = Utemp + ak(kk)*sin(kk*pi*0.5*x(i))*cos(kk*pi*0.5*t(m));
   end
    u(i) = Utemp;
end

result =u;
end

