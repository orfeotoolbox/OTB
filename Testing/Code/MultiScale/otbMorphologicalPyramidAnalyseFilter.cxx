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
#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

#include "itkMacro.h"

int otbMorphologicalPyramidAnalyseFilter(int argc, char * argv[])
{
  try
    {
      const char * inputFilename = argv[1];
      const char * outputFilenamePrefix = argv[2];
      const char * outputFilenameSuffix = argv[3];
      const unsigned int numberOfIterations = atoi(argv[4]);
      const float subSampleScale = atof(argv[5]);

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
	PyramidFilterType;
      typedef PyramidFilterType::OutputImageListType::Iterator ImageListIterator;
      
      // Reading input image
      ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(inputFilename);

      // Instantiation
      PyramidFilterType::Pointer pyramid = PyramidFilterType::New();
      pyramid->SetNumberOfIterations(numberOfIterations);
      pyramid->SetSubSampleScale(subSampleScale);
      pyramid->SetInput(reader->GetOutput());
      pyramid->Update();


      // Retrieving iterators on the results images
      ImageListIterator itAnalyse = pyramid->GetOutput()->Begin();
      ImageListIterator itSupFiltre = pyramid->GetSupFiltre()->Begin();
      ImageListIterator itInfFiltre = pyramid->GetInfFiltre()->Begin();
      ImageListIterator itInfDeci = pyramid->GetInfDeci()->Begin();
      ImageListIterator itSupDeci =  pyramid->GetSupDeci()->Begin();

      WriterType::Pointer writer =  WriterType::New();

      int i=1;
//      std::stringstream oss;
      itk::OStringStream oss;
      // Writing the results images
      while((itAnalyse!=pyramid->GetOutput()->End())
	    &&(itSupFiltre!=pyramid->GetSupFiltre()->End())
	    &&(itInfFiltre!=pyramid->GetInfFiltre()->End())
	    &&(itInfDeci!=pyramid->GetInfDeci()->End())
	    &&(itSupDeci!=pyramid->GetSupDeci()->End())
	    )
	{
	  oss<<outputFilenamePrefix<<"_an_"<<i<<"."<<outputFilenameSuffix;
	  writer->SetInput(itAnalyse.Get());
	  writer->SetFileName(oss.str().c_str());	  
	  writer->Update();
	  oss.str("");
	  oss<<outputFilenamePrefix<<"_sf_"<<i<<"."<<outputFilenameSuffix;
	  writer->SetInput(itSupFiltre.Get());
	  writer->SetFileName(oss.str().c_str());
	  writer->Update();
	  oss.str("");
	  oss<<outputFilenamePrefix<<"_if_"<<i<<"."<<outputFilenameSuffix;
	  writer->SetInput(itInfFiltre.Get());
	  writer->SetFileName(oss.str().c_str());
	  writer->Update();
	  oss.str("");
	  oss<<outputFilenamePrefix<<"_id_"<<i<<"."<<outputFilenameSuffix;
	  writer->SetInput(itInfDeci.Get());
	  writer->SetFileName(oss.str().c_str());
	  writer->Update();
	  std::cout<<"File "<<oss<<" written"<<std::endl;
	  oss.str("");
	  oss<<outputFilenamePrefix<<"_sd_"<<i<<"."<<outputFilenameSuffix;
	  writer->SetInput(itSupDeci.Get());
	  writer->SetFileName(oss.str().c_str());
	  writer->Update();
	  oss.str("");
	  ++itAnalyse;
	  ++itSupFiltre;
	  ++itInfFiltre;
	  ++itInfDeci;
	  ++itSupDeci;
	  ++i;
	}
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
