% 语音信号的端点检测matlab实现:
[x,fs,nbits]=wavread('E:\程序\团队项目\个性化文本语音转换系统\语音\微软默认.wav');%首先打开经录好的信号
x = x / max(abs(x));%幅度归一化到[-1,1]

%参数设置
FrameLen = 256;     %帧长
inc = 90;           %未重叠部分，这里涉及到信号分帧的问题，在后边再解释。

amp1 = 10;          %短时能量阈值
amp2 = 2;           %即设定能量的两个阈值。
zcr1 = 10;          %过零率阈值
zcr2 = 5;           %过零率的两个阈值，感觉第一个没有用到。
 
minsilence = 6;     %用无声的长度来判断语音是否结束
minlen  = 15;       %判断是语音的最小长度

status  = 0;        %记录语音段的状态
count   = 0;        %语音序列的长度
silence = 0;        %无声的长度
 
%计算过零率
tmp1  = enframe(x(1:end-1), FrameLen,inc);                      %对x进行分帧，每帧长FrameLen,分得的帧数是fix(（x的长度-FrameLen+step)/step）
tmp2  = enframe(x(2:end)  , FrameLen,inc);
signs = (tmp1.*tmp2)<0;
diffs = (tmp1 - tmp2)>0.02;
zcr   = sum(signs.*diffs,2);
 
%计算短时能量
amp = sum((abs(enframe( x, FrameLen, inc))).^2, 2);             %通过把filter给去掉，发现结果差不多，所以个人感觉没必要加一个滤波器，上边出现的enframe函数放到后边分析。这里知道是求出x各帧的能量值就行。
 
%调整能量门限
amp1 = min(amp1, max(amp)/4);
amp2 = min(amp2, max(amp)/8);                                   %min函数是求最小值的，没必要说了。
 
%开始端点检测
for n=1:length(zcr)                                             %从这里开始才是整个程序的思路。Length（zcr）得到的是整个信号的帧数。
   goto = 0;
   switch status
   case {0,1}                                              % 0 = 静音, 1 = 可能开始
      if amp(n) > amp1                                     % 确信进入语音段
         x1 = max(n-count-1,1);                            % 记录语音段的起始点
         status  = 2;
         silence = 0;
         count   = count + 1;
      else 
          if amp(n) > amp2 || zcr(n) > zcr2                % 可能处于语音段
              status = 1;
              count  = count + 1;
          else                                             % 静音状态
              status  = 0;
              count   = 0;
          end
      end
   case 2,                                                 % 2 = 语音段
       if amp(n) > amp2 ||zcr(n) > zcr2                    % 保持在语音段
           count = count + 1;
       else                                                % 语音将结束
           silence = silence+1;
           if silence < minsilence                         % 静音还不够长，尚未结束
               count  = count + 1;
           else
               if count < minlen                           % 语音长度太短，认为是噪声
                   status  = 0;
                   silence = 0;
                   count = 0;
               else                                        % 语音结束
                   status  = 3;
               end
           end
       end
   case 3,
       break;
   end
end  
 
count = count-silence/2;
x2 = x1 + count -1;              %记录语音段结束点
%后边的程序是找出语音端，然后用红线给标出来，没多少技术含量，就不多说了。
subplot(3,1,1)
plot(x);
axis([1 length(x) -1 1])%限制x轴与y轴的范围。
ylabel('Speech');
line([x1*inc x1*inc], [-1 1], 'Color', 'red');
line([x2*inc x2*inc], [-1 1], 'Color', 'red');%注意下line函数的用法：基于两点连成一条直线，就清楚了。
 
subplot(3,1,2)
plot(amp);
axis([1 length(amp) 0 max(amp)])
ylabel('Energy');
line([x1 x1], [min(amp),max(amp)], 'Color', 'red');
line([x2 x2], [min(amp),max(amp)], 'Color', 'red');
 
subplot(3,1,3)
plot(zcr);
axis([1 length(zcr) 0 max(zcr)])
ylabel('ZCR');
line([x1 x1], [min(zcr),max(zcr)], 'Color', 'red');
line([x2 x2], [min(zcr),max(zcr)], 'Color', 'red');