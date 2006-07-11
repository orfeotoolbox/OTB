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

#include "itkExceptionObject.h"
#include "itkImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbHarrisImageToPointSetFilter.h"
#include <fstream>

int otbHarrisToPointSet( int argc, char * argv[] )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
 	
	double SigmaD((double)::atof(argv[3]));
	double SigmaI((double)::atof(argv[4]));
	double AlphaValue((double)::atof(argv[5]));
	        
        typedef unsigned char                                   InputPixelType;
        const   unsigned int        	                        Dimension = 2;
        typedef unsigned char   	                        OutputPixelType;

	InputPixelType Threshold((OutputPixelType)::atoi(argv[6]));
	
        typedef itk::Image< InputPixelType,  Dimension>            InputImageType;
        typedef itk::Image< OutputPixelType, Dimension>            OutputImageType;
        typedef otb::ImageFileReader< InputImageType  >            ReaderType;  
        typedef otb::ImageFileWriter< OutputImageType >            WriterType;
	typedef otb::HarrisImageToPointSetFilter<InputImageType>   FunctionType;
        typedef FunctionType::OutputPointSetType                   OutputPointSetType;
	typedef OutputPointSetType::PointType                      OutputPointType;
        typedef itk::Image< OutputPixelType, Dimension >	   OutputImageType;
  
        ReaderType::Pointer          reader    = ReaderType::New();
	FunctionType::Pointer        harris    = FunctionType::New();
	OutputPointSetType::Pointer  pointList = OutputPointSetType::New();
        OutputImageType::Pointer     outImage  = OutputImageType::New();
        WriterType::Pointer          writer    = WriterType::New();
	OutputPointType              *CoordPoint = NULL;
		
        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
 	
	harris->SetInput( 0,reader->GetOutput() );
        harris->SetSigmaD( SigmaD );
        harris->SetSigmaI( SigmaI );
	harris->SetAlpha( AlphaValue );
	harris->SetThreshold( Threshold );
        harris->GraftOutput( pointList );

        harris->Update();

	std::ofstream file;
	file.open(outputFilename);

	unsigned long  NbPoints  = pointList->GetNumberOfPoints();
	file << "NbPoints : " << NbPoints <<std::endl;
	
	for (unsigned long i = 0 ; i < NbPoints ; i++)
	   {
	   pointList->GetPoint(i,CoordPoint);
           file << i <<" / " << NbPoints;
	   file << CoordPoint << std::endl;
	   }
	
	file.close();
	
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}

