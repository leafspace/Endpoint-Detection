[wav_file,fs,bits]=wavread('filename.wav');

L=length(wav_file);                    %计算音频信号的长度
noise=0.05*randn(L,1);                 %产生等长度的随机噪声信号(这里的噪声的大小取决于随机函数的幅度倍数）可修改数值 
wav_file_handle=wav_file+noise;        %将两个信号叠加成一个新的信号——加噪声处理

figure(1)
subplot(2,1,1);
plot(wav_file);
title('原始语音信号波形');
xlabel('时间轴')
ylabel('幅值')
subplot(2,1,2);
plot(wav_file_handle);
title('原始语音信号加噪后波形');
xlabel('时间轴');
ylabel('幅值');

sound(wav_file_handle,fs);
wavwrite(wav_file_handle,fs,bits,'测试语音-随机噪声.wav');