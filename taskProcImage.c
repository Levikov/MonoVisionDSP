#include <MonoGlobal.h>
#include <string.h>

const char *status_string[4] = {"NORM","FEWTAR","NOTAR","NOLINE"};

void taskProcImage()
{
#ifndef POSE_CALC_TEST
        VLIB_CCHandle ccHandle;
        double points[3][TARGET_NUM];
        Pose pose = {{0,0,0},{0,0,0}};
        static unsigned char i=0;
        int sizeCC;
        char status;

        if(*emifFlagAddr==1)
        {
        i++;
        // Copy debug image into buffer;
        recvEMIF(emifRecvAddr,image);

        //Threshold
        IMG_thr_le2min_8(image,threshold,IMG_WIDTH,IMG_HEIGHT,IMG_THRES);

        //Binarize
        VLIB_packMask32(threshold,binary,IMG_SIZE);

        //Connected component analysis
        connectedComponent(binary,&ccHandle);

        //Blob Analysis
        status = blob(&ccHandle,&points);
        if(status)
        goto emifSend;

        //Pose Calculation
        poseCalc(&points,&pose);

        //Send result
        emifSend:
        sendEMIF(emifSendAddr,&pose,status);
        printf("%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%s\n",
                i,
                pose.R.pitch,
                pose.R.roll,
                pose.R.yaw,
                pose.T.X,
                pose.T.Y,
                pose.T.Z,
                status_string[status]
                );
        }
#else
        poseCalcTest();
#endif

}
