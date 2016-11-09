/*****************************************************************************************/
/*
 * File name: DistributedMilAddConstantC.cpp
 *
 * Synopsis:  This example shows how to use of the MIL Function Development module to 
 *            create a custom MIL function that adds a constant to an image using  
 *            custom C code.
 *
 *            The example creates a Master MIL function that registers all the parameters
 *            to MIL and calls the Slave function on the target system.
 *             
 *            Note: For simplicity, the images are assumed to be 8-bit unsigned and of 
 *            the same dimensions.
 */

/* Standard MIL header. */
#include <mil.h>

/* Master MIL function declaration */
void MFTYPE AddConstantC(MIL_ID SrcImage, MIL_ID DstImage, MIL_UINT Constant);

/* Master MIL Function definition. */
/* ------------------------------- */

/* Number of parameters of the MIL function. */
#define FUNCTION_NB_PARAM    3

/* Opcode of the MIL function. */
#define FUNCTION_OPCODE_ADD_CONST_C (M_USER_FUNCTION+4)

/* Slave function name and DLL path */
#define SLAVE_FUNC_NAME      MIL_TEXT("SlaveAddConstantC")
#define SLAVE_DLL_NAME       MIL_TEXT("DMilAddConstantSlave")

void MFTYPE AddConstantC(MIL_ID SrcImage, MIL_ID DstImage, MIL_UINT Constant)
   {
   MIL_ID   Func;
   
   /* Allocate a MIL function context to call the Slave function on the Target system. */
   MfuncAlloc(MIL_TEXT("AddConstantC"), 
              FUNCTION_NB_PARAM,
              M_NULL, SLAVE_DLL_NAME, SLAVE_FUNC_NAME,
              FUNCTION_OPCODE_ADD_CONST_C, 
              M_ASYNCHRONOUS_FUNCTION, 
              &Func
             );

   /* Register the parameters. */
   MfuncParamMilId  (Func, 1, SrcImage, M_IMAGE, M_IN  + M_PROC);
   MfuncParamMilId  (Func, 2, DstImage, M_IMAGE, M_OUT + M_PROC);
   MfuncParamMilUint(Func, 3, Constant);

   /* Call the Slave function. */
   MfuncCall(Func);

   /* Free the MIL function context. */
   MfuncFree(Func);
   }
