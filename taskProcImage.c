#include <MonoGlobal.h>
#include <string.h>

void taskProcImage(UArg a0)
{
        VLIB_CCHandle ccHandle;
        double points[3][TARGET_NUM];
        Pose pose;
        void *buffer_CC;
        int sizeCC;
        
        while(true)
        {
        
        #ifdef DEBUG_IMG
        // Copy debug image into buffer;
        recvEMIF(emifRecvAddr,image);

        //Threshold
        IMG_thr_le2min_8(image,threshold,IMG_WIDTH,IMG_HEIGHT,IMG_THRES);

        //Binarize
        binarize(threshold,binary);

        //Connected component analysis
        connectedComponent(binary,&ccHandle,&buffer_CC,&sizeCC);

        //Blob Analysis
        blob(&ccHandle,&points);

        //Pose Calculation
        poseCalc(&points,&pose);
        
        #else
        Pose poseTest;
        poseTest.T.X = 0;
        poseTest.T.Y = 0;
        poseTest.T.Z = 800;
        
        for(poseTest.R.roll = POSE_CALC_TEST_ROLL_LOW;
            poseTest.R.roll<= POSE_CALC_TEST_ROLL_HIGH;
            poseTest.R.roll+= POSE_CALC_TEST_ROLL_DEV)
        for(poseTest.R.yaw = POSE_CALC_TEST_YAW_LOW;
            poseTest.R.yaw<= POSE_CALC_TEST_YAW_HIGH;
            poseTest.R.yaw+= POSE_CALC_TEST_YAW_DEV)
        for(poseTest.R.pitch = POSE_CALC_TEST_PITCH_LOW;
            poseTest.R.pitch<= POSE_CALC_TEST_PITCH_HIGH;
            poseTest.R.pitch+= POSE_CALC_TEST_PITCH_DEV)
        {
                generateCoordinates(poseTest,&p);
                poseCalc(&p,&pose);
        }
        #endif

        //Send result
        sendEMIF(emifSendAddr,&pose);

        #ifdef DEBUG_IMG
        Memory_free(NULL,buffer_CC,sizeCC);
        #endif

        };

}
