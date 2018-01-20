#include <MonoGlobal.h>
#include <string.h>

Void taskReceiveNewImage(UArg a0)
{
        while(TRUE)
        {
                //Cache_inv((xdc_Ptr)&inputBuffer,sizeof(inputBuffer),Cache_Type_ALL,1);
                #ifdef DEBUG
                //log processing time
                unsigned int t_start = Timestamp_get32();
                #endif
                //Copy debug image into buffer;
                // memcpy(&inputBuffer.buffer[inputBuffer.headId%IMG_BUFFER_SIZE].buffer,debug_img,IMG_SIZE);
                // inputBuffer.buffer[inputBuffer.headId%IMG_BUFFER_SIZE].FrameId = inputBuffer.headId;
                // inputBuffer.headId++;

                // //Threshold
                // taskThreshold();

                // //Binarize
                // taskBinarize();

                // //Connected component analysis
                // taskConnectedComponentAnalysis();

                // //Blob Analysis
                // taskBlobAnalysis();

                //Pose Calculation
                poseCalc(&debug_pos,&pose);
                
                #ifdef DEBUG
                debug_imageReceive_time = (float)(Timestamp_get32() - t_start)/1000;
                #endif
                //Cache_wbAll();
        }
}
