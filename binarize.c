#include <MonoGlobal.h>

void binarize(unsigned char *p,unsigned char *q)
{
    int i=0;
    VLIB_packMask32(p,q,IMG_SIZE);
    for(i=0;i<IMG_SIZE/32;i++)
    {
        *q = ~(*q);
        q++;
    }
}
