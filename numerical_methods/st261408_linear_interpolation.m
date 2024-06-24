clear all
close all

a = 0.0;
b = pi;
refine = 5;
f = @(x) sin(x);
f_prim = @(x) cos(x);


N = [50, 100, 200, 400, 800];
er = [];
er_1 = [];

for n = N
    x_coarse = linspace(a, b, n + 1);
    points = arrayfun(f, x_coarse);
    
    x_dense = linspace(a, b, refine * (n + 1));  %  grid to compare approximation with exact values
    y_approx = linear_interpolation(x_coarse, points, x_dense);
    y_approx_prime = div(y_approx, x_dense);
    
    y_exact = arrayfun(f, x_dense);
    y_exact_prime = arrayfun(f_prim, x_dense);
    
    max_error = max(abs(y_exact - y_approx));
    er = [er max_error];
    
    errors_1 = abs(y_exact - y_approx) + abs(y_exact_prime - y_approx_prime);
    max_error_1 = max(errors_1);
    er_1 = [er_1 max_error_1];
    
    figure;
    plot(x_coarse, points, '.', x_dense ,y_approx, '-')
    xlim([0, pi]);
    lgd = legend( "exact","approximated","location",'south' );

end

er = er';
wykl = er(1:end-1) ./ er(2:end);
alpha = log2(wykl);


er_1 = er_1';
wykl_1 = er_1(1:end-1) ./ er_1(2:end);
alpha_1 = log2(wykl_1);

for i = 2:length(er)
    fprintf('For %d error = %f , alpha = %f, er_1 = %f, alpha_1 = %f \n', N(i), er(i),alpha(i-1),er_1(i),alpha_1(i-1));
end

function y_dense = linear_interpolation(x_coarse, y_coarse, x_dense)
    y_dense = zeros(size(x_dense));
    for i = 1:length(x_dense)
        for j = 1:length(x_coarse) - 1
            if x_coarse(j) <= x_dense(i) && x_dense(i) <= x_coarse(j + 1)
                y_dense(i) = y_coarse(j) + (y_coarse(j + 1) - y_coarse(j)) * ...
                    (x_dense(i) - x_coarse(j)) / (x_coarse(j + 1) - x_coarse(j));
                break;
            end
        end
    end
end
function y_prime_dense = div(y_dense, x_dense)
    y_prime_dense = zeros(size(y_dense));
    h = x_dense(2) - x_dense(1);
    y_prime_dense(2:end-1) = (y_dense(3:end) - y_dense(1:end-2)) / (2 * h);
    y_prime_dense(1) = (y_dense(2) - y_dense(1)) / h;
    y_prime_dense(end) = (y_dense(end) - y_dense(end-1)) / h;
end
