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
  typedef otb::MultiChannelExtractROI<FloatVectorImageType::InternalPixelType, 
                                      FloatVectorImageType::InternalPixelType> ExtractROIFilterType;
  
private:
  ExtractROI()
  {
    SetName("ExtractROI");
    SetDescription("Extract a ROI defined by the user.");
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

    // Channelist Parameters
    AddParameter(ParameterType_ListView,  "cl", "Output Image channels");
  }

  void DoUpdateParameters()
  {
  std::cout<<"update????????????????????????????????????????????"<<std::endl;
    // Update the sizes only if the user does not defined a size
    if ( HasValue("in") )
      {
      ExtractROIFilterType::InputImageType* inImage = GetParameterImage("in");
      ExtractROIFilterType::InputImageType::RegionType  largestRegion = inImage->GetLargestPossibleRegion();

      // Update the values of the channels to be selected
      unsigned int nbComponents = inImage->GetNumberOfComponentsPerPixel();
      ClearChoices("cl");
      for (unsigned int idx = 0; idx < nbComponents; ++idx)
        {
        std::ostringstream key, item;
        key<<"cl.channel"<<idx+1;
        item<<"Channel"<<idx+1;
        AddChoice(key.str(), item.str());
        }

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
    FloatVectorImageType::RegionType region;
    region.SetSize(0,  GetParameterInt("sizex"));
    region.SetSize(1,  GetParameterInt("sizey"));
    region.SetIndex(0, GetParameterInt("startx"));
    region.SetIndex(1, GetParameterInt("starty"));
    
    if ( HasValue("in") )
      {
        if (region.Crop(GetParameterImage("in")->GetLargestPossibleRegion()))
          {
            SetParameterInt("sizex", region.GetSize(0));
            SetParameterInt("sizey", region.GetSize(1));
            SetParameterInt("startx", region.GetIndex(0));
            SetParameterInt("starty", region.GetIndex(1));
            return true;
          }
      }
    return false;
  }

  void DoExecute()
  { 
    std::cout<<"goooooooooooooooooooooooooooooooooooooooooooooooooooooooo"<<std::endl;
    ExtractROIFilterType::InputImageType* inImage = GetParameterImage("in");
    inImage->UpdateOutputInformation();

    m_ExtractROIFilter = ExtractROIFilterType::New();
    m_ExtractROIFilter->SetInput(inImage);
    m_ExtractROIFilter->SetStartX(GetParameterInt("startx"));
    m_ExtractROIFilter->SetStartY(GetParameterInt("starty"));
    m_ExtractROIFilter->SetSizeX(GetParameterInt("sizex"));
    m_ExtractROIFilter->SetSizeY(GetParameterInt("sizey"));

    // std::cout <<"startx"<<GetParameterInt("startx") << std::endl;
    // std::cout <<"starty"<<GetParameterInt("starty") << std::endl;
    // std::cout <<"sizex"<<GetParameterInt("sizex") << std::endl;
    // std::cout <<"sizey"<<GetParameterInt("sizey") << std::endl;

    std::cout <<"Channels added : "<<std::endl;
    for (unsigned int idx = 0; idx < GetSelectedItems("cl").size(); ++idx)
      {
        std::cout << GetSelectedItems("cl")[idx] + 1<<  " ";
      m_ExtractROIFilter->SetChannel(GetSelectedItems("cl")[idx] + 1 );
      }
    std::cout<<std::endl;

    SetParameterOutputImage("out", m_ExtractROIFilter->GetOutput());
  }

  ExtractROIFilterType::Pointer   m_ExtractROIFilter;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ExtractROI)
