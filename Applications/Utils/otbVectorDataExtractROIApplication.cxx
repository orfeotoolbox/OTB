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

#include "otbVectorDataExtractROI.h"
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataProperties.h"

//Misc
#include "otbRemoteSensingRegion.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

#include <iostream>

namespace otb
{
namespace Wrapper
{

class VectorDataExtractROIApplication : public Application
{
public:
  /** Standard class typedefs. */
  typedef VectorDataExtractROIApplication  Self;
  typedef Application                      Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(VectorDataExtractROIApplication, otb::Application);

  /** Filters typedef */
  // Images
  
  typedef FloatVectorImageType::PointType                 PointType;
  typedef FloatVectorImageType::IndexType                 IndexType;
  typedef FloatVectorImageType::SizeType                  SizeType;
  typedef FloatVectorImageType::SpacingType               SpacingType;
  
  typedef VectorDataProjectionFilter<
    VectorDataType, VectorDataType>                       VectorDataProjectionFilterType;
  typedef VectorDataExtractROI<VectorDataType>            VectorDataExtractROIType;

  // Misc
  typedef otb::RemoteSensingRegion<double>                RemoteSensingRegionType;

private:
  void DoInit()
  {
    SetName("VectorDataExtractROIApplication");
    SetDescription("Perform an extract ROI on the input vector data according to the input image extent");

    SetDocName("VectorData Extract ROI");
    SetDocLongDescription("This application extracts the vector data features belonging to a region specified by the support image envelope");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Vector);

    AddParameter(ParameterType_Group,"io","Input and output data");
    SetParameterDescription("io", "Group containing input and output parameters");
    
    AddParameter(ParameterType_InputVectorData, "io.vd", "Input Vector data");
    SetParameterDescription("io.vd", "Input vector data");
    
    AddParameter(ParameterType_InputImage,      "io.in", "Support image");
    SetParameterDescription("io.in", "Support image that specifies the extracted region");
    
    AddParameter(ParameterType_OutputVectorData,"io.out","Output Vector data");
    SetParameterDescription("io.out", "Output extracted vector data");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    // Doc example parameter settings
    SetDocExampleParameterValue("io.in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("io.vd", "qb_RoadExtract_classification.shp");
    SetDocExampleParameterValue("io.out", "apTvUtVectorDataExtractROIApplicationTest.shp");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here for the parameters : all are independent
  }

  void DoExecute()
  {
    // Get the inputs
    VectorDataType*        vd      = GetParameterVectorData("io.vd");
    FloatVectorImageType*  inImage = GetParameterImage("io.in");
  
    // Extracting the VectorData
    m_VdExtract = VectorDataExtractROIType::New();
    m_VdExtract->SetInput(vd);
    
    // Find the geographic region of interest
    // Get the index of the corner of the image
    IndexType ul, ur, ll, lr;
    PointType pul, pur, pll, plr;
    ul = inImage->GetLargestPossibleRegion().GetIndex();
    ur = ul;
    ll = ul;
    lr = ul;
    ur[0] += inImage->GetLargestPossibleRegion().GetSize()[0];
    lr[0] += inImage->GetLargestPossibleRegion().GetSize()[0];
    lr[1] += inImage->GetLargestPossibleRegion().GetSize()[1];
    ll[1] += inImage->GetLargestPossibleRegion().GetSize()[1];

    // Transform to physical point
    inImage->TransformIndexToPhysicalPoint(ul, pul);
    inImage->TransformIndexToPhysicalPoint(ur, pur);
    inImage->TransformIndexToPhysicalPoint(ll, pll);
    inImage->TransformIndexToPhysicalPoint(lr, plr);

    // Build the cartographic region
    RemoteSensingRegionType            rsRegion;
    RemoteSensingRegionType::IndexType rsOrigin;
    RemoteSensingRegionType::SizeType  rsSize;
    rsOrigin[0] = std::min(pul[0], plr[0]);
    rsOrigin[1] = std::min(pul[1], plr[1]);
    rsSize[0] = vcl_abs(pul[0] - plr[0]);
    rsSize[1] = vcl_abs(pul[1] - plr[1]);
  
    rsRegion.SetOrigin(rsOrigin);
    rsRegion.SetSize(rsSize);
    rsRegion.SetRegionProjection(inImage->GetProjectionRef());
    rsRegion.SetKeywordList(inImage->GetImageKeywordlist());

    // Set the cartographic region to the extract roi filter
    m_VdExtract->SetRegion(rsRegion);

    // Reprojecting the VectorData
    m_VdProj = VectorDataProjectionFilterType::New();
    m_VdProj->SetInput(m_VdExtract->GetOutput());
    m_VdProj->SetInputProjectionRef(inImage->GetProjectionRef());
    m_VdProj->SetOutputKeywordList(inImage->GetImageKeywordlist());
    m_VdProj->SetOutputProjectionRef(inImage->GetProjectionRef());
    m_VdProj->SetOutputOrigin(inImage->GetOrigin());
    m_VdProj->SetOutputSpacing(inImage->GetSpacing());

    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

    // Set the output vectorData
    SetParameterOutputVectorData("io.out", m_VdProj->GetOutput());
  }
  
  VectorDataExtractROIType::Pointer       m_VdExtract;
  VectorDataProjectionFilterType::Pointer m_VdProj;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorDataExtractROIApplication)
