N = 251;
t = 0:.001:.25;
x = sin(2*pi*50*t) + sin(2*pi*120*t);
y = x + 2*randn(size(t));
subplot(3,1,1)
plot(y(1:50))
title('Noisy time domain signal')

Y = fft(y,N);
Pyy = Y.*conj(Y)/N;
f = 1000/N*(0:127);
subplot(3,1,2)
plot(f,Pyy(1:128))
title('Power spectral density')
xlabel('Frequency (Hz)')

subplot(3,1,3)
plot(f(1:50),Pyy(1:50))
title('Power spectral density')
xlabel('Frequency (Hz)')