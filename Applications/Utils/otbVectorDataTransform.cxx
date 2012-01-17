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

#include "otbVectorDataTransformFilter.h"
#include "otbVectorDataProjectionFilter.h"
#include "itkCenteredSimilarity2DTransform.h"
#include "otbMath.h"

namespace otb
{

namespace Wrapper
{

class VectorDataTransform : public Application
{
public:
/** Standard class typedefs. */
  typedef VectorDataTransform           Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(VectorDataTransform, otb::Application);

  /** Convenient typedefs */
  typedef otb::VectorDataTransformFilter<VectorDataType,
                                         VectorDataType>   VectorDataTransformFilterType;
  typedef otb::VectorDataProjectionFilter<VectorDataType,
                                          VectorDataType>  VectorDataProjectionFilterType;
  typedef itk::CenteredSimilarity2DTransform<double>       TransformType;

private:
  void DoInit()
  {
    SetName("VectorDataTransform");
    SetDescription("Apply a transform to each vertex of the input VectorData");
    
    SetDocName("Vector Data Transformation");
    SetDocLongDescription("This application performs a transformation of an input vector data transforming each vertex that composed the vector data. The applied transformation manages translation, rotation and scale, and can be centered or not.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
  
    AddDocTag(Tags::Vector);

    AddParameter(ParameterType_InputVectorData, "vd", "Input Vector data");
    AddParameter(ParameterType_OutputVectorData,"out","Output Vector data");

    AddParameter(ParameterType_InputImage, "in", "Support image");
    SetParameterDescription("in","Image needed as a support of the VectorData");
  
    // Transform Group
    AddParameter(ParameterType_Group, "transform", "Transform parameters");

    AddParameter(ParameterType_Float, "transform.tx", "Translation X");
    SetParameterDescription("transform.tx","Translation in the X direction");
    AddParameter(ParameterType_Float, "transform.ty", "Translation Y");
    SetParameterDescription("transform.ty","Translation in the Y direction");
    SetDefaultParameterFloat("transform.tx", 0.);
    SetDefaultParameterFloat("transform.ty", 0.);
      
    AddParameter(ParameterType_Float, "transform.ro", "Rotation Angle");
    SetParameterDescription("transform.ro","Angle of the rotation to apply in degrees");
    SetDefaultParameterFloat("transform.ro", 0.);

    AddParameter(ParameterType_Float, "transform.centerx", "Center X");
    SetParameterDescription("transform.centerx","The first coordinate of the rotation center");
    AddParameter(ParameterType_Float, "transform.centery", "Center Y");
    SetParameterDescription("transform.centery","The second coordinate of the rotation center");
    SetDefaultParameterFloat("transform.centerx", 0.);
    SetDefaultParameterFloat("transform.centery", 0.);

    AddParameter(ParameterType_Float, "transform.scale", "Scale");
    SetParameterDescription("transform.scale","The scale to apply");
    SetDefaultParameterFloat("transform.scale", 1.);

    // Doc example parameter settings
    SetDocExampleParameterValue("vd", "qb_RoadExtract_easyClassification.shp");
    SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("out", "VectorDataTransform.shp");
    SetDocExampleParameterValue("transform.ro", "5");
  }

  void DoUpdateParameters()
  {
    // nothing to update
  }

  void DoExecute()
  {
    // Get the support image
    FloatVectorImageType*  inImage = GetParameterImage("in");
    
    // Get the VectorData to apply the transform on
    VectorDataType*        vd      = GetParameterVectorData("vd");
      
    // Reproject the VectorData in the image coordinate system
    m_VectorDataProj = VectorDataProjectionFilterType::New();
    m_VectorDataProj->SetInput(vd);
    m_VectorDataProj->SetInputProjectionRef(vd->GetProjectionRef());
    m_VectorDataProj->SetOutputKeywordList(inImage->GetImageKeywordlist());
    m_VectorDataProj->SetOutputProjectionRef(inImage->GetProjectionRef());
    
    // Set up the transform
    m_Transform = TransformType::New();
    TransformType::ParametersType parameters(6);

    // Get parameters if any
    parameters[0] = GetParameterFloat("transform.scale");
    parameters[1] = CONST_PI * GetParameterFloat("transform.ro")/180.;
    parameters[2] = GetParameterFloat("transform.centerx");
    parameters[3] = GetParameterFloat("transform.centery");
    parameters[4] = inImage->GetSpacing()[0] * GetParameterFloat("transform.tx");
    parameters[5] = vcl_abs(inImage->GetSpacing()[1]) * GetParameterFloat("transform.ty");
    
    // Set the parameters to the transform
    m_Transform->SetParameters(parameters);
    
    m_TransformFilter = VectorDataTransformFilterType::New();
    m_TransformFilter->SetInput(m_VectorDataProj->GetOutput());
    m_TransformFilter->SetTransform(m_Transform);
  
    // retransform int the input vector projection
    m_ReverseVectorDataProj = VectorDataProjectionFilterType::New();
    m_ReverseVectorDataProj->SetInput(m_TransformFilter->GetOutput());
    m_ReverseVectorDataProj->SetOutputProjectionRef(vd->GetProjectionRef());
    m_ReverseVectorDataProj->SetInputKeywordList(inImage->GetImageKeywordlist());
    m_ReverseVectorDataProj->SetInputProjectionRef(inImage->GetProjectionRef());

    // Set the output image
    SetParameterOutputVectorData("out", m_ReverseVectorDataProj->GetOutput());
  }

  VectorDataTransformFilterType::Pointer   m_TransformFilter;
  VectorDataProjectionFilterType::Pointer  m_VectorDataProj;
  VectorDataProjectionFilterType::Pointer  m_ReverseVectorDataProj;
  TransformType::Pointer                   m_Transform;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorDataTransform)
