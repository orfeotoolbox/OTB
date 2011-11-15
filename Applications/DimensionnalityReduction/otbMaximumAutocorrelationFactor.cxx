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

#include "otbMaximumAutocorrelationFactorImageFilter.h"

namespace otb
{
namespace Wrapper
{

class MaximumAutocorrelationFactor: public Application
{
public:
  /** Standard class typedefs. */
  typedef MaximumAutocorrelationFactor          Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MaximumAutocorrelationFactor, otb::Wrapper::Application);

private:
  MaximumAutocorrelationFactor()
  {
    SetName("MaximumAutocorrelationFactor");
    SetDescription("Performs Kernel maximum autocorrelation factor transformationMaximum autocorrelation factor filter.");
    SetDocName("Maximum autocorrelation factor filter");
    SetDocLongDescription("This application calculates the Maximum Autocorrelation Factor based on A. Nielsen works (\"Kernel maximum autocorrelation factor and minimum"
                       " noise fraction transformations,\" IEEE Transactions on Image Processing, vol. 20, no. 3, pp. 612-624, (2011))."
        "Maximum Autocorrelation Factor can be considered as a spatial extension of the PCA.");
    SetDocLimitations("Though the inverse transform can be computed, this filter only provides the forward transform for now.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    SetDocCLExample("otbApplicationLauncherCommandLine MaximumAutocorrelationFactor ${OTB-BIN}/bin  --in ${OTB-Data}/Input/cupriteSubHsi.tif--out MaximumAutocorrelationFactorImageFilterOutput.tif");
    AddDocTag(Tags::DimensionReduction);
    AddDocTag(Tags::Filter);

  }

  virtual ~MaximumAutocorrelationFactor()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in", "Input Image");
    AddParameter(ParameterType_OutputImage, "out", "Change Map");
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    typedef otb::MaximumAutocorrelationFactorImageFilter<
        FloatVectorImageType,
        FloatVectorImageType> MAFFilterType;

    MAFFilterType::Pointer changeFilter = MAFFilterType::New();

    changeFilter->SetInput(GetParameterImage("in"));
    m_Ref = changeFilter;

    SetParameterOutputImage("out", changeFilter->GetOutput());
  }

  itk::LightObject::Pointer m_Ref;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::MaximumAutocorrelationFactor)
