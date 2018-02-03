#include <MonoGlobal.h>
#include <float.h>
#include <matop.h>

double cmean(Circle *array,const int N)
{
    double sum = 0;
    int i;
    for(i=0;i<N;i++)
    {
        sum+=array[i].ratio;
    }
    return sum/(double)N;
}
double cvar(Circle *array,const double mean,const int N)
{
    double sum = 0;
    int i;
    for(i=0;i<N;i++)
    {
        sum+= (array[i].ratio - mean)*(array[i].ratio - mean);
    }
    return sqrtdp(sum);
}

void cmaxdev(Circle *array,const double mean,const double var,const int N,double *restrict maxdev,int *restrict maxdevidx)
{
    double max = DBL_MIN,dev;
    int i,idx;
    for(i=0;i<N;i++)
    {
        dev = abs(array[i].ratio - mean);
        if(dev>max)
        {
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
    char status = 1;
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
    if(blob.numBlobs<TARGET_NUM)
    {
        status = -1;
        goto end;
    }
    Circle * circles = malloc(n*sizeof(Circle));
    for(i=0;i<blob.numBlobs;i++)
    {
        VLIB_CC cc;
        VLIB_createBlobIntervalImg(ccHandle,(AVMii *)pBuf,&blob.blobList[i]);
        VLIB_getCCFeatures(ccHandle,&cc,i);
        VLIB_calcBlobPerimeter(i+1,IMG_WIDTH,(AVMii *)pBuf,pBufCCMap,&perimeter);
        circles[i].ratio =(double)(perimeter*perimeter)/cc.area;
        circles[i].X = (double)cc.xsum/cc.area;
        circles[i].Y = (double)cc.ysum/cc.area;
        circles[i].Z = 1;
    }
    rank(circles,n);
    double mean,var,maxvar;
    int maxvaridx;
    for(i=TARGET_NUM;i<n;i++)
    {
        mean =cmean(circles,TARGET_NUM);
        var  = cvar(circles,mean,TARGET_NUM);
        cmaxdev(circles,mean,var,TARGET_NUM,&maxvar,&maxvaridx);
        if(maxvar<3*var)break;
        else
        {
            swap(circles+maxvaridx,circles+i);
        }
    }

    if(maxvar>=3*var)
    {
        status = -2;
        goto end;
    }


    double *varX = malloc(TARGET_NUM*TARGET_NUM*sizeof(double));
    double *varY = malloc(TARGET_NUM*TARGET_NUM*sizeof(double));
    double *dist = malloc(TARGET_NUM*TARGET_NUM*sizeof(double));

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
    status = -3;
    for(i=0;i<TARGET_NUM&&flag;i++)
    for(j=0;j<TARGET_NUM&&flag;j++)
    {
        if(j==i)continue;
        for(k=0;k<TARGET_NUM&&flag;k++)
        {
            if(k==j||k==i)continue;
            else
            {
                double product = (varX[j*n+i]*varX[k*n+j]+varY[j*n+i]*varY[k*n+j])/dist[j*n+i]/dist[k*n+j];
                if(product>LINE_DETECT_THRESHOLD)
                {
                        flag = 0;
                        status = 1;
                }
                else continue;
            }
        }
    }
    if(flag)goto end;

    swap(circles,circles+i-1);
    swap(circles+1,circles+j-1);
    swap(circles+2,circles+k-1);

    double vec_a[3],vec_b[3],vec_c[3];
    vec_a[0] = circles[2].X - circles[0].X;
    vec_a[1] = circles[2].Y - circles[0].Y;
    vec_a[2] = 0;
    vec_b[0] = circles[3].X - circles[0].X;
    vec_b[1] = circles[3].Y - circles[0].Y;
    vec_b[2] = 0;

    DSPF_dp_vec_cross(vec_a,vec_b,vec_c);
    if(vec_c[2]>0)
    swap(circles,circles+2);


    for(i=0;i<TARGET_NUM;i++)
    {
        (*points)[0][i] = circles[i].X; 
        (*points)[1][i] = circles[i].Y; 
        (*points)[2][i] = circles[i].Z; 
    }
    free(varX);
    free(varY);
    free(dist);
    Memory_free(NULL,pBuf,size);
    Memory_free(NULL,pBufCCMap,IMG_SIZE);
    free(blob.blobList);
    free(circles);

    end:return status;
}
