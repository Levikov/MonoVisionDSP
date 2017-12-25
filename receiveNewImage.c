#include <MonoGlobal.h>
#include <string.h>

Void taskReceiveNewImage(UArg a0)
{
        //Cache_inv((xdc_Ptr)&inputBuffer,sizeof(inputBuffer),Cache_Type_ALL,1);
        #ifdef DEBUG
        //log processing time
        unsigned int t_start = Timestamp_get32();
        #endif
        //Copy debug image into buffer;
        memcpy(&inputBuffer.buffer[inputBuffer.headId%IMG_BUFFER_SIZE].buffer,debug_img,IMG_SIZE);
        inputBuffer.buffer[inputBuffer.headId%IMG_BUFFER_SIZE].FrameId = inputBuffer.headId;
        inputBuffer.headId++;

        //Threashold
        unsigned char *p = (inputBuffer.buffer[inputBuffer.tailId%IMG_BUFFER_SIZE].buffer);
        unsigned char *restrict q = (thresholdBuffer.buffer[thresholdBuffer.headId%IMG_BUFFER_SIZE].buffer);
        IMG_thr_le2min_8(p,q,IMG_WIDTH,IMG_HEIGHT,IMG_THRES);
        inputBuffer.tailId++;
        thresholdBuffer.headId++;

        //Binarize
        p = (thresholdBuffer.buffer[thresholdBuffer.tailId%IMG_BUFFER_SIZE].buffer);
        unsigned int *restrict r = (binaryBuffer.buffer[binaryBuffer.headId%IMG_BUFFER_SIZE].buffer);
        VLIB_packMask32(p,r,IMG_SIZE);







        #ifdef DEBUG
        debug_imageReceive_time = (float)(Timestamp_get32() - t_start)/1000;
        #endif
        //Cache_wbAll();
}
