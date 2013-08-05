clc;
close all;

format long

x = 0 : 0.001 : 1;

for n = 1 : 10
    figure(n)
    T = 0;
    for k = 1 : n
        t = x;
        for t_ = 1 : k
            t = 2 .* t .* sign(0.5 - t) + sign(t - 0.5) + 1;
        end
        T = T + t / (2 ^ k);
    end
    plot(x, T)
end
