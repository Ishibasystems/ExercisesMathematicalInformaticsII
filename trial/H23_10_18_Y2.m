clc;
close all;

format long

N = input('‰€‚©‚çN€‚Ü‚ÅŒvŽZ‚µ‚Ü‚·...\nN‚ð“ü—Í >> ')

x = -12 : 0.01 : 12;

y1 = sign(sin(x));

y2 = 0;
for k = 1 : N
    y2 = y2 + 4 / pi * sin((2 * k - 1) * x) / (2 * k - 1);
end

figure(1);
plot(x, y2, '-b', x, y1, '-r'); hold on
% axis([-5, 5, -5, 5]);
grid on
xlabel('x', 'FontSize', 14);
ylabel('y', 'FontSize', 14);
