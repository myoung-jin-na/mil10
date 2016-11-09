//***************************************************************************************
// 
// File name: defectdetectionexample.h
//
// Synopsis: This file contains the declaration of the CDefectDetectionExamplMngr class
//           that handles all the aspects of the defect detection example except the 
//           processing part.
//

#ifndef DEFECT_DETECTION_EXAMPLE_H
#define DEFECT_DETECTION_EXAMPLE_H

// Forward enum declares.
enum DifferenceExtractionMethod;
enum BinarizationMethod;

class CDefectDetectionExampleMngr
   {
   public:
     
      // Constructor.
      CDefectDetectionExampleMngr(MIL_CONST_TEXT_PTR SystemDescriptor);
      
      // Destructor.
      virtual ~CDefectDetectionExampleMngr();
     
      // Function to load the template.
      bool LoadTemplate(MIL_CONST_TEXT_PTR TemplateImagePath);

      // Function to initialize the example based on the target an on the processing methods.
      bool InitializeExample(MIL_CONST_TEXT_PTR TargetImagePath, DifferenceExtractionMethod DiffMethod, BinarizationMethod BinMethod);

      // Main running function.
      void Run(MIL_CONST_TEXT_PTR CaseInformationText);

      // Function to ask if the example will be ran with interactivity.
      bool AskForInteractive();
      
   protected:
      
   private:
      
      //Function that resets the display
      void ResetDisplay(MIL_INT TargetSizeX, MIL_INT TargetSizeY);
      
      //Function that frees the different images
      void FreeDisplayImages();
      void FreeWorkImages();

      //Function that creates the luminance image
      void CreateLuminanceImage(MIL_ID MilSrc, MIL_INT SizeX, MIL_INT SizeY, MIL_ID *MilLumImagePtr);

      // Intermediate display and information printing functions.
      void ShowTargetAndTemplate(MIL_CONST_TEXT_PTR TargetImagePath);
      void ShowAllOccurrences();
      void ShowWarpedOccurrence(MIL_INT OccurenceIdx);
      void ShowDefectDetection();
      
      // Functions to draw the histogram
      void InitHistogram();
      void ShowHistogram();

      // Function to clear the defect detection
      void ClearDefectDetection();
      
      // Function to draw the display childs info
      void DrawDisplayChildInfo(MIL_ID MilGraContext, MIL_INT ImageSizeX, MIL_INT ImageSizeY, MIL_CONST_TEXT_PTR ImageName);

      // Functions to display and ask for the difference and binarization method.
      void PrintMethodChoices(MIL_CONST_TEXT_PTR MethodTypeTag, MIL_CONST_TEXT_PTR *MethodsTags, MIL_INT NbMethod, MIL_TEXT_CHAR StartChar);
      void PrintCurrentMethods(MIL_TEXT_CHAR DiffStartChar, MIL_TEXT_CHAR BinStartChar, DifferenceExtractionMethod DiffMethod, BinarizationMethod BinMethod);
      bool AskForMethod(DifferenceExtractionMethod &DiffMethod, BinarizationMethod &BinMethod);
      
      // The difference extraction and binarization methods currently in use in the example.
      DifferenceExtractionMethod m_DiffMethod;
      BinarizationMethod m_BinMethod;
   
      // The refresh status of the example
      bool m_ChangeBin;
      bool m_ChangeDiff;
      bool m_IsInteractive;

      // The transparent color of the overlay.
      MIL_DOUBLE   m_OverlayTransparentColor;

      // The size of the template.
      MIL_INT   m_TemplateSizeX;
      MIL_INT   m_TemplateSizeY;

      // The binarization value
      MIL_INT   m_BinValue;

      // The global MIL objects.
      MIL_ID    m_MilApplication;
      MIL_ID    m_MilSystem;
      MIL_ID    m_MilDisplay;
      MIL_ID    m_MilDisplayImage;
      MIL_ID    m_MilGraList;

      // The histogram display identifiers
      MIL_ID    m_MilHistDisplay;
      MIL_ID    m_MilHistDisplayImage;
      MIL_ID    m_MilHistDispGraphicList;
      MIL_ID    m_MilHistResult;

      // The graphic context for the overlay childs.
      MIL_ID   m_MilGraContextTemplate;
      MIL_ID   m_MilGraContextTarget;
      MIL_ID   m_MilGraContextWarpedTarget;
      MIL_ID   m_MilGraContextDifference;
      MIL_ID   m_MilGraContextInspection;

      // The spacing of the display images
      MIL_INT  m_ImageSpacing;

      // The loaded template.
      MIL_ID    m_MilTemplateImage;

      // The display image childs.
      MIL_ID    m_MilDispTemplateImage;                    
      MIL_ID    m_MilTargetImage;           
      MIL_ID    m_MilWarpedTargetImage;              
      MIL_ID    m_MilDifferenceImage;           
      MIL_ID    m_MilInspectionImage;           
      
      // The work images.
      MIL_ID    m_MilTemplateGradientMask;
      MIL_ID    m_MilTemplateLumGradientMask;
      MIL_ID    m_MilTemplateLumImage;          
      MIL_ID    m_MilTargetLumImage;            
      MIL_ID    m_MilDifferenceGrayImage;
      
      // The structuring element for the morphology.
      MIL_ID    m_MilStructElement;

      // The Model Finder context and result.
      MIL_ID    m_MilModContext;
      MIL_ID    m_MilModResult;
      MIL_INT   m_NbOccurences;

      // The blob context and result.
      MIL_ID    m_MilBlobResult;
      MIL_ID    m_MilBlobFeatureList;
      MIL_INT   m_BlobDrawLabel;

      // The drawing important index
      MIL_INT   m_ModOccDrawStartIndex;

      // The fixturing offset.
      MIL_ID    m_MilFixturingOffset;

      // Utility function to get the filename from a path
      inline MIL_CONST_TEXT_PTR GetFileName(MIL_CONST_TEXT_PTR FilePath)
         {
         if(FilePath)
            {
            MIL_CONST_TEXT_PTR FileName = FilePath + MosStrlen(FilePath);
            for(;FileName > FilePath && FileName[0] != MIL_TEXT('/') && FileName[0] != MIL_TEXT('\\'); FileName--);
            return FileName == FilePath ? NULL : FileName + 1;    
            }
         else
            return MIL_TEXT("");
         
         }
   };

#endif