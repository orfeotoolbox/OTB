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

#include "otbImageToNoDataMaskFilter.h"

namespace otb
{
namespace Wrapper
{


class NoDataMask : public Application
{
public:
  /** Standard class typedefs. */
  typedef NoDataMask                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(NoDataMask, otb::Application);

  /** Filters typedef */
  typedef otb::ImageToNoDataMaskFilter<FloatVectorImageType,UInt8ImageType> FilterType;

private:
  void DoInit()
  {
    SetName("NoDataMask");
    SetDescription("NoDataMask");
    // Documentation
    SetDocName("No Data Mask management");
    SetDocLongDescription("This application builds a nodata mask from the no data flags found in metadata or from NaN pixels.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("BanMath");
    AddDocTag("Conversion");
    AddDocTag("Image Dynamic");
    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImage,  "in",   "Input image");
    SetParameterDescription("in", "Input image");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image");

    AddParameter(ParameterType_Int,"inv","Inside Value");
    SetParameterDescription("inv","Value given in the output mask to pixels that are not no data pixels");
    SetDefaultParameterInt("inv",1);

    AddParameter(ParameterType_Int,"outv","Outside Value");
    SetParameterDescription("outv","Value given in the output mask to pixels that are no data pixels");
    SetDefaultParameterInt("outv",0);


    AddRAMParameter();

  }

  void DoUpdateParameters()
  {
    // Nothing to do here for the parameters : all are independent
  }


 void DoExecute()
  {
    m_Filter = FilterType::New();
    m_Filter->SetInsideValue(this->GetParameterInt("inv"));
    m_Filter->SetOutsideValue(this->GetParameterInt("outv"));
    m_Filter->SetInput(this->GetParameterImage("in"));

    SetParameterOutputImage("out",m_Filter->GetOutput());
  }

  FilterType::Pointer m_Filter;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::NoDataMask)

