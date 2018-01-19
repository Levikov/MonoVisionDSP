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

Void taskBlobAnalysis()
{
    int i=0,j=0;
    unsigned int size;
    unsigned int cnt;
    int status;
    status = VLIB_getNumCCs(&ccBuffer.buffer[ccBuffer.tailId%IMG_BUFFER_SIZE],&cnt);
    if(blobBuffer.buffer[blobBuffer.headId%IMG_BUFFER_SIZE].blobList!=0x00000000);
    free(blobBuffer.buffer[blobBuffer.headId%IMG_BUFFER_SIZE].blobList);
    blobBuffer.buffer[blobBuffer.headId%IMG_BUFFER_SIZE].blobList = malloc(cnt*sizeof(VLIB_blob));
    status = VLIB_createBlobList(&ccBuffer.buffer[ccBuffer.tailId%IMG_BUFFER_SIZE],&(blobBuffer.buffer[blobBuffer.headId%IMG_BUFFER_SIZE]));
    status = VLIB_getblobIIBufSize(IMG_HEIGHT,blobBuffer.buffer[blobBuffer.headId%IMG_BUFFER_SIZE].maxNumItervals,&size);
    unsigned char *pBuf = Memory_alloc(NULL,size,8,NULL);
    unsigned char *pBufCCMap = Memory_alloc(NULL,IMG_SIZE,8,NULL);
    status = VLIB_createCCMap8Bit(&ccBuffer.buffer[ccBuffer.tailId%IMG_BUFFER_SIZE],pBufCCMap,IMG_WIDTH,IMG_HEIGHT);
    unsigned int perimeter;
    int n = blobBuffer.buffer[blobBuffer.headId%IMG_BUFFER_SIZE].numBlobs;
    Coord *pPoints = malloc(n*sizeof(Coord));
    for(i=0;i<blobBuffer.buffer[blobBuffer.headId%IMG_BUFFER_SIZE].numBlobs;i++)
    {
        status = VLIB_createBlobIntervalImg(&ccBuffer.buffer[ccBuffer.tailId%IMG_BUFFER_SIZE],(AVMii *)pBuf,&blobBuffer.buffer[blobBuffer.headId%IMG_BUFFER_SIZE].blobList[i]);
        VLIB_CC cc;
        status = VLIB_getCCFeatures(&ccBuffer.buffer[ccBuffer.tailId%IMG_BUFFER_SIZE],&cc,i);
        status = VLIB_calcBlobPerimeter(i+1,IMG_WIDTH,pBuf,pBufCCMap,&perimeter);
        pPoints[i].ratio =(float)(perimeter*perimeter)/cc.area;
        pPoints[i].X = (float)cc.xsum/cc.area;
        pPoints[i].Y = (float)cc.ysum/cc.area;
        pPoints[i].Z = 1;
    }
    float *varX = malloc(n*n*sizeof(float));
    float *varY = malloc(n*n*sizeof(float));
    float *dist = malloc(n*n*sizeof(float));

    int k=0;
    for(i=0;i<n;i++)
    for(j=0;j<n;j++)
    {
        varX[k] = pPoints[i].X - pPoints[j].X;
        varY[k] = pPoints[i].Y - pPoints[j].Y;
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
    swap(pPoints,pPoints+i-1);
    swap(pPoints+1,pPoints+j-1);
    swap(pPoints+2,pPoints+k-1);

    int l=0;
    float mean = (pPoints[0].ratio+pPoints[1].ratio+pPoints[2].ratio)/3;
    float var = sqrtsp(((pPoints[0].ratio-mean)*(pPoints[0].ratio-mean)+(pPoints[1].ratio-mean)*(pPoints[1].ratio-mean)+(pPoints[2].ratio-mean)*(pPoints[2].ratio-mean))/3);
    float min = 3*var;
    for(l=0;l<n;l++)
    {
        if(l==0||l==1||l==2)continue;
        else
        {
            if(abs(pPoints[l].ratio-mean)<min)
            {
                swap(pPoints+3,pPoints+l);
            }
        }
        
    }

    memcpy(posBuffer.buffer,pPoints,TARGET_NUM*sizeof(Coord));
    

    ccBuffer.tailId++;
    blobBuffer.headId++;
    blobBuffer.tailId++;
    posBuffer.headId++;

    free(varX);
    free(varY);
    free(dist);
    Memory_free(NULL,pBuf,size);
    Memory_free(NULL,pBufCCMap,IMG_SIZE);
    free(pPoints);
    
}
