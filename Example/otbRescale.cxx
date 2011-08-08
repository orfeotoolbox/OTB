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

#include "otbStreamingMinMaxVectorImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"

namespace otb
{
namespace Wrapper
{


class Rescale : public Application
{
public:
  /** Standard class typedefs. */
  typedef Rescale                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Rescale, otb::Application);

  /** Filters typedef */
  typedef otb::StreamingMinMaxVectorImageFilter<VectorImageType>  MinMaxFilterType;
  typedef otb::VectorRescaleIntensityImageFilter<VectorImageType> RescaleImageFilterType;


private:
   Rescale()
  {
    SetName("Rescale");
    SetDescription("Rescale the image between two given values.");
    m_RescaleFilter = RescaleImageFilterType::New();
    m_MinMaxFilter = MinMaxFilterType::New();

    this->AddProgressSource(m_MinMaxFilter);
  }

  virtual ~Rescale()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");

    AddParameter(ParameterType_Float,      "outmin", "Output min value");
    AddParameter(ParameterType_Float,      "outmax", "Output max value");
    SetParameterFloat("outmin", 0);
    SetParameterFloat("outmax", 255);
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {  
    std::cout<<"rescale::DoExecute starts"<<std::endl;
    VectorImageType::Pointer inImage = GetParameterImage("in");
   
    std::cout<<"first step"<<std::endl;
 
    m_MinMaxFilter = MinMaxFilterType::New();
   std::cout<<"minmax instanciated"<<std::endl;
    m_RescaleFilter = RescaleImageFilterType::New();
   
    m_MinMaxFilter->SetInput( inImage );
    m_MinMaxFilter->Update();
    std::cout<<"Input min value: "<<m_MinMaxFilter->GetMinimum()<<std::endl;
    std::cout<<"Input max value: "<<m_MinMaxFilter->GetMaximum()<<std::endl;

    m_RescaleFilter->SetInput( inImage );
    m_RescaleFilter->SetInputMinimum( m_MinMaxFilter->GetMinimum() );
    m_RescaleFilter->SetInputMaximum( m_MinMaxFilter->GetMaximum() );
    std::cout<<"input min/max set"<<std::endl;

    m_RescaleFilter->SetOutputMinimum( GetParameterFloat("outmin") );
    m_RescaleFilter->SetOutputMaximum( GetParameterFloat("outmax") );

    std::cout<<"output min/max set"<<std::endl;
    m_RescaleFilter->UpdateOutputInformation();
    std::cout<<"UpdateOutputInformation done"<<std::endl;
    
    SetParameterOutputImage("out", m_RescaleFilter->GetOutput());
    std::cout<<"output updated"<<std::endl;

    std::cout<<"rescale::DoExecute starts"<<std::endl;
  }
  
  RescaleImageFilterType::Pointer m_RescaleFilter;
  MinMaxFilterType::Pointer m_MinMaxFilter;
};



}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Rescale)

