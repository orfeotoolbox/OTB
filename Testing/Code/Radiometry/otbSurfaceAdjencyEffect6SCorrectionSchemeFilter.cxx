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

#include "otbSurfaceAdjencyEffect6SCorrectionSchemeFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbSurfaceAdjencyEffect6SCorrectionSchemeFilter(int argc, char * argv[])
{
  try
    {
      const char * inputFileName  = argv[1];
      const char * outputFileName = argv[2];
        
      const unsigned int Dimension = 2;
      typedef double PixelType;
      typedef otb::VectorImage<PixelType,Dimension> InputImageType;
      typedef otb::VectorImage<PixelType,Dimension> OutputImageType;
      typedef otb::ImageFileReader<InputImageType>  ReaderType;
      typedef otb::ImageFileWriter<OutputImageType> WriterType;
     
      typedef otb::SurfaceAdjencyEffect6SCorrectionSchemeFilter<InputImageType, OutputImageType> SurfaceAdjencyEffect6SCorrectionSchemeFilterType;
      typedef SurfaceAdjencyEffect6SCorrectionSchemeFilterType::PonderationMatrixType            PonderationMatrixType;
      typedef SurfaceAdjencyEffect6SCorrectionSchemeFilterType::PonderationValuesContainerType   PonderationValuesContainerType;

      // Instantiating object
      SurfaceAdjencyEffect6SCorrectionSchemeFilterType::Pointer filter = SurfaceAdjencyEffect6SCorrectionSchemeFilterType::New();
     
      ReaderType::Pointer reader  = ReaderType::New();
      WriterType::Pointer writer = WriterType::New();
      reader->SetFileName(inputFileName);
      writer->SetFileName(outputFileName);
      reader->GenerateOutputInformation();
      
      filter->SetWindowRadius(atoi(argv[3]));
      filter->SetUpwardTransmission(static_cast<double>(atof(argv[4])));
      filter->SetUpwardDirectTransmission(static_cast<double>(atof(argv[5])));
      filter->SetUpwardDiffusTransmission(static_cast<double>(atof(argv[6])));

      // Poderation matrix vector construction
      unsigned int nbOfComponent = reader->GetOutput()->GetNumberOfComponentsPerPixel();
      unsigned int id = 7;
      PonderationValuesContainerType container;
      PonderationMatrixType mat(2*atoi(argv[3])+1, 2*atoi(argv[3])+1);

      for(unsigned int i=0; i<nbOfComponent; i++)
	{ 
	  mat.Fill(0.);
	  for(unsigned int r=0; r<mat.Rows(); r++)
	    {
	      for(unsigned int c=0; c<mat.Rows(); c++)
		{ 
		  mat(r, c) = static_cast<double>(atof(argv[id]));
		  id++;
		}
	    }
	  container.push_back(mat);
	}


      filter->SetPonderationValues(container);
      filter->SetInput(reader->GetOutput());
      writer->SetInput(filter->GetOutput());

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
