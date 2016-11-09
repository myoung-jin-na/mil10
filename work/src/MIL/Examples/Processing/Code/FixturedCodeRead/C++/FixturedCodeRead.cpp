/***************************************************************************************/
/* 
* File name: FixturedCodeRead.cpp  
*
* Synopsis:  This program contains an example of reading a 1D Code 39 with fixturing 
*            using the Code Reader module.
*            See the PrintHeader() function below for detailed description.
*
*/
#include <mil.h>


///***************************************************************************
// Example description.
///***************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n"));
   MosPrintf(MIL_TEXT("FixturedCodeRead\n\n"));

   MosPrintf(MIL_TEXT("[SYNOPSIS]\n"));
   MosPrintf(MIL_TEXT("This example will establish a fixtured code reading operation in a setup image,\n"));
   MosPrintf(MIL_TEXT("by:\n"));
   MosPrintf(MIL_TEXT("- Defining a model in the setup image.\n"));
   MosPrintf(MIL_TEXT("- Learning the offset of this model from the origin of the setup image.\n"));
   MosPrintf(MIL_TEXT("- Preparing a region around the code.\n\n"));

   MosPrintf(MIL_TEXT("Then, for each image in a set of images, this example will perform a fixtured\n"));
   MosPrintf(MIL_TEXT("code read operation, by:\n"));
   MosPrintf(MIL_TEXT("- Searching for model in the image.\n"));
   MosPrintf(MIL_TEXT("- Moving the relative coordinate system according to the found model\n"));
   MosPrintf(MIL_TEXT("  location and the learned offset.\n"));
   MosPrintf(MIL_TEXT("- Setting the region in the image,\n"));
   MosPrintf(MIL_TEXT("- Reading the code in the region of the image.\n\n"));

   MosPrintf(MIL_TEXT("[MODULES USED]\n"));
   MosPrintf(MIL_TEXT("Modules used: application, system, display, buffer, graphic, code and\n"));
   MosPrintf(MIL_TEXT("pattern matching.\n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

// Setup image.
static const MIL_TEXT_CHAR IMAGE_FILE[] = M_IMAGE_PATH MIL_TEXT("FixturedCodeRead/BoardSetup.mim");

// Number of images.
static const MIL_INT NUMBER_OF_IMAGES = 5L;

// More images.
static const MIL_TEXT_CHAR* ImageFileName[NUMBER_OF_IMAGES] =
   {
   M_IMAGE_PATH MIL_TEXT("FixturedCodeRead/Board1.mim"),
   M_IMAGE_PATH MIL_TEXT("FixturedCodeRead/Board2.mim"),
   M_IMAGE_PATH MIL_TEXT("FixturedCodeRead/Board3.mim"),
   M_IMAGE_PATH MIL_TEXT("FixturedCodeRead/Board4.mim"),
   M_IMAGE_PATH MIL_TEXT("FixturedCodeRead/Board5.mim"),
   };

// Code type.
static const MIL_INT CODE_TYPE          = M_CODE39;

// Model offset and size.
static const MIL_INT MODEL_OFFSET_X     = 196;
static const MIL_INT MODEL_OFFSET_Y     = 226;
static const MIL_INT MODEL_SIZE_X       =  93;
static const MIL_INT MODEL_SIZE_Y       =  91;

// region offset and size.
static const MIL_INT REGION_OFFSET_X    =  70;
static const MIL_INT REGION_OFFSET_Y    = 317;
static const MIL_INT REGION_SIZE_X      = 430;
static const MIL_INT REGION_SIZE_Y      = 100;

// CFixturedCodeRead
//
// Class used to hold the MIL objects and methods to setup and do a fixtured code read operation.
class CFixturedCodeRead
   {
   public:
      CFixturedCodeRead();
      ~CFixturedCodeRead();

      void Setup(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilOverlayImage, MIL_ID MilSrcImage);
      void Run(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilOverlayImage, MIL_ID MilSrcImage);
      void Free();

   protected:
      MIL_ID m_MilModel;
      MIL_ID m_MilFixturingOffset;
      MIL_ID m_MilRegionGraphicList;
      MIL_ID m_MilCodeContext;
      MIL_ID m_MilCodeResult;
      void Reset();
      void ReadCodeWithRegion(MIL_ID MilSrcImage, MIL_ID MilOverlayImage);
   };

///***************************************************************************
// Main.
///***************************************************************************
int MosMain(void)
   {
   MIL_ID      MilApplication,        // Application identifier.
               MilSystem,             // System identifier.
               MilDisplay;            // Display identifier.

   // Allocate MIL objects.
   MappAlloc(M_NULL, M_DEFAULT, &MilApplication);
   MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &MilSystem);
   MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDisplay);

   // Print Header.
   PrintHeader();

   // Restore setup image.
   MIL_ID MilSrcImage;
   MbufRestore(IMAGE_FILE, MilSystem, &MilSrcImage);
   McalUniform(MilSrcImage, 0.0, 0.0, 1.0, 1.0, 0.0, M_DEFAULT);

   // Display the image buffer.
   MdispSelect(MilDisplay, MilSrcImage);

   // Prepare for overlay annotations.
   MIL_ID MilOverlayImage;
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   // Setup display such that it will display the region by
   // painting all the pixels outside the region.
   MdispControl(MilDisplay, M_REGION_OUTSIDE_SHOW, M_OPAQUE);

   // Allocate fixtured code read object.
   CFixturedCodeRead Fixture;

   // Setup the fixtured code read operation.
   Fixture.Setup(MilSystem, MilDisplay, MilOverlayImage, MilSrcImage);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   for(MIL_INT ii = 0; ii < NUMBER_OF_IMAGES; ii++)
      {
      // Restore an image.
      MbufLoad(ImageFileName[ii], MilSrcImage);

      // Find the model and read the code inside the region.
      Fixture.Run(MilSystem, MilDisplay, MilOverlayImage, MilSrcImage);
      }

   // Free allocated resources.
   Fixture.Free();

   // Free source image.
   MbufFree(MilSrcImage);

   // Free other allocations.
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }



///***************************************************************************
// CFixturedCodeRead member functions
///***************************************************************************
CFixturedCodeRead::CFixturedCodeRead()
   {
   Reset();
   }

CFixturedCodeRead::~CFixturedCodeRead()
   {
   Free();
   }

void CFixturedCodeRead::Reset()
   {
   m_MilModel             = M_NULL;
   m_MilFixturingOffset   = M_NULL;
   m_MilRegionGraphicList = M_NULL;
   m_MilCodeContext       = M_NULL;
   m_MilCodeResult        = M_NULL;
   }

void CFixturedCodeRead::Free()
   {
   if (m_MilModel             != M_NULL)  MpatFree(m_MilModel            );
   if (m_MilFixturingOffset   != M_NULL)  McalFree(m_MilFixturingOffset  );
   if (m_MilRegionGraphicList != M_NULL)  MgraFree(m_MilRegionGraphicList);
   if (m_MilCodeContext       != M_NULL)  McodeFree(m_MilCodeContext     );
   if (m_MilCodeResult        != M_NULL)  McodeFree(m_MilCodeResult      );
   Reset();
   }

///***************************************************************************
// CFixturedCodeRead::Setup
//
// - Allocate a model in an image
// - Learn the offset of the model
// - Prepare a read region around the model
// 
// It also run the fixtured code read operation on this image to make
// sure that the setup is correct.
///***************************************************************************
void CFixturedCodeRead::Setup(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilOverlayImage, MIL_ID MilSrcImage)
   {
   // Allocate a pattern matching model.
   MpatAllocModel(MilSystem, MilSrcImage, 
                  MODEL_OFFSET_X, MODEL_OFFSET_Y, 
                  MODEL_SIZE_X, MODEL_SIZE_Y, 
                  M_NORMALIZED, &m_MilModel);

   // Set search angle for model.
   MpatSetAngle(m_MilModel, M_SEARCH_ANGLE_MODE, M_ENABLE);
   MpatSetAngle(m_MilModel, M_SEARCH_ANGLE_DELTA_NEG, 180.0);
   MpatSetAngle(m_MilModel, M_SEARCH_ANGLE_DELTA_POS, 180.0);
   MpatSetAngle(m_MilModel, M_SEARCH_ANGLE_ACCURACY , 0.5);

   // Draw the model.
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   MgraColor(M_DEFAULT, M_COLOR_RED);
   MgraControl(M_DEFAULT, M_INPUT_UNITS, M_PIXEL);
   MpatDraw(M_DEFAULT, m_MilModel, MilOverlayImage, M_DRAW_BOX+M_DRAW_POSITION, M_DEFAULT, M_ORIGINAL);      

   MosPrintf(MIL_TEXT("A model is defined in the source image.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Learn the offset of this model.
   McalAlloc(MilSystem, M_FIXTURING_OFFSET, M_DEFAULT, &m_MilFixturingOffset);
   McalFixture(M_NULL, m_MilFixturingOffset, M_LEARN_OFFSET, M_MODEL_PAT, m_MilModel, MilSrcImage, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Draw the relative coordinate system and the fixturing offset.
   MgraColor(M_DEFAULT, M_COLOR_LIGHT_BLUE);
   MgraBackColor(M_DEFAULT, M_COLOR_GRAY);
   McalDraw(M_DEFAULT, M_NULL, MilOverlayImage, 
            M_DRAW_RELATIVE_COORDINATE_SYSTEM, M_DEFAULT, M_DEFAULT);

   MgraColor(M_DEFAULT, M_COLOR_GREEN);
   McalDraw(M_DEFAULT, m_MilFixturingOffset, MilOverlayImage, 
            M_DRAW_FIXTURING_OFFSET, M_DEFAULT, M_DEFAULT);

   MosPrintf(MIL_TEXT("The offset, from the origin of the template image to the model, is learned.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Allocate a read region around the code.
   MgraAllocList(MilSystem, M_DEFAULT, &m_MilRegionGraphicList);
   MgraControl(M_DEFAULT, M_INPUT_UNITS, M_WORLD);
   MgraRectAngle(M_DEFAULT, m_MilRegionGraphicList, 
                 REGION_OFFSET_X, REGION_OFFSET_Y, 
                 REGION_SIZE_X, REGION_SIZE_Y, 
                 0.0, M_CORNER_AND_DIMENSION+M_FILLED);

   // Allocate code objects.
   McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, &m_MilCodeContext);
   McodeModel(m_MilCodeContext, M_ADD, CODE_TYPE, M_NULL, M_DEFAULT, M_NULL);

   // Set some code control.
   McodeControl(m_MilCodeContext, M_SEARCH_ANGLE, M_ACCORDING_TO_REGION);
   McodeControl(m_MilCodeContext, M_POSITION_ACCURACY, M_HIGH);

   // Prepare bar code results buffer.
   McodeAllocResult(MilSystem, M_DEFAULT, &m_MilCodeResult);

   // Read code with region in the setup image to make sure that setup is ok.
   ReadCodeWithRegion(MilSrcImage, MilOverlayImage);
   }

///***************************************************************************
// CFixturedCodeRead::Run
//
// Search model and read the code in a region. 
///***************************************************************************
void CFixturedCodeRead::Run(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilOverlayImage, MIL_ID MilSrcImage)
   {
   // Clear overlay.
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   // Remove the region of interest.
   MbufSetRegion(MilSrcImage, M_NULL, M_DEFAULT, M_DELETE, M_DEFAULT);

   // Allocate a pattern finder result.
   MIL_ID MilPatResult;
   MpatAllocResult(MilSystem, 1, &MilPatResult);

   // Find the model in the new source image.
   MpatFindModel(MilSrcImage, m_MilModel, MilPatResult);

   if (MpatGetNumber(MilPatResult, NULL) != 1)
      {
      MosPrintf(MIL_TEXT("Unable to find the model in source image.\n\n"));
      }
   else
      {
      // Draw Box around the model.
      MgraControl(M_DEFAULT, M_INPUT_UNITS, M_PIXEL);
      MgraColor(M_DEFAULT, M_COLOR_RED);
      MpatDraw(M_DEFAULT, MilPatResult, MilOverlayImage, M_DRAW_BOX+M_DRAW_POSITION, 0, M_DEFAULT);

      // Move the relative coordinate system of the image according to the location of the found model.
      McalFixture(MilSrcImage, m_MilFixturingOffset, M_MOVE_RELATIVE, M_RESULT_PAT, 
                  MilPatResult, 0, 
                  M_DEFAULT, M_DEFAULT, M_DEFAULT);

      // Draw the relative coordinate system and the fixturing offset.
      MgraColor(M_DEFAULT, M_COLOR_LIGHT_BLUE);
      MgraBackColor(M_DEFAULT, M_COLOR_GRAY);
      McalDraw(M_DEFAULT, M_NULL, MilOverlayImage, 
               M_DRAW_RELATIVE_COORDINATE_SYSTEM, M_DEFAULT, M_DEFAULT);

      MgraColor(M_DEFAULT, M_COLOR_GREEN);
      McalDraw(M_DEFAULT, m_MilFixturingOffset, MilOverlayImage, 
               M_DRAW_FIXTURING_OFFSET, M_DEFAULT, M_DEFAULT);

      MosPrintf(MIL_TEXT("The model is found in the source image.\n\n"));

      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      // Read code with region.
      ReadCodeWithRegion(MilSrcImage, MilOverlayImage);
      }

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MpatFree(MilPatResult);
   }

///***************************************************************************
// ReadCodeWithRegion
//
// Read a code inside an image with a region. 
///***************************************************************************
void CFixturedCodeRead::ReadCodeWithRegion(MIL_ID MilSrcImage, MIL_ID MilOverlayImage)
   {
   // Set the region of interest.
   MbufSetRegion(MilSrcImage, m_MilRegionGraphicList, M_DEFAULT, M_NO_RASTERIZE, M_DEFAULT);

   MosPrintf(MIL_TEXT("A read region is set.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Read code from image.
   McodeRead(m_MilCodeContext, MilSrcImage, m_MilCodeResult);

   // Get decoding status.
   MIL_INT ReadStatus;
   McodeGetResult(m_MilCodeResult, M_STATUS+M_TYPE_MIL_INT, &ReadStatus);

   // Check if the decode operation was successful.
   if (ReadStatus == M_STATUS_READ_OK)
      {
      // Get decoded string.
      MIL_INT ResultStringSize;
      McodeGetResult(m_MilCodeResult, M_STRING_SIZE+M_TYPE_MIL_INT, &ResultStringSize);
      MIL_TEXT_CHAR* ResultString = new MIL_TEXT_CHAR[ResultStringSize+1]; // Array of characters read.
      McodeGetResult(m_MilCodeResult, M_STRING, ResultString);

      // Add prefix to the string.
      const MIL_TEXT_CHAR PrefixString[] = MIL_TEXT("Read code: ");
      MIL_INT OutputStringSize = ResultStringSize + MosStrlen(PrefixString) + 1;
      MIL_TEXT_CHAR* OutputString = new MIL_TEXT_CHAR[OutputStringSize]; // Array of characters to draw.
      MosSprintf(OutputString, OutputStringSize, MIL_TEXT("%s%s"), PrefixString, ResultString);

      // Draw a box around the code.
      MgraColor(M_DEFAULT, M_COLOR_GREEN); 
      McodeDraw(M_DEFAULT, m_MilCodeResult, MilOverlayImage, M_DRAW_BOX, 0, M_DEFAULT);

      // Get coordinates of the code.
      double PositionX, PositionY;
      McodeGetResult(m_MilCodeResult, M_POSITION_X, &PositionX);
      McodeGetResult(m_MilCodeResult, M_POSITION_Y, &PositionY);

      // Draw read string.
      MgraControl(M_DEFAULT, M_INPUT_UNITS, M_WORLD);
      MgraBackColor(M_DEFAULT, M_COLOR_GRAY);
      MgraControl(M_DEFAULT, M_TEXT_ALIGN_HORIZONTAL, M_CENTER);
      MgraControl(M_DEFAULT, M_TEXT_ALIGN_VERTICAL, M_CENTER);
      MgraText(M_DEFAULT, MilOverlayImage, PositionX, PositionY, OutputString);

      MosPrintf(MIL_TEXT("Reading in the region was successful and the string was written on the code.\n\n"));
      }
   else
      {
      MosPrintf(MIL_TEXT("Code read operation failed.\n\n"));
      }
   }