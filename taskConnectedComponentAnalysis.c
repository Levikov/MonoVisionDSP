#include <MonoGlobal.h>

void taskConnectedComponentAnalysis()
{
        int status;
        int size;
        status = VLIB_calcConnectedComponentsMaxBufferSize(IMG_WIDTH,IMG_HEIGHT,900,&size); 
        buffer_CC = Memory_alloc(NULL,size,8,NULL);
        status = VLIB_initConnectedComponentsList(&ccBuffer.buffer[ccBuffer.headId%IMG_BUFFER_SIZE],buffer_CC, size);
        
        status = VLIB_createConnectedComponentsList(
                &ccBuffer.buffer[ccBuffer.headId%IMG_BUFFER_SIZE],
                IMG_WIDTH, IMG_HEIGHT,
                (unsigned int *)binaryBuffer.buffer[binaryBuffer.tailId % IMG_BUFFER_SIZE].buffer,
        900, 1);
        binaryBuffer.tailId++;
        ccBuffer.headId++;
}
