clc;
close all;

format long

figure(1);
grid on
xlabel('x', 'FontSize', 14);
ylabel('y', 'FontSize', 14);
hold on
axis square

N = 10000

count = 0;

for k = 0 : N
    x = 2 * (rand - 0.5);
    y = 2 * (rand - 0.5);
    if x ^ 2 + y ^ 2 <= 1
        count = count + 1;
        plot(x, y, 'r', 'Marker', '+');
    else
        plot(x, y, 'b', 'Marker', '+');
    end
end

dtheta = 2 * pi /100;
theta = 0 : dtheta : 2 * pi;
plot(cos(theta), sin(theta), 'g');

axis([-1, 1, -1, 1]);

count

ANS = 4 * count / N
