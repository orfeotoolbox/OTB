/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbMultiChannelExtractROI.h"
#include "otbStandardFilterWatcher.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperTypes.h"

namespace otb
{
namespace Wrapper
{

class ExtractROI : public Application
{
public:
  /** Standard class typedefs. */
  typedef ExtractROI                    Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ExtractROI, otb::Application);

  /** Filters typedef */
  typedef otb::MultiChannelExtractROI<VectorImageType::InternalPixelType, 
                                      VectorImageType::InternalPixelType> ExtractROIFilterType;
  
private:
  ExtractROI()
  {
    SetName("ExtractROI");
    SetDescription("Extract a ROI defined by the user.");
    m_ExtractROIFilter = ExtractROIFilterType::New();
  }

  virtual ~ExtractROI()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");

    AddParameter(ParameterType_Int,  "startx", "Start X");
    AddParameter(ParameterType_Int,  "starty", "Start Y");
    AddParameter(ParameterType_Int,  "sizex",  "Size X");
    AddParameter(ParameterType_Int,  "sizey",  "Size Y");
    // Default values
    SetParameterInt("startx", 0);
    SetParameterInt("starty", 0);
    SetParameterInt("sizex",  0);
    SetParameterInt("sizey",  0);

    // Output pixel type choices
    AddParameter(ParameterType_Choice,  "t", "Output Pixel Type");
    AddChoice("t.double",   "double");
    AddChoice("t.uchar",    "unsigned Char");
    AddChoice("t.shortint", "short int");
    AddChoice("t.int",      "int");
    AddChoice("t.float",    "float");
    AddChoice("t.uishort",  "unsigned short int");
    AddChoice("t.uint",     "unsigned int");

    // Channelist Parameters
    
  }

  void DoUpdateParameters()
  {
    
    // Update the sizes only if the user does not defined a size
    if ( HasValue("in") )
      {
      ExtractROIFilterType::InputImageType* inImage = GetParameterImage("in");
      ExtractROIFilterType::InputImageType::RegionType  largestRegion = inImage->GetLargestPossibleRegion();
      
      if (!HasUserValue("sizex")  && !HasUserValue("sizey") )
        {
        SetParameterInt("sizex", largestRegion.GetSize()[0]);
        SetParameterInt("sizey", largestRegion.GetSize()[1]);       
        }
      
      // Put the limit of the index and the size relative the image
      dynamic_cast< NumericalParameter<int> * >(GetParameterByKey("sizex"))->SetMinimumValue(0); 
      dynamic_cast< NumericalParameter<int> * >(GetParameterByKey("sizex"))->SetMaximumValue(largestRegion.GetSize(0));

      dynamic_cast< NumericalParameter<int> * >(GetParameterByKey("sizey"))->SetMinimumValue(0); 
      dynamic_cast< NumericalParameter<int> * >(GetParameterByKey("sizey"))->SetMaximumValue(largestRegion.GetSize(1));

      dynamic_cast< NumericalParameter<int> * >(GetParameterByKey("startx"))->SetMinimumValue(0); 
      dynamic_cast< NumericalParameter<int> * >(GetParameterByKey("startx"))->SetMaximumValue(largestRegion.GetSize(0));
        
      dynamic_cast< NumericalParameter<int> * >(GetParameterByKey("starty"))->SetMinimumValue(0); 
      dynamic_cast< NumericalParameter<int> * >(GetParameterByKey("starty"))->SetMaximumValue(largestRegion.GetSize(1));

      // Update the channel list
      }
    
    // Crop the roi region to be included in the largest possible
    // region  
    if(!this->CropRegionOfInterest())
      {
      // Put the index of the ROI to origin and try to crop again
      SetParameterInt("startx", 0);
      SetParameterInt("starty", 0);
      this->CropRegionOfInterest();
      }
  }

  bool CropRegionOfInterest()
  {
    VectorImageType::RegionType region;
    region.SetSize(0,  GetParameterInt("sizex"));
    region.SetSize(1,  GetParameterInt("sizey"));
    region.SetIndex(0, GetParameterInt("startx"));
    region.SetIndex(1, GetParameterInt("starty"));
    
    if (region.Crop(GetParameterImage("in")->GetLargestPossibleRegion()))
      {
      SetParameterInt("sizex", region.GetSize(0));
      SetParameterInt("sizey", region.GetSize(1));
      SetParameterInt("startx", region.GetIndex(0));
      SetParameterInt("starty", region.GetIndex(1));
      return true;
      }
    return false;
  }

  void DoExecute()
  { 
    typename ExtractROIFilterType::InputImageType* inImage = GetParameterImage("in");
    
    m_ExtractROIFilter->SetInput(inImage);
    m_ExtractROIFilter->SetStartX(GetParameterInt("startx"));
    m_ExtractROIFilter->SetStartY(GetParameterInt("starty"));
    m_ExtractROIFilter->SetSizeX(GetParameterInt("sizey"));
    m_ExtractROIFilter->SetSizeY(GetParameterInt("sizey"));

    m_ExtractROIFilter->UpdateOutputInformation();
    SetParameterOutputImage("out", m_ExtractROIFilter->GetOutput());   
  }

  ExtractROIFilterType::Pointer   m_ExtractROIFilter;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ExtractROI)

