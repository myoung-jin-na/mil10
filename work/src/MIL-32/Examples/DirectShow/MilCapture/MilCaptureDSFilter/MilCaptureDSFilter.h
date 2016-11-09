#pragma once

/* Convenient macro for releasing interfaces.*/
#define HELPER_RELEASE(x)  if (x != NULL) { \
                           x->Release(); \
                           x = NULL; }

#define CHECK_FOR_ERROR(hr)   if(FAILED(hr)) { \
                                 MosGetch(); \
                                 CleanUpDirectShow(&pMilCapture, &pMilCapturePin, &pGraphBuilder, &pMediaControl); \
                                 return 1; } 

/*Directshow interfaces release*/ 
void CleanUpDirectShow(IMilCapture** pMilCapture, IMilCapturePin** pMilCapturePin, IGraphBuilder** pGraphBuilder, IMediaControl** pMediaControl);
