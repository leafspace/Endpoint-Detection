[wav_file,fs,bits]=wavread('filename.wav');

f=fs*(0:511)/1024;
t=0:1/fs:(size(wav_file)-1)/fs;        %将所加噪声信号的点数调整到与原始信号相同
Au=0.03;
noise=[Au*cos(2*pi*5000*t)]';          %噪声为5kHz的余弦信号            可修改数值
wav_file_handle=wav_file+noise;

figure(1)
subplot(2,1,1);
plot(wav_file);
title('原始语音信号波形');
xlabel('时间轴');
ylabel('幅值');

subplot(2,1,2);
plot(wav_file_handle)
title('原始语音信号加噪后波形');
xlabel('时间轴');
ylabel('幅值');

sound(wav_file_handle,fs);             %播放加噪声后的语音信号
wavwrite(wav_file_handle,fs,bits,'测试语音高频余弦噪声.wav');