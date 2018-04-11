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

#include "otbStopwatch.h"

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
    AddApplication("LSMSSmallRegionsMerging", "merging", "Small region merging step");
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
    
    AddParameter(ParameterType_Int, "minsize", "Minimum Segment Size");
    SetParameterDescription("minsize", "Minimum Segment Size. If, after the segmentation, "
      "a segment is of size lower than this criterion, the segment is merged with the segment "
      "that has the closest sepctral signature.");
    SetMinimumParameterIntValue("minsize", 0);
    MandatoryOff("minsize");
    
    ShareParameter("filter","filtering.expr");
    MandatoryOff("filter");
    
    AddParameter( ParameterType_Int, "cleanup", "Temporary files cleaning" );
    EnableParameter( "cleanup" );
    SetParameterDescription( "cleanup",
      "If activated, the application will try to clean all temporary files it created" );
    SetMinimumParameterIntValue("cleanup", 0);
    SetMaximumParameterIntValue("cleanup", 1);
    SetDefaultParameterInt("cleanup", 1);
    MandatoryOff( "cleanup" );

    Connect("spectral.tile","vectorization.tile");
    Connect("geometric.in","vectorization.out");
    Connect("spectral.in","segmentation.in");
    Connect("merging.in","segmentation.in");
    Connect("spectral.vec","vectorization.out");
    Connect("filtering.in","vectorization.out");

    // Setup RAM
    ShareParameter("ram","segmentation.ram");
    Connect("vectorization.ram","segmentation.ram");
    Connect("merging.ram","segmentation.ram");
    Connect("geometric.ram","segmentation.ram");
    Connect("spectral.ram","segmentation.ram");
    Connect("filtering.ram","segmentation.ram");
    
    SetOfficialDocLink();
    }

  void DoUpdateParameters() ITK_OVERRIDE
  {}

  void DoExecute() ITK_OVERRIDE
  {
    // Start Timer for the application
    auto Timer = Stopwatch::StartNew();
    
    // Temporary files (deleted depending on "cleanup" parameter)
    std::string outPath(this->GetParameterString("out"));
    std::vector<std::string> tmpFilenames;
    tmpFilenames.push_back(outPath+std::string("_labelmap.tif"));
    tmpFilenames.push_back(outPath+std::string("_labelmap_merged.tif"));
      
    // If the LSMSSmallRegionsMerging application is used, we can't use in-memory connection, so we have to write all outputs image files.
    if (IsParameterEnabled("minsize") && HasValue("minsize"))
    {
      // Image to image connected components segmentation step
      GetInternalApplication("segmentation")->SetParameterString("out",
        tmpFilenames[0]);
      //ExecuteInternal("segmentation");
      GetInternalApplication("segmentation")->ExecuteAndWriteOutput();

      // Optional small segments merging step
      GetInternalApplication("merging")->SetParameterInt("minsize",this->GetParameterInt("minsize"));
      GetInternalApplication("merging")->SetParameterString("inseg",tmpFilenames[0] );
      GetInternalApplication("merging")->SetParameterString("out",tmpFilenames[1] );
      GetInternalApplication("merging")->ExecuteAndWriteOutput();
      //ExecuteInternal("merging");
      GetInternalApplication("vectorization")->SetParameterString("in", tmpFilenames[1]);
    }
    // In this case in-memory connection is used
    else
    {
      ExecuteInternal("segmentation");
      GetInternalApplication("vectorization")->SetParameterInputImage("in", GetInternalApplication("segmentation")->GetParameterOutputImage("out"));
    }
    
    // Label Image vectorization step
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
    
    // Delete temporary files
    if(this->GetParameterInt("cleanup") == 1)
    {
      otbAppLogINFO( <<"Final clean-up ..." );
      for (unsigned int i=0 ; i<tmpFilenames.size() ; ++i)
      {
        if(itksys::SystemTools::FileExists(tmpFilenames[i].c_str()))
        {
          itksys::SystemTools::RemoveFile(tmpFilenames[i].c_str());
        }
      }
    }
    
    Timer.Stop();
    otbAppLogINFO( "Total elapsed time: "<< float(Timer.GetElapsedMilliseconds())/1000 <<" seconds.");
  }

};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::LargeScaleConnectedComponent)
