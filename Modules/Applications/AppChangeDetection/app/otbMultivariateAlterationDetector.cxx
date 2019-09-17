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

#include "otbMultivariateAlterationDetectorImageFilter.h"

namespace otb
{
namespace Wrapper
{

class MultivariateAlterationDetector : public Application
{
public:
  /** Standard class typedefs. */
  typedef MultivariateAlterationDetector Self;
  typedef Application                    Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MultivariateAlterationDetector, otb::Wrapper::Application);

private:
  void DoInit() override
  {
    SetName("MultivariateAlterationDetector");
    SetDescription("Change detection by Multivariate Alteration Detector (MAD) algorithm");

    // Documentation
    SetDocLongDescription(
        "This application performs change detection between two multispectral"
        " images using the Multivariate Alteration Detector (MAD) [1]"
        " algorithm.\n\n"
        "The MAD algorithm produces a set of N change maps (where N is the"
        " maximum number of bands in first and second input images), with the"
        " following properties:\n\n"

        "* Change maps are differences of a pair of linear combinations of "
        " bands from image 1 and bands from image 2 chosen to maximize the "
        " correlation, \n"
        "* Each change map is orthogonal to the others.\n\n"

        "This is a statistical method which can handle different modalities"
        " and even different bands and number of bands between images. \n"
        " \n"
        "The application will output all change maps into a single multiband"
        " image. Change maps are sorted by increasing correlation. \n"
        " \n"
        "The application will also print the following information:\n\n"
        "- Mean1 and Mean2 which are the mean values of bands for both input"
        " images,\n"
        "- V1 and V2 which are the two linear transform that are applied to"
        " input image 1 and input image 2 to build the change map,\n"
        "- Rho, the vector of correlation associated to each change map.\n"
        " \n"
        "The OTB filter used in this application has been implemented from the"
        " Matlab code kindly made available by the authors here [2]. Both cases"
        " (same and different number of bands) have been validated"
        " by comparing the output image to the output produced by the Matlab "
        " code, and the reference images for testing have been generated from "
        " the Matlab code using Octave.");


    SetDocLimitations("Input images 1 and 2 should share exactly the same origin, spacing, size, and projection if any.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(
        "[1] Nielsen, A. A., & Conradsen, K. (1997). Multivariate alteration"
        "detection (MAD) in multispectral, bi-temporal image data: A new"
        "approach to change detection studies.\n"
        "[2] http://www2.imm.dtu.dk/~aa/software.html");

    AddDocTag(Tags::ChangeDetection);

    AddParameter(ParameterType_InputImage, "in1", "Input Image 1");
    SetParameterDescription("in1", "Multiband image of the scene before perturbations");
    AddParameter(ParameterType_InputImage, "in2", "Input Image 2");
    SetParameterDescription("in2", "Mutliband image of the scene after perturbations.");
    AddParameter(ParameterType_OutputImage, "out", "Change Map");
    SetParameterDescription("out", "Multiband image containing change maps.");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in1", "Spot5-Gloucester-before.tif");
    SetDocExampleParameterValue("in2", "Spot5-Gloucester-after.tif");
    SetDocExampleParameterValue("out", "detectedChangeImage.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    typedef otb::MultivariateAlterationDetectorImageFilter<FloatVectorImageType, FloatVectorImageType> ChangeFilterType;

    ChangeFilterType::Pointer changeFilter = ChangeFilterType::New();

    changeFilter->SetInput1(GetParameterImage("in1"));
    changeFilter->SetInput2(GetParameterImage("in2"));
    changeFilter->GetOutput()->UpdateOutputInformation();

    otbAppLogINFO("Input 1 mean: " << changeFilter->GetMean1());
    otbAppLogINFO("Input 2 mean: " << changeFilter->GetMean2());
    otbAppLogINFO("Input 1 transform: " << changeFilter->GetV1());
    otbAppLogINFO("Input 2 transform: " << changeFilter->GetV2());
    otbAppLogINFO("Rho: " << changeFilter->GetRho());

    m_Ref = changeFilter;

    SetParameterOutputImage("out", changeFilter->GetOutput());
  }

  itk::LightObject::Pointer m_Ref;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::MultivariateAlterationDetector)
