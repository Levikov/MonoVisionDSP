#include "MonoGlobal.h"
#include <xdc/std.h>

Void taskImageSegment(UArg a0)
{
        #ifdef DEBUG
        //log processing time
        unsigned int t_start = Timestamp_get32();
        #endif
        Cache_inv((xdc_Ptr)&(inputBuffer.headId),4,Cache_Type_ALLD,1);
        Cache_inv((xdc_Ptr)&(thresholdBuffer.tailId),4,Cache_Type_ALLD,1);
        if((inputBuffer.headId - inputBuffer.tailId)%IMG_BUFFER_SIZE==4)
        {

                unsigned char *p = (inputBuffer.buffer[inputBuffer.tailId%IMG_BUFFER_SIZE].buffer);
                unsigned char *q = (thresholdBuffer.buffer[thresholdBuffer.headId%IMG_BUFFER_SIZE].buffer);
                IMG_thr_le2min_8(p,q,IMG_WIDTH,IMG_HEIGHT,IMG_THRES);
                inputBuffer.tailId++;
                thresholdBuffer.headId++;
                Cache_wb((xdc_Ptr)&(inputBuffer.tailId),4,Cache_Type_L1D,1);
                Cache_wb((xdc_Ptr)&(inputBuffer.tailId),4,Cache_Type_L2D,1);
                Cache_wb((xdc_Ptr)&(thresholdBuffer.headId),4,Cache_Type_L1D,1);
                Cache_wb((xdc_Ptr)&(thresholdBuffer.headId),4,Cache_Type_L2D,1);
        }
        

        #ifdef DEBUG
        debug_imageSegment_time = (float)(Timestamp_get32() - t_start)/1000;
        Cache_wb((xdc_Ptr)&(debug_imageSegment_time),sizeof(float),Cache_Type_L1D,1);
        Cache_wb((xdc_Ptr)&(debug_imageSegment_time),sizeof(float),Cache_Type_L2D,1);
        #endif

}
