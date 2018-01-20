#include <MonoGlobal.h>

Void taskBinarize()
{
    unsigned char *p = (thresholdBuffer.buffer[thresholdBuffer.tailId%IMG_BUFFER_SIZE].buffer);
    unsigned int *q = (binaryBuffer.buffer[binaryBuffer.headId%IMG_BUFFER_SIZE].buffer);
    int i=0;
    binaryBuffer.buffer[binaryBuffer.headId%IMG_BUFFER_SIZE].FrameId = binaryBuffer.headId;
    VLIB_packMask32(p,q,IMG_SIZE);
    for(i=0;i<IMG_SIZE/32;i++)
    {
        *q = ~(*q);
        q++;
    }
    thresholdBuffer.tailId++;
    binaryBuffer.headId++;
}
