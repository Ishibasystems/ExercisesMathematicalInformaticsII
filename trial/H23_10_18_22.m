clc;
close all;

format long

N = input('��������N���܂Ōv�Z���܂�...\nN����� >> ')

x = -12 : 0.01 : 12;

y1 = 1 / 2 * (mod(x - 4, 8) - 4);
y2 = 0;
for k = 1 : N
    y2 = y2 + 4 / pi * (-1) ^ (k + 1) / k *sin(k * x * pi / 4);
end

figure(1);
plot(x, y2, '-b', x, y1, '-r'); hold on
% axis([-5, 5, -5, 5]);
grid on
xlabel('x', 'FontSize', 14);
ylabel('y', 'FontSize', 14);
