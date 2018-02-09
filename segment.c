#include <MonoGlobal.h>
// #pragma DATA_ALIGN(pBufGradX,8)
// short pBufGradX[IMG_HEIGHT][IMG_WIDTH];
// #pragma DATA_ALIGN(pBufGradY,8)
// short pBufGradY[IMG_HEIGHT][IMG_WIDTH];
// #pragma DATA_ALIGN(pBufMag,8)
// short pBufGradMag[IMG_HEIGHT][IMG_WIDTH];
// #pragma DATA_ALIGN(pBufOut,8)
// unsigned char pBufOut[IMG_HEIGHT][IMG_WIDTH];
// #pragma DATA_ALIGN(pBufScratch,8)
// unsigned char pBufScratch[IMG_HEIGHT][IMG_WIDTH];


/**
 * @brief 
 * Image segment function
 * -----------------------
 * Segment image using edge detection and double thresholding.
 * @param  image [IN] pointer to original image of size IMG_WIDTH x IMG_HEIGHT
 * @param binary [OUT]  pointer to binary buffer of size IMG_SIZE/32
 */
void segment(const unsigned char *image,
             unsigned char *restrict threshold,
             unsigned int  *restrict binary,
             const unsigned char thres_low,
             const unsigned char thres_high)
{
  register unsigned int i,j;
  register unsigned int sum = 0;

  for(i=0;i<IMG_SIZE;i++)
  {
    if((image)[i]>=thres_low && (image)[i]<=thres_high)
      threshold[i] = 255;
    else
      threshold[i] = 0; 
  }
  VLIB_packMask32(threshold,binary,IMG_SIZE);
}
