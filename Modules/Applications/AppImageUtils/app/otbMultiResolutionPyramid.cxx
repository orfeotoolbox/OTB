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

#include "otbPerBandVectorImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkShrinkImageFilter.h"


namespace otb
{
namespace Wrapper
{

class MultiResolutionPyramid : public Application
{

public:
  /** Standard class typedefs. */
  typedef MultiResolutionPyramid        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MultiResolutionPyramid, otb::Application);

  /** Image and filters typedef */
  typedef itk::DiscreteGaussianImageFilter<FloatImageType, FloatImageType> SmoothingImageFilterType;

  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, SmoothingImageFilterType> SmoothingVectorImageFilterType;

  typedef itk::ShrinkImageFilter<FloatVectorImageType, FloatVectorImageType> ShrinkFilterType;

private:
  void DoInit() override
  {
    SetName("MultiResolutionPyramid");
    SetDescription("Build a multi-resolution pyramid of the image.");

    // Documentation
    SetDocLongDescription(
        "This application builds a multi-resolution pyramid of the input image. User can specified the number of levels of the pyramid and the subsampling "
        "factor. To speed up the process, you can use the fast scheme option");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Manip);
    AddDocTag("Conversion");
    AddDocTag(Tags::Multi);
    AddDocTag("Util");

    AddParameter(ParameterType_InputImage, "in", "Input Image");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "will be used to get the prefix and the extension of the images to write");

    AddParameter(ParameterType_Int, "level", "Number Of Levels");
    SetDefaultParameterInt("level", 1);
    SetParameterDescription("level", "Number of levels in the pyramid (default is 1).");
    SetMinimumParameterIntValue("level", 1);

    AddParameter(ParameterType_Int, "sfactor", "Subsampling factor");
    SetDefaultParameterInt("sfactor", 2);
    SetParameterDescription("sfactor", "Subsampling factor between each level of the pyramid (default is 2).");

    AddParameter(ParameterType_Float, "vfactor", "Variance factor");
    SetDefaultParameterFloat("vfactor", 0.6);
    SetParameterDescription("vfactor",
                            "Variance factor use in smoothing. It is multiplied by the subsampling factor of each level in the  pyramid (default is 0.6).");

    // Boolean Fast scheme
    AddParameter(ParameterType_Bool, "fast", "Use Fast Scheme");
    std::ostringstream desc;
    desc << "If used, this option allows one to speed-up computation by iteratively"
         << " subsampling previous level of pyramid instead of processing the full input.";
    SetParameterDescription("fast", desc.str());

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "multiResolutionImage.tif");
    SetDocExampleParameterValue("level", "1");
    SetDocExampleParameterValue("sfactor", "2");
    SetDocExampleParameterValue("vfactor", "0.6");
    SetDocExampleParameterValue("fast", "false");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here for the parameters : all are independent

    // Reinitialize the internal process used
  }

  void DoExecute() override
  {
    // Initializing the process
    m_SmoothingFilter = SmoothingVectorImageFilterType::New();
    m_ShrinkFilter    = ShrinkFilterType::New();

    // Extract Parameters
    unsigned int nbLevels       = GetParameterInt("level");
    unsigned int shrinkFactor   = GetParameterInt("sfactor");
    double       varianceFactor = GetParameterFloat("vfactor");

    bool fastScheme = GetParameterInt("fast");

    // Get the input image
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    // Get the Initial Output Image FileName
    std::string path, fname, ext;
    std::string ofname = GetParameterString("out");

    // Get the extension and the prefix of the filename
    path  = itksys::SystemTools::GetFilenamePath(ofname);
    fname = itksys::SystemTools::GetFilenameWithoutExtension(ofname);
    ext   = itksys::SystemTools::GetFilenameExtension(ofname);

    unsigned int currentLevel  = 1;
    unsigned int currentFactor = shrinkFactor;

    while (currentLevel <= nbLevels)
    {
      otbAppLogDEBUG(<< "Processing level " << currentLevel << " with shrink factor " << currentFactor);

      m_SmoothingFilter->SetInput(inImage);

      // According to
      // http://www.ipol.im/pub/algo/gjmr_line_segment_detector/
      // This is a good balance between blur and aliasing
      double variance = varianceFactor * static_cast<double>(currentFactor);
      m_SmoothingFilter->GetFilter()->SetVariance(variance);

      m_ShrinkFilter->SetInput(m_SmoothingFilter->GetOutput());
      m_ShrinkFilter->SetShrinkFactors(currentFactor);

      if (!fastScheme)
      {
        currentFactor *= shrinkFactor;
      }
      else
      {
        otbAppLogWARNING("fast scheme enabled : not implemented for the moment ");
      }

      // Create an output parameter to write the current output image
      OutputImageParameter::Pointer paramOut = OutputImageParameter::New();

      // build the current image filename
      std::ostringstream oss;
      if (!path.empty())
      {
        oss << path << "/";
      }
      oss << fname << "_" << currentLevel << ext;

      // writer label
      std::ostringstream osswriter;
      osswriter << "writer (level " << currentLevel << ")";

      // Set the filename of the current output image
      paramOut->SetFileName(oss.str());
      otbAppLogINFO(<< "File: " << paramOut->GetFileName() << " will be written.");
      paramOut->SetValue(m_ShrinkFilter->GetOutput());
      paramOut->SetPixelType(this->GetParameterOutputImagePixelType("out"));
      // Add the current level to be written
      paramOut->InitializeWriters();
      AddProcess(paramOut->GetWriter(), osswriter.str());
      paramOut->Write();

      ++currentLevel;
    }

    // Disable this parameter since the images have already been produced
    DisableParameter("out");
  }

  SmoothingVectorImageFilterType::Pointer m_SmoothingFilter;
  ShrinkFilterType::Pointer               m_ShrinkFilter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::MultiResolutionPyramid)
