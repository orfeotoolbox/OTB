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

#include <numeric>

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbMultiToMonoChannelExtractROI.h"

namespace otb
{
namespace Wrapper
{

class MemoryTest : public Application
{
public:
  /** Standard class typedefs. */
  typedef MemoryTest                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MemoryTest, otb::Application)

  typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,
               FloatVectorImageType::InternalPixelType>  ExtractROIFilterType;



private:

  void DoInit() ITK_OVERRIDE
  {
    SetName("MemoryTest");
    SetDescription("This application is made for test");
    // Documentation
    SetDocName("Memory Test");
    SetDocLongDescription("");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("");

    AddParameter(ParameterType_InputImage, "in", "Input image");
    SetParameterDescription("in", "Input image");
    SetParameterString("in", "/home/antoine/dev/my_data/anaglyphInput2.tif");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image");
    SetDefaultOutputPixelType("out",ImagePixelType_uint8);
    SetParameterString("out", "/home/antoine/dev/my_data/anaglyphtestInput2.tif");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "input.tif");
    SetDocExampleParameterValue("out", "output.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
  }

  void DoExecute() ITK_OVERRIDE
  {
    #ifdef MEM_DEBUG
    std::cout<<"Debug on input "<<std::endl;
    GetParameterImage("in")->DebugOn();
    #endif
    ExtractROIFilterType::Pointer extractor = ExtractROIFilterType::New();
    #ifdef MEM_DEBUG
    extractor->DebugOn();
    #endif
    // m_Filters.push_back(extractor.GetPointer());
    #ifdef MEM_DEBUG
    std::cout<<"Debug on extractor "<<std::endl;
    extractor->DebugOn();
    #endif
    extractor->SetInput(GetParameterImage("in"));
    extractor->SetChannel(1);
    extractor->UpdateOutputInformation();
    #ifdef MEM_DEBUG
    std::cout<<"Debug on extractor's output "<<std::endl;
    extractor->GetOutput()->DebugOn();
    #endif
    SetParameterOutputImage("out" , extractor->GetOutput() );
    std::cout<<"Registering"<<std::endl;
    RegisterPipeline();
    std::cout<<"Registered"<<std::endl;
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::MemoryTest)

