/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbSarBurstExtractionImageFilter.h"

namespace otb
{
namespace Wrapper
{
class SARBurstExtraction : public Application
{
public:
  /** Standard class typedefs. */
  typedef SARBurstExtraction     Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SARBurstExtraction, otb::Application);

  typedef otb::SarBurstExtractionImageFilter<FloatVectorImageType>     BurstExtractionFilterType;

private:
  void DoInit() override
  {
    SetName("SARBurstExtraction");
    SetDescription("This application performs a burst extraction by keeping only lines and samples of a required burst.");

    // Documentation
    SetDocLongDescription("Sentinel1 IW SLC products are composed of several burst overlapping in"
                          " azimuth time for each subswath, separated by black lines [1]. The burst"
                          " extraction consist in extracting a single burst, by selecting only lines" 
			  " and samples of the wanted burst.\n\n"
                          
                          "Note that the output sensor model is updated accordingly. This burst"
                          " extraction is the perfect preprocessing step for S1 IW SLC"
                          " product with OTB without suffering from artifacts caused by"
                          " bursts separation\n."
			  
			   "Two modes are available for the output image : with all pixels and"
			  "with only valid pixels ");
    
    SetDocLimitations("Only Sentinel1 IW SLC products are supported for now. Processing of"
                      " other Sentinel1 modes or TerrasarX images will result in no changes in"
                      " the image and metadata. Images from other sensors will lead to an"
                      " error."
		      " Burst extraction needs as inputs a image composed of several Bursts. A burst has to "
		      " contain all its lines in order to have a correct extraction. "
		      " ie : Careful with ROI.");
    
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("[1] Sentinel1 User Handbook, p. 52: https://sentinel.esa.int/documents/247904/685163/Sentinel-1_User_Handbook\n");

    AddDocTag(Tags::SAR);
    AddDocTag(Tags::Calibration);

    AddParameter(ParameterType_InputImage,  "in", "Input Sentinel1 IW SLC Image");
    SetParameterDescription("in", "Raw Sentinel1 IW SLC image, or any extract of such made by OTB (geom file needed)");

    AddParameter(ParameterType_Int,  "burstindex", "Index of Burst");
    SetParameterDescription("burstindex", "Index for the required Burst (By default -1). If this parameter remains at -1, all bursts will be extracted");
    MandatoryOff("burstindex");
    SetDefaultParameterInt("burstindex", -1);

    AddParameter(ParameterType_OutputImage,  "out", "Output Image");
    SetParameterDescription("out", "The output filename will be used to get the prefix "
			    "and the extension of thise output written's image. For example with outimage.tif as"
			    "output filename, the generated images will had an indice (corresponding at each "
			    "burst) between the prefix and the extension, such as: outimage_Burst0.tif and "
			    "outimage_Burst1.tif (if 2 bursts).");

    AddParameter(ParameterType_Bool, "allpixels", "Select the modes for output image");
    SetParameterDescription("allpixels", "If true, all pixels of the current burst are selected.");

    AddRAMParameter();

    SetDocExampleParameterValue("in","s1_iw_slc.tif");
    SetDocExampleParameterValue("out","s1_iw_slc_burst0.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {}

  void DoExecute() override
  {
    // Get the input complex image
    FloatVectorImageType*  in = GetParameterImage("in");

    // Get the Burst index 
    int burst_index = GetParameterInt("burstindex"); // If -1 or > nb_Bursts => all bursts will be extracted
    

    // Get the path/fileWithoutextension/extension of the output images filename
    std::string ofname = GetParameterString("out");

    // Get the extension and the prefix of the filename
    const std::string path  = itksys::SystemTools::GetFilenamePath(ofname);
    const std::string fname = itksys::SystemTools::GetFilenameWithoutExtension(ofname);
    const std::string ext   = itksys::SystemTools::GetFilenameExtension(ofname);

    // Set the filer input
    m_BurstExtractionFilter = BurstExtractionFilterType::New();
    m_BurstExtractionFilter->SetInput(in);

    if (IsParameterEnabled("allpixels"))
      {
	m_BurstExtractionFilter->SetAllPixels(true);
      }

    // Get the number of Bursts
    unsigned int nbBursts = 1;
    try
      {
	nbBursts = std::stoi(in->GetImageKeywordlist().GetMetadataByKey("support_data.geom.bursts.number"));
      }
    catch( ... )
      {
	// Throw an execption
	otbAppLogFATAL(<<"Failed to retrieve bursts.number value from .geom file.");
      }
      
    const bool extractAll = burst_index <= -1 || burst_index >= static_cast<int>(nbBursts);
    
    // Extract burst index only
    if (! extractAll)
      {
	// Set the burst index
	m_BurstExtractionFilter->SetBurstIndex(burst_index);
	
	SetParameterOutputImage("out", m_BurstExtractionFilter->GetOutput());
	
      }
    // Extract all bursts
    else
      {	
	for (unsigned int i = 0; i < nbBursts; ++i)
	  {
	    // Set the burst index
	    m_BurstExtractionFilter->SetBurstIndex(i);

	    // Build the current output filename
	    std::ostringstream oss;
	    if (!path.empty())
	      {
		oss <<path<<"/";
	      }
	    oss <<fname<<"_burst"<<i<<ext;

	    // Create an output parameter to write the current output image
	    OutputImageParameter::Pointer paramOut = OutputImageParameter::New();

	    // Writer label
	    std::ostringstream osswriter;
	    osswriter<< "writer (Burst : "<< i<<")";

	    // Set the filename of the current output image
	    paramOut->SetFileName(oss.str());
	    otbAppLogINFO(<< "File: "<<paramOut->GetFileName() << " will be written.");
	    paramOut->SetValue(m_BurstExtractionFilter->GetOutput());
	    paramOut->SetPixelType(this->GetParameterOutputImagePixelType("out"));
	    // Add the current level to be written
	    paramOut->InitializeWriters();
	    AddProcess(paramOut->GetWriter(), osswriter.str());
	    paramOut->Write();
	  }

	// Disable the output Image parameter to avoid writing
	// the last image (Application::ExecuteAndWriteOutput method)
	DisableParameter("out");
      }
    RegisterPipeline();
  }

  BurstExtractionFilterType::Pointer   m_BurstExtractionFilter;

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SARBurstExtraction)
