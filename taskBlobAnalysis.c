#include <MonoGlobal.h>

Void taskBlobAnalysis()
{
    int i=0;
    unsigned int size;
    VLIB_createBlobList(ccBuffer.buffer[ccBuffer.tailId],&(blobBuffer.buffer[blobBuffer.headId]));
    VLIB_getblobIIBufSize(IMG_HEIGHT,blobBuffer.buffer[blobBuffer.headId].maxNumItervals,&size);
    unsigned char pBuf[size];
    for(i=0;i<blobBuffer.buffer[blobBuffer.headId].numBlobs;i++)
    {
        VLIB_blob b;
        VLIB_createBlobIntervalImg(ccBuffer.buffer[ccBuffer.tailId],(AVMii *)pBuf,&b);
        
    }
    ccBuffer.tailId++;
    blobBuffer.headId++;
    
}
