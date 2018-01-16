#include <iostream>  
#include <fstream>  
#include "opencv2/opencv.hpp"  
#include "ReadWriteWav.h"  
#include <string>  
  
using namespace std;  
using namespace cv;  
  
#define MAXLENGTH 1000  
  
void wav2image(Mat &img, vector<short> wavData, int wav_start, int width, int max_amplitude)  
{  
     short max(0), min(0);  
     for (int i = 0; i < wavData.size(); i++)  
     {  
          if (wavData[i] > max)  
                max = wavData[i];  
          if (wavData[i] < min)  
                min = wavData[i];  
     }  
     cout<<max<<'\t'<<min<<endl;  
  
     max_amplitude = max_amplitude > 480 ? 480 : max_amplitude;  
  
     // normalize  
     for (int i = 0; i < wavData.size(); i++)  
     {  
         wavData[i] = (wavData[i] - min) * max_amplitude / (max - min);  
     }  
  
     int j = 0;  
     Point prePoint, curPoint;  
     if (width >= 400)  
     {  
         img.create(max_amplitude, width, CV_8UC3);  
         img.setTo(Scalar(0, 0, 0));  
         for (int i = wav_start; i < wav_start + width; i++)  
         {  
              prePoint = Point(j, img.rows - (int)wavData[i]);  
              if (j)  
                    line(img, prePoint, curPoint, Scalar(0, 255, 0), 2);  
              curPoint = prePoint;  
              j++;  
         }   
  
         if (width > MAXLENGTH)  
         {  
             cout<<"The wav is too long to show, and it will be resized to 1200"<<endl;  
             resize(img, img, Size(MAXLENGTH, img.rows));  
         }  
     }  
     else  
     {  
         img.create(max_amplitude, 400, CV_8UC3);  
         img.setTo(Scalar(0, 0, 0));  
         for (int i = wav_start; i < wav_start + width; i++)  
         {  
             prePoint = Point(j*400/width, img.rows - (int)wavData[i]);  
             circle(img, prePoint, 3, Scalar(0, 0, 255), CV_FILLED);  
             j++;  
         }  
         cout<<"The wav is too small to show, and it will be resized to 400"<<endl;  
     }  
}  
  
short calOneFrameACF(vector<short> wavFrame, int sampleRate)  
{  
    vector<float> acf;  
    acf.empty();  
  
    // calculate ACF  
    for (int k = 0; k < wavFrame.size(); k++)  
    {  
        float sum = 0.0;  
        for (int i = 0; i < wavFrame.size() - k; i++)  
        {  
            sum = sum + wavFrame[i] * wavFrame[ i + k ];  
        }  
        acf.push_back(sum);  
    }  
  
    // find the max one  
    float max(-999);  
    int index = 0;  
    for (int k = 0; k < wavFrame.size(); k++)  
    {  
        if (k > 25 && acf[k] > max)  
        {  
            max = acf[k];  
            index = k;  
        }  
    }  
    return (short)sampleRate / index;  
}  
  
int main()  
{  
    const char *wavFile = "bofang.wav";      
    vector<short> data;  
    int nodesPerFrame = 400;  
  
  
    /************* Write data to file part Start ***************/  
    fstream writeFile;  
    writeFile.open("statistics.txt", ios::out);  
    /************* Write data to file part End ***************/  
  
  
    /************* Read and show the input wave part Start ***************/  
    int sampleRate;  
    int dataLength = wav2allsample(wavFile, data, sampleRate);  
    if (!dataLength)  
    {  
        cout <<"Reading wav file error!"<<endl;  
        return -1;  
    }  
    Mat originalWave;  
    wav2image(originalWave, data, 0, dataLength, 400);  
    line(originalWave, Point(0, originalWave.rows * 0.5), Point(originalWave.cols, originalWave.rows * 0.5), Scalar(0, 0, 255), 2);  
    imshow("originalWave", originalWave);  
  
    // write data  
    writeFile<<"Filename: "<<wavFile<<endl<<"SampleRate: "<<sampleRate<<"Hz"<<endl<<"dataLength: "<<dataLength<<endl;  
  
    cout<<"Press space key to continue"<<endl;  
    while (waitKey(30) != ' ');  
    /************* Read and show the input wave part End ***************/  
  
  
    /******** Calculate energy to separate voice and unvoice part Start *********/  
    int nodeCount = 0;  
  
    // The sum must be double type  
    vector<double> energyTmp;  
    double maxEnergy(0);  
    while(nodeCount < (dataLength - nodesPerFrame))  
    {  
        double sum(0);  
        for (int i = nodeCount; i < (nodeCount + nodesPerFrame); i++)  
        {  
            sum += (double)data[i] * data[i];  
        }  
        if (sum > maxEnergy)  
        {  
            maxEnergy = sum;  
        }  
        energyTmp.push_back(sum);  
        nodeCount++;  
    }  
  
    // Transform to short type for show  
    vector<short> energy;  
  
    // Fill element of boundary  
    short tmp = (short)(energyTmp[0] * 400 / maxEnergy);  
    for (int i = 0; i < nodesPerFrame * 0.5; i++)  
    {  
        energy.push_back(tmp);  
    }  
    for (int i = 0; i < energyTmp.size(); i++)  
    {  
        energy.push_back((short)(energyTmp[i] * 400 / maxEnergy));  
    }  
    // Fill element of boundary  
    tmp = (short)(energyTmp[energyTmp.size() - 1] * 400 / maxEnergy);  
    for (int i = 0; i < nodesPerFrame * 0.5; i++)  
    {  
        energy.push_back(tmp);  
    }  
  
    // show  
    Mat showEnergy;  
    wav2image(showEnergy, energy, 0, energy.size(), 400);  
    line(showEnergy, Point(0, showEnergy.rows - 1), Point(showEnergy.cols, showEnergy.rows - 1), Scalar(0, 0, 255), 2);  
    imshow("showEnergy", showEnergy);  
    while (waitKey(30) != ' ');  
  
    // separate voice and unvoice  
    float thresVoice = 400 * 0.15;  
    line(showEnergy, Point(0, showEnergy.rows - thresVoice), Point(showEnergy.cols, showEnergy.rows - thresVoice), Scalar(0, 255, 255), 2);  
    imshow("showEnergy", showEnergy);  
    while (waitKey(30) != ' ');  
  
    // Find the Transition point and draw them  
    bool high = false;  
    vector<int> separateNode;  
    for (int i = 0; i < energy.size(); i++)  
    {  
        if ( !high && energy[i] > thresVoice)  
        {  
            separateNode.push_back(i);  
            high = true;  
            writeFile<<"UnVoice to Voice: "<<i<<endl;  
            line(showEnergy, Point(i * MAXLENGTH / dataLength, 0), Point(i * MAXLENGTH / dataLength, showEnergy.rows), Scalar(255, 255, 255), 2);  
            putText(showEnergy, "Voice", Point(i * MAXLENGTH / dataLength, showEnergy.rows * 0.5 + 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);  
            imshow("showEnergy", showEnergy);  
            while (waitKey(30) != ' ');  
        }  
        if ( high && energy[i] < thresVoice)  
        {  
            separateNode.push_back(i);  
            high = false;  
            writeFile<<"Voice to UnVoice: "<<i<<endl;  
            line(showEnergy, Point(i * MAXLENGTH / dataLength, 0), Point(i * MAXLENGTH / dataLength, showEnergy.rows), Scalar(255, 0, 0), 2);  
            putText(showEnergy, "UnVoice", Point(i * MAXLENGTH / dataLength, showEnergy.rows * 0.5 + 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);  
            imshow("showEnergy", showEnergy);  
            while (waitKey(30) != ' ');  
        }  
    }  
    /******** Calculate energy to separate voice and unvoice part End ***********/  
  
  
    /******************* Calculate all frame part Start ***************/  
    int frames = 0;  
    vector<short> allPitchFre;  
    writeFile<<"The pitch frequency is:"<<endl;  
    while(frames < 2 * dataLength / nodesPerFrame)  
    {  
        vector<short> wavFrame;  
        wavFrame.empty();  
  
        // get one frame, 400 nodes per frame, and shift 200 nodes, or overlap 200 nodes  
        int start = frames * nodesPerFrame * 0.5;  
        for (int i = start; i < start + nodesPerFrame; i++)  
            wavFrame.push_back(data[i]);  
  
        // calculate the ACF of this frame  
        float pitchFreqency = calOneFrameACF(wavFrame, sampleRate);  
        allPitchFre.push_back(pitchFreqency);  
  
        cout<<"The pitch frequency is: "<<pitchFreqency <<" Hz"<<endl;  
        writeFile<<pitchFreqency<<endl;  
  
        // show current frame in the whole wave  
        Mat originalWave;  
        wav2image(originalWave, data, 0, dataLength, 400);  
        line(originalWave, Point(0, originalWave.rows * 0.5), Point(originalWave.cols, originalWave.rows * 0.5), Scalar(0, 0, 255), 2);  
        line(originalWave, Point(start * MAXLENGTH / dataLength, 0), Point(start * MAXLENGTH / dataLength, originalWave.rows), Scalar(0, 0, 255), 2);  
        line(originalWave, Point((start + nodesPerFrame)* MAXLENGTH / dataLength, 0), Point((start + nodesPerFrame)* MAXLENGTH / dataLength, originalWave.rows), Scalar(0, 0, 255), 2);  
          
        // put the pitchFreqency of this frame in the whole wave  
        stringstream buf;  
        buf << pitchFreqency;  
        string num = buf.str();  
        putText(originalWave, num, Point(start * MAXLENGTH / dataLength, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);  
        imshow("originalWave", originalWave);  
  
        // show current frame in zoom out model  
        Mat oneSelectFrame;  
        wav2image(oneSelectFrame, wavFrame, 0, wavFrame.size(), 400);  
        imshow("oneSelectFrame", oneSelectFrame);  
  
        if (!frames)  
            while (waitKey(30) != ' ');  
  
        frames++;  
        waitKey(50);  
    }  
    cout<<"Num of frames is: "<<frames<<endl;  
    /******************* Calculate all frame part End ***************/  
  
  
    // show all pitch frequency before smooth  
    Mat showAllPitchFre;  
    wav2image(showAllPitchFre, allPitchFre, 0, allPitchFre.size(), 400);  
    putText(showAllPitchFre, "Before smooth", Point(10, showAllPitchFre.rows - 20), FONT_HERSHEY_SIMPLEX, 1, Scalar(60, 200, 255), 1);  
    imshow("showAllPitchFre", showAllPitchFre);  
  
  
    /******************* Smooth by medium filter part Start **************/  
    int kernelSize = 5;  
    vector<short> afterMedFilter;  
    short sum(0);  
    afterMedFilter.assign(allPitchFre.size(), allPitchFre[0]);  
  
    for (int k = cvFloor(kernelSize/2); k < allPitchFre.size(); k++)  
    {  
        vector<short> kernelData;  
        for (int i = -cvFloor(kernelSize/2); i < cvCeil (kernelSize/2); i++)  
            kernelData.push_back(allPitchFre[k+i]);  
        nth_element(kernelData.begin(), kernelData.begin() + cvCeil (kernelSize/2), kernelData.end());  
        afterMedFilter[k] = kernelData[cvCeil (kernelSize/2)];  
        sum += afterMedFilter[k];  
        cout<<afterMedFilter[k]<<endl;  
    }  
      
    // show all pitch frequency and mean pitch frequency after smooth  
    Mat showAfterMedFilter;  
    wav2image(showAfterMedFilter, afterMedFilter, 0, afterMedFilter.size(), 400);  
    putText(showAfterMedFilter, "After smooth", Point(10, showAfterMedFilter.rows - 20), FONT_HERSHEY_SIMPLEX, 1, Scalar(60, 200, 255), 1);  
      
    short mean = sum / (afterMedFilter.size() - cvFloor(kernelSize/2));  
    writeFile<<"The mean pitch frequency is: "<<mean<<endl;  
    stringstream buf;  
    buf << mean;  
    string num = "Mean: " + buf.str() + "Hz";  
    putText(showAfterMedFilter, num, Point(10, 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 200, 255), 2);  
    imshow("showAfterMedFilter", showAfterMedFilter);  
    /******************* Smooth by medium filter part End ***************/  
  
    while (waitKey(30) != 27);  
  
    return 0;  
}