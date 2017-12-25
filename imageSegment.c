#include "MonoGlobal.h"
#include <xdc/std.h>

Void taskImageSegment(UArg a0)
{
        #ifdef DEBUG
        //log processing time
        unsigned int t_start = Timestamp_get32();
        #endif
        Cache_inv((xdc_Ptr)&(inputBuffer.headId),4,Cache_Type_ALLD,1);
        if(inputBuffer.tailId<inputBuffer.headId-1)
        inputBuffer.tailId++;
        Cache_wb((xdc_Ptr)&(inputBuffer.tailId),4,Cache_Type_L1D,1);
        Cache_wb((xdc_Ptr)&(inputBuffer.tailId),4,Cache_Type_L2D,1);

        #ifdef DEBUG
        debug_imageSegment_time = (float)(Timestamp_get32() - t_start)/1000;
        #endif
        
}
