#ifndef MONOGLOBAL_H
#define MONOGLOBAL_H

#define IMG_WIDTH 768
#define IMG_HEIGHT 768
#define IMG_SIZE 589824
#define IMG_BUFFER_SIZE 8


//===========Data Structure===========//
#pragma PACK(8)
typedef struct
{
    unsigned long long FrameId;
    unsigned char buffer[IMG_SIZE];
}Frame;

typedef struct
{
    unsigned long long headId;
    unsigned long long tailId;
    Frame *pointer[IMG_BUFFER_SIZE];
}InputBuffer;

//===========Global Variables===========//

InputBuffer inputBuffer = {0,0,0};

Frame inputFrames[IMG_BUFFER_SIZE];









#endif