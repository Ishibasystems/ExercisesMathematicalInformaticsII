clc;
close all;

format long

N = input('��������N���܂Ōv�Z���܂�...\nN����� >> ')

x = -12 : 0.01 : 12;

% y1 = cos(x) .* (sign(cos(x)) + 1) / 2;

y2 = 1 / pi + cos(x) / 2;
for k = 1 : N
    y2 = y2 + 2 / pi * ((-1) ^ k / (1 + 2 * k) / (1 - 2 * k) * cos(2 * k * x));
end

figure(1);
plot(x, y2, '-b'); hold on
% axis([-5, 5, -5, 5]);
grid on
xlabel('x', 'FontSize', 14);
ylabel('y', 'FontSize', 14);
