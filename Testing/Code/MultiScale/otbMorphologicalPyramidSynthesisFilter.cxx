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
#include "itkExceptionObject.h"

#include "otbMorphologicalPyramidAnalyseFilter.h"
#include "otbMorphologicalPyramidSynthesisFilter.h"
#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbMorphologicalPyramidSynthesisFilter(int argc, char * argv[])
{
  try
    {
      const char * inputFilename = argv[1];
      const char * outputFilename = argv[2];
      const unsigned int numberOfIterations = atoi(argv[3]);
      const float subSampleScale = atof(argv[4]);
      
      const unsigned int Dimension = 2;
      typedef unsigned char InputPixelType;
      typedef unsigned char OutputPixelType;

      typedef otb::Image<InputPixelType,Dimension> InputImageType;
      typedef otb::Image<OutputPixelType,Dimension> OutputImageType;

      typedef otb::ImageFileReader<InputImageType> ReaderType;
      typedef otb::ImageFileWriter<OutputImageType> WriterType;

      typedef itk::BinaryBallStructuringElement<InputPixelType,Dimension> StructuringElementType;
      typedef otb::OpeningClosingMorphologicalFilter<InputImageType,InputImageType,StructuringElementType>
	OpeningClosingFilterType;
      typedef otb::MorphologicalPyramidAnalyseFilter<InputImageType,OutputImageType,OpeningClosingFilterType>
	PyramidAnalyseFilterType;
      typedef otb::MorphologicalPyramidSynthesisFilter<InputImageType,OutputImageType>
	PyramidSynthesisFilterType;

      // Reading input image
      ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(inputFilename);

      // Analysis
      PyramidAnalyseFilterType::Pointer pyramidAnalyse = PyramidAnalyseFilterType::New();
      pyramidAnalyse->SetNumberOfIterations(numberOfIterations);
      pyramidAnalyse->SetSubSampleScale(subSampleScale);
      pyramidAnalyse->SetInput(reader->GetOutput());
      pyramidAnalyse->Update();

      // Synthesis
      PyramidSynthesisFilterType::Pointer pyramidSynthesis = PyramidSynthesisFilterType::New();      
      pyramidSynthesis->SetInput(pyramidAnalyse->GetOutput()->Back());
      pyramidSynthesis->SetSupFiltre(pyramidAnalyse->GetSupFiltre());
      pyramidSynthesis->SetSupDeci(pyramidAnalyse->GetSupDeci());
      pyramidSynthesis->SetInfFiltre(pyramidAnalyse->GetInfFiltre());
      pyramidSynthesis->SetInfDeci(pyramidAnalyse->GetInfDeci());
      pyramidSynthesis->Update();

      // Writing the output image
      WriterType::Pointer writer = WriterType::New();
      writer->SetFileName(outputFilename);
      writer->SetInput(pyramidSynthesis->GetOutput()->Back());
      writer->Update();
      
    }
      catch( itk::ExceptionObject & err ) 
	{ 
	  std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
	  std::cout << err << std::endl; 
	  return EXIT_FAILURE;
	} 

      catch( ... ) 
	{ 
	  std::cout << "Unknown exception thrown !" << std::endl; 
	  return EXIT_FAILURE;
	} 

      return EXIT_SUCCESS;
    }
