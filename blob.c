#include <MonoGlobal.h>
#include <float.h>
#include <matop.h>

void cmean(Circle *array,const int N,double *restrict mean)
{
    double sum = 0,sumAera=0;
    int i;
    for(i=0;i<N;i++)
    {
        sum+=array[i].ratio;
        sumAera+=array[i].area;
    }
    mean[0] = sum/(double)N;
    mean[1] = sumAera/(double)N;
}
void cvar(Circle *array,const double *mean,const int N,double *restrict var)
{
    double sum = 0,sumArea = 0;
    int i;
    for(i=0;i<N;i++)
    {
        sum+= (array[i].ratio - mean[0])*(array[i].ratio - mean[0]);
        sumArea+= (array[i].area - mean[1])*(array[i].area - mean[1]);
    }
    var[0] = sqrtdp(sum);
    var[1] = sqrtdp(sumArea);
}

void cmaxdev(Circle *array,const double *mean,const double *var,const int N,double *restrict maxdev,int *restrict maxdevidx)
{
    double max = 0,dev;
    int i,idx;
    for(i=0;i<N;i++)
    {
        dev = fabs(array[i].ratio - mean[0])/var[0]*fabs(array[i].area - mean[1])/var[1];
        if(dev>max)
        {
            max = dev;
            *maxdev = dev;
            *maxdevidx = i;   
        }
    }
}

void swap(Circle *a,Circle *b)
{
    Circle temp;
    temp = *a;
    *a = *b;
    *b = temp;
    return;
}

void swap_pCircle(Circle **a,Circle **b)
{
    Circle *temp;
    temp = *a;
    *a = *b;
    *b = temp;
    return;
}

void swap_f64(double *a,double *b)
{
    double temp;
    temp = *a;
    *a = *b;
    *b = temp;
    return;
}
void rank(Circle *array,const int N)
{
    int i,j;
    for(i=0;i<N-1;i++)
    {
        for(j=i+1;j<N;j++)
        if(array[i].ratio>array[j].ratio)
        swap(array+i,array+j);
    }
}

char blob(VLIB_CCHandle *ccHandle,double (* restrict points)[3][TARGET_NUM])
{
    int i=0,j=0;
    unsigned int size;
    int cnt;
    char status = 0;
    VLIB_blobList blob;
    VLIB_getNumCCs(ccHandle,&cnt);
    blob.blobList = malloc(cnt*sizeof(VLIB_blob));
    VLIB_createBlobList(ccHandle,&(blob));
    VLIB_getblobIIBufSize(IMG_HEIGHT,blob.maxNumItervals,&size);
    unsigned char *pBuf = Memory_alloc(NULL,size,8,NULL);
    unsigned char *pBufCCMap = Memory_alloc(NULL,IMG_SIZE,8,NULL);
    VLIB_createCCMap8Bit(ccHandle,pBufCCMap,IMG_WIDTH,IMG_HEIGHT);
    unsigned int perimeter;
    int n = blob.numBlobs;
    Circle * circles = malloc(n*sizeof(Circle));
    double *varX = malloc(TARGET_NUM*TARGET_NUM*sizeof(double));
    double *varY = malloc(TARGET_NUM*TARGET_NUM*sizeof(double));
    double *dist = malloc(TARGET_NUM*TARGET_NUM*sizeof(double));
   if(blob.numBlobs<TARGET_NUM)
   {
       status = 1;
       free(varX);
       free(varY);
       free(dist);
       Memory_free(NULL,pBuf,size);
       Memory_free(NULL,pBufCCMap,IMG_SIZE);
       free(blob.blobList);
       free(circles);
       return status;
   }
    for(i=0;i<blob.numBlobs;i++)
    {
        VLIB_CC cc;
        VLIB_createBlobIntervalImg(ccHandle,(AVMii *)pBuf,&blob.blobList[i]);
        VLIB_getCCFeatures(ccHandle,&cc,i);
        VLIB_calcBlobPerimeter(i+1,IMG_WIDTH,(AVMii *)pBuf,pBufCCMap,&perimeter);
        if(cc.area>MAX_AREA)
        circles[i].ratio = DBL_MAX;
        else
        circles[i].ratio =(double)(perimeter*perimeter)/cc.area;
        circles[i].X = (double)cc.xsum/cc.area;
        circles[i].Y = (double)cc.ysum/cc.area;
        circles[i].Z = 1;
        circles[i].area = cc.area;
    }
    rank(circles,n);
    double mean[2],var[2],maxvar;
    int maxvaridx;
    for(i=TARGET_NUM;i<n;i++)
    {
        cmean(circles,TARGET_NUM,mean);
        cvar(circles,mean,TARGET_NUM,var);
        cmaxdev(circles,mean,var,TARGET_NUM,&maxvar,&maxvaridx);
        if(maxvar<1&&var[0]<3&&var[1]<0.3*mean[1])break;
        else
        {
            swap(circles+maxvaridx,circles+i);
        }
    }

    cmean(circles,TARGET_NUM,mean);
    cvar(circles,mean,TARGET_NUM,var);
    cmaxdev(circles,mean,var,TARGET_NUM,&maxvar,&maxvaridx);
    if(maxvar>=1||var[0]>=3||var[1]>=0.5*mean[1])
    {
        status = 2;
        goto end;
    }



    int k=0;
    for(i=0;i<TARGET_NUM;i++)
    for(j=0;j<TARGET_NUM;j++)
    {
        varX[k] = circles[i].X - circles[j].X;
        varY[k] = circles[i].Y - circles[j].Y;
        dist[k] = sqrtsp(varX[k]*varX[k]+varY[k]*varY[k]);
        k++;
    }

    int flag = 1;
    status = 3;
    Circle *aux[TARGET_NUM];
    for(i=0;i<TARGET_NUM;i++)
        aux[i] = circles + i;
    for(i=0;i<TARGET_NUM&&flag;i++)
    for(j=0;j<TARGET_NUM&&flag;j++)
    {
        if(j==i)continue;
        for(k=0;k<TARGET_NUM&&flag;k++)
        {
            if(k==j||k==i)continue;
            else
            {
                double product = (varX[j*TARGET_NUM+i]*varX[k*TARGET_NUM+j]+varY[j*TARGET_NUM+i]*varY[k*TARGET_NUM+j])/dist[j*TARGET_NUM+i]/dist[k*TARGET_NUM+j];
                if(product>LINE_DETECT_THRESHOLD)
                {
                        flag = 0;
                        status = 0;
                }
                else continue;
            }
        }
    }
    if(flag)goto end;

    swap(circles,aux[i-1]);
    swap_pCircle(aux,aux+i-1);
    swap(circles+1,aux[j-1]);
    swap_pCircle(aux+1,aux+j-1);
    swap(circles+2,aux[k-1]);

    double vec_a[3],vec_b[3],vec_c[3];
    vec_a[0] = circles[2].X - circles[0].X;
    vec_a[1] = circles[2].Y - circles[0].Y;
    vec_a[2] = 0;
    vec_b[0] = circles[3].X - circles[0].X;
    vec_b[1] = circles[3].Y - circles[0].Y;
    vec_b[2] = 0;

    DSPF_dp_vec_cross(vec_a,vec_b,vec_c);
    if(vec_c[2]<0)
    swap(circles,circles+2);

    vec_a[0] = circles[3].X - circles[0].X;
    vec_a[1] = circles[3].Y - circles[0].Y;
    vec_a[2] = 0;
    vec_b[0] = circles[4].X - circles[0].X;
    vec_b[1] = circles[4].Y - circles[0].Y;
    vec_b[2] = 0;
    DSPF_dp_vec_cross(vec_a,vec_b,vec_c);
    if(vec_c[2]>0)
    swap(circles+3,circles+4);

    for(i=0;i<TARGET_NUM;i++)
    {
        (*points)[0][i] = circles[i].X; 
        (*points)[1][i] = circles[i].Y; 
        (*points)[2][i] = circles[i].Z; 
    }
end:
    free(varX);
    free(varY);
    free(dist);
    Memory_free(NULL,pBuf,size);
    Memory_free(NULL,pBufCCMap,IMG_SIZE);
    free(blob.blobList);
    free(circles);

    return status;
}
