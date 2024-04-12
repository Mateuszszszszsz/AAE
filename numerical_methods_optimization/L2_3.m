clear all;

y = [28 ; 30 ; 21 ; 23 ; 23]; % yield of field
x = [
    1 50 18 10 ; 
    1 40 20 16 ; 
    1 35 14 10 ;  
    1 40 12 12 ; 
    1 30 16 14];
t1  = tic
a = inv(x'*x)*x'*y
t1 = toc(t1)
a = x\y

y = [28 30  21  23  23];
X1 = [50 40 35 40 30]
X2 = [18 20 14 12 16]
X3 = [10 16 10 12 14]
% yEst = [];
% yEst = x*a
% 
% S = [length(y) sum(x(:,2)) sum(x(:,3)) sum(x(:,4)); 
%     sum(x(:,2)) sum(x(:,2).^2) sum(x(:,3).^2) sum(x(:,4).^2) ];
% Y  = sum()
%yEst2 = inv(S)*Y


% nsIdx = Smoker == 0;
% smIdx = Smoker == 1;
y2 = x*a

% Plot 3D scatter plot
figure(1)
scatter3(X1, X2, X3, 100, y, 'filled');
xlabel('Days of sunshine [-]');
ylabel('Rainfall [cm]');
zlabel('Fertilizer [kg]');
title('Yield of wheat in quintals per hectare');
subtitle("Data")
colorbar;
% Add text labels for y values
for i = 1:numel(y)
    text(X1(i), X2(i), X3(i), sprintf('%.2f', y(i)), 'FontSize', 12, 'HorizontalAlignment', 'left', 'VerticalAlignment', 'baseline');
end
figure(2)
% Plot 3D scatter plot
scatter3(X1, X2, X3, 100, y2, 'filled');
xlabel('Days of sunshine [-]');
ylabel('Rainfall [cm]');
zlabel('Fertilizer [kg]');
title('Yield of wheat in quintals per hectare');
subtitle("Approximation")
colorbar;
for i = 1:numel(y)
    text(X1(i), X2(i), X3(i), sprintf('%.2f', y2(i)), 'FontSize', 12, 'HorizontalAlignment', 'left', 'VerticalAlignment', 'baseline');
end

