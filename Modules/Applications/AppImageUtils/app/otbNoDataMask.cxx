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
#include "otbChangeNoDataValueFilter.h"

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
  typedef otb::ChangeNoDataValueFilter<FloatVectorImageType,FloatVectorImageType> ChangeNoDataFilterType;
  
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


    AddParameter(ParameterType_Choice,"mode","No-data handling mode");
    SetParameterDescription("mode","Allows to choose between different no-data handling options");

    AddChoice("mode.buildmask","Build a no-data Mask");
    
    AddParameter(ParameterType_Float,"mode.buildmask.inv","Inside Value");
    SetParameterDescription("mode.buildmask.inv","Value given in the output mask to pixels that are not no data pixels");
    SetDefaultParameterInt("mode.buildmask.inv",1);

    AddParameter(ParameterType_Float,"mode.buildmask.outv","Outside Value");
    SetParameterDescription("mode.buildmask.outv","Value given in the output mask to pixels that are no data pixels");
    SetDefaultParameterInt("mode.buildmask.outv",0);

    AddChoice("mode.changevalue","Change the no-data value");

    AddParameter(ParameterType_Float,"mode.changevalue.newv","The new no-data value");
    SetParameterDescription("mode.changevalue.newv","The new no-data value");
    SetDefaultParameterInt("mode.changevalue.newv",0);

    SetParameterString("mode","buildmask");
    
    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "QB_Toulouse_Ortho_XS_nodatamask.tif uint8");
    SetDocExampleParameterValue("mode.buildmask.inv", "255");
    SetDocExampleParameterValue("mode.buildmask.outv", "0");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here for the parameters : all are independent
  }


 void DoExecute()
  {
    FloatVectorImageType::Pointer inputPtr = this->GetParameterImage("in");
    
    m_Filter = FilterType::New();
    m_Filter->SetInsideValue(this->GetParameterFloat("mode.buildmask.inv"));
    m_Filter->SetOutsideValue(this->GetParameterFloat("mode.buildmask.outv"));
    m_Filter->SetInput(inputPtr);

    m_ChangeNoDataFilter = ChangeNoDataFilterType::New();
    m_ChangeNoDataFilter->SetInput(inputPtr);


    std::vector<double> newNoData(inputPtr->GetNumberOfComponentsPerPixel(),GetParameterFloat("mode.changevalue.newv"));

    m_ChangeNoDataFilter->SetNewNoDataValues(newNoData);
    
    if(GetParameterString("mode") == "buildmask")
      {
      SetParameterOutputImage("out",m_Filter->GetOutput());
      }
    else if(GetParameterString("mode") == "changevalue")
      {
      SetParameterOutputImage("out",m_ChangeNoDataFilter->GetOutput());
      }
  }

  FilterType::Pointer m_Filter;
  ChangeNoDataFilterType::Pointer m_ChangeNoDataFilter;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::NoDataMask)

