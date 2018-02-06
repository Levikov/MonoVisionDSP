#include <MonoGlobal.h>
#pragma DATA_ALIGN(buffer_CC,8)
unsigned char buffer_CC[20695748];
void connectedComponent(unsigned int * binary, VLIB_CCHandle *ccHandle)
{
        int size;
        VLIB_calcConnectedComponentsMaxBufferSize(IMG_WIDTH,IMG_HEIGHT,MIN_AREA,&size);
        VLIB_initConnectedComponentsList(ccHandle,buffer_CC, size);
        VLIB_createConnectedComponentsList(ccHandle,IMG_WIDTH, IMG_HEIGHT,binary,MIN_AREA, 1);
}
