#include <MonoGlobal.h>

void taskConnectedComponentAnalysis()
{
        int status;

        status = VLIB_initConnectedComponentsList(&ccBuffer.buffer[ccBuffer.headId],buffer_CC, 19170404);
        
        status = VLIB_createConnectedComponentsList(
                &ccBuffer.buffer[ccBuffer.headId],
                IMG_WIDTH, IMG_HEIGHT,
                (unsigned int *)binaryBuffer.buffer[binaryBuffer.tailId % IMG_BUFFER_SIZE].buffer,
        900, 1);
        binaryBuffer.tailId++;
        ccBuffer.headId++;
}
