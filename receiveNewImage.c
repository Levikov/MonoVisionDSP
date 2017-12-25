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
        thresholdBuffer.buffer[thresholdBuffer.headId%IMG_BUFFER_SIZE].FrameId = thresholdBuffer.headId;
        IMG_thr_le2min_8(p,q,IMG_WIDTH,IMG_HEIGHT,IMG_THRES);
        inputBuffer.tailId++;
        thresholdBuffer.headId++;

        //Binarize
        p = (thresholdBuffer.buffer[thresholdBuffer.tailId%IMG_BUFFER_SIZE].buffer);
        unsigned int *restrict r = (binaryBuffer.buffer[binaryBuffer.headId%IMG_BUFFER_SIZE].buffer);
        binaryBuffer.buffer[binaryBuffer.headId%IMG_BUFFER_SIZE].FrameId = binaryBuffer.headId;
        VLIB_packMask32(p,r,IMG_SIZE);
        DSP_neg32(r,r,IMG_SIZE/32);
        thresholdBuffer.tailId++;
        binaryBuffer.headId++;

        //Connected component analysis
        VLIB_createConnectedComponentsList(
                &ccBuffer.buffer[ccBuffer.headId%IMG_BUFFER_SIZE].buffer,
                IMG_WIDTH,IMG_HEIGHT,
                &binaryBuffer.buffer[binaryBuffer.tailId%IMG_BUFFER_SIZE].buffer,
                10000,8);
        unsigned char buf[IMG_SIZE];
        VLIB_initConnectedComponentsList(&ccBuffer.buffer[ccBuffer.headId%IMG_BUFFER_SIZE].buffer,
                (void *)buf,IMG_SIZE);
        

        



        ccBuffer.buffer[ccBuffer.headId%IMG_BUFFER_SIZE].FrameId = ccBuffer.headId;
        binaryBuffer.tailId++;
        ccBuffer.headId++;













        #ifdef DEBUG
        debug_imageReceive_time = (float)(Timestamp_get32() - t_start)/1000;
        #endif
        //Cache_wbAll();
}
