//***************************************************************************************
// 
// File name: InspectionTask.h
//
// Synopsis: This file contains the declaration of the CInspectionTask class
//           which is the base class of all the inspection tasks.
//

#ifndef INSPECTION_TASK_H
#define INSPECTION_TASK_H

static const MIL_INT RESULT_TEXT_LINE_SPACING_Y = 24;
static const MIL_INT TITLE_LINE_SPACING_Y = 32;

class CInspectionTask
   {
   public:

      // Constructor.
      CInspectionTask(MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL, CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CInspectionTask();

      // Initialization function.
      virtual void Init(MIL_ID MilSystem, MIL_INT ImageSizeX = 0, MIL_INT ImageSizeY = 0);    

      // Free function.
      virtual void Free();

      // Inspection function.
      virtual void InspectImage(MIL_ID MilImage);
                  
      // Function to allocate the outputs (image, region).
      void AllocateOutputImage(MIL_ID MilSystem, MIL_INT NbBands, MIL_INT SizeX, MIL_INT SizeY, MIL_INT Type, MIL_INT64 BufAttribute);
      void AllocateRegionGraList(MIL_ID MilSystem);

      // Functions to get the outputs(fixture, image, regions) of the task if available.
      MIL_ID GetFixture() const { return m_MilWorkImage; }
      MIL_ID GetOutputImage() const { return m_MilOutputImage; }
      MIL_ID GetOutputRegionList() const { return m_MilOutputRegionGraList; }

      // Function to get the validity of the inspection task result.
      bool IsResultValid() const { return m_IsResultValid; }

      // Function to get whether there was a provided region.
      const MIL_ID GetInputRegionList() const { return m_RegionProvider->GetOutputRegionList(); }
      bool HasRegionProvider() const { return m_RegionProvider != M_NULL; }

      // Drawing functions.
      void DrawInspectionGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest) = 0;

      // Function to draw the output fixture.
      void DrawOutputFixture(MIL_ID MilGraContext, MIL_ID MilDest);

      // Virtual function to draw the output region.
      virtual void DrawOutputRegion(MIL_ID MilGraContext, MIL_ID MilDest){};
      
      // Function to draw the input region.
      void DrawInputRegion(MIL_ID MilGraContext, MIL_ID MilDest);

   protected:

      // Inspection function.
      virtual bool Inspect(MIL_ID MilImage) = 0;

      // Virtual function that calls the drawing function of the current task.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest) = 0;
      
      // Function to set the output fixture.
      void SetOutputFixture(MIL_INT LocationType, MIL_ID FixturingOffset, MIL_INT CalOrLocSourceId, MIL_DOUBLE Param1, MIL_DOUBLE Param2, MIL_DOUBLE Param3, MIL_DOUBLE Param4);
                  
      // Function to create the converted image.
      MIL_ID CreateConvertedImage(MIL_ID MilColorImage, MIL_INT ConversionType);

      // Function to get the color conversion.
      MIL_INT GetColorConversion() const { return m_ColorConversion; }

   private:

      // Function to set the image fixture.
      void SetImageFixture(MIL_ID MilImage);

      // The fixture provider step that might be used by the task.
      CInspectionTask* m_FixtureProvider;
      CInspectionTask* m_ImageProvider;
      CInspectionTask* m_RegionProvider;

      // The output fixture from the inspection task.
      MIL_ID m_MilWorkImage;

      // The output image from the inspection task.
      MIL_ID m_MilOutputImage;

      // The output region from the inspection task.
      MIL_ID m_MilOutputRegionGraList;
      
      // The conversion task to be done .
      MIL_INT m_ColorConversion;

      // A boolean indicating the result valid status.
      bool m_IsResultValid;
   };

// Utility function to clone a path.
static void CloneString(MIL_TEXT_PTR &Dest, MIL_CONST_TEXT_PTR Source)
   {
   MIL_INT PathLength = MosStrlen(Source);
   Dest = new MIL_TEXT_CHAR[PathLength+1];
   MosStrcpy(Dest, PathLength+1, Source);
   }

// Utility function to put an angle between -180 and 180.
static void PutAngleBetweenMinus180And180(MIL_DOUBLE &rAngle)
   {
   if(rAngle > 180.0)
      { rAngle -= 360.0; }
   }

// Utility function to move the graphic context by a certain number of lines.
static void MoveGraphicContextYOffset(MIL_ID MilGraContext, MIL_INT NbLines)
   {
   MgraControl(MilGraContext, M_DRAW_OFFSET_Y, (MIL_DOUBLE)(MgraInquire(MilGraContext, M_DRAW_OFFSET_Y, M_NULL) - NbLines * RESULT_TEXT_LINE_SPACING_Y));
   }

#endif // INSPECTION_TASK_H