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

Void taskBlobAnalysis()
{
    int i=0,j=0;
    unsigned int size;
    unsigned int cnt;
    int status;
    status = VLIB_getNumCCs(&ccBuffer.buffer[ccBuffer.tailId],&cnt);
    blobBuffer.buffer[blobBuffer.headId].blobList = malloc(cnt*sizeof(VLIB_blob));
    status = VLIB_createBlobList(&ccBuffer.buffer[ccBuffer.tailId],&(blobBuffer.buffer[blobBuffer.headId]));
    status = VLIB_getblobIIBufSize(IMG_HEIGHT,blobBuffer.buffer[blobBuffer.headId].maxNumItervals,&size);
    unsigned char *pBuf = malloc(size);
    unsigned char *pBufCCMap = malloc(IMG_WIDTH*IMG_HEIGHT);
    status = VLIB_createCCMap8Bit(&ccBuffer.buffer[ccBuffer.tailId],pBufCCMap,IMG_WIDTH,IMG_HEIGHT);
    unsigned int perimeter;
    float ratio;
    int n = blobBuffer.buffer[blobBuffer.headId].numBlobs;
    Coord *pPoints = malloc(n*sizeof(Coord));
    for(i=0;i<blobBuffer.buffer[blobBuffer.headId].numBlobs;i++)
    {
        status = VLIB_createBlobIntervalImg(&ccBuffer.buffer[ccBuffer.tailId],(AVMii *)pBuf,&blobBuffer.buffer[blobBuffer.headId].blobList[i]);
        VLIB_CC cc;
        status = VLIB_getCCFeatures(&ccBuffer.buffer[ccBuffer.tailId],&cc,i);
        status = VLIB_calcBlobPerimeter(i+1,IMG_WIDTH,pBuf,pBufCCMap,&perimeter);
        ratio =(float)(perimeter*perimeter)/cc.area;
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
                if(product>0.95)
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
    memcpy(posBuffer.buffer,pPoints,TARGET_NUM*sizeof(Coord));
    

    ccBuffer.tailId++;
    blobBuffer.headId++;
    blobBuffer.tailId++;
    posBuffer.headId++;

    free(varX);
    free(varY);
    free(dist);
    free(pBuf);
    free(pBufCCMap);
    
}
