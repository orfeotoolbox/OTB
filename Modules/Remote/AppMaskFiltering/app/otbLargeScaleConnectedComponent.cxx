/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperCompositeApplication.h"
#include "otbWrapperApplicationFactory.h"

namespace otb
{
namespace Wrapper
{

/**
 * \class LargeScaleConnectedComponent
 *
 * \brief All-in-one application for the Large Scale connected component framework
 *
 * 
 */
class LargeScaleConnectedComponent : public CompositeApplication
{
public:
  /** Standard class typedefs. */
  typedef LargeScaleConnectedComponent            Self;
  typedef CompositeApplication                    Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(LargeScaleConnectedComponent, otb::CompositeApplication);

private:
  void DoInit() ITK_OVERRIDE
    {
    SetName("LargeScaleConnectedComponent");
    SetDescription("Large-scale segmentation using MeanShift");

    // Documentation
    SetDocName("Large-Scale MeanShift");
    SetDocLongDescription("This application chains together the 5 steps of the "
      "Connected components framework, that is the ImageConnectedComponentSegmentation [1], the "
      "LabelImageVectorization [2], the ComputePolygonsGeometricFeatures [3], the ComputePolygonsSpectralFeatures [4] and the "
      "ObjectBasedFiltering [5].\n\n"
      "It generates a vector data file containing the regions extracted with "
      "the Connected components algorithm using a user defined criterion, features (size, perimeter, min, max and mean for each band, and the covariance matrix) "
      " are then extracted for each segment. Optionally, the segment are filtered using a criterion on the computed features."
      );
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("[1] ImageConnectedComponentSegmentation\n"
      "[2] ComputePolygonsGeometricFeatures\n"
      "[3] ComputePolygonsSpectralFeatures\n"
      "[4] ObjectBasedFiltering");

    AddDocTag(Tags::Segmentation);
    AddDocTag("LSCC");

    ClearApplications();
    AddApplication("ImageConnectedComponentSegmentation", "segmentation", "connected component segmentation step");
    AddApplication("LabelImageVectorization", "vectorization", "Vectorization step");
    AddApplication("ComputePolygonsGeometricFeatures", "geometric", "Geometric features computation step");
    AddApplication("ComputePolygonsSpectralFeatures", "spectral", "Spectral features computation step");
    AddApplication("ObjectBasedFiltering", "filtering", "Filtering step");

    ShareParameter("in","segmentation.in");
    ShareParameter("expr","segmentation.expr");
    ShareParameter("mask","segmentation.mask");
    MandatoryOff("mask");
    ShareParameter("out","vectorization.out");
    ShareParameter("tile","vectorization.tile");
    MandatoryOff("tile");
    ShareParameter("fusion","vectorization.fusion");
    MandatoryOff("fusion");
    ShareParameter("filter","filtering.expr");
    MandatoryOff("filter");
    
    Connect("spectral.tile","vectorization.tile");
    Connect("geometric.in","vectorization.out");
    Connect("spectral.in","segmentation.in");
    Connect("spectral.vec","vectorization.out");
    Connect("filtering.in","vectorization.out");

    // Setup RAM
    ShareParameter("ram","segmentation.ram");
    Connect("vectorization.ram","segmentation.ram");
    Connect("geometric.ram","segmentation.ram");
    Connect("spectral.ram","segmentation.ram");
    Connect("filtering.ram","segmentation.ram");
    

    SetOfficialDocLink();
    }

  void DoUpdateParameters() ITK_OVERRIDE
  {}

  void DoExecute() ITK_OVERRIDE
  {
    // Image to image connected components segmentation step
    ExecuteInternal("segmentation");
    
    // Label Image vectorization step
    GetInternalApplication("vectorization")->SetParameterInputImage("in", GetInternalApplication("segmentation")->GetParameterOutputImage("out"));
    ExecuteInternal("vectorization");
    
    // Geometric features computation step
    GetInternalApplication("geometric")->SetParameterString("sizefield", "size");
    GetInternalApplication("geometric")->SetParameterString("perimeterfield", "perimeter");
    ExecuteInternal("geometric");
    
    // Spectral features computation step
    ExecuteInternal("spectral");
    
    // Object based filtering step
    if (IsParameterEnabled("filter") && HasValue("filter"))
    {  
      ExecuteInternal("filtering");
    }
      
    
  }
};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::LargeScaleConnectedComponent)
