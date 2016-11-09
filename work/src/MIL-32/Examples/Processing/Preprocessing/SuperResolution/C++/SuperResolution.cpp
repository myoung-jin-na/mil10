//***************************************************************************************/
// 
// File name: SuperResolution.cpp  
//
// Synopsis:  This program contains an example of registration and
//            super-resolution of images in a video sequence using the 
//            registration module.
//            See the PrintHeader() function below for detailed description.
//
//

#include <mil.h>

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("SuperResolution\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This example aligns all images in a video sequence\n")
             MIL_TEXT("with the child of the first image of the sequence.\n\n")

             MIL_TEXT("It then uses super-resolution to build an image with\n")
             MIL_TEXT("a resolution that is higher that the individual images\n")
             MIL_TEXT("of the sequence.\n\n")
  
             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: application, system, buffer, display,\n")
             MIL_TEXT("graphic, registration\n\n")
  
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

//****************************************************************************
// Structures declarations.
//****************************************************************************

// Hold the description of the sequence to be processed.
struct SSequenceDescription
   {
   MIL_CONST_TEXT_PTR Filename;
   MIL_INT            StartImage;
   MIL_INT            NumberOfImages;
   MIL_INT            ChildOffsetX;
   MIL_INT            ChildOffsetY;
   MIL_INT            ChildSizeX;
   MIL_INT            ChildSizeY;
   MIL_DOUBLE         PsfRadius;
   MIL_INT            PsfType;
   MIL_DOUBLE         Smoothness;
   };

// Processing object.
class CSuperResolution
   {
   public:
      CSuperResolution(MIL_ID MilSystem);
      ~CSuperResolution();

      void InitializeWithFirstImage(MIL_ID MilFirstImage, 
                                    MIL_INT NumberOfImages,
                                    MIL_INT ChildOffsetX, MIL_INT ChildOffsetY,
                                    MIL_INT ChildSizeX, MIL_INT ChildSizeY);
      bool AddImage(MIL_ID MilImage);
      void SuperResolution(MIL_DOUBLE PsfRadius, MIL_INT PsfType, MIL_DOUBLE Smoothness);

   protected:
      void InitializeDisplay(MIL_ID MilFirstImage, MIL_INT ChildSizeX, MIL_INT ChildSizeY);
      void DrawCurrentResult();

      MIL_ID m_MilSystem;

      // Registration context and result.
      MIL_ID m_MilRegContext;
      MIL_ID m_MilRegResult;

      // Images of the sequence.
      MIL_ID* m_MilSequenceImageTable;
      MIL_ID* m_MilPartialSequenceImageTable;
      MIL_INT m_NumberOfImagesInTable;
      MIL_INT m_NumberOfAllocatedTableElements;

      // Various images for display.
      MIL_ID m_MilDisplay;
      MIL_ID m_MilOverlayImage;
      MIL_ID m_MilFullDisplayImage;
      MIL_ID m_MilTrackingImage;
      MIL_ID m_MilZoomedWithAverageImage;
      MIL_ID m_MilZoomedWithSuperResolutionImage;
      MIL_ID m_MilTrackingOverlayImage;

      MIL_INT m_FirstImageChildOffsetX;
      MIL_INT m_FirstImageChildOffsetY;
   };

//****************************************************************************
// Constants definitions.
//****************************************************************************

// Note that the 2 car sequences are imaged with a very good focus, therefore
// the Point Spread Function (PSF) is simply the size of one CCD pixel
// (square of "radius" = 0.5 pixel).
// 
// All the images have low noise, the super-resolution smoothness can be
// reduced less than its default behavior of 50.
static const MIL_INT NumberOfSequences = 3;
static const SSequenceDescription Sequences[NumberOfSequences] = {
   // Filename                                              StartImage
   // |                                                     |   NumberOfImages
   // |                                                     |   |    ChildOffsetX
   // |                                                     |   |    |    ChildOffsetY
   // |                                                     |   |    |    |    ChildSizeX
   // |                                                     |   |    |    |    |   ChildSizeY
   // |                                                     |   |    |    |    |   |   PsfRadius
   // |                                                     |   |    |    |    |   |   |   PsfType
   // |                                                     |   |    |    |    |   |   |   |          Smoothness
   // |                                                     |   |    |    |    |   |   |   |          |
   {M_IMAGE_PATH MIL_TEXT("SuperResolution/car_far.avi"),   0, 25, 285, 200,  45, 30, 0.5, M_SQUARE , 30.0},
   {M_IMAGE_PATH MIL_TEXT("SuperResolution/car_near.avi"),  0, 25, 210, 300, 160, 90, 0.5, M_SQUARE , 30.0},
   {M_IMAGE_PATH MIL_TEXT("SuperResolution/res_chart.avi"), 0, 19, 250, 125, 105, 70, 0.5, M_DEFAULT, 30.0}
   };

static const MIL_DOUBLE MOSAIC_SCALE = 3.0;

// Utility function.
template <class T>
inline T Max(T InValue1, T InValue2)
   {
   if (InValue1 > InValue2)
      return InValue1;
   else 
      return InValue2;
   }

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Allocate MIL objects. 
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   MIL_ID MilSystem = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &MilSystem);

   // Main application code here. 
   PrintHeader();

   MIL_INT i;
   for (i = 0; i < NumberOfSequences; i++)
      {
      const SSequenceDescription SequenceDescription = Sequences[i];

      // Open sequence.
      MbufImportSequence(SequenceDescription.Filename, M_DEFAULT, M_NULL, M_NULL, M_NULL, M_NULL, M_NULL, M_OPEN);

      MIL_INT NumberOfImagesInSequence = MbufDiskInquire(SequenceDescription.Filename, M_NUMBER_OF_IMAGES, M_NULL);
      MIL_DOUBLE FrameRate;
      MbufDiskInquire(SequenceDescription.Filename, M_FRAME_RATE, &FrameRate);

      // Restore first image.
      MIL_ID MilSequenceImage;
      MbufImportSequence(SequenceDescription.Filename, M_DEFAULT, 
         M_RESTORE, MilSystem, &MilSequenceImage, 
         SequenceDescription.StartImage, 1, M_READ);
      MIL_INT FrameIndex = 1;

      // Allocate processing object.
      CSuperResolution SuperResolution(MilSystem);
      SuperResolution.InitializeWithFirstImage(MilSequenceImage,
                                               SequenceDescription.NumberOfImages, 
                                               SequenceDescription.ChildOffsetX,
                                               SequenceDescription.ChildOffsetY,
                                               SequenceDescription.ChildSizeX,
                                               SequenceDescription.ChildSizeY);

      MosPrintf(MIL_TEXT("This is the first image of the sequence.\n")
                MIL_TEXT("A rectangular region is set. All the other images of the sequence\n")
                MIL_TEXT("will be realigned such that their content matches the content of the\n")
                MIL_TEXT("first image in the region.\n\n"));

      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      MIL_DOUBLE PreviousTime;
      MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &PreviousTime);

      bool Success = true;
      while (Success && FrameIndex < NumberOfImagesInSequence)
         {
         // Read and process next image in sequence.
         MbufImportSequence(SequenceDescription.Filename, M_DEFAULT, 
            M_LOAD, M_NULL, &MilSequenceImage, 
            M_DEFAULT, 1, M_READ);

         Success = SuperResolution.AddImage(MilSequenceImage);
         FrameIndex++;

         // Wait to have a proper frame rate.
         MIL_DOUBLE EndTime;
         MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &EndTime);
         MIL_DOUBLE WaitTime = (1.0/FrameRate) - (EndTime - PreviousTime);
         if (WaitTime > 0)
            MappTimer(M_DEFAULT, M_TIMER_WAIT, &WaitTime);
         MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &PreviousTime);
         }

      MosPrintf(MIL_TEXT("All images have been aligned.\n\n"));

      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      // Perform image enhancement with super-resolution.
      SuperResolution.SuperResolution(SequenceDescription.PsfRadius ,
                                      SequenceDescription.PsfType   ,
                                      SequenceDescription.Smoothness);

      MosPrintf(MIL_TEXT("The data contained in the selected region of all the images is composed\n")
                MIL_TEXT("to form a scaled image using two methods:\n")
                MIL_TEXT("- Averaging all images.\n")
                MIL_TEXT("- Super-resolution.\n\n"));

      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      // Free image and close sequence.
      MbufFree(MilSequenceImage);
      MbufImportSequence(SequenceDescription.Filename, M_DEFAULT, 
         M_NULL, M_NULL, M_NULL, 
         M_NULL, M_NULL, M_CLOSE);
      }
   
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));

   // Free MIL objects.     
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

//*****************************************************************************
// Constructor of the processing object.
// Most initialization will be done later in InitializeWithFirstImage.
//*****************************************************************************
CSuperResolution::CSuperResolution(MIL_ID MilSystem) :
   m_MilSystem                         (MilSystem),
   m_MilRegContext                     (M_NULL),
   m_MilRegResult                      (M_NULL),

   m_MilSequenceImageTable             (NULL),
   m_MilPartialSequenceImageTable      (NULL),
   m_NumberOfImagesInTable             (0),
   m_NumberOfAllocatedTableElements    (0),

   m_MilDisplay                        (M_NULL),
   m_MilOverlayImage                   (M_NULL),
   m_MilFullDisplayImage               (M_NULL),
   m_MilTrackingImage                  (M_NULL),
   m_MilZoomedWithAverageImage         (M_NULL),
   m_MilZoomedWithSuperResolutionImage (M_NULL),
   m_MilTrackingOverlayImage           (M_NULL),
   m_FirstImageChildOffsetX            (0),
   m_FirstImageChildOffsetY            (0)
   {
   }

//*****************************************************************************
// Destructor of the processing object.
// Free all objects.
//*****************************************************************************
CSuperResolution::~CSuperResolution()
   {
   // Free all allocated MIL objects.
   while (m_NumberOfImagesInTable)
      {
      m_NumberOfImagesInTable--;
      MbufFree(m_MilSequenceImageTable[m_NumberOfImagesInTable]);
      }
   delete [] m_MilPartialSequenceImageTable;
   delete [] m_MilSequenceImageTable;

   if (m_MilRegResult != M_NULL)
      MregFree(m_MilRegResult);
   if (m_MilRegContext != M_NULL)
      MregFree(m_MilRegContext);

   MbufFree(m_MilTrackingOverlayImage);
   MbufFree(m_MilZoomedWithSuperResolutionImage);
   MbufFree(m_MilZoomedWithAverageImage);
   MbufFree(m_MilTrackingImage);
   MbufFree(m_MilFullDisplayImage);
   MdispFree(m_MilDisplay);
   }

//*****************************************************************************
// Initialize the processing object.
//
// The processing object will be ready to align a series of images with the
// child specified when calling this function.
//*****************************************************************************
void CSuperResolution::InitializeWithFirstImage(MIL_ID MilFirstImage, 
                                                MIL_INT NumberOfImages,
                                                MIL_INT ChildOffsetX, MIL_INT ChildOffsetY,
                                                MIL_INT ChildSizeX, MIL_INT ChildSizeY)
   {
   // Allocate registration context and result.
   MregAlloc(m_MilSystem, M_CORRELATION, M_DEFAULT, &m_MilRegContext);
   MregAllocResult(m_MilSystem, M_DEFAULT, &m_MilRegResult);

   MregControl(m_MilRegContext, M_CONTEXT, M_NUMBER_OF_ELEMENTS, NumberOfImages);

   m_FirstImageChildOffsetX = ChildOffsetX;
   m_FirstImageChildOffsetY = ChildOffsetY;

   // Allocate an array to store the images of the sequence.
   m_MilSequenceImageTable        = new MIL_ID[NumberOfImages];
   m_MilPartialSequenceImageTable = new MIL_ID[NumberOfImages];
   m_NumberOfAllocatedTableElements = NumberOfImages;
   m_NumberOfImagesInTable = 0;

   // Initialize the display.
   InitializeDisplay(MilFirstImage, ChildSizeX, ChildSizeY);
   MbufCopy(MilFirstImage, m_MilFullDisplayImage);

   MgraColor(M_DEFAULT, M_COLOR_GREEN);
   MgraRectAngle(M_DEFAULT, m_MilOverlayImage, 
      ChildOffsetX, ChildOffsetY, ChildSizeX-1, ChildSizeY-1, 
      0, M_CORNER_AND_DIMENSION);

   // Keep a copy of the specified child of the first image.
   // All the other images of the sequence will be aligned with
   // this child.
   MbufAlloc2d(m_MilSystem,
               ChildSizeX, ChildSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, 
               &(m_MilSequenceImageTable[0]));
   m_NumberOfImagesInTable++;

   MbufCopyClip(MilFirstImage, m_MilSequenceImageTable[0], -ChildOffsetX, -ChildOffsetY);

   // Dummy calculate, just to initialize the result.
   MregCalculate(m_MilRegContext, m_MilSequenceImageTable, 
      m_MilRegResult, m_NumberOfImagesInTable, M_DEFAULT);

   // Specify the initial rough location of the next image
   // with respect to the child of the first image.
   MregSetLocation(m_MilRegContext, m_NumberOfImagesInTable, 0, 
      M_POSITION_XY, 
      static_cast<MIL_DOUBLE>(-ChildOffsetX), 
      static_cast<MIL_DOUBLE>(-ChildOffsetY), 
      M_DEFAULT, M_DEFAULT, M_DEFAULT);
   }

//*****************************************************************************
// Add one image to the sequence of images to align.
//
// The image will be copied in the internal array of images to be processed.
// Registration is used such that the child specified in InitializeWithFirstImage
// is aligned in the new image.
//*****************************************************************************
bool CSuperResolution::AddImage(MIL_ID MilImage)
   {
   if (m_NumberOfImagesInTable >= m_NumberOfAllocatedTableElements)
      return false;

   // Display the image.
   MbufCopy(MilImage, m_MilFullDisplayImage);

   // TBR : to help draw.
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_STATIC_INDEX, m_NumberOfImagesInTable-1);
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_OFFSET_X, 0);
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_OFFSET_Y, 0);

   // Keep a copy of the sequence image.
   m_MilSequenceImageTable[m_NumberOfImagesInTable] = MbufAllocColor(
      MbufInquire(MilImage, M_OWNER_SYSTEM, M_NULL),
      MbufInquire(MilImage, M_SIZE_BAND   , M_NULL),
      MbufInquire(MilImage, M_SIZE_X      , M_NULL),
      MbufInquire(MilImage, M_SIZE_Y      , M_NULL),
      MbufInquire(MilImage, M_TYPE        , M_NULL),
      M_IMAGE+M_PROC,
      M_NULL);
   MbufCopy(MilImage, m_MilSequenceImageTable[m_NumberOfImagesInTable]);
   m_NumberOfImagesInTable++;

   // Create an array of images that contains M_NULL in all elements
   // except for image 0 and current image.
   m_MilPartialSequenceImageTable[0] = m_MilSequenceImageTable[0];
   MIL_INT i;
   for (i = 1; i < m_NumberOfImagesInTable-1; i++)
      m_MilPartialSequenceImageTable[i] = M_NULL;
   m_MilPartialSequenceImageTable[m_NumberOfImagesInTable-1] = m_MilSequenceImageTable[m_NumberOfImagesInTable-1];

   // First alignment:
   // Use translation only.
   MregControl(m_MilRegContext, M_CONTEXT, M_TRANSFORMATION_TYPE, M_TRANSLATION);

   MregCalculate(m_MilRegContext, m_MilPartialSequenceImageTable, 
      m_MilRegResult, m_NumberOfImagesInTable, M_DEFAULT);

   MIL_INT Status;
   MregGetResult(m_MilRegResult, M_GENERAL, M_RESULT+M_TYPE_MIL_INT, &Status);
   if (Status != M_SUCCESS)
      return false;

   // Second alignment will use the result of the first 
   // alignment as initial rough location.
   MregSetLocation(m_MilRegContext, 
      M_ALL, M_UNCHANGED, // All the images to be aligned will be aligned with the same targets ...
      M_COPY_REG_RESULT,  // ... using the initial location specified by the result ...
      m_MilRegResult,     // ... of the alignment just performed.
      M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Second alignment:
   // Translation and rotation.
   MregControl(m_MilRegContext, M_CONTEXT, M_TRANSFORMATION_TYPE, M_TRANSLATION_ROTATION_SCALE);

   MregCalculate(m_MilRegContext, m_MilPartialSequenceImageTable, 
      m_MilRegResult, m_NumberOfImagesInTable, M_DEFAULT);

   MregGetResult(m_MilRegResult, M_GENERAL, M_RESULT+M_TYPE_MIL_INT, &Status);
   if (Status != M_SUCCESS)
      return false;

   // The alignment of this image is done. 
   // Do not re-do the alignment of this image when we will receive 
   // other images in the sequence.
   // Simply copy the results of the alignment to the registration
   // context. Since this image will not be part of m_MilPartialSequenceImageTable
   // in future calls to MregCalculate, its alignment will not be
   // recalculated.
   MregSetLocation(m_MilRegContext, M_ALL, M_UNCHANGED, 
      M_COPY_REG_RESULT, 
      m_MilRegResult, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   if (m_NumberOfImagesInTable < m_NumberOfAllocatedTableElements)
      {
      // The initial rough location of the next image of
      // the sequence is copied from the alignment result
      // of the just aligned image.
      MregSetLocation(m_MilRegContext, 
         m_NumberOfImagesInTable, 0,   // The next image will be aligned with image 0...
         M_COPY_REG_RESULT,            // ... using the initial location specified by the result ...
         m_MilRegResult,               // ... of the alignment just performed ...
         static_cast<MIL_DOUBLE>(m_NumberOfImagesInTable-1), 0, // ... of the just aligned image with image 0.
         M_DEFAULT, M_DEFAULT);
      }

   // Draw the result.
   DrawCurrentResult();

   return true;
   }

//*****************************************************************************
// Perform super-resolution on the sequence of images and display the results.
// Also show the result of averaging all the images, for comparison
// purpose.
//*****************************************************************************
void CSuperResolution::SuperResolution(MIL_DOUBLE PsfRadius, MIL_INT PsfType, MIL_DOUBLE Smoothness)
   {
   // Clear tracking image since the display space will be reused for 
   // super-resolution result.
   MbufClear(m_MilTrackingImage, 0);
   MbufClear(m_MilTrackingOverlayImage, static_cast<double>(MdispInquire(m_MilDisplay, M_TRANSPARENT_COLOR, M_NULL)));

   // Setup the registration result to do averaging of all images.
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_STATIC_INDEX, 0);
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_SCALE , MOSAIC_SCALE );
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_COMPOSITION, M_AVERAGE_IMAGE);

   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_OFFSET_X, 0);
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_OFFSET_Y, 0);

   // Perform averaging of all images.
   MregTransformImage(m_MilRegResult,
                      m_MilSequenceImageTable,
                      m_MilZoomedWithAverageImage,
                      m_NumberOfImagesInTable,
                      M_BILINEAR, M_DEFAULT);

   // Draw text to to indicate "Average".
   MIL_INT Margin = 10;
   MIL_INT TextPositionX = MbufInquire(m_MilZoomedWithAverageImage, M_ANCESTOR_OFFSET_X, M_NULL) + MbufInquire(m_MilZoomedWithAverageImage, M_SIZE_X, M_NULL) - Margin;
   MIL_INT TextPositionY = MbufInquire(m_MilZoomedWithAverageImage, M_ANCESTOR_OFFSET_Y, M_NULL) + Margin;
   
   MgraColor(M_DEFAULT, M_COLOR_GREEN);
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_HORIZONTAL, M_RIGHT);
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_VERTICAL, M_TOP);
   MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
   MgraText(M_DEFAULT, m_MilOverlayImage, TextPositionX, TextPositionY, MIL_TEXT("Average"));

   // Setup the registration result to do super-resolution with all images.
   MregControl(m_MilRegResult, M_GENERAL, M_SR_PSF_RADIUS, PsfRadius );
   MregControl(m_MilRegResult, M_GENERAL, M_SR_PSF_TYPE  , PsfType   );
   MregControl(m_MilRegResult, M_GENERAL, M_SR_SMOOTHNESS, Smoothness);
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_COMPOSITION, M_SUPER_RESOLUTION);

   MregTransformImage(
      m_MilRegResult, 
      m_MilSequenceImageTable, 
      m_MilZoomedWithSuperResolutionImage, 
      m_NumberOfImagesInTable, 
      M_BILINEAR, M_DEFAULT);

   // Draw text to to indicate "Super-resolution".
   TextPositionX = MbufInquire(m_MilZoomedWithSuperResolutionImage, M_ANCESTOR_OFFSET_X, M_NULL) + MbufInquire(m_MilZoomedWithSuperResolutionImage, M_SIZE_X, M_NULL) - Margin;
   TextPositionY = MbufInquire(m_MilZoomedWithSuperResolutionImage, M_ANCESTOR_OFFSET_Y, M_NULL) + Margin;
   
   MgraText(M_DEFAULT, m_MilOverlayImage, TextPositionX, TextPositionY, MIL_TEXT("Super-resolution"));
   }

//*****************************************************************************
// Initialize a display that will show 
// - The sequence image.
// - A result of the tracking of the child of the first image in all 
//   the sequence image.
// - The result of super-resolution and of averaging of all images of the
//   sequence.
//*****************************************************************************
void CSuperResolution::InitializeDisplay(MIL_ID MilFirstImage, MIL_INT ChildSizeX, MIL_INT ChildSizeY)
   {
   // Allocate display.
   m_MilDisplay = MdispAlloc(m_MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);
   MdispControl(m_MilDisplay, M_OVERLAY, M_ENABLE);

   MIL_INT SequenceImageSizeX = MbufInquire(MilFirstImage, M_SIZE_X, M_NULL);
   MIL_INT SequenceImageSizeY = MbufInquire(MilFirstImage, M_SIZE_Y, M_NULL);

   // Find the size of the zoomed child.
   MIL_INT ZoomedChildSizeX = static_cast<MIL_INT>(ChildSizeX * MOSAIC_SCALE);
   MIL_INT ZoomedChildSizeY = static_cast<MIL_INT>(ChildSizeY * MOSAIC_SCALE);

   // Allocate full display image.
   MIL_INT FullDisplayImageSizeX = SequenceImageSizeX + Max(ZoomedChildSizeX, SequenceImageSizeX);
   MIL_INT FullDisplayImageSizeY = Max(SequenceImageSizeY, 2*ZoomedChildSizeY);
   m_MilFullDisplayImage = MbufAlloc2d(m_MilSystem,
      FullDisplayImageSizeX, FullDisplayImageSizeY,
      8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
   MbufClear(m_MilFullDisplayImage, M_COLOR_BLACK);

   MdispSelect(m_MilDisplay, m_MilFullDisplayImage);

   MdispInquire(m_MilDisplay, M_OVERLAY_ID, &m_MilOverlayImage);

   // Allocate children in the full display image to 
   // show current image aligned with first image and
   // to show zoomed version of the sequence.
   m_MilTrackingImage = MbufChild2d(m_MilFullDisplayImage,
      SequenceImageSizeX, 0,
      SequenceImageSizeX, SequenceImageSizeY, M_NULL);
   m_MilTrackingOverlayImage = MbufChild2d(m_MilOverlayImage,
      SequenceImageSizeX, 0,
      SequenceImageSizeX, SequenceImageSizeY, M_NULL);

   m_MilZoomedWithAverageImage = MbufChild2d(
      m_MilFullDisplayImage, 
      SequenceImageSizeX, 0,
      ZoomedChildSizeX, ZoomedChildSizeY, M_NULL);

   m_MilZoomedWithSuperResolutionImage = MbufChild2d(
      m_MilFullDisplayImage, 
      SequenceImageSizeX, ZoomedChildSizeY,
      ZoomedChildSizeX, ZoomedChildSizeY, M_NULL);
   }

//*****************************************************************************
// Display the result of the alignment of the child of the first image of the
// sequence with the current image of the sequence.
//*****************************************************************************
void CSuperResolution::DrawCurrentResult()
   {
   // Disable display update.
   MdispControl(m_MilDisplay, M_UPDATE, M_DISABLE);

   // Clear overlay.
   MdispControl(m_MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   // Draw the box of the image 0 in the current image.
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_OFFSET_X, 0);
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_OFFSET_Y, 0);
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_STATIC_INDEX, m_NumberOfImagesInTable-1);

   MgraColor(M_DEFAULT, M_COLOR_GREEN);
   MregDraw(M_DEFAULT, m_MilRegResult, m_MilOverlayImage, M_DRAW_BOX, 0, M_DEFAULT);

   MgraControl(M_DEFAULT, M_TEXT_ALIGN_HORIZONTAL, M_LEFT);
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_VERTICAL, M_TOP);
   MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_OPAQUE);
   MIL_INT Margin = 10;
   MgraText(M_DEFAULT, m_MilOverlayImage, Margin, Margin, MIL_TEXT("Source image"));

   // Draw the current image as aligned with image 0.
   MIL_INT i;
   for (i = 0; i < m_NumberOfImagesInTable-1; i++) // Create an array of images that contains ...
      m_MilPartialSequenceImageTable[i] = M_NULL;  // ... M_NULL in all elements ...
   m_MilPartialSequenceImageTable[m_NumberOfImagesInTable-1] = m_MilSequenceImageTable[m_NumberOfImagesInTable-1]; // ... except for the current image.

   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_COMPOSITION, M_LAST_IMAGE);
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_OFFSET_X, m_FirstImageChildOffsetX);
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_OFFSET_Y, m_FirstImageChildOffsetY);
   MregControl(m_MilRegResult, M_GENERAL, M_MOSAIC_STATIC_INDEX, 0);
   MregTransformImage(m_MilRegResult, m_MilPartialSequenceImageTable, m_MilTrackingImage, m_NumberOfImagesInTable, M_BILINEAR+M_OVERSCAN_CLEAR, M_DEFAULT);
   MregDraw(M_DEFAULT, m_MilRegResult, m_MilTrackingOverlayImage, M_DRAW_BOX, 0, M_DEFAULT);

   MgraText(M_DEFAULT, m_MilTrackingOverlayImage, Margin, Margin, MIL_TEXT("Realigned image"));

   // Re-enable display update.
   MdispControl(m_MilDisplay, M_UPDATE, M_ENABLE);
   }
