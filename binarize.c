#include <MonoGlobal.h>

void binarize(const unsigned char *p,unsigned int *q)
{
    int i=0;
    VLIB_packMask32(p,q,IMG_SIZE);
    for(i=0;i<IMG_SIZE/32;i++)
    {
        *(q+i) = ~(*(q+i));
    }
}
