/*************************************************************************************/
/*
 * File name: DistributedMilSyncAsyncMaster.cpp 
 *
 * Synopsis:  This example shows how to use the MIL Function Development module to 
 *            create custom synchronous and asynchronous MIL functions.
 *
 *            It contains the SynchronousFunction() and AsynchronousFunction() master 
 *            functions.
 */

#include <mil.h>

/* Master MIL functions declarations */
MIL_INT MFTYPE SynchronousFunction(MIL_ID SrcImage, MIL_ID DstImage, MIL_INT Option);
void MFTYPE AsynchronousFunction(MIL_ID SrcImage, MIL_ID DstImage, MIL_INT Option);

/* Master Synchronous MIL Function definition. */
/* ------------------------------------------- */

#define SYNC_FUNCTION_OPCODE	(M_USER_FUNCTION+2)
#define SYNC_FUNCTION_NB_PARAM  4

/* Slave function name and DLL path */
#define SLAVE_SYNC_FUNC_NAME       MIL_TEXT("SlaveSynchronousFunction")
#define SLAVE_DLL_NAME             MIL_TEXT("DMilSyncAsyncSlave")

MIL_INT MFTYPE SynchronousFunction(MIL_ID SrcImage, MIL_ID DstImage, MIL_INT Option)
{
   MIL_ID Func;
   MIL_INT ReturnValue;
   
   /* Allocate a MIL function context. */
   MfuncAlloc(MIL_TEXT("SynchronousFunction"), SYNC_FUNCTION_NB_PARAM,
              M_NULL, SLAVE_DLL_NAME, SLAVE_SYNC_FUNC_NAME, 
              SYNC_FUNCTION_OPCODE, 
              M_SYNCHRONOUS_FUNCTION,
              &Func);

   /* Register the parameters. */
   MfuncParamMilId      (Func, 1, SrcImage, M_IMAGE, M_IN  + M_PROC);
   MfuncParamMilId      (Func, 2, DstImage, M_IMAGE, M_OUT + M_PROC);
   MfuncParamMilInt     (Func, 3, Option);
   MfuncParamArrayMilInt(Func, 4, &ReturnValue, 1, M_OUT);

   /* Call the target Slave function. */
   MfuncCall(Func);

   /* Free the MIL function context. */
   MfuncFree(Func);

   /* Return the value returned by the slave function. */
   return(ReturnValue);
}


/* Master Asynchronous MIL Function definition. */
/* -------------------------------------------- */

#define ASYNC_FUNCTION_OPCODE	 (M_USER_FUNCTION+3)
#define ASYNC_FUNCTION_NB_PARAM  3

/* Slave function name */
#define SLAVE_ASYNC_FUNC_NAME      MIL_TEXT("SlaveAsynchronousFunction")

void MFTYPE AsynchronousFunction(MIL_ID SrcImage, MIL_ID DstImage, MIL_INT Option)
{
   MIL_ID Func;
   
   MfuncAlloc(MIL_TEXT("AsynchronousFunction"), 
              ASYNC_FUNCTION_NB_PARAM,
              M_NULL, SLAVE_DLL_NAME, SLAVE_ASYNC_FUNC_NAME, 
              ASYNC_FUNCTION_OPCODE, 
              M_ASYNCHRONOUS_FUNCTION,
              &Func);

   /* Register the parameters. */
   MfuncParamMilId (Func, 1, SrcImage, M_IMAGE, M_IN  + M_PROC);
   MfuncParamMilId (Func, 2, DstImage, M_IMAGE, M_OUT + M_PROC);
   MfuncParamMilInt(Func, 3, Option);

   /* Call the target Slave function. */
   MfuncCall(Func);

   /* Free the MIL function context. */
   MfuncFree(Func);
}

