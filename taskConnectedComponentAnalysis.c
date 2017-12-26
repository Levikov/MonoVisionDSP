#include <MonoGlobal.h>

void taskConnectedComponentAnalysis()
{
        unsigned int gateKey;
        GateAll_Handle gateAll;
        GateAll_Params prms;
        Error_Block eb;
        Error_init(&eb);
        GateAll_Params_init(&prms);
        gateAll = GateAll_create(&prms,&eb);
        gateKey = GateAll_enter(gateAll);

    int status =VLIB_createConnectedComponentsList(
                test,
                IMG_WIDTH,IMG_HEIGHT,
                binaryBuffer.buffer[binaryBuffer.tailId%IMG_BUFFER_SIZE].buffer,
                900,1);
        GateAll_leave(gateAll,gateKey);

        VLIB_initConnectedComponentsList(test,
                (void *)buf,IMG_SIZE);

        ccBuffer.buffer[ccBuffer.headId] = test;
        binaryBuffer.tailId++;
        ccBuffer.headId++;
        System_printf("CC buffer head id = %d\n",ccBuffer.headId);
}
