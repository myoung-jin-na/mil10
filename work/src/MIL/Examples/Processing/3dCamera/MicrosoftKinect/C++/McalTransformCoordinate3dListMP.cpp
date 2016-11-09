//***************************************************************************************/
// 
// File name: McalTransformCoordinate3dListMP.cpp
//
// Synopsis:  Implementation of the MP version of McalTransformCoordinate3dList.
//            

#include <mil.h>
#include "McalTransformCoordinate3dListMP.h"

//*****************************************************************************
// Function to transform the coordinate list using multiple cores.
//*****************************************************************************
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
                                     MIL_INT ModeFlag)
   {
   // Get the number of cores available.
   MIL_INT NbEffectiveCore = MthrInquireMp(M_DEFAULT, M_CORE_NUM_EFFECTIVE, M_DEFAULT, M_NULL, M_NULL);
      
   if(NbEffectiveCore > 1)
      {
      MIL_INT NbPointsPerThread = NumPoints / NbEffectiveCore;
      MIL_INT NbPointRemain = NumPoints % NbEffectiveCore;           
      
      ScalTransformCoordinate3dListMPCommon CommonParam;
      CommonParam.CalibrationOrImageId = CalibrationOrImageId;
      CommonParam.SrcCoordinateSystem = SrcCoordinateSystem;
      CommonParam.DstCoordinateSystem = DstCoordinateSystem;
      CommonParam.XSrcArray = XSrcArray;
      CommonParam.YSrcArray = YSrcArray;
      CommonParam.ZSrcArray = ZSrcArray;
      CommonParam.XDstArray = XDstArray;
      CommonParam.YDstArray = YDstArray;
      CommonParam.ZDstArray = ZDstArray;
      CommonParam.ModeFlag = ModeFlag;

      ScalTransformCoordinate3dListMP* ThreadParamArray = new ScalTransformCoordinate3dListMP[NbEffectiveCore];
      MIL_ID* MilMPThreads = new MIL_ID[NbEffectiveCore];

      MIL_INT CoreIdx = 0;
      MIL_INT StartIdx = 0;

      // Start the threads that have one more points to process. 
      for(; NbPointRemain > 0 ; NbPointRemain--)
         {
         ThreadParamArray[CoreIdx].pCommonParams = &CommonParam;
         ThreadParamArray[CoreIdx].StartIdx = StartIdx;
         ThreadParamArray[CoreIdx].NumPoints = NbPointsPerThread + 1;
         MthrAlloc(MilSystem, M_THREAD, M_DEFAULT, TransformCoordinate3dListThread,
                   &ThreadParamArray[CoreIdx], &MilMPThreads[CoreIdx]); 
         StartIdx += NbPointsPerThread + 1;
         CoreIdx++;
         }

      // Start the threads that have NbPointsPerThread.
      if(NbPointsPerThread > 0)
         {
         for(; CoreIdx < NbEffectiveCore; CoreIdx++)
            {
            ThreadParamArray[CoreIdx].pCommonParams = &CommonParam;
            ThreadParamArray[CoreIdx].StartIdx = StartIdx;
            ThreadParamArray[CoreIdx].NumPoints = NbPointsPerThread;
            MthrAlloc(MilSystem, M_THREAD, M_DEFAULT, TransformCoordinate3dListThread,
                      &ThreadParamArray[CoreIdx], &MilMPThreads[CoreIdx]);
            StartIdx += NbPointsPerThread;
            }
         }

      // Wait for all the threads to finish and delete them.
      for(MIL_INT ThreadIdx = 0; ThreadIdx < CoreIdx; ThreadIdx++)
         {
         MthrWait(MilMPThreads[ThreadIdx], M_THREAD_END_WAIT, M_NULL);
         MthrFree(MilMPThreads[ThreadIdx]);
         }

      // Delete the arrays
      delete [] MilMPThreads;
      delete [] ThreadParamArray;    
      }
   else
      McalTransformCoordinate3dList(CalibrationOrImageId,
                                    SrcCoordinateSystem,
                                    DstCoordinateSystem,
                                    NumPoints,
                                    XSrcArray,
                                    YSrcArray,
                                    ZSrcArray,
                                    XDstArray,
                                    YDstArray,
                                    ZDstArray,
                                    ModeFlag);
   }

//*****************************************************************************
// Thread function that effectively transforms the coordinates.
//*****************************************************************************
MIL_UINT32 MFTYPE TransformCoordinate3dListThread(void *UserDataPtr)
   {
   ScalTransformCoordinate3dListMP* pThreadParam = (ScalTransformCoordinate3dListMP*) UserDataPtr;
   ScalTransformCoordinate3dListMPCommon* pCommonParam = pThreadParam->pCommonParams; 

   // Call the transform coordinate on the number of points.
   McalTransformCoordinate3dList(pCommonParam->CalibrationOrImageId,
                                 pCommonParam->SrcCoordinateSystem,
                                 pCommonParam->DstCoordinateSystem,
                                 pThreadParam->NumPoints,
                                 &pCommonParam->XSrcArray[pThreadParam->StartIdx],
                                 &pCommonParam->YSrcArray[pThreadParam->StartIdx],
                                 pCommonParam->ZSrcArray == M_NULL ? M_NULL : &pCommonParam->ZSrcArray[pThreadParam->StartIdx],
                                 &pCommonParam->XDstArray[pThreadParam->StartIdx],
                                 &pCommonParam->YDstArray[pThreadParam->StartIdx],
                                 pCommonParam->ZDstArray == M_NULL ? M_NULL : &pCommonParam->ZDstArray[pThreadParam->StartIdx],
                                 pCommonParam->ModeFlag);

   return 0;
   }