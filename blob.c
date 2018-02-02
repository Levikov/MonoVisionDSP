#include <MonoGlobal.h>

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

void blob(VLIB_CCHandle *ccHandle,double (* restrict points)[3][TARGET_NUM])
{
    int i=0,j=0;
    unsigned int size;
    int cnt;
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
    Circle * point = malloc(n*sizeof(Circle));
    for(i=0;i<blob.numBlobs;i++)
    {
        VLIB_createBlobIntervalImg(ccHandle,(AVMii *)pBuf,&blob.blobList[i]);
        VLIB_CC cc;
        VLIB_getCCFeatures(ccHandle,&cc,i);
        VLIB_calcBlobPerimeter(i+1,IMG_WIDTH,(AVMii *)pBuf,pBufCCMap,&perimeter);
        point[i].ratio =(double)(perimeter*perimeter)/cc.area;
        point[i].X = (double)cc.xsum/cc.area;
        point[i].Y = (double)cc.ysum/cc.area;
        point[i].Z = 1;
    }
    rank(point,n);

    double *varX = malloc(n*n*sizeof(double));
    double *varY = malloc(n*n*sizeof(double));
    double *dist = malloc(n*n*sizeof(double));

    int k=0;
    for(i=0;i<n;i++)
    for(j=0;j<n;j++)
    {
        varX[k] = point[i].X - point[j].X;
        varY[k] = point[i].Y - point[j].Y;
        dist[k] = sqrtsp(varX[k]*varX[k]+varY[k]*varY[k]);
        k++;
    }

    int flag = 1;
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
                if(product>0.98)
                {
                        flag = 0;
                }
                else continue;
            }
        }
    }
    swap(point,point+i-1);
    swap(point+1,point+j-1);
    swap(point+2,point+k-1);

    int l=0;
    double mean = (point[0].ratio+point[1].ratio+point[2].ratio)/3;
    double var = sqrtsp(((point[0].ratio-mean)*(point[0].ratio-mean)+(point[1].ratio-mean)*(point[1].ratio-mean)+(point[2].ratio-mean)*(point[2].ratio-mean))/3);
    double min = 3*var;
    for(l=0;l<n;l++)
    {
        if(l==0||l==1||l==2)continue;
        else
        {
            if(abs(point[l].ratio-mean)<min)
            {
                swap(point+3,point+l);
            }
        }
        
    }
    for(i=0;i<TARGET_NUM;i++)
    {
        (*points)[0][i] = point[i].X; 
        (*points)[1][i] = point[i].Y; 
        (*points)[2][i] = point[i].Z; 
    }
    free(varX);
    free(varY);
    free(dist);
    Memory_free(NULL,pBuf,size);
    Memory_free(NULL,pBufCCMap,IMG_SIZE);
    free(blob.blobList);
    free(point);
}
