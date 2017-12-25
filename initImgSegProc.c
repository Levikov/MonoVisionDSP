#include "MonoGlobal.h"

void initImgSegProc()
{
    Error_Block eb;
    Error_init(&eb);
    Clock_Params_init(&clockParams[DNUM]);
    clockParams[DNUM].period = 1;
    clockParams[DNUM].arg = (UArg)DNUM;
    clockParams[DNUM].startFlag = TRUE;
    clockHandle[DNUM] = Clock_create(taskImageSegment,2,&clockParams[DNUM],&eb);
    Clock_start(clockHandle[DNUM]);

}
