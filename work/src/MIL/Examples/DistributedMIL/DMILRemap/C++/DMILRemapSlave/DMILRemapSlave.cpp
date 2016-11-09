/*****************************************************************************************/
/*
 * File name: DistributedMilRemapSlave.cpp
 *
 * Synopsis:  This example shows how to use the MIL Function Development module to 
 *            create a custom asynchronous MIL function that does a series of MIL 
 *            commands on a target system in a single call from the host.
 *
 *            It contains the SlaveCustomRemap() slave function.
 */
#include <mil.h>

/* Slave MIL Function prototypes. */
#ifdef __cplusplus
extern "C" {
#endif
void MFTYPE SlaveCustomRemap(MIL_ID Func);

#ifdef __cplusplus
   }
#endif
/* Slave MIL Function definition. */
/* ------------------------------ */

void MFTYPE SlaveCustomRemap(MIL_ID Func)
{
  MIL_ID SrcImage, DstImage, OwnerSystem, ExtremeResult, Lut;
  MIL_UINT Option;
  MIL_INT MinAndMax[2];

  /* Read the parameters. */
  MfuncParamValue(Func, 1, &SrcImage);
  MfuncParamValue(Func, 2, &DstImage);
  MfuncParamValue(Func, 3, &Option); 

  /* Find the Owner system of the destination image. */
  MbufInquire(SrcImage, M_OWNER_SYSTEM, &OwnerSystem);

  /* Find the Minimum and Maximum values of the image. */ 
  MimAllocResult(OwnerSystem, 2L, M_EXTREME_LIST, &ExtremeResult);
  MimFindExtreme(SrcImage, ExtremeResult, M_MIN_VALUE+M_MAX_VALUE);
  MimGetResult(ExtremeResult, M_VALUE, MinAndMax);
  MimFree(ExtremeResult);

  //* Create a LUT to remap the values from Min to Max to 0x00-0xFF. */
  MbufAlloc1d(OwnerSystem, 256, 8+M_UNSIGNED, M_LUT, &Lut);
  MgenLutRamp(Lut, MinAndMax[0], 0x00, MinAndMax[1], 0xFF);

  //* Remap the values. */
  MimLutMap(SrcImage, DstImage, Lut);

  //* Free the Lut buffer */
  MbufFree(Lut);
}
