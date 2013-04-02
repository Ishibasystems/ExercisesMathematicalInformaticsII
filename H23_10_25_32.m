clc;
close all;

format long

% (i)
A = [-1 / 3, 1 / 3; 1 / 3, 1 / 6];
n = 8;

% (ii)
% A = [2, 5; 2, -1];
% n = 6;

X_0 = [1; 1];

X = zeros(2, n);

for k = 1 : n
    if k == 1
        X(:, k) = A * X_0;
    else
        X(:, k) = A * X(:, k - 1);
    end
end

[D, V] = eig(A);
abs(V)

figure(1);
plot([0, 1], [X_0(2), X(2, 1)], '-b'); hold on
plot(X(2, :), '-b');
plot([0, 1], [X_0(1), X(1, 1)], '-r');
plot(X(1, :), '-r');
grid on
xlabel('n', 'FontSize', 14);
ylabel('x', 'FontSize', 14);

figure(2);
plot([X_0(1), X(1, 1)], [X_0(2), X(2, 1)], '-b'); hold on
plot(X(1, :), X(2, :), '-b'); hold on
grid on
xlabel('x1', 'FontSize', 14);
ylabel('x2', 'FontSize', 14);
