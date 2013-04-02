clc;
close all;

format long

x = -5 : 0.01 : 5;
y = f(x);

figure(1);
plot(x, y, '-r'); hold on
% axis([-5, 5, -5, 5]);
grid on
xlabel('x', 'FontSize', 14);
ylabel('y', 'FontSize', 14);

d = input('��������t�߂����:');
e = input('���̌덷�����:');
a = d;
b = d;
c = 0;

while f(b) * f(a) > 0
    a = a - 0.1;
    b = b + 0.1;
    if abs(a) > 20
        break
    end
end

if abs(a) > 20
	disp('#Error:a, b�_��ݒ�ł��܂���B');
else
    a_ = a;
    b_ = b;
    
    while abs(b - a) >= e
        c = (a + b) / 2;

        if f(a) * f(c) < 0
            b = c;
        else
            a = c;
        end
    end

    fprintf('�񕪖@\t\t��: %d\n', c)

    a = a_;
    b = b_;
    c = 0;

    while abs(b - a) >= e
        c = (a * f(b) - b * f(a)) / (f(b) - f(a));

        if f(a) * f(c) < 0
            b = c;
        else
            a = c;
        end
    end

    fprintf('�͂��݂����@\t��: %d\n', c)
    
    a = a_;
    b = b_;
    c = 0;

    while abs(b - a) >= e
        c = a - f(a) / (f(a + e) - f(a)) * e;
        a = b;
        b = c;
    end

    fprintf('�j���[�g���@\t��: %d\n', c)

    a = a_;
    b = b_;
    c = 0;

    while abs(b - a) >= e
        c = b - f(b) * (b - a) / (f(b) - f(a));
        a = b;
        b = c;
    end

    fprintf('�����@\t\t��: %d\n', c)
end
