#include <MonoGlobal.h>

void swap(Coord *a,Coord *b)
{
    Coord *temp = malloc(sizeof(Coord));
    *temp = *a;
    *a = *b;
    *b = *temp;
    free(temp);
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

void blob(VLIB_CCHandle ccHandle,Coord * restrict points)
{
    int i=0,j=0;
    unsigned int size;
    unsigned int cnt;
    VLIB_blobList blob;
    VLIB_getNumCCs(&ccHandle,&cnt);
    blob.blobList = malloc(cnt*sizeof(VLIB_blob));
    VLIB_createBlobList(&ccHandle,&(blob));
    VLIB_getblobIIBufSize(IMG_HEIGHT,blob.maxNumItervals,&size);
    unsigned char *pBuf = Memory_alloc(NULL,size,8,NULL);
    unsigned char *pBufCCMap = Memory_alloc(NULL,IMG_SIZE,8,NULL);
    VLIB_createCCMap8Bit(&ccHandle,pBufCCMap,IMG_WIDTH,IMG_HEIGHT);
    unsigned int perimeter;
    int n = blob.numBlobs;
    for(i=0;i<blob.numBlobs;i++)
    {
        VLIB_createBlobIntervalImg(&ccHandle,(AVMii *)pBuf,&blob.blobList[i]);
        VLIB_CC cc;
        VLIB_getCCFeatures(&ccHandle,&cc,i);
        VLIB_calcBlobPerimeter(i+1,IMG_WIDTH,pBuf,pBufCCMap,&perimeter);
        points[i].ratio =(float)(perimeter*perimeter)/cc.area;
        points[i].X = (float)cc.xsum/cc.area;
        points[i].Y = (float)cc.ysum/cc.area;
        points[i].Z = 1;
    }
    float *varX = malloc(n*n*sizeof(float));
    float *varY = malloc(n*n*sizeof(float));
    float *dist = malloc(n*n*sizeof(float));

    int k=0;
    for(i=0;i<n;i++)
    for(j=0;j<n;j++)
    {
        varX[k] = points[i].X - points[j].X;
        varY[k] = points[i].Y - points[j].Y;
        dist[k] = sqrtsp(varX[k]*varX[k]+varY[k]*varY[k]);
        k++;
    }

    int flag = 1;
    for(i=0;i<n&&flag;i++)
    for(j=0;j<n&&flag;j++)
    {
        if(j==i)continue;
        for(k=0;k<n&&flag;k++)
        {
            if(k==j||k==i)continue;
            else
            {
                float product = (varX[j*n+i]*varX[k*n+j]+varY[j*n+i]*varY[k*n+j])/dist[j*n+i]/dist[k*n+j];
                if(product>0.98)
                {
                    float rate = dist[k*n+j]/dist[j*n+i];
                    if(rate>1.0 && rate<1.4)
                        flag = 0;
                    else
                        continue;

                }
                else continue;
            }
        }
    }
    swap(points,points+i-1);
    swap(points+1,points+j-1);
    swap(points+2,points+k-1);

    int l=0;
    float mean = (points[0].ratio+points[1].ratio+points[2].ratio)/3;
    float var = sqrtsp(((points[0].ratio-mean)*(points[0].ratio-mean)+(points[1].ratio-mean)*(points[1].ratio-mean)+(points[2].ratio-mean)*(points[2].ratio-mean))/3);
    float min = 3*var;
    for(l=0;l<n;l++)
    {
        if(l==0||l==1||l==2)continue;
        else
        {
            if(abs(points[l].ratio-mean)<min)
            {
                swap(points+3,points+l);
            }
        }
        
    }
    
    free(varX);
    free(varY);
    free(dist);
    Memory_free(NULL,pBuf,size);
    Memory_free(NULL,pBufCCMap,IMG_SIZE);
    
}
