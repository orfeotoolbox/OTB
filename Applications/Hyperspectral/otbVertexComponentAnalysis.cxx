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

#include "otbVcaImageFilter.h"
#include "otbVectorImageToMatrixImageFilter.h"


namespace otb
{
namespace Wrapper
{

const unsigned int Dimension = 2;

typedef otb::VCAImageFilter<DoubleVectorImageType>                 VCAFilterType;
typedef otb::VectorImageToMatrixImageFilter<DoubleVectorImageType> VectorImageToMatrixImageFilterType;

class VertexComponentAnalysis : public Application
{
public:
  /** Standard class typedefs. */
  typedef VertexComponentAnalysis         Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(VertexComponentAnalysis, otb::Application);

private:
  void DoInit()
  {
    SetName("VertexComponentAnalysis");
    SetDescription("Find endmembers in hyperspectral images with Vertex Component Analysis");

    // Documentation
    SetDocName("Vertex Component Analysis");
    SetDocLongDescription("Applies the Vertex Component Analysis to an hyperspectral image to extract endmembers");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Hyperspectral);
    AddDocTag(Tags::DimensionReduction);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in","Input hyperspectral data cube");

    AddParameter(ParameterType_Int, "ne", "Number of endmembers");
    SetParameterDescription("ne","The number of endmembers to extract from the data cube");
    SetParameterInt("ne", 1);
    MandatoryOn("ne");

    AddParameter(ParameterType_OutputImage, "outendm", "Output Endmembers");
    SetParameterDescription("outendm","The endmebers, stored in a one-line multi-spectral image, each pixel representing an endmember");
    MandatoryOn("outendm");

    AddRANDParameter();
    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("ne", "5");
    SetDocExampleParameterValue("outendm", "VertexComponentAnalysis.tif double");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    DoubleVectorImageType::Pointer inputImage = GetParameterDoubleVectorImage("in");
    DoubleVectorImageType::Pointer endmembersImage;

    const unsigned int nbEndmembers = GetParameterInt("ne");
    VCAFilterType::Pointer vca = VCAFilterType::New();
    vca->SetNumberOfEndmembers(nbEndmembers);
    vca->SetInput(inputImage);

    endmembersImage = vca->GetOutput();
    m_Ref = vca.GetPointer();

    SetParameterOutputImage<DoubleVectorImageType>("outendm", endmembersImage);
  }

  itk::LightObject::Pointer m_Ref;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::VertexComponentAnalysis)
