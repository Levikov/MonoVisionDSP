#include <MonoGlobal.h>

/**
 * @brief 
 * Receive image from EMIF
 * @param address[IN/OUT] pointer to EMIF
 * @param image[OUT] pointer 
 */
void recvEMIF(const void * address, unsigned char * image)
{
  memcpy(image,address,IMG_SIZE);
}

/**
 * @brief 
 * Send pose detection result
 * @param address[IN/OUT] pointer to EMIF.
 * @param pose[IN] pointer to pose result. 
 */
void sendEMIF(const void * address,const Pose * pose)
{

}

