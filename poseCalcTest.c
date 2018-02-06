#include <MonoGlobal.h>
#ifdef POSE_CALC_TEST
#include <math.h>
#include <matop.h>

#ifdef PRINT_FILE
FILE *pFile;
FILE *pBinary;
#endif


void evaluatePoseCalc(Pose start,Pose dev,Pose end,double sigma,unsigned int testCase,char *title)
{
    Pose poseTest,pose;
#ifdef PRINT_FILE
    fprintf(pFile,"#%d %s with sigma = %.2f\n",testCase,title,sigma);
    fprintf(pFile,"=====================================================================================================\n");
    fprintf(pFile,"case\tpitch\troll\tyaw\tX\tY\tZ\tpitch'\troll'\tyaw'\tX'\tY'\tZ'\n");
    fprintf(pFile,"-----------------------------------------------------------------------------------------------------\n");
#endif
#ifdef PRINT_CONSOLE
    printf("%s with sigma = %.2f\n",title,sigma);
    printf("=====================================================================================================\n");
    printf("case\tpitch\troll\tyaw\tX\tY\tZ\tpitch'\troll'\tyaw'\tX'\tY'\tZ'\n");
    printf("-----------------------------------------------------------------------------------------------------\n");
#endif
    register int i=0,j,k;
    void *poly = malloc(3);
    void *state = malloc(7);
    float vector[TARGET_NUM][2];
    double error[6] = {0};
    
    DSP_urand32_init(0x12345678,poly,state);
    
    for(poseTest.T.Z = start.T.Z;
        poseTest.T.Z<=   end.T.Z;
        poseTest.T.Z+=   dev.T.Z)
    for(poseTest.T.Y = start.T.Y;
        poseTest.T.Y<=   end.T.Y;
        poseTest.T.Y+=   dev.T.Y)
    for(poseTest.T.X = start.T.X;
        poseTest.T.X<=   end.T.X;
        poseTest.T.X+=   dev.T.X)
    for(poseTest.R.pitch = start.R.pitch;
        poseTest.R.pitch<=   end.R.pitch;
        poseTest.R.pitch+=   dev.R.pitch)
    for(poseTest.R.roll  = start.R.roll;
        poseTest.R.roll <=   end.R.roll;
        poseTest.R.roll +=   dev.R.roll)
    for(poseTest.R.yaw   = start.R.yaw;
        poseTest.R.yaw  <=   end.R.yaw;
        poseTest.R.yaw  +=   dev.R.yaw)
    {
        i++;
        
        generateCoordinates(poseTest,&p);
        DSPF_sp_nrand(2*TARGET_NUM,vector,state);
        for(j=0;j<TARGET_NUM;j++)
        for(k=0;k<2;k++)
        {
            p[k][j] = p[k][j] + vector[j][k]*sigma;
        }
        poseCalc(&p,&pose);
#ifdef PRINT_FILE
        fprintf(pFile,"%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",i,
        poseTest.R.pitch,
        poseTest.R.roll,
        poseTest.R.yaw,
        poseTest.T.X,
        poseTest.T.Y,
        poseTest.T.Z,
        pose.R.pitch,
        pose.R.roll,
        pose.R.yaw,
        pose.T.X,
        pose.T.Y,
        pose.T.Z);
        fwrite(&poseTest,sizeof(Pose),1,pBinary);
        fwrite(&pose,sizeof(Pose),1,pBinary);
#endif
        printf("%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",i,
        poseTest.R.pitch,
        poseTest.R.roll,
        poseTest.R.yaw,
        poseTest.T.X,
        poseTest.T.Y,
        poseTest.T.Z,
        pose.R.pitch,
        pose.R.roll,
        pose.R.yaw,
        pose.T.X,
        pose.T.Y,
        pose.T.Z);
        error[0] += (poseTest.R.pitch - pose.R.pitch)*(poseTest.R.pitch - pose.R.pitch);
        error[1] += (poseTest.R.roll - pose.R.roll)*(poseTest.R.roll - pose.R.roll);
        error[2] += (poseTest.R.yaw - pose.R.yaw)*(poseTest.R.yaw - pose.R.yaw);
        error[3] += (poseTest.T.X - pose.T.X)*(poseTest.T.X - pose.T.X);
        error[4] += (poseTest.T.Y - pose.T.Y)*(poseTest.T.Y - pose.T.Y);
        error[5] += (poseTest.T.Z - pose.T.Z)*(poseTest.T.Z - pose.T.Z);
    }
#ifdef PRINT_FILE
    fprintf(pFile,"-----------------------------------------------------------------------------------------------------\n");
    fprintf(pFile,"RMSE\t - \t - \t - \t - \t - \t - \t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",
            sqrtdp(error[0]/i),
            sqrtdp(error[1]/i),
            sqrtdp(error[2]/i),
            sqrtdp(error[3]/i),
            sqrtdp(error[4]/i),
            sqrtdp(error[5]/i)
            );
#endif
#ifdef PRINT_CONSOLE
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("RMSE\t - \t - \t - \t - \t - \t - \t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",
            sqrtdp(error[0]/i),
            sqrtdp(error[1]/i),
            sqrtdp(error[2]/i),
            sqrtdp(error[3]/i),
            sqrtdp(error[4]/i),
            sqrtdp(error[5]/i)
            );
#endif
#ifdef PRINT_FILE
    fprintf(pFile,"=====================================================================================================\n");
    fprintf(pFile,"\n");
#endif
#ifdef PRINT_CONSOLE
    printf("=====================================================================================================\n");
    printf("\n");
#endif
    free(poly);
    free(state);
}

void poseCalcTest()
{
    Pose testCase[3];
    register int i;
#ifdef PRINT_FILE
    pFile = fopen("C:\\Users\\Victo\\OneDrive\\Project\\MonoVision\\Data\\DSPEvaluate.txt","w");
    pBinary = fopen("C:\\Users\\Victo\\OneDrive\\Project\\MonoVision\\Data\\DSPEvaluate.bin","wb");
#endif
    testCase[0].T.X = POSE_CALC_TEST_X_LOW;
    testCase[1].T.X = POSE_CALC_TEST_X_DEV;
    testCase[2].T.X = POSE_CALC_TEST_X_HIGH;
    testCase[0].T.Y = 0;
    testCase[1].T.Y = POSE_CALC_TEST_Y_DEV;
    testCase[2].T.Y = 0;
    testCase[0].T.Z = 1000;
    testCase[1].T.Z = POSE_CALC_TEST_Z_DEV;
    testCase[2].T.Z = 1000;
    testCase[0].R.pitch = 0;
    testCase[1].R.pitch = POSE_CALC_TEST_PITCH_DEV;
    testCase[2].R.pitch = 0;
    testCase[0].R.roll = 0;
    testCase[1].R.roll = POSE_CALC_TEST_ROLL_DEV;
    testCase[2].R.roll = 0;
    testCase[0].R.yaw = 0;
    testCase[1].R.yaw = POSE_CALC_TEST_YAW_DEV;
    testCase[2].R.yaw = 0;
    evaluatePoseCalc(testCase[0],testCase[1],testCase[2],POSE_CALC_TEST_SIGMA,1,"X axis test");

    testCase[0].T.X = 0;
    testCase[1].T.X = POSE_CALC_TEST_X_DEV;
    testCase[2].T.X = 0;
    testCase[0].T.Y = POSE_CALC_TEST_Y_LOW;
    testCase[1].T.Y = POSE_CALC_TEST_Y_DEV;
    testCase[2].T.Y = POSE_CALC_TEST_Y_HIGH;
    testCase[0].T.Z = 1000;
    testCase[1].T.Z = POSE_CALC_TEST_Z_DEV;
    testCase[2].T.Z = 1000;
    testCase[0].R.pitch = 0;
    testCase[1].R.pitch = POSE_CALC_TEST_PITCH_DEV;
    testCase[2].R.pitch = 0;
    testCase[0].R.roll = 0;
    testCase[1].R.roll = POSE_CALC_TEST_ROLL_DEV;
    testCase[2].R.roll = 0;
    testCase[0].R.yaw = 0;
    testCase[1].R.yaw = POSE_CALC_TEST_YAW_DEV;
    testCase[2].R.yaw = 0;
    evaluatePoseCalc(testCase[0],testCase[1],testCase[2],POSE_CALC_TEST_SIGMA,2,"Y axis test");

    testCase[0].T.X = 0;
    testCase[1].T.X = POSE_CALC_TEST_X_DEV;
    testCase[2].T.X = 0;
    testCase[0].T.Y = 0;
    testCase[1].T.Y = POSE_CALC_TEST_Y_DEV;
    testCase[2].T.Y = 0;
    testCase[0].T.Z = POSE_CALC_TEST_Z_LOW;
    testCase[1].T.Z = POSE_CALC_TEST_Z_DEV;
    testCase[2].T.Z = POSE_CALC_TEST_Z_HIGH;
    testCase[0].R.pitch = 0;
    testCase[1].R.pitch = POSE_CALC_TEST_PITCH_DEV;
    testCase[2].R.pitch = 0;
    testCase[0].R.roll = 0;
    testCase[1].R.roll = POSE_CALC_TEST_ROLL_DEV;
    testCase[2].R.roll = 0;
    testCase[0].R.yaw = 0;
    testCase[1].R.yaw = POSE_CALC_TEST_YAW_DEV;
    testCase[2].R.yaw = 0;
    evaluatePoseCalc(testCase[0],testCase[1],testCase[2],POSE_CALC_TEST_SIGMA,3,"Z axis test");

    testCase[0].T.X = 0;
    testCase[1].T.X = POSE_CALC_TEST_X_DEV;
    testCase[2].T.X = 0;
    testCase[0].T.Y = 0;
    testCase[1].T.Y = POSE_CALC_TEST_Y_DEV;
    testCase[2].T.Y = 0;
    testCase[0].T.Z = 1000;
    testCase[1].T.Z = POSE_CALC_TEST_Z_DEV;
    testCase[2].T.Z = 1000;
    testCase[0].R.pitch = POSE_CALC_TEST_PITCH_LOW;
    testCase[1].R.pitch = 1;
    testCase[2].R.pitch = POSE_CALC_TEST_PITCH_HIGH;
    testCase[0].R.roll = 0;
    testCase[1].R.roll = POSE_CALC_TEST_ROLL_DEV;
    testCase[2].R.roll = 0;
    testCase[0].R.yaw = 0;
    testCase[1].R.yaw = POSE_CALC_TEST_YAW_DEV;
    testCase[2].R.yaw = 0;
    evaluatePoseCalc(testCase[0],testCase[1],testCase[2],POSE_CALC_TEST_SIGMA,4,"pitch test");

    testCase[0].T.X = 0;
    testCase[1].T.X = POSE_CALC_TEST_X_DEV;
    testCase[2].T.X = 0;
    testCase[0].T.Y = 0;
    testCase[1].T.Y = POSE_CALC_TEST_Y_DEV;
    testCase[2].T.Y = 0;
    testCase[0].T.Z = 1000;
    testCase[1].T.Z = POSE_CALC_TEST_Z_DEV;
    testCase[2].T.Z = 1000;
    testCase[0].R.pitch = 0;
    testCase[1].R.pitch = POSE_CALC_TEST_PITCH_DEV;
    testCase[2].R.pitch = 0;
    testCase[0].R.roll = POSE_CALC_TEST_ROLL_LOW;
    testCase[1].R.roll = 1;
    testCase[2].R.roll = POSE_CALC_TEST_ROLL_HIGH;
    testCase[0].R.yaw = 0;
    testCase[1].R.yaw = POSE_CALC_TEST_YAW_DEV;
    testCase[2].R.yaw = 0;
    evaluatePoseCalc(testCase[0],testCase[1],testCase[2],POSE_CALC_TEST_SIGMA,5,"roll test");

    testCase[0].T.X = 0;
    testCase[1].T.X = POSE_CALC_TEST_X_DEV;
    testCase[2].T.X = 0;
    testCase[0].T.Y = 0;
    testCase[1].T.Y = POSE_CALC_TEST_Y_DEV;
    testCase[2].T.Y = 0;
    testCase[0].T.Z = 1000;
    testCase[1].T.Z = POSE_CALC_TEST_Z_DEV;
    testCase[2].T.Z = 1000;
    testCase[0].R.pitch = 0;
    testCase[1].R.pitch = POSE_CALC_TEST_PITCH_DEV;
    testCase[2].R.pitch = 0;
    testCase[0].R.roll = 0;
    testCase[1].R.roll = POSE_CALC_TEST_ROLL_DEV;
    testCase[2].R.roll = 0;
    testCase[0].R.yaw = POSE_CALC_TEST_YAW_LOW;
    testCase[1].R.yaw = 1;
    testCase[2].R.yaw = POSE_CALC_TEST_YAW_HIGH;
    evaluatePoseCalc(testCase[0],testCase[1],testCase[2],POSE_CALC_TEST_SIGMA,6,"yaw test");

    testCase[0].T.X = 0;
    testCase[1].T.X = POSE_CALC_TEST_X_DEV;
    testCase[2].T.X = 0;
    testCase[0].T.Y = 0;
    testCase[1].T.Y = POSE_CALC_TEST_Y_DEV;
    testCase[2].T.Y = 0;
    testCase[0].T.Z = 500;
    testCase[1].T.Z = POSE_CALC_TEST_Z_DEV;
    testCase[2].T.Z = 500;
    testCase[0].R.pitch = POSE_CALC_TEST_PITCH_LOW;
    testCase[1].R.pitch = POSE_CALC_TEST_PITCH_DEV;
    testCase[2].R.pitch = POSE_CALC_TEST_PITCH_HIGH;
    testCase[0].R.roll = POSE_CALC_TEST_ROLL_LOW;
    testCase[1].R.roll = POSE_CALC_TEST_ROLL_DEV;
    testCase[2].R.roll = POSE_CALC_TEST_ROLL_HIGH;
    testCase[0].R.yaw = POSE_CALC_TEST_YAW_LOW;
    testCase[1].R.yaw = POSE_CALC_TEST_YAW_DEV;
    testCase[2].R.yaw = POSE_CALC_TEST_YAW_HIGH;
    evaluatePoseCalc(testCase[0],testCase[1],testCase[2],POSE_CALC_TEST_SIGMA,7,"combined rotation test at 500mm");
    
    testCase[0].T.X = 0;
    testCase[1].T.X = POSE_CALC_TEST_X_DEV;
    testCase[2].T.X = 0;
    testCase[0].T.Y = 0;
    testCase[1].T.Y = POSE_CALC_TEST_Y_DEV;
    testCase[2].T.Y = 0;
    testCase[0].T.Z = 1000;
    testCase[1].T.Z = POSE_CALC_TEST_Z_DEV;
    testCase[2].T.Z = 1000;
    testCase[0].R.pitch = POSE_CALC_TEST_PITCH_LOW;
    testCase[1].R.pitch = POSE_CALC_TEST_PITCH_DEV;
    testCase[2].R.pitch = POSE_CALC_TEST_PITCH_HIGH;
    testCase[0].R.roll = POSE_CALC_TEST_ROLL_LOW;
    testCase[1].R.roll = POSE_CALC_TEST_ROLL_DEV;
    testCase[2].R.roll = POSE_CALC_TEST_ROLL_HIGH;
    testCase[0].R.yaw = POSE_CALC_TEST_YAW_LOW;
    testCase[1].R.yaw = POSE_CALC_TEST_YAW_DEV;
    testCase[2].R.yaw = POSE_CALC_TEST_YAW_HIGH;
    evaluatePoseCalc(testCase[0],testCase[1],testCase[2],POSE_CALC_TEST_SIGMA,8,"combined rotation test at 1000mm");
    
    testCase[0].T.X = 0;
    testCase[1].T.X = POSE_CALC_TEST_X_DEV;
    testCase[2].T.X = 0;
    testCase[0].T.Y = 0;
    testCase[1].T.Y = POSE_CALC_TEST_Y_DEV;
    testCase[2].T.Y = 0;
    testCase[0].T.Z = 1500;
    testCase[1].T.Z = POSE_CALC_TEST_Z_DEV;
    testCase[2].T.Z = 1500;
    testCase[0].R.pitch = POSE_CALC_TEST_PITCH_LOW;
    testCase[1].R.pitch = POSE_CALC_TEST_PITCH_DEV;
    testCase[2].R.pitch = POSE_CALC_TEST_PITCH_HIGH;
    testCase[0].R.roll = POSE_CALC_TEST_ROLL_LOW;
    testCase[1].R.roll = POSE_CALC_TEST_ROLL_DEV;
    testCase[2].R.roll = POSE_CALC_TEST_ROLL_HIGH;
    testCase[0].R.yaw = POSE_CALC_TEST_YAW_LOW;
    testCase[1].R.yaw = POSE_CALC_TEST_YAW_DEV;
    testCase[2].R.yaw = POSE_CALC_TEST_YAW_HIGH;
    evaluatePoseCalc(testCase[0],testCase[1],testCase[2],POSE_CALC_TEST_SIGMA,9,"combined rotation test at 1500mm");
#ifdef PRINT_FILE
    fclose(pFile);
    fclose(pBinary);
#endif
}


#endif
