//***************************************************************************************/
// 
// File name: McalTransformCoordinate3dListMP.h
//
// Synopsis:  Declaration of the MP version of McalTransformCoordinate3dList
//

#ifndef __MCAL_TRANSFORMCOORDINATE3DLIST_MP_H
#define __MCAL_TRANSFORMCOORDINATE3DLIST_MP_H


struct ScalTransformCoordinate3dListMPCommon
   {
   MIL_ID CalibrationOrImageId;
   MIL_INT SrcCoordinateSystem;
   MIL_INT DstCoordinateSystem;
   MIL_INT ModeFlag;
   const MIL_DOUBLE* XSrcArray;
   const MIL_DOUBLE* YSrcArray;
   const MIL_DOUBLE* ZSrcArray;
   MIL_DOUBLE* XDstArray;
   MIL_DOUBLE* YDstArray;
   MIL_DOUBLE* ZDstArray;
   };

struct ScalTransformCoordinate3dListMP
   {
   ScalTransformCoordinate3dListMPCommon* pCommonParams;
   MIL_INT NumPoints;
   MIL_INT StartIdx;
   };

MIL_UINT32 MFTYPE TransformCoordinate3dListThread(void *UserDataPtr);

void McalTransformCoordinate3dListMP(MIL_ID MilSystem,
                                     MIL_ID CalibrationOrImageId,
                                     MIL_INT SrcCoordinateSystem,
                                     MIL_INT DstCoordinateSystem,
                                     MIL_INT NumPoints,
                                     const MIL_DOUBLE* XSrcArray,
                                     const MIL_DOUBLE* YSrcArray,
                                     const MIL_DOUBLE* ZSrcArray,
                                     MIL_DOUBLE* XDstArray,
                                     MIL_DOUBLE* YDstArray,
                                     MIL_DOUBLE* ZDstArray,
                                     MIL_INT ModeFlag);

#endif