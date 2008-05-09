
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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbMarkovClassificationFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbMRFEnergy.h"
#include "otbMRFEnergyPotts.h"
#include "otbMRFEnergyGaussianClassification.h"
#include "otbMRFOptimizerMetropolis.h"
#include "otbMRFSamplerRandom.h"

int otbMarkovClassificationFilter( int argc, char* argv[] ) 
{
  //std::cerr << " inputImage inputInitialization output lambda iterations temperature" << std::endl;
  
  
  const unsigned int Dimension = 2;
  
  typedef double                                    InternalPixelType;
  typedef unsigned char                             LabelledPixelType;
  typedef otb::Image<InternalPixelType, Dimension>  InputImageType;
  typedef otb::Image<LabelledPixelType, Dimension>  LabelledImageType;
  typedef otb::ImageFileReader< InputImageType >    ReaderType;
  typedef otb::ImageFileReader< LabelledImageType > ReaderLabelledType;
  typedef otb::ImageFileWriter< LabelledImageType > WriterType;
  
  ReaderType::Pointer         reader  = ReaderType::New();
  ReaderLabelledType::Pointer reader2 = ReaderLabelledType::New();
  WriterType::Pointer         writer  = WriterType::New();
  
  const char * inputFilename     = argv[1];
  const char * labelledFilename  = argv[2];
  const char * outputFilename    = argv[3];
  
  reader->SetFileName( inputFilename );
  reader2->SetFileName( labelledFilename );
  writer->SetFileName( outputFilename );

  typedef otb::MarkovClassificationFilter<InputImageType,
					  LabelledImageType>        MarkovClassificationFilterType;
  typedef otb::MRFSamplerRandom< InputImageType, LabelledImageType> SamplerType;
  typedef otb::MRFOptimizerMetropolis                               OptimizerType;
  typedef otb::MRFEnergyPotts<LabelledImageType, LabelledImageType> EnergyRegularizationType;
  typedef otb::MRFEnergyGaussianClassification<InputImageType, 
					       LabelledImageType>   EnergyFidelityType;

  MarkovClassificationFilterType::Pointer markovFilter         = MarkovClassificationFilterType::New();
  EnergyRegularizationType::Pointer       energyRegularization = EnergyRegularizationType::New();
  EnergyFidelityType::Pointer             energyFidelity       = EnergyFidelityType::New();
  OptimizerType::Pointer                  optimizer            = OptimizerType::New();
  SamplerType::Pointer                    sampler              = SamplerType::New();


  unsigned int nClass = 4;
  itk::VariableLengthVector<double> mean;
  itk::VariableLengthVector<double> stdDev;
  mean.SetSize(nClass);
  stdDev.SetSize(nClass);
  mean[0]=10;
  mean[1]=80;
  mean[2]=150;
  mean[3]=220;

  stdDev[0]=10;
  stdDev[1]=10;
  stdDev[2]=10;
  stdDev[3]=10;
  
  energyFidelity->SetNumberOfParameters(2*nClass); 
  EnergyFidelityType::ParametersType parameters;
  parameters.SetSize(energyFidelity->GetNumberOfParameters());
  parameters[0]=10.0; //Class 0 mean
  parameters[1]=10.0; //Class 0 stdev
  parameters[2]=80.0;//Class 1 mean
  parameters[3]=10.0; //Class 1 stdev
  parameters[4]=150.0; //Class 2 mean
  parameters[5]=10.0; //Class 2 stdev
  parameters[6]=220.0;//Class 3 mean
  parameters[7]=10.0; //Class 3 stde
  energyFidelity->SetParameters(parameters);
  OptimizerType::ParametersType paramOpt(1);
  paramOpt.Fill(atof(argv[6]));
  optimizer->SetParameters(paramOpt);
  markovFilter->SetNumberOfClasses(nClass);
  markovFilter->SetValueInsteadRandom(500); // Unable rand() calculation
  markovFilter->SetMaximumNumberOfIterations(atoi(argv[5]));
  markovFilter->SetErrorTolerance(-1.0);
  markovFilter->SetLambda(atof(argv[4]));
  markovFilter->SetNeighborhoodRadius(1);
  
  markovFilter->SetEnergyRegularization(energyRegularization);
  markovFilter->SetEnergyFidelity(energyFidelity);
  markovFilter->SetOptimizer(optimizer);
  markovFilter->SetSampler(sampler);
   
  markovFilter->SetTrainingInput(reader2->GetOutput());
  markovFilter->SetInput(reader->GetOutput());
    
  typedef itk::RescaleIntensityImageFilter< LabelledImageType, LabelledImageType > RescaleType;
  RescaleType::Pointer rescaleFilter = RescaleType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  
  rescaleFilter->SetInput( markovFilter->GetOutput() );
  
  writer->SetInput( rescaleFilter->GetOutput() );
  writer->Update();  
  
  
  return 0;
  
}

