#ifndef CONFIG_H
#define CONFIG_H

#define PI 3.14159265358979323846

#define IMG_WIDTH 1280
#define IMG_HEIGHT 1280
#define IMG_SIZE 1638400
#define IMG_THRES 50

#define IMG_BUFFER_SIZE 8

#define TARGET_NUM 4

#define CORE_NUM 8
#define CLOCK_TICK_TIMEOUT 250000

#define EMIF_IMG 0x7000000;
#define EMIF_POSE 0x7400000;

#define POSE_CALC_METHOD_OI
#define POSE_CALC_METHOD_OI_EPSILON 10e5*DBL_EPSILON

//#define DEBUG_IMG
#define POSE_CALC_TEST_PITCH_LOW  0
#define POSE_CALC_TEST_PITCH_HIGH 0
#define POSE_CALC_TEST_PITCH_DEV  10

#define POSE_CALC_TEST_ROLL_LOW   0
#define POSE_CALC_TEST_ROLL_HIGH  0
#define POSE_CALC_TEST_ROLL_DEV   10

#define POSE_CALC_TEST_YAW_LOW    0
#define POSE_CALC_TEST_YAW_HIGH   0
#define POSE_CALC_TEST_YAW_DEV    1

#define POSE_CALC_TEST_X_LOW      0
#define POSE_CALC_TEST_X_HIGH     0
#define POSE_CALC_TEST_X_DEV      1

#define POSE_CALC_TEST_Y_LOW      0
#define POSE_CALC_TEST_Y_HIGH     0
#define POSE_CALC_TEST_Y_DEV      1

#define POSE_CALC_TEST_Z_LOW      40
#define POSE_CALC_TEST_Z_HIGH     200
#define POSE_CALC_TEST_Z_DEV      5

#define PC_DEBUG

#define FOCUS 12.5f
#define DX 5.5e-3
#define DY 5.5e-3

#endif