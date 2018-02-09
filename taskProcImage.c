#include <MonoGlobal.h>
#include <string.h>
#include <motionPredict.h>

Pose poseLastCorrect;
#ifdef DEBUG
const char *status_string[6] = {"NORM","FEWTAR","NOTAR","NOLINE","POSERR","TIMEOUT"};
double debug_total_cnt = 0;
double debug_total_detected = 0;
double debug_detection_rate = 0;
double debug_time = 0;
double debug_interval = 0;
#endif
void taskProcImage()
{
#ifndef POSE_CALC_TEST
        VLIB_CCHandle ccHandle;
        double points[3][TARGET_NUM];
        Pose pose = {{0,0,0},{0,0,0}};
        static unsigned char i=0;
        int sizeCC;
        char status;
        static unsigned char thres_low = IMG_THRES;
        static unsigned char thres_high = 255;

        if(*emifFlagAddr==1)
        {
        i++;
        // Copy debug image into buffer;
        recvEMIF(emifRecvAddr,image);

        //Segment
        segment(image,threshold,binary,thres_low,thres_high);

        //Connected component analysis
        connectedComponent(binary,&ccHandle);

        //Blob Analysis
        status = blob(&ccHandle,&points,&thres_low,&thres_high);
        if(status)
        goto emifSend;

        //Pose Calculation
        status = poseCalc(&points,&pose);
        if(status)
        goto emifSend;

        //Send result
        emifSend:
        sendEMIF(emifSendAddr,&pose,status);
#ifdef PRINT_CONSOLE
        debug_total_cnt++;
        if(!status)debug_total_detected++;
        debug_detection_rate = debug_total_detected / debug_total_cnt *100;
        printf("%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%s\t%.2f%%\n",
                i,
                pose.R.pitch,
                pose.R.roll,
                pose.R.yaw,
                pose.T.X,
                pose.T.Y,
                pose.T.Z,
                status_string[status],
                debug_detection_rate
                );
#endif
        if(!status)
        {
                poseLastCorrect = pose;
                if(!kalmanFlag)
                {
                        motionPredictInit(&poseLastCorrect.T);
                        kalmanFlag = 1;
                }
                else
                {
                        motionPredictCorrect(&poseLastCorrect.T);
                        kalmanFlag = 1;
                }
        }
        else
        {
                if(status == ERROR_FEWTAR || status == ERROR_NOTAR || status == ERROR_NOLINE)
                {
                        thres_low = IMG_THRES;
                        thres_high = 255;
                }
                kalmanFlag = 0;
        }
        }
#else
        poseCalcTest();
#endif

}
