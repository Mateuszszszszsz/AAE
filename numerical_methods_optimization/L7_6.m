clear all
close all

% Plot at the two specified points
plot_contours_and_trust_region([1; -1]);
plot_contours_and_trust_region([0; 0.5]);

function plot_contours_and_trust_region(x0)
f = @(x1, x2) 10*(x2 - x1^2)^2 + (1 - x1)^2;
hess_f = @(x1, x2) [-40*x2 + 120*x1^2 + 2, -40*x1;
               -40*x1, 20];

grad_f = @(x1,x2) [-40*x1*(x2 - x1^2) - 2*(1 - x1);
                20*(x2 - x1^2)];

m_k = @(p, x, B_k) f(x(1),x(2)) + grad_f(x(1),x(2))'*p + 0.5*p'*B_k*p;
    % Grid for contour plot
    [X, Y] = meshgrid(-3:0.1:3, -3:0.1:3);
    Z = arrayfun(@(x, y) f(x, y), X, Y);
    
    % Hessian at x0
    B_k = hess_f(x0(1),x0(2));
    
    % Plot contour of the original function
    figure;
    contour(X, Y, Z, 50);
    hold on;
    
    % Plot quadratic model contours and trust-region solutions
    for delta = 0:0.1:2
        % Quadratic model
        p = @(d) quadprog(B_k, grad_f(x0(1),x0(2)), [], [], [], [], [], [], [], optimoptions('quadprog', 'Display', 'off', 'ConstraintTolerance', delta));
        
        % Solve the trust-region problem
        p_opt = p(delta);
        
        % Plot the trust-region circle
      %  viscircles(x0', delta, 'LineStyle', '--');
        
        % Plot the solution point
        plot(x0(1) + p_opt(1), x0(2) + p_opt(2), 'ro');
        
        % Plot the quadratic model contour
        Z_quad = arrayfun(@(x, y) m_k([x; y] - x0, x0, B_k), X, Y);
        contour(X, Y, Z_quad, 50, 'LineStyle', ':');
    end
    
    % Labels and title
    xlabel('x1');
    ylabel('x2');
    title(['Trust-Region Solutions and Contour Plot at x0 = [', num2str(x0(1)), '; ', num2str(x0(2)), ']']);
    hold off;
end


