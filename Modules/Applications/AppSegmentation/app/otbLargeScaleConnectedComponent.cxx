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
 * \brief All-in-one application for the Large Scale connected component
 * framework
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
  void DoInit() override
    {
    SetName("LargeScaleConnectedComponent");
    SetDescription("Large-scale segmentation using MeanShift");

    // Documentation
    SetDocName("Large-Scale MeanShift");
    SetDocLongDescription("This application chains together the 5 steps of the"
      " Connected components framework, that is the"
      " ImageConnectedComponentSegmentation step [1], the" 
      "label image vectorization  step [2], the small regions merging step [3],"
      " the computation of geometric features [4] and the object based"
      " filtering step [5].\n"
      "It generates a vector data file containing the regions extracted with"
      " the Connected components algorithm using a user defined criterion, then"
      " there is an optional step to remove small regions whose size (in"
      " pixels) is less than the given 'minsize' parameter : these regions"
      " will be merged with the adjacent region with the closest radiometry."
      " Features are then extracted for each segment. The computed features"
      " are: "
      "- size (field name 'size')"
      "- perimeter (field name 'perimeter')"
      " Optionally, the segment are filtered using a criterion on the computed"
      " features."
      );
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("[1] ImageConnectedComponentSegmentation\n"
      "[2] LabelImageVectorization\n"
      "[3] SmallRegionsMerging\n"
      "[4] ComputePolygonsGeometricFeatures\n"
      "[5] ObjectBasedFiltering");

    AddDocTag(Tags::Segmentation);
    AddDocTag("LSCC");

    ClearApplications();
    AddApplication("ImageConnectedComponentSegmentation", "segmentation", 
      "connected component segmentation step");
    AddApplication("LabelImageVectorization", "vectorization",
      "Vectorization step");
    AddApplication("SmallRegionsMerging", "merging",
      "Small region merging step");
    AddApplication("ComputePolygonsGeometricFeatures", "geometric",
      "Geometric features computation step");
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
    SetParameterDescription("minsize", "Minimum Segment Size. If, after the"
    " segmentation, a segment is of size lower than this criterion, the segment"
    " is merged with the segment that has the closest sepctral signature. If"
    " this option is used, the application will create temporary files, and"
    " will remove them (if the parameter cleanup is set to true) at the end of"
    " the app");
    SetMinimumParameterIntValue("minsize", 0);
    MandatoryOff("minsize");
    
    ShareParameter("filter","filtering.expr");
    MandatoryOff("filter");
    
    AddParameter( ParameterType_Bool, "cleanup", "Temporary files cleaning" );
    EnableParameter( "cleanup" );
    SetParameterDescription( "cleanup",
      "If activated, the application will try to clean all temporary files it"
      " created. Note that temporary files are only created if the minsize"
      " option is activated");
    SetParameterInt("cleanup", 1);
    MandatoryOff( "cleanup" );

    AddParameter(ParameterType_Directory,"tmpdir","Directory where to "
      "write temporary files");
    SetParameterDescription("tmpdir","This applications need to write temporary"
    " files for each tile (only if the minsize option is activated). This"
    " parameter allows choosing the path where to write those files. If"
    " disabled, the directory of the out parameter will be used.");
    MandatoryOff("tmpdir");
    DisableParameter("tmpdir");

    Connect("geometric.in","vectorization.out");
    Connect("merging.in","segmentation.in");
    Connect("filtering.in","vectorization.out");

    // Setup RAM
    ShareParameter("ram","segmentation.ram");
    Connect("vectorization.ram","segmentation.ram");
    Connect("merging.ram","segmentation.ram");
    Connect("geometric.ram","segmentation.ram");
    Connect("filtering.ram","segmentation.ram");
    
    SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("out","segmentation.sqlite");
    SetDocExampleParameterValue("mask","(b4-b1)/(b4+b1)>0.65");
    SetDocExampleParameterValue("expr","distance<50");
    SetDocExampleParameterValue("filter","meanb1<150");
    
    SetOfficialDocLink();
    }

  void DoUpdateParameters() override
  {}

  void DoExecute() override
  {
    // Start Timer for the application
    auto Timer = Stopwatch::StartNew();
    
    // Temporary files (deleted depending on "cleanup" parameter)
    std::string outPath(this->GetParameterString("out"));
    std::vector<std::string> tmpFilenames;
    
    // The temporay folder will be removed if this flag is set to true
    bool TmpDirCleanup = false;
    
    if (IsParameterEnabled("tmpdir") && HasValue("tmpdir"))
      {
      // Test if the temporary directory already exists
      if(!itksys::SystemTools::FileExists(GetParameterString("tmpdir").c_str()))
        {
        TmpDirCleanup = true;
        itksys::SystemTools::MakeDirectory(GetParameterString("tmpdir").c_str());
        }
      tmpFilenames.push_back
        (GetParameterString("tmpdir") + std::string("/labelmap.tif"));
      tmpFilenames.push_back
        (GetParameterString("tmpdir") + std::string("/labelmap_merged.tif"));
      }
    else
      {
      tmpFilenames.push_back(outPath+std::string("_labelmap.tif"));
      tmpFilenames.push_back(outPath+std::string("_labelmap_merged.tif"));
      }    
      
    /* If the LSMSSmallRegionsMerging application is used, we can't use 
     * in-memory connection, so we have to write all outputs image files.*/
    if (IsParameterEnabled("minsize") && HasValue("minsize"))
      {
      // Image to image connected components segmentation step
      GetInternalApplication("segmentation")->SetParameterString("out",
        tmpFilenames[0]);
      //ExecuteInternal("segmentation");
      GetInternalApplication("segmentation")->ExecuteAndWriteOutput();

      // Optional small segments merging step
      GetInternalApplication("merging")
        ->SetParameterInt("minsize",this->GetParameterInt("minsize"));
      GetInternalApplication("merging")
        ->SetParameterString("inseg",tmpFilenames[0] );
      GetInternalApplication("merging")
        ->SetParameterString("out",tmpFilenames[1] );
      GetInternalApplication("merging")->ExecuteAndWriteOutput();
      //ExecuteInternal("merging");
      GetInternalApplication("vectorization")
        ->SetParameterString("in", tmpFilenames[1]);
      }
    // In this case in-memory connection is used
    else
      {
      ExecuteInternal("segmentation");
      GetInternalApplication("vectorization")
        ->SetParameterInputImage("in", GetInternalApplication("segmentation")
                                            ->GetParameterOutputImage("out"));
      }
    
    // Label Image vectorization step
    ExecuteInternal("vectorization");
    
    // Geometric features computation step
    GetInternalApplication("geometric")
      ->SetParameterString("sizefield", "size");
    GetInternalApplication("geometric")
      ->SetParameterString("perimeterfield", "perimeter");
    ExecuteInternal("geometric");
    
    // Object based filtering step
    if (IsParameterEnabled("filter") && HasValue("filter"))
      {
      ExecuteInternal("filtering");
      }

    // Clean temporary files
    if(GetParameterInt("cleanup"))
      {
      otbAppLogINFO(<<"Final clean-up ...");
      for (unsigned int i=0 ; i<tmpFilenames.size() ; ++i)
        {
        itksys::SystemTools::RemoveFile(tmpFilenames[i].c_str());
        }

      if(IsParameterEnabled("tmpdir") && TmpDirCleanup)
        {
        otbAppLogINFO( "Removing tmp directory "
          << GetParameterString("tmpdir")
          << ", since it has been created by the application");
        itksys::SystemTools::RemoveADirectory
          (GetParameterString("tmpdir").c_str());
        }
      }

    Timer.Stop();
    otbAppLogINFO( "Total elapsed time: "
      << float(Timer.GetElapsedMilliseconds())/1000 <<" seconds.");
  }

};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::LargeScaleConnectedComponent)
