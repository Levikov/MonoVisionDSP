#include <stdio.h>
#include <math.h>
#include "MonoGlobal.h"

#pragma DATA_ALIGN(M,8)
const double M[3][3] = {FOCUS/DX,0,IMG_WIDTH/2,0,FOCUS/DY,IMG_HEIGHT/2,0,0,1};

//Target Location Matrix
#pragma DATA_ALIGN(P, 8)
const double P[4][TARGET_NUM] = {-90, -90, -90, 0, 0, 0, 90, 90,
                -90,0,90,-90,0,90,-90,0,
                 1, 1, 1, 1,1,1,1,1,
                 0,0,0,0,0,0,0,0};

#pragma DATA_ALIGN(p, 8)
double p[4][TARGET_NUM] = {0};


int main(int argc,char **argv)
{
    Pose poseTest,pose;
    printf("case\troll\tyaw\tpitch\tZ\troll\tyaw\tpitch\tZ\t\n");
    int i=0;
    
    for(poseTest.R.roll = POSE_CALC_TEST_ROLL_LOW;
            poseTest.R.roll<= POSE_CALC_TEST_ROLL_HIGH;
            poseTest.R.roll+= POSE_CALC_TEST_ROLL_DEV)
        for(poseTest.R.yaw = POSE_CALC_TEST_YAW_LOW;
            poseTest.R.yaw<= POSE_CALC_TEST_YAW_HIGH;
            poseTest.R.yaw+= POSE_CALC_TEST_YAW_DEV)
        for(poseTest.R.pitch = POSE_CALC_TEST_PITCH_LOW;
            poseTest.R.pitch<= POSE_CALC_TEST_PITCH_HIGH;
            poseTest.R.pitch+= POSE_CALC_TEST_PITCH_DEV)
        for(poseTest.T.X = POSE_CALC_TEST_X_LOW;
            poseTest.T.X<= POSE_CALC_TEST_X_HIGH;
            poseTest.T.X+= POSE_CALC_TEST_X_DEV)
        for(poseTest.T.Y = POSE_CALC_TEST_Y_LOW;
            poseTest.T.Y<= POSE_CALC_TEST_Y_HIGH;
            poseTest.T.Y+= POSE_CALC_TEST_Y_DEV)
        for(poseTest.T.Z = POSE_CALC_TEST_Z_LOW;
            poseTest.T.Z<= POSE_CALC_TEST_Z_HIGH;
            poseTest.T.Z+= POSE_CALC_TEST_Z_DEV)
        {
            i++;
            generateCoordinates(poseTest,&p);
            poseCalc(&p,&pose);
            printf("%d\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\n",i,
            poseTest.R.roll,
            poseTest.R.yaw,
            poseTest.R.pitch,
            poseTest.T.Z,
            pose.R.roll,
            pose.R.yaw,
            pose.R.pitch,
            pose.T.Z);
        }
}