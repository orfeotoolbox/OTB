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
  void DoInit()
  {
    SetName("MaximumAutocorrelationFactor");
    SetDescription("Perform a Maximum Auto-correlation Factor (MAF) decomposition of the input image.");
    SetDocName("Maximum Auto-correlation Factor Decomposition");
    SetDocLongDescription("Maximum Autocorrelation Factor can be considered as a spatial extension of the PCA, in which new variates try to maximize auto-correlation between neighboring pixels instead of variance.");
    SetDocLimitations("Though the inverse transform can be computed, this application only provides the forward transform for now.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("\"Kernel maximum autocorrelation factor and minimum noise fraction transformations,\" IEEE Transactions on Image Processing, vol. 20, no. 3, pp. 612-624, (2011)");
   
    AddDocTag(Tags::DimensionReduction);
    AddDocTag(Tags::Filter);

    AddParameter(ParameterType_InputImage,  "in", "Input Image");
    SetParameterDescription("in","The input image to apply MAF decomposition on.");
    AddParameter(ParameterType_OutputImage, "out", "MAF output");
    SetParameterDescription("out","MAF decomposition of the input image. Components are ordered by decreasing eigenvalues.");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("out", "MaximumAutocorrelationFactorImageFilterOutput.tif");
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
