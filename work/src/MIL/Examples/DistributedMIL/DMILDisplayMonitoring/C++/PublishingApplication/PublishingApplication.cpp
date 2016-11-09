/******************************************************************************/
/*
 * File name: PublishingApplication.cpp 
 *
 * Synopsis:  This program shows how to allow the monitoring of a MIL application
 *            and how to publish a display image with its overlay. 
 *            The published MIL objects can then be accessed by an external MIL application.
 *            (see MonitoringApplication.cpp example).
 *            
 */
#include <mil.h>

// Target image specifications.
#define IMAGE_FILE               M_IMAGE_PATH MIL_TEXT("BaboonMono.mim")

// Title for the display window.
#define WINDOW_TITLE   MIL_TEXT("Publishing Application")

int MosMain(void)
{
   MIL_ID MilApplication   = M_NULL,
          MilSystem        = M_NULL,
          MilDisplay       = M_NULL,
          MilSrcImage      = M_NULL,
          MilDisplayImage  = M_NULL;
   MIL_INT LicenseModules = 0;

   // Allocate defaults.
   // We do not rely on the DMIL Cluster Manager being setup and hard-code a non-default 
   // Cluster Node number using M_CLUSTER_NODE() to be sure the example works even without
   // the Cluster Manager server.
   MappAllocDefault(M_DEFAULT+M_CLUSTER_NODE(15), &MilApplication, &MilSystem, &MilDisplay, 
                    M_NULL, M_NULL);

   // Now we check if the system is local
   if(MsysInquire(MilSystem, M_LOCATION, M_NULL) != M_LOCAL)
      {
      MosPrintf(MIL_TEXT("This example requires the default system to be a local system.\n"));
      MosPrintf(MIL_TEXT("Please select a local system as the default.\n"));

      MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);
      MosGetch();
      return -1;
      }
   /* Inquire MIL licenses. */
   MappInquire(M_DEFAULT, M_LICENSE_MODULES, &LicenseModules);

   // Restore the source image.
   MbufRestore(IMAGE_FILE, MilSystem, &MilSrcImage);

   // Retrieve the image sizes of the restored image
   MIL_INT ImageSizeX = MbufInquire(MilSrcImage, M_SIZE_X, M_NULL);
   MIL_INT ImageSizeY = MbufInquire(MilSrcImage, M_SIZE_Y, M_NULL);

   // Allocate a display image and show it.
   MbufAllocColor(MilSystem, 3, ImageSizeX, ImageSizeY, 8, M_IMAGE+M_DISP+M_PROC, 
                  &MilDisplayImage);
   MdispSelect(MilDisplay, MilDisplayImage);
   MdispControl(MilDisplay, M_TITLE, M_PTR_TO_DOUBLE(WINDOW_TITLE));

   //************* Prepare the overlay **********************
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MIL_ID MilOverlayImage =0 ;
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);

   // Inquire overlay size.
   MIL_INT ImageWidth  = MbufInquire(MilOverlayImage, M_SIZE_X, M_NULL);
   MIL_INT ImageHeight = MbufInquire(MilOverlayImage, M_SIZE_Y, M_NULL);

   // Draw MIL overlay annotations. 
   MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
   MgraColor(M_DEFAULT, M_COLOR_GREEN);

   //************** Allow Monitoring ***************************
   MappControl(M_DEFAULT, M_DMIL_CONNECTION, M_DMIL_CONTROL);

   MappControlObject(M_DEFAULT, MilDisplayImage, M_OBJECT_NAME, 
                     M_PTR_TO_DOUBLE(MIL_TEXT("DisplayImage")));
   MappControlObject(M_DEFAULT, MilDisplayImage, M_DMIL_PUBLISH, M_READ_ONLY);
   
   MappControlObject(M_DEFAULT, MilOverlayImage, M_OBJECT_NAME, 
                     M_PTR_TO_DOUBLE(MIL_TEXT("OverlayImage")));
   MappControlObject(M_DEFAULT, MilOverlayImage, M_DMIL_PUBLISH, M_READ_ONLY);
   
   //************** Processsing *********************************
   MIL_INT Angle = 0;
   MIL_DOUBLE CenterX = (MIL_DOUBLE)(ImageSizeX)/2;
   MIL_DOUBLE CenterY = (MIL_DOUBLE)(ImageSizeX)/2;
   MIL_TEXT_CHAR Text[64];

   MosPrintf(MIL_TEXT("DMIL MONITORING (Publishing):\n"));
   MosPrintf(MIL_TEXT("-------------------------------------------\n"));
   MosPrintf(MIL_TEXT("The image displayed is published using DMIL in order to\n"));
   MosPrintf(MIL_TEXT("make it available for external monitoring.\n\n"));
   MosPrintf(MIL_TEXT("You can now run the monitoring example.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));



   while(!MosKbhit())
      {
      Angle = (Angle + 5)%360;

      // In order to optimize display updates and network transfers,
      // modifications hooks and display updates are disabled
      MdispControl(MilDisplay, M_UPDATE, M_DISABLE);
      MbufControl(MilDisplayImage, M_MODIFICATION_HOOK, M_DISABLE);
      MbufControl(MilOverlayImage, M_MODIFICATION_HOOK, M_DISABLE);
      
      // Rotate the image
#if (!M_MIL_LITE)
      if(LicenseModules & M_LICENSE_IM)
         {
         MimRotate(MilSrcImage, MilDisplayImage, (MIL_DOUBLE)Angle, CenterX, CenterX, 
                   CenterX, CenterY, M_NEAREST_NEIGHBOR);
         }
      else
#endif
         {
         MbufCopy(MilSrcImage,MilDisplayImage);
         MgraColor(M_DEFAULT, 0x80);
         MgraArcFill(M_DEFAULT, MilDisplayImage, CenterX, CenterY, (MIL_DOUBLE)Angle, 
                     (MIL_DOUBLE)Angle, 0, 360);
         }
      
      // Modify the overlay
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
      MosSprintf(Text, 64, MIL_TEXT(" - MIL Overlay Text (%d)- "), Angle);
      MgraText(M_DEFAULT, MilOverlayImage, CenterX, CenterX, Text);
      MdispControl(MilDisplay, M_UPDATE, M_ENABLE);
      MbufControl(MilDisplayImage, M_MODIFICATION_HOOK, M_ENABLE);
      MbufControl(MilOverlayImage, M_MODIFICATION_HOOK, M_ENABLE);
      MbufControl(MilDisplayImage, M_MODIFIED, M_DEFAULT);
      MbufControl(MilOverlayImage, M_MODIFIED, M_DEFAULT);
      }

   MbufFree(MilSrcImage);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, MilDisplayImage);
}  