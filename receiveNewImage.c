#include <MonoGlobal.h>
#include <string.h>

Void taskReceiveNewImage(UArg a0)
{
        #ifdef DEBUG
        //log processing time
        unsigned int t_start = Timestamp_get32();

        #endif
        //Copy debug image into buffer;
        memcpy(&inputBuffer.inputFrames[inputBuffer.headId%IMG_BUFFER_SIZE].buffer,debug_img,IMG_SIZE);
        inputBuffer.inputFrames[inputBuffer.headId%IMG_BUFFER_SIZE].FrameId = inputBuffer.headId;
        inputBuffer.headId++;

        #ifdef DEBUG
        debug_imageReceive_time = (float)(Timestamp_get32() - t_start)/1000;
        #endif
}
