clc;
close all;

format long

% (i)
a = -2 / 3;
n = 20;

% (ii)
% a = 2;
% n = 8;

x_0 = 1;
x = 0;

for k = 1 : n
    if k == 1
        x(k) = a * x_0;
    else
        x(k) = a * x(k - 1);
    end
end

figure(1);
plot([0, 1], [x_0, x(1)], '-b'); hold on
plot(x, '-b');
grid on
xlabel('n', 'FontSize', 14);
ylabel('x', 'FontSize', 14);
