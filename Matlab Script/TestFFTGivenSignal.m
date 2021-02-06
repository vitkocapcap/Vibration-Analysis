%Read data from a specific file
%fileID = fopen('D:\eclipse-workspace\Reading Data\runnable\Matlab_runnable.txt','r'); 
fileID = fopen('C:\Users\Microsoft Windows\Desktop\Reading Data\runnable\Matlab_runnable.txt','r'); 
formatSpec = '%f';
x = fscanf(fileID,formatSpec);
fclose(fileID);

x=hampel(x,10); %filter to eliminate abnormal peak(too small or too high compared to others)
x = x-mean(x); %

NFFT = numel(x); %//number of fft bins
samplingtime = 0.5;
tAxis = linspace(0,samplingtime,NFFT); %linear space during sampling period and number of samples
dt = samplingtime/NFFT; %//sample period from time axis
fs = 1/dt;%//sample rate from sample period

Y =(abs(fft(x, NFFT))); %power spectrum

%//Calculate frequency axis
df = fs/NFFT;
fAxis = 0:df:(fs-df);


%//Plot it all
figure;

subplot(211);
plot(tAxis,x);
xlabel('Time in s');

ylabel('Amplitude');

subplot(212);
plot(fAxis(1:NFFT/2+1), Y(1:NFFT/2+1),'r')
xlabel('Frequency in Hz')

ylabel('Power')