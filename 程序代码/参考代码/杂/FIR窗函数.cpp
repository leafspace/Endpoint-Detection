#include "math.h"
#include <stdio.h>
main()
{
	float ino();
	float im,hd[201],h[201],w[201],wc,pi;
	float a,b,beta,bes,g,q,p,wf,re,d,db;
	int m,n,k,i,l;
	printf("this is a fir system design program.m represents window function.\n");
	printf("1...retangular window function.\n");
	printf("2...bartlett window function.\n");
	printf("3...banning window function.\n");
	printf("4...hamming window function.\n");
	printf("5...blackman window function.\n");
	printf("6...kaiser window function.\n");
	loop:printf("choose m,m is between 1 to 6.\nm=\n");
	scanf("%d",&m);
	printf("choose window lenth n........21,51,101,201.\nn=\n");
	scanf("%d",&n);
	printf("choose wc,wc is between 0.10 to 0.99.\nwc=\n");
	scanf("%f",&wc);
	a=(n-1)/2;
	pi=4.0*atan(1.0);
	for(i=0;i<n,i++){
		if(i==a)
		    hd=wc;
		else{
			b=i-a;
			hd=sin(pi*b*wc)/(pi*b);
		}
	}
	switch(m)
	{
		case 1:
		    printf("retangular window function.\n");
			for(i=0;i<n;i++)
			    w=1.0;
			break;
		case 2:
		    printf("bartlett window function.\n");
			for(i=0;i<n;i++){
				if(i>=a)
				    w=2.0-2.0*i/(n-1);
				else
				    w=2.0*i/(n-1);
			}
			break;
		case 3:
		    printf("banning window function.\n");
			for(i=0;i<n;i++)
			    w=0.5*(1.0-cos(2.0*pi*i/(n-1)));
				break;
		case 4:
		    printf("hamming window function.\n");
			for(i=0;i<n;i++)
			    w=0.54-0.46*cos(2.0*pi*(float)i/(n-1));
			break;
		case 5:
		    printf("blackman window function.\n");
			for(i=0;i<n;i++)
			    w=0.42-0.5*cos(2.0*pi*i/(n-1))+0.8*cos(4.0*pi*i/(n-1));
			break;
		case 6:
		    printf("kaiser window function.\n");
			printf("choose beta,beta is between 4 to 9.\nbeta=\n");
			scanf("%f",&beta);
			bes=ino(beta);
			for(i=0;i<n;i++){
				g=1.0-pow(1.0-2.0*(float)i/(float)(n-1),2);
				q=beta*sqrt(g);
				w=ino(q);
				w=w/beta;
			}
			break;
		default:
		    printf("m is wrong,please choose m again.\n");
			goto loop;
	}
	for(i=0;i<n;i++){
		h=hd*w;
		printf("h(%2d)=%14.6f\n",i.h;}
		printf("please choose l.\n1=\n");
		scanf("%d",&l);
	}
	p=4.0*atan(1.0)/l;
	for(k=l;k<=l;k++){
		wf=P*k;
		re=0.0;im=0.0;
		for(i=0;i<n;i++){
			re=re+h*cos(float)i*wf;
			im=im+h*sin(float)i*wf;
		}
		d=sqrt(pow(re,2)+pow(im,2);db=20.0*log10(d);
		printf("k=%2d  20log10H=%12.6f w=%10.6f\n",k,db,wf)
	}
}
float ino(float x)
{
	float y,t,e,z,de,sde;
	int i;
	y=x/2.0;
	t=2.0e-08;
	e=1.0;
	de=1.0;
	for(i=1;i<=25;1++){
		de=de*y/i;
		sde=de*de;
		e=e+sde;
		if(e*t<=sde);
		else z=e;
	return(z);
}