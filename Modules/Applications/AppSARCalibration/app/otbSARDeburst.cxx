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

#include "otbSarDeburstImageFilter.h"

namespace otb
{
namespace Wrapper
{
class SARDeburst : public Application
{
public:
  /** Standard class typedefs. */
  typedef SARDeburst     Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SARDeburst, otb::Application);

  typedef otb::SarDeburstImageFilter<FloatVectorImageType>     DeburstFilterType;

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("SARDeburst");
    SetDescription("TODO \n");

    // Documentation
    SetDocName("SAR Deburst");
    SetDocLongDescription("TODO \n");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Calibration);
    AddDocTag(Tags::SAR);

    AddParameter(ParameterType_ComplexInputImage,  "in", "Input Image");
    SetParameterDescription("in", "Input image");

    AddParameter(ParameterType_OutputImage,  "out", "Output Image");
    SetParameterDescription("out", "Output deburst image");

    AddRAMParameter();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {

  }

  void DoExecute() ITK_OVERRIDE
  {
    // Get the input complex image
    FloatVectorImageType*  in = GetParameterImage("in");

    // Set the filer input
    m_DeburstFilter = DeburstFilterType::New();
    m_DeburstFilter->SetInput(in);

    // Set the output image
    SetParameterOutputImage("out", m_DeburstFilter->GetOutput());
  }

  DeburstFilterType::Pointer   m_DeburstFilter;

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SARDeburst)
