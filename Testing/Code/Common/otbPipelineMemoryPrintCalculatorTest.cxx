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
#include "otbPipelineMemoryPrintCalculator.h"

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbVectorImageToIntensityImageFilter.h"

int otbPipelineMemoryPrintCalculatorNew(int argc, char * argv[])
{
  otb::PipelineMemoryPrintCalculator::Pointer calculator = otb::PipelineMemoryPrintCalculator::New();

  return EXIT_SUCCESS;
}


int otbPipelineMemoryPrintCalculatorTest(int argc, char * argv[])
{
  typedef otb::VectorImage<double,2>            VectorImageType;
  typedef otb::Image<double,2>                  ImageType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::VectorImageToIntensityImageFilter
    <VectorImageType,ImageType>                 IntensityImageFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  
  IntensityImageFilterType::Pointer intensity = IntensityImageFilterType::New();
  intensity->SetInput(reader->GetOutput());

  otb::PipelineMemoryPrintCalculator::Pointer calculator = otb::PipelineMemoryPrintCalculator::New();
  calculator->SetDataToWrite(intensity->GetOutput());
  calculator->SetAvailableMemory(0.1);
  calculator->Compute();

  std::cout<<"Pipeline memory print:   "<<calculator->GetMemoryPrint()<<" Mo"<<std::endl;
  std::cout<<"Optimal stream division: "<<calculator->GetOptimalNumberOfStreamDivisions()<<" tiles"<<std::endl;

  return EXIT_SUCCESS;
}
