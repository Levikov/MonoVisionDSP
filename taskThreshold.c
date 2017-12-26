#include <MonoGlobal.h>
Void taskThreshold()
{
        unsigned char *p = (inputBuffer.buffer[inputBuffer.tailId%IMG_BUFFER_SIZE].buffer);
        unsigned char * q = (thresholdBuffer.buffer[thresholdBuffer.headId%IMG_BUFFER_SIZE].buffer);
        thresholdBuffer.buffer[thresholdBuffer.headId%IMG_BUFFER_SIZE].FrameId = thresholdBuffer.headId;
        IMG_thr_le2min_8(p,q,IMG_WIDTH,IMG_HEIGHT,IMG_THRES);
        inputBuffer.tailId++;
        thresholdBuffer.headId++;
}
