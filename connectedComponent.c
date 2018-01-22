#include <MonoGlobal.h>

void connectedComponent(unsigned int * binary, VLIB_CCHandle *ccHandle,unsigned char * bufferCC,int * size)
{
        VLIB_calcConnectedComponentsMaxBufferSize(IMG_WIDTH,IMG_HEIGHT,900,size);
        bufferCC = Memory_alloc(NULL,*size,8,NULL);
        VLIB_initConnectedComponentsList(ccHandle,bufferCC, *size);
        VLIB_createConnectedComponentsList(ccHandle,IMG_WIDTH, IMG_HEIGHT,binary,900, 1);
}
