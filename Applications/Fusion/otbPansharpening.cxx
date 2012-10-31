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

#include "itkVectorIndexSelectionCastImageFilter.h"
#include "otbGenericRSResampleImageFilter.h"
#include "otbBCOInterpolateImageFunction.h"
#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"
#include "itkPixelBuilder.h"
#include "itkFixedArray.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{


class Pansharpening : public Application
{
public:
  /** Standard class typedefs. */
  typedef Pansharpening                 Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Pansharpening, otb::Application);

private:

  void DoInit()
  {
    SetName("Pansharpening");
    SetDescription("Perform P+XS pansharpening");

    // Documentation
    SetDocName("Pansharpening");
    SetDocLongDescription("This application performs P+XS pansharpening.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Geometry);
    AddDocTag(Tags::Pansharpening);

    AddParameter(ParameterType_InputImage,   "inp",   "Input PAN Image");
    SetParameterDescription("inp"," Input panchromatic image.");
    AddParameter(ParameterType_InputImage,   "inxs",  "Input XS Image");
    SetParameterDescription("inxs"," Input XS image.");

    AddParameter(ParameterType_OutputImage,  "out",   "Output image");
    SetParameterDescription("out"," Output image.");
    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("inp", "QB_Toulouse_Ortho_PAN.tif");
    SetDocExampleParameterValue("inxs", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "Pansharpening.png uchar");

  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    FloatVectorImageType* panchroV = GetParameterImage("inp");
    if ( panchroV->GetNumberOfComponentsPerPixel() != 1 )
      {
      itkExceptionMacro(<< "The panchromatic image must be a single channel image")
      }

    // Transform the PAN image to otb::Image
    typedef otb::Image<FloatVectorImageType::InternalPixelType> FloatImageType;
    typedef itk::VectorIndexSelectionCastImageFilter<FloatVectorImageType, FloatImageType> VectorIndexSelectionCastImageFilterType;

    VectorIndexSelectionCastImageFilterType::Pointer channelSelect = VectorIndexSelectionCastImageFilterType::New();
    m_Ref.push_back(channelSelect.GetPointer());
    channelSelect->SetIndex(0);
    channelSelect->SetInput(panchroV);
    channelSelect->UpdateOutputInformation();
    FloatImageType::Pointer panchro = channelSelect->GetOutput();


    FloatVectorImageType* xs = GetParameterImage("inxs");

    typedef otb::SimpleRcsPanSharpeningFusionImageFilter<FloatImageType, FloatVectorImageType, FloatVectorImageType> FusionFilterType;

    FusionFilterType::Pointer  fusionFilter = FusionFilterType::New();
    m_Ref.push_back(fusionFilter.GetPointer());

    fusionFilter->SetPanInput(panchro);
    fusionFilter->SetXsInput(xs);

    fusionFilter->UpdateOutputInformation();

    SetParameterOutputImage("out", fusionFilter->GetOutput());
  }

  std::vector<itk::ProcessObject::Pointer> m_Ref;

};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Pansharpening)
