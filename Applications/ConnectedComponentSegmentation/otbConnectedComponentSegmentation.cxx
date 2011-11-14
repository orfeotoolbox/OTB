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

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbVectorData.h"
#include "otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.h"

#include "otbVectorDataProjectionFilter.h"

namespace otb
{
namespace Wrapper
{

class ConnectedComponentSegmentation : public Application
{
public:
  /** Standard class typedefs. */
  typedef ConnectedComponentSegmentation  Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ConnectedComponentSegmentation, otb::Application);

  /** Typedefs */
  typedef float InputPixelType;
  static const unsigned int Dimension = 2;
  typedef otb::VectorImage<InputPixelType, Dimension> InputVectorImageType;
  typedef otb::Image<unsigned int, Dimension>         LabelImageType;
  typedef otb::Image<unsigned int, Dimension>         MaskImageType;
  
  typedef otb::VectorData<double, Dimension>          VectorDataType;
  typedef VectorDataType::Pointer                     VectorDataPointerType;
  
  typedef otb::StreamingConnectedComponentSegmentationOBIAToVectorDataFilter
    < InputVectorImageType,
      LabelImageType,
      MaskImageType,
      VectorDataType >  SegmentationFilterType;
      
  typedef otb::VectorDataProjectionFilter
        <VectorDataType, VectorDataType>                     VectorDataProjectionFilterType;

private:
  ConnectedComponentSegmentation()
  {
    SetName("ConnectedComponentSegmentation");
    SetDescription("Compute a connected component segmentation.");
    SetDocName("Connected Component Segmentation Application");
    SetDocLongDescription("Compute a connected component segmentation, which takes mathematical formula as a neighborhood thresholding criteria.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    SetDocCLExample("otbApplicationLauncherCommandLine ConnectedComponentSegmentation ${OTB-BIN}/bin "
      "--in ${OTB-DATA}/Input/ROI_QB_MUL_4.tif --mask \"((b1>80)*intensity>95)\" --expr \"distance<10\" "
      "--minsize 15 --obia \"SHAPE_Elongation>8\" --out ./result.shp");
    AddDocTag("Image Analysis");
  }

  virtual ~ConnectedComponentSegmentation()
  {
  }
  
  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The image to segment.");
    
    AddParameter(ParameterType_OutputVectorData, "out", "Output Shape");
    SetParameterDescription("out", "The segmentation shape.");
    
    AddParameter(ParameterType_String, "expr", "Connected Component Expression");
    SetParameterDescription("expr", "Formula used for connected component segmentation");
    
    AddParameter(ParameterType_String, "mask", "Mask expression");
    SetParameterDescription("mask", "Mask mathematical expression (only if support image is given)");
    MandatoryOff("mask");
    DisableParameter("mask");
    
    AddParameter(ParameterType_Int, "minsize", "Minimum Object Size");
    SetParameterDescription("minsize", "Min object size (area in pixel)");
    SetDefaultParameterInt("minsize",2);
    SetMinimumParameterIntValue("minsize",1);
    MandatoryOff("minsize");
    
    AddParameter(ParameterType_String, "obia", "OBIA Expression");
    SetParameterDescription("obia", "OBIA mathematical expression");
    MandatoryOff("obia");
    DisableParameter("obia");
    
    AddParameter(ParameterType_Directory, "dem", "DEM Directory");
    SetParameterDescription("dem", "DEM directory (used to reproject output in WGS84 if input image is in sensor model geometry)");
    MandatoryOff("dem");
    DisableParameter("dem");
  }
  
  void DoUpdateParameters()
  {
    // Nothing to do here for the parameters : all are independent
  }
  
  void DoExecute()
  {
    InputVectorImageType::Pointer inputImage = GetParameterImage("in");
    
    m_Connected  = SegmentationFilterType::FilterType::New();
    m_Connected->GetFilter()->SetInput(inputImage);
    
    if (IsParameterEnabled("mask") && HasValue("mask"))
      m_Connected->GetFilter()->SetMaskExpression(GetParameterString("mask"));
    
    m_Connected->GetFilter()->SetConnectedComponentExpression(GetParameterString("expr"));
    
    m_Connected->GetFilter()->SetMinimumObjectSize(GetParameterInt("minsize"));
    
    if (IsParameterEnabled("obia") && HasValue("obia"))
      m_Connected->GetFilter()->SetOBIAExpression(GetParameterString("obia"));
    
    AddProcess(m_Connected,"Computing segmentation");
    m_Connected->Update();
    
    /*
    * Reprojection of the output VectorData
    *
    * The output of the filter is in image physical coordinates,
    * projection WKT applied if the input image has one
    *
    * We need to reproject in WGS84 if the input image is in sensor model geometry
    */
    
    std::string projRef = inputImage->GetProjectionRef();
    ImageKeywordlist kwl = inputImage->GetImageKeywordlist();
  
    VectorDataType::Pointer projectedVD = m_Connected->GetFilter()->GetOutputVectorData();
  
    if ( projRef.empty() && kwl.GetSize() > 0 )
      {
      // image is in sensor model geometry
      
      // Reproject VectorData in image projection
      m_Vproj = VectorDataProjectionFilterType::New();
      m_Vproj->SetInput(m_Connected->GetFilter()->GetOutputVectorData());
      m_Vproj->SetInputKeywordList(inputImage->GetImageKeywordlist());
      m_Vproj->SetInputOrigin(inputImage->GetOrigin());
      m_Vproj->SetInputSpacing(inputImage->GetSpacing());
      
      // Configure DEM directory
      if(IsParameterEnabled("dem") && HasValue("dem"))
        {
        m_Vproj->SetDEMDirectory(GetParameterString("dem"));
        }
      else
        {
        if ( otb::ConfigurationFile::GetInstance()->IsValid() )
          {
          m_Vproj->SetDEMDirectory(otb::ConfigurationFile::GetInstance()->GetDEMDirectory());
          }
        }
      
      m_Vproj->Update();
      
      projectedVD = m_Vproj->GetOutput();
      }
    
    SetParameterOutputVectorData("out",projectedVD); 
  }

  /** Members */
  SegmentationFilterType::FilterType::Pointer m_Connected;
  VectorDataProjectionFilterType::Pointer m_Vproj;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ConnectedComponentSegmentation)
