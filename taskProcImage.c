#include <MonoGlobal.h>
#include <string.h>

void taskProcImage(UArg a0)
{
        VLIB_CCHandle ccHandle;
        double points[3][TARGET_NUM];
        Pose pose = {{1/0,1/0,1/0},{1/0,1/0,1/0}};
        void *buffer_CC;
        int sizeCC;
        
        // Copy debug image into buffer;
        recvEMIF(emifRecvAddr,image);

        //Threshold
        IMG_thr_le2min_8(image,threshold,IMG_WIDTH,IMG_HEIGHT,IMG_THRES);

        //Binarize
        binarize(threshold,binary);

        //Connected component analysis
        connectedComponent(binary,&ccHandle,&buffer_CC,&sizeCC);

        //Blob Analysis
        if(blob(&ccHandle,&points)<0)
        goto emifSend;

        //Pose Calculation
        poseCalc(&points,&pose);
        
        //Send result
        emifSend:sendEMIF(emifSendAddr,&pose);

        Memory_free(NULL,buffer_CC,sizeCC);
}
