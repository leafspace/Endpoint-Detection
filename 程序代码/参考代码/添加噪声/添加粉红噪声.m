[wav_file,fs,bits]=wavread('filename.wav');

len=length(wav_file);
[pink_file]=wavread('pink.wav',len);
wav_file_handle=wav_file+pink_file(1:len,1)*0.3;

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

sound(wav_file_handle,fs);
wavwrite(wav_file_handle,fs,bits,'测试语音-粉红噪声.wav');