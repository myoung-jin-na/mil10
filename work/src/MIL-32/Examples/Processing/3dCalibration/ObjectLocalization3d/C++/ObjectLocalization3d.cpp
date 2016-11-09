//***************************************************************************************/
// 
// File name: ObjectLocalization3d.cpp  
//
// Synopsis:  See the PrintHeader() function below for detailed description.
//

#include "common.h"
#include "FeatureFinder.h"

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("ObjectLocalization3d\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This example demonstrates how the calibration module can be used to determine\n")
             MIL_TEXT("the 3d position and orientation of an object after locating known fiducials on\n")
             MIL_TEXT("the object.\n\n")
 
             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: application, system, display, digitizer, buffer, graphic,\n")
             MIL_TEXT("              image processing, calibration, code.\n\n")

             MIL_TEXT("Press <Enter> to start.\n\n"));
   MosGetch();
   }

//****************************************************************************
// Constants.
//****************************************************************************

// Directory containing all files used in this example.
#define EXAMPLE_IMAGE_PATH          M_IMAGE_PATH MIL_TEXT("ObjectLocalization3d/")

// File containing the calibration context for the camera to use.
static const MIL_TEXT_CHAR* const CALIBRATION_CONTEXT_FILE = EXAMPLE_IMAGE_PATH MIL_TEXT("Calib.mca");
static const MIL_TEXT_CHAR* const SEQ_FILE                 = EXAMPLE_IMAGE_PATH MIL_TEXT("seq.avi");

// Colors used to draw in the overlay.
static const MIL_DOUBLE COORDINATE_SYSTEM_COLOR = M_COLOR_GRAY;
static const MIL_DOUBLE BOUNDING_BOX_COLOR      = M_COLOR_CYAN;

// Coordinates used to draw a bounding box around the object to locate.
static const MIL_INT NB_VERTICES_PER_SIDE = 7;
static const MIL_INT NB_CAR_VERTICES      = 2*NB_VERTICES_PER_SIDE;

static const MIL_DOUBLE CAR_VERTEX_X[NB_CAR_VERTICES] = { 130.1,  126.6,  115.7,  120.3,  128.6,  127.2,  135.1,   40.6,   45.4,   56.3,   49.5,   42.0,   39.5,   35.1};
static const MIL_DOUBLE CAR_VERTEX_Y[NB_CAR_VERTICES] = { 223.7,  213.3,  196.1,   82.1,   43.5,  -17.7,  -24.2,  221.8,  214.0,  198.1,   80.6,   41.9,  -17.5,  -25.3};
static const MIL_DOUBLE CAR_VERTEX_Z[NB_CAR_VERTICES] = { -21.4,  -64.0,  -89.9,  -87.7,  -64.4,  -49.9,  -26.7,  -27.5,  -63.7,  -87.3,  -87.7,  -63.8,  -50.4,  -24.0};

//****************************************************************************
// Functions declarations.
//****************************************************************************

bool LocateObject(MIL_ID MilCalibration, const SObjectFeatures* pObjectFeatures);
void DisplayBoundingBox(MIL_ID MilDisplay, MIL_ID MilCalibration);

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   PrintHeader();

   // Allocate MIL objects. 
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   MIL_ID MilSystem      = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);
   MIL_ID MilDisplay     = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);

   // Get image size from the video file.
   MIL_INT SizeX = MbufDiskInquire(SEQ_FILE, M_SIZE_X, M_NULL);
   MIL_INT SizeY = MbufDiskInquire(SEQ_FILE, M_SIZE_Y, M_NULL);

   // Setup display image.
   MIL_ID MilDisplayImage = MbufAlloc2d(MilSystem, SizeX, SizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
   MbufClear(MilDisplayImage, 0.0);

   MdispSelect(MilDisplay, MilDisplayImage);

   // Prepare overlay for annotations.
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);

   // Reload the 3d calibration context from disk.
   MIL_ID MilCalibration = McalRestore(CALIBRATION_CONTEXT_FILE, MilSystem, M_DEFAULT, M_NULL);
   if (MilCalibration != M_NULL)
      {
      MosPrintf(MIL_TEXT("The Tsai-based camera calibration context was restored from disk.\n\n"));

      MosPrintf(MIL_TEXT("For each frame of the video sequence:\n")
                MIL_TEXT(" - The circular fiducials are found using blob analysis (red crosses).\n")
                MIL_TEXT(" - Each fiducial is uniquely identified by reading its bar code\n")
                MIL_TEXT("   (green numeric labels).\n")
                MIL_TEXT(" - The fiducials' pixel coordinates are paired with the known world\n")
                MIL_TEXT("   coordinates using their labels.\n")
                MIL_TEXT(" - McalList() is called to determine the position and orientation of the car.\n")
                MIL_TEXT(" - The bounding box of the car is drawn in the overlay.\n\n"));

      MosPrintf(MIL_TEXT("Press <Enter> to exit.\n\n"));

      // Allocate all necessary objects to search for fiducials.
      CFeatureFinder  FeatureFinder(MilSystem, SizeX, SizeY);
      SObjectFeatures ObjectFeatures;

      // Open the video sequence and inquire some informations.
      MbufImportSequence(SEQ_FILE, M_AVI_MIL, M_NULL, M_NULL, M_NULL, M_NULL, M_NULL, M_OPEN);
      MIL_INT NbFrames = MbufDiskInquire(SEQ_FILE, M_NUMBER_OF_IMAGES, M_NULL);

      MIL_DOUBLE ImagePeriod; // seconds/image
      MbufDiskInquire(SEQ_FILE, M_FRAME_RATE, &ImagePeriod);

      ImagePeriod = 1.0 / ImagePeriod;

      // Start at the first frame of the video.
      MIL_INT CurFrame = 0;

      bool MustQuit = false;
      while (!MustQuit)
         {
         MIL_DOUBLE StartTime = MappTimer(M_DEFAULT, M_TIMER_READ, M_NULL);

         // Disable display updates.
         MdispControl(MilDisplay, M_UPDATE, M_DISABLE);

         // Clear overlay.
         MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

         // Load the next frame in the video sequence.
         MbufImportSequence(SEQ_FILE, M_AVI_MIL, M_LOAD, M_NULL, &MilDisplayImage, CurFrame, 1, M_READ);

         // Find in the image some features of the object at known world locations.
         FeatureFinder.Find(MilDisplay, MilDisplayImage, &ObjectFeatures);

         // Check that there are enough features to locate the object.
         if (ObjectFeatures.m_NumPoint >= MIN_NB_FEATURES)
            {
            // Use the calibration module to find the position and orientation of the object.
            bool ObjectWasLocated = LocateObject(MilCalibration, &ObjectFeatures);

            // Display a bounding box around the object, in the overlay.
            if (ObjectWasLocated)
               {
               DisplayBoundingBox(MilDisplay, MilCalibration);
               }
            }

         // Enable (and force) display updates.
         MdispControl(MilDisplay, M_UPDATE, M_ENABLE);

         // Go to the next frame of the video, or wrap-around (loop endlessly).
         if (++CurFrame == NbFrames)
            CurFrame = 0;

         // Compute elapsed time for this iteration.
         MIL_DOUBLE ElapsedTime;
         MappTimer(M_DEFAULT, M_TIMER_READ, &ElapsedTime);
         ElapsedTime -= StartTime;

         // Wait until the end of the image period to keep a correct video display speed.
         MIL_DOUBLE WaitTime = ImagePeriod - ElapsedTime;
         if (WaitTime > 0.0)
            MappTimer(M_DEFAULT, M_TIMER_WAIT, &WaitTime);

         // Read all characters typed by the user during this iteration.
         while (MosKbhit())
            {
            switch (MosGetch())
               {
               // If user presses <ENTER>, quit this loop.
               case MIL_TEXT('\r'):
               case MIL_TEXT('\n'):
                  MustQuit = true;
                  break;
               }
            }
         }

      // Close the video file.
      MbufImportSequence(SEQ_FILE, M_AVI_MIL, M_NULL, M_NULL, M_NULL, M_NULL, M_NULL, M_CLOSE);
      }
   else
      {
      MosPrintf(MIL_TEXT("Unable to reload the calibration context.\n")
                MIL_TEXT("Press <Enter> to exit.\n\n"));
      MosGetch();
      }

   // Free MIL objects.
   McalFree(MilCalibration);
   MbufFree(MilDisplayImage);
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

//*****************************************************************************
// Use McalList() to find the position and orientation of the object. Returns
// true if successful.
//*****************************************************************************
bool LocateObject(MIL_ID MilCalibration, const SObjectFeatures* pObjectFeatures)
   {
   // Disable error printing to avoid runtime errors about collinear points.
   MIL_INT PreviousErrorPrintingState;
   MappInquire(M_DEFAULT, M_ERROR, &PreviousErrorPrintingState);
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);

   // Try locating the object.
   McalList(MilCalibration,
            pObjectFeatures->m_XPixArray,
            pObjectFeatures->m_YPixArray,
            pObjectFeatures->m_XWorldArray,
            pObjectFeatures->m_YWorldArray,
            pObjectFeatures->m_ZWorldArray,
            pObjectFeatures->m_NumPoint,
            M_DISPLACE_RELATIVE_COORD,
            M_DEFAULT);

   // Re-enable error printing if necessary.
   MappControl(M_DEFAULT, M_ERROR, PreviousErrorPrintingState);

   // Check if the call was successful.
   MIL_INT LocateStatus = McalInquire(MilCalibration, M_CALIBRATION_STATUS, M_NULL);
   return (LocateStatus == M_CALIBRATED);
   }

//*****************************************************************************
// Draw a bounding box in the overlay around the object.
//*****************************************************************************
void DisplayBoundingBox(MIL_ID MilDisplay, MIL_ID MilCalibration)
   {
   // Get overlay Id.
   MIL_ID MilOverlayImage;
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);

   // Draw the coordinate system, i.e. the "floor" on which the car is found.
   MgraColor(M_DEFAULT, COORDINATE_SYSTEM_COLOR);
   McalDraw(M_DEFAULT, MilCalibration, MilOverlayImage, M_DRAW_RELATIVE_COORDINATE_SYSTEM, M_DEFAULT, M_DEFAULT);

   // Convert the bounding box coordinates from CAD to image.
   MIL_DOUBLE ImageVertexX[NB_CAR_VERTICES];
   MIL_DOUBLE ImageVertexY[NB_CAR_VERTICES];

   McalTransformCoordinate3dList(MilCalibration,
                                 M_RELATIVE_COORDINATE_SYSTEM,
                                 M_PIXEL_COORDINATE_SYSTEM,
                                 NB_CAR_VERTICES,
                                 CAR_VERTEX_X,
                                 CAR_VERTEX_Y,
                                 CAR_VERTEX_Z,
                                 ImageVertexX,
                                 ImageVertexY,
                                 M_NULL,
                                 M_DEFAULT);

   MgraColor(M_DEFAULT, BOUNDING_BOX_COLOR);
   MgraControl(M_DEFAULT, M_INPUT_UNITS, M_PIXEL);

   // Draw one side of the car as a single polygon.
   MgraLines(M_DEFAULT, MilOverlayImage, NB_VERTICES_PER_SIDE,
             ImageVertexX, ImageVertexY,
             M_NULL, M_NULL, M_POLYGON);

   // Draw the other side as another polygon.
   MgraLines(M_DEFAULT, MilOverlayImage, NB_VERTICES_PER_SIDE,
             ImageVertexX+NB_VERTICES_PER_SIDE, ImageVertexY+NB_VERTICES_PER_SIDE,
             M_NULL, M_NULL, M_POLYGON);

   // Draw lines between the corresponding vertices of the two polygons.
   MgraLines(M_DEFAULT, MilOverlayImage, NB_VERTICES_PER_SIDE,
             ImageVertexX, ImageVertexY,
             ImageVertexX+NB_VERTICES_PER_SIDE, ImageVertexY+NB_VERTICES_PER_SIDE, M_DEFAULT);
   }
