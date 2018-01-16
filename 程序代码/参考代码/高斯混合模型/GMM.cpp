 1 #include "GMM.h"
  2 #include <iostream>
  3 #include <fstream>
  4 #include <stdlib.h>
  5 #include <Windows.h>
  6 using namespace std;
  7 
  8 void GMM::Init( const vector<double> &inputData,const int clustNum,double eps,double max_steps )
  9 {
 10     this->data=inputData;
 11     this->dataNum=data.size();
 12     this->clusterNum=clustNum;
 13     this->epslon=eps;
 14     this->max_steps=max_steps;
 15     this->means.resize(clusterNum);
 16     this->means_bkp.resize(clusterNum);
 17     this->sigmas.resize(clusterNum);
 18     this->sigmas_bkp.resize(clusterNum);
 19     this->memberships.resize(clusterNum);
 20     this->memberships_bkp.resize(clusterNum);
 21     for (int i=0;i<clusterNum;i++)
 22     {
 23         memberships[i].resize(data.size());
 24         memberships_bkp[i].resize(data.size());
 25     }
 26     this->probilities.resize(clusterNum);
 27     this->probilities_bkp.resize(clusterNum);
 28     //initialize mixture probabilities
 29     for (int i=0;i<clusterNum;i++)
 30     {
 31         probilities[i]=probilities_bkp[i]=1.0/(double)clusterNum;
 32         //init means
 33         means[i]=means_bkp[i]=255.0*i/(clusterNum);
 34         //init sigma
 35         sigmas[i]=sigmas_bkp[i]=50;
 36     }
 37 }
 38 void GMM::train()
 39 {
 40     //compute membership probabilities
 41     int i,j,k,m;
 42     double sum=0,sum2;
 43     int steps=0;
 44     bool go_on;
 45     do 
 46     {
 47     for (k=0;k<clusterNum;k++)
 48     {
 49         //compute membership probabilities
 50         for (j=0;j<data.size();j++)
 51         {
 52             //计算p(k|n)
 53             sum=0;
 54             for (m=0;m<clusterNum;m++)
 55             {
 56                 sum+=probilities[m]*gauss(data[j],means[m],sigmas[m]);
 57             }
 58             //求分子
 59             memberships[k][j]=probilities[k]*gauss(data[j],means[k],sigmas[k])/sum;
 60         }
 61         //求均值
 62             //求条件概率的和
 63         sum=0;
 64         for (i=0;i<dataNum;i++)
 65         {
 66             sum+=memberships[k][i];
 67         }
 68         sum2=0;
 69         for (j=0;j<dataNum;j++)
 70         {
 71             sum2+=memberships[k][j]*data[j];
 72         }
 73         means[k]=sum2/sum;
 74         //求方差
 75         sum2=0;
 76         for (j=0;j<dataNum;j++)
 77         {
 78             sum2+=memberships[k][j]*(data[j]-means[k])*(data[j]-means[k]);
 79         }
 80         sigmas[k]=sqrt(sum2/sum);
 81         //求概率
 82         probilities[k]=sum/dataNum;
 83     }//end for k
 84     //check improvement
 85     go_on=false;
 86     for (k=0;k<clusterNum;k++)
 87     {
 88         if (means[k]-means_bkp[k]>epslon)
 89         {
 90             go_on=true;
 91             break;
 92         }
 93     }
 94     //back up
 95     this->means_bkp=means;
 96     this->sigmas_bkp=sigmas;
 97     this->probilities_bkp=probilities;
 98     } while (go_on&&steps++<max_steps);//end do while
 99 }
100 
101 double GMM::gauss( const double x,const double m,const double sigma )
102 {
103     return 1.0/(sqrt(2*3.1415926)*sigma)*exp(-0.5*(x-m)*(x-m)/(sigma*sigma));
104 }
105 int GMM::predicate(double x)
106 {
107     double max_p=-100;
108     int i;
109     double current_p;
110     int bestIdx=0;
111     for (i=0;i<clusterNum;i++)
112     {
113         current_p=gauss(x,means[i],sigmas[i]);
114         if (current_p>max_p)
115         {
116             max_p=current_p;
117             bestIdx=i;
118         }
119     }
120     return bestIdx;
121 }
122 void GMM::print()
123 {
124     int i;
125     for (i=0;i<clusterNum;i++)
126     {
127         cout<<"Mean: "<<means[i]<<" Sigma: "<<sigmas[i]<<" Mixture Probability: "<<probilities[i]<<endl;
128     }
129 }
130 //void mainxx()
131 //{
132 //    GMM gmm;
133 //    gmm.test();
134 //}
135 //void GMM::test()
136 //{
137 //    //test guass
138 //    for (int j=0;j<10;j++)
139 //    {
140 //        cout<<gauss(j,0,1)<<endl;
141 //    }
142 //    srand(GetTickCount());
143 //    vector<double> datas;
144 //    int i;
145 //    double d;
146 //    for (i=0;i<20;i++)
147 //    {
148 //        d=(rand()/(double)RAND_MAX)*255;
149 //        cout<<d<<endl;
150 //        datas.push_back(d);
151 //    }
152 //    this->Init(datas,2);
153 //    this->train();
154 //    this->predicate(100);
155 //}