int DFT(int dir,int m,double *x1,double *y1)
{
    long i,k;
    double arg;
    double cosarg,sinarg;
    double *x2=NULL,*y2=NULL;
    x2=malloc(m*sizeof(double));
    y2=malloc(m*sizeof(double));
    if(x2==NULL||y2==NULL)return(FALSE);
    for(i=0;i<m;i++)
    {
        x2[i]=0;
        y2[i]=0;
        arg=-dir*2.0*3.141592654*(double)i/(double)m;
        for(k=0;k<m;k++)
        {
            cosarg=cos(k*arg);
            sinarg=sin(k*arg);
            x2[i]+=(x1[k]*cosarg-y1[k]*sinarg);
            y2[i]+=(x1[k]*sinarg+y1[k]*cosarg);
        }
    }
/*Copythedataback*/
    if(dir==1)
    {
        for(i=0;i<m;i++)
        {
            x1[i]=x2[i]/(double)m;
            y1[i]=y2[i]/(double)m;
        }
    }
    else
    {
        for(i=0;i<m;i++)
        {
            x1[i]=x2[i];
            y1[i]=y2[i];
        }
    }
    free(x2);
    free(y2);
    return(TRUE);
}