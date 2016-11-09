/******************************************************************************/
/*
 * File name: MonitoringApplication.cpp 
 *
 * Synopsis:  This program monitors an application (see PublishingApplication.cpp example)
 *            It shows how to copy the content of a published remote display image and its
 *            overlay in a local display image and a local overlay, to be viewed locally.
 */
#include "mil.h"

// Title for the display window.
#define WINDOW_TITLE   MIL_TEXT("Monitoring Application")

int MosMain(void)
{
   // Local resources
   MIL_ID MilApplication      = M_NULL,
          MilSystem           = M_NULL,
          LocalDisplay        = M_NULL,
          LocalDisplayImage   = M_NULL,
          LocalOverlayImage   = M_NULL;
   
   // Remote Resources
   MIL_ID RemoteDisplayImage     = M_NULL, 
          RemoteOverlayImage     = M_NULL,
          MilRemoteApplication   = M_NULL;

   // Allocate defaults
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &LocalDisplay, M_NULL, M_NULL);
   
   // Initiate the monitoring
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
   MappOpenConnection(MIL_TEXT("dmiltcp://localhost"), M_DEFAULT, M_DEFAULT,
                      &MilRemoteApplication);
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

   if(MilRemoteApplication == 0)
      {
      MosPrintf(MIL_TEXT("Cannot open a connection to the remote application.\n"));
      MosPrintf(MIL_TEXT("Please make sure the publishing example is running.\n"));
      }
   else
      {

      // Retrieve published objects
      MappInquireConnection(MilRemoteApplication, M_DMIL_PUBLISHED_NAME, 
         M_PTR_TO_MIL_INT(MIL_TEXT("DisplayImage")), M_DEFAULT, &RemoteDisplayImage);
      MappInquireConnection(MilRemoteApplication, M_DMIL_PUBLISHED_NAME, 
         M_PTR_TO_MIL_INT(MIL_TEXT("OverlayImage")), M_DEFAULT, &RemoteOverlayImage);

      // If a remote image is published, we want to display it locally.
      if(RemoteDisplayImage)
         {
         // Allocate a local display image in which remote image will be copied
         MbufAllocColor(MilSystem, 
                        MbufInquire(RemoteDisplayImage, M_SIZE_BAND, M_NULL),
                        MbufInquire(RemoteDisplayImage, M_SIZE_X, M_NULL),
                        MbufInquire(RemoteDisplayImage, M_SIZE_Y, M_NULL),
                        MbufInquire(RemoteDisplayImage, M_TYPE, M_NULL),
                        M_IMAGE+M_DISP,
                        &LocalDisplayImage);
      
         // Copy remote image contain
         MbufCopy(RemoteDisplayImage, LocalDisplayImage);

         // Select this local image on a local display
         MdispSelect(LocalDisplay, LocalDisplayImage);
         MdispControl(LocalDisplay, M_TITLE, M_PTR_TO_DOUBLE(WINDOW_TITLE));

         // Hook to remote image modifications
         MbufLink(RemoteDisplayImage, LocalDisplayImage, M_LINK, M_DEFAULT);

         // If an overlay has been published, show it.
         if(RemoteOverlayImage)
            {
            // Enable the overlay on the local display and inquire its ID
            MdispControl(LocalDisplay, M_OVERLAY, M_ENABLE);
            LocalOverlayImage = (MIL_ID)MdispInquire(LocalDisplay, M_OVERLAY_ID, M_NULL);

            // Copy remote overlay contain in local overlay
            MbufCopy(RemoteOverlayImage, LocalOverlayImage);
         
            // Hook to the modifications of the remote overlay (will be copied in local overlay
            MbufLink(RemoteOverlayImage, LocalOverlayImage, M_LINK, M_DEFAULT);
            }

         MosPrintf(MIL_TEXT("DMIL MONITORING (Monitoring):\n"));
         MosPrintf(MIL_TEXT("-------------------------------------------\n"));
         MosPrintf(MIL_TEXT("The image displayed is the image published by another "));
         MosPrintf(MIL_TEXT("application for DMIL monitoring.\n\n"));
         MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
         MosGetch();
      
         // Make sure to unlink.
         MbufLink(RemoteDisplayImage, LocalDisplayImage, M_UNLINK, M_DEFAULT);
      
         if(RemoteOverlayImage)
            MbufLink(RemoteOverlayImage, LocalOverlayImage, M_UNLINK, M_DEFAULT);
         }
      else
         {
         MosPrintf(MIL_TEXT("No image to display\n"));
         MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
         MosGetch();
         }
   
      MappCloseConnection(MilRemoteApplication);
      }
   MappFreeDefault(MilApplication, MilSystem, LocalDisplay, M_NULL, LocalDisplayImage);
}  
