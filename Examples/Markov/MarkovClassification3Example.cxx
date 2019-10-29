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


/* Example usage:
./MarkovClassification3Example Input/QB_Suburb.png Output/MarkovRandomField3_gray_value.png Output/MarkovRandomField3_color_value.png 1.0 20 1.0 1
*/


//  This example illustrates the details of the MarkovRandomFieldFilter by using the Fisher distribution
//  to model the likelihood energy.
//  This filter is an application of the Markov Random Fields for classification.
//
//  This example applies the MarkovRandomFieldFilter to
//  classify an image into four classes defined by their Fisher distribution parameters L, M and mu.
//  The optimization is done using a Metropolis algorithm with a random sampler. The
//  regularization energy is defined by a Potts model and the fidelity or likelihood energy is modelled by a
//  Fisher distribution.
//  The parameter of the Fisher distribution was determined for each class in a supervised step.
//  ( See the File OtbParameterEstimatioOfFisherDistribution )
//  This example is a contribution from Jan Wegner.


#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbMarkovRandomFieldFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"


#include "itkScalarToRGBPixelFunctor.h"


#include "otbMRFEnergyPotts.h"
#include "otbMRFEnergyFisherClassification.h"
#include "otbMRFOptimizerMetropolis.h"
#include "otbMRFSamplerRandom.h"


int main(int argc, char* argv[])
{
  if (argc != 8)
  {
    std::cerr << "Missing Parameters " << argc << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage output_gray_label output_color_label lambda iterations "
                 "optimizerTemperature useRandomValue "
              << std::endl;
    return 1;
  }
  //  Then we must decide what pixel type to use for the image. We
  //  choose to make all computations with double precision.
  //  The labeled image is of type unsigned char which allows up to 256 different
  //  classes.

  const unsigned int Dimension = 2;
  using InternalPixelType      = double;
  using LabelledPixelType      = unsigned char;

  using InputImageType    = otb::Image<InternalPixelType, Dimension>;
  using LabelledImageType = otb::Image<LabelledPixelType, Dimension>;

  //  We define a reader for the image to be classified, an initialization for the
  //  classification (which could be random) and a writer for the final
  //  classification.

  using ReaderType = otb::ImageFileReader<InputImageType>;
  using WriterType = otb::ImageFileWriter<LabelledImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char* inputFilename               = argv[1];
  const char* outputFilename              = argv[2];
  const char* outputRescaledImageFileName = argv[3];

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  //  Finally, we define the different classes necessary for the Markov classification.
  //  A MarkovRandomFieldFilter is instantiated, this is the
  // main class which connect the other to do the Markov classification.

  using MarkovRandomFieldFilterType = otb::MarkovRandomFieldFilter<InputImageType, LabelledImageType>;

  //  An MRFSamplerRandomMAP, which derives from the
  //  MRFSampler, is instantiated. The sampler is in charge of
  // proposing a modification for a given site. The
  // MRFSamplerRandomMAP, randomly pick one possible value
  // according to the MAP probability.

  using SamplerType = otb::MRFSamplerRandom<InputImageType, LabelledImageType>;

  //  An MRFOptimizerMetropolis, which derives from the
  // MRFOptimizer, is instantiated. The optimizer is in charge
  // of accepting or rejecting the value proposed by the sampler. The
  // MRFSamplerRandomMAP, accept the proposal according to the
  // variation of energy it causes and a temperature parameter.

  using OptimizerType = otb::MRFOptimizerMetropolis;

  // Two energy, deriving from the MRFEnergy class need to be instantiated. One energy
  // is required for the regularization, taking into account the relationship between neighboring pixels
  // in the classified image. Here it is done with the MRFEnergyPotts, which implements
  // a Potts model.
  //
  // The second energy is used for the fidelity to the original data. Here it is done with a
  // MRFEnergyFisherClassification class, which defines a Fisher distribution to model the data.

  using EnergyRegularizationType = otb::MRFEnergyPotts<LabelledImageType, LabelledImageType>;
  using EnergyFidelityType       = otb::MRFEnergyFisherClassification<InputImageType, LabelledImageType>;

  // The different filters composing our pipeline are created by invoking their
  // New() methods, assigning the results to smart pointers.

  MarkovRandomFieldFilterType::Pointer markovFilter         = MarkovRandomFieldFilterType::New();
  EnergyRegularizationType::Pointer    energyRegularization = EnergyRegularizationType::New();
  EnergyFidelityType::Pointer          energyFidelity       = EnergyFidelityType::New();
  OptimizerType::Pointer               optimizer            = OptimizerType::New();
  SamplerType::Pointer                 sampler              = SamplerType::New();

  // Parameter for the MRFEnergyFisherClassification class are created. The shape parameters M, L
  // and the weighting parameter mu are computed in a supervised step


  if ((bool)(atoi(argv[6])) == true)
  {
    // Overpass random calculation(for test only):
    sampler->InitializeSeed(0);
    optimizer->InitializeSeed(1);
    markovFilter->InitializeSeed(1);
  }

  unsigned int nClass = 4;
  energyFidelity->SetNumberOfParameters(3 * nClass);
  EnergyFidelityType::ParametersType parameters;
  parameters.SetSize(energyFidelity->GetNumberOfParameters());
  // Class 0
  parameters[0] = 12.353042; // Class 0 mu
  parameters[1] = 2.156422;  // Class 0 L
  parameters[2] = 4.920403;  // Class 0 M
  // Class 1
  parameters[3] = 72.068291; // Class 1 mu
  parameters[4] = 11.000000; // Class 1 L
  parameters[5] = 50.950001; // Class 1 M
  // Class 2
  parameters[6] = 146.665985; // Class 2 mu
  parameters[7] = 11.000000;  // Class 2 L
  parameters[8] = 50.900002;  // Class 2 M
  // Class 3
  parameters[9]  = 200.010132; // Class 3 mu
  parameters[10] = 11.000000;  // Class 3 L
  parameters[11] = 50.950001;  // Class 3 M

  energyFidelity->SetParameters(parameters);

  // Parameters are given to the different classes and the sampler, optimizer and
  // energies are connected with the Markov filter.

  OptimizerType::ParametersType param(1);
  param.Fill(atof(argv[6]));
  optimizer->SetParameters(param);
  markovFilter->SetNumberOfClasses(nClass);
  markovFilter->SetMaximumNumberOfIterations(atoi(argv[5]));
  markovFilter->SetErrorTolerance(0.0);
  markovFilter->SetLambda(atof(argv[4]));
  markovFilter->SetNeighborhoodRadius(1);

  markovFilter->SetEnergyRegularization(energyRegularization);
  markovFilter->SetEnergyFidelity(energyFidelity);
  markovFilter->SetOptimizer(optimizer);
  markovFilter->SetSampler(sampler);

  // The pipeline is connected. An itkRescaleIntensityImageFilter
  // rescales the classified image before saving it.

  markovFilter->SetInput(reader->GetOutput());

  using RescaleType                  = itk::RescaleIntensityImageFilter<LabelledImageType, LabelledImageType>;
  RescaleType::Pointer rescaleFilter = RescaleType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);

  rescaleFilter->SetInput(markovFilter->GetOutput());

  writer->SetInput(rescaleFilter->GetOutput());
  writer->Update();

  // convert output image to color
  using RGBPixelType        = itk::RGBPixel<unsigned char>;
  using RGBImageType        = otb::Image<RGBPixelType, 2>;
  using ColorMapFunctorType = itk::Functor::ScalarToRGBPixelFunctor<unsigned long>;

  using ColorMapFilterType                = itk::UnaryFunctorImageFilter<LabelledImageType, RGBImageType, ColorMapFunctorType>;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();

  colormapper->SetInput(rescaleFilter->GetOutput());
  // We can now create an image file writer and save the image.

  using WriterRescaledType = otb::ImageFileWriter<RGBImageType>;

  WriterRescaledType::Pointer writerRescaled = WriterRescaledType::New();

  writerRescaled->SetFileName(outputRescaledImageFileName);
  writerRescaled->SetInput(colormapper->GetOutput());

  writerRescaled->Update();

  // Figure~\ref{fig:MRF_CLASSIFICATION3} shows the output of the Markov Random
  // Field classification into four classes using the
  // Fisher-distribution as likelihood term.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{MarkovRandomField3_color_value.eps}
  // \itkcaption[MRF restoration]{Result of applying
  // the \doxygen{otb}{MarkovRandomFieldFilter} to an extract from a PAN Quickbird
  // image for classification into four classes using the Fisher-distribution as
  // likehood term. From left to right : original image,
  // classification.}
  // \label{fig:MRF_CLASSIFICATION3}
  // \end{figure}

  return EXIT_SUCCESS;
}
