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

int otbPipelineMemoryPrintCalculatorNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  otb::PipelineMemoryPrintCalculator::Pointer calculator = otb::PipelineMemoryPrintCalculator::New();

  return EXIT_SUCCESS;
}


int otbPipelineMemoryPrintCalculatorTest(int itkNotUsed(argc), char * argv[])
{
  typedef otb::VectorImage<double, 2>            VectorImageType;
  typedef otb::Image<double, 2>                  ImageType;
  typedef otb::ImageFileReader<VectorImageType>  ReaderType;
  typedef otb::VectorImageToIntensityImageFilter
    <VectorImageType, ImageType>                 IntensityImageFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  IntensityImageFilterType::Pointer intensity = IntensityImageFilterType::New();
  intensity->SetInput(reader->GetOutput());

  otb::PipelineMemoryPrintCalculator::Pointer calculator = otb::PipelineMemoryPrintCalculator::New();
  calculator->SetDataToWrite(intensity->GetOutput());
  calculator->Compute();

  otb::PipelineMemoryPrintCalculator::MemoryPrintType availableMemoryInBytes = 104858;
  otb::PipelineMemoryPrintCalculator::MemoryPrintType memoryPrint = calculator->GetMemoryPrint();

  unsigned int optimalNbDiv = otb::PipelineMemoryPrintCalculator::EstimateOptimalNumberOfStreamDivisions(memoryPrint, availableMemoryInBytes);

  std::ofstream ofs(argv[2]);
  ofs<<"Memory print of whole pipeline:     "<<calculator->GetMemoryPrint()
    * otb::PipelineMemoryPrintCalculator::ByteToMegabyte << " Mb"<<std::endl;
  ofs<<"Available memory:                   "<<availableMemoryInBytes
    * otb::PipelineMemoryPrintCalculator::ByteToMegabyte << " Mb"<<std::endl;
  ofs<<"Optimal number of stream divisions: "<<optimalNbDiv<<std::endl;
  ofs<<"Bias correction factor applied:     "<<calculator->GetBiasCorrectionFactor()<<std::endl;
  ofs.close();

  return EXIT_SUCCESS;
}
