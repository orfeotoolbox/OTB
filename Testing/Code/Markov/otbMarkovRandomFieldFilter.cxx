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


#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbMarkovRandomFieldFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbMRFEnergy.h"
#include "otbMRFEnergyPotts.h"
#include "otbMRFEnergyGaussianClassification.h"
#include "otbMRFOptimizerMetropolis.h"
#include "otbMRFSamplerRandom.h"

int otbMarkovRandomFieldFilter(int itkNotUsed(argc), char* argv[])
{
  const unsigned int Dimension = 2;

  typedef double                                   InternalPixelType;
  typedef unsigned char                            LabelledPixelType;
  typedef otb::Image<InternalPixelType, Dimension> InputImageType;
  typedef otb::Image<LabelledPixelType, Dimension> LabelledImageType;
  typedef otb::ImageFileReader<InputImageType>     ReaderType;
  typedef otb::ImageFileWriter<LabelledImageType>  WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  typedef otb::MarkovRandomFieldFilter<InputImageType, LabelledImageType>         MarkovRandomFieldFilterType;
  typedef otb::MRFSamplerRandom<InputImageType, LabelledImageType>                SamplerType;
  typedef otb::MRFOptimizerMetropolis                                             OptimizerType;
  typedef otb::MRFEnergyPotts<LabelledImageType, LabelledImageType>               EnergyRegularizationType;
  typedef otb::MRFEnergyGaussianClassification<InputImageType, LabelledImageType> EnergyFidelityType;

  MarkovRandomFieldFilterType::Pointer markovFilter         = MarkovRandomFieldFilterType::New();
  EnergyRegularizationType::Pointer    energyRegularization = EnergyRegularizationType::New();
  EnergyFidelityType::Pointer          energyFidelity       = EnergyFidelityType::New();
  OptimizerType::Pointer               optimizer            = OptimizerType::New();
  SamplerType::Pointer                 sampler              = SamplerType::New();

  // Overpass random calculation(for test only):
  sampler->InitializeSeed(0);
  optimizer->InitializeSeed(1);
  markovFilter->InitializeSeed(2);

  unsigned int nClass = 4;
  energyFidelity->SetNumberOfParameters(2 * nClass);
  EnergyFidelityType::ParametersType parameters;
  parameters.SetSize(energyFidelity->GetNumberOfParameters());
  parameters[0] = 10.0; //Class 0 mean
  parameters[1] = 10.0; //Class 0 stdev
  parameters[2] = 80.0; //Class 1 mean
  parameters[3] = 10.0; //Class 1 stdev
  parameters[4] = 150.0; //Class 2 mean
  parameters[5] = 10.0; //Class 2 stdev
  parameters[6] = 220.0; //Class 3 mean
  parameters[7] = 10.0; //Class 3 stde
  energyFidelity->SetParameters(parameters);

  optimizer->SetSingleParameter(atof(argv[5]));
  markovFilter->SetNumberOfClasses(nClass);
  markovFilter->SetMaximumNumberOfIterations(atoi(argv[4]));
  markovFilter->SetErrorTolerance(0.0);
  markovFilter->SetLambda(atof(argv[3]));
  markovFilter->SetNeighborhoodRadius(1);

  markovFilter->SetEnergyRegularization(energyRegularization);
  markovFilter->SetEnergyFidelity(energyFidelity);
  markovFilter->SetOptimizer(optimizer);
  markovFilter->SetSampler(sampler);

  markovFilter->SetInput(reader->GetOutput());

  typedef itk::RescaleIntensityImageFilter
  <LabelledImageType, LabelledImageType> RescaleType;
  RescaleType::Pointer rescaleFilter = RescaleType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);

  rescaleFilter->SetInput(markovFilter->GetOutput());

  writer->SetInput(rescaleFilter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;

}
