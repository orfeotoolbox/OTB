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
	        
        typedef unsigned char                                   PixelType;
        const   unsigned int        	                        Dimension = 2;

	PixelType Threshold((PixelType)::atoi(argv[6]));
	
        typedef itk::Image< PixelType,  Dimension>                 ImageType;
        typedef otb::ImageFileReader< ImageType  >                 ReaderType;  
	typedef otb::HarrisImageToPointSetFilter<ImageType>        FunctionType;
        typedef FunctionType::OutputPointSetType                   OutputPointSetType;
	typedef OutputPointSetType::PointType                      OutputPointType;
  
        ReaderType::Pointer           reader    = ReaderType::New();
	FunctionType::Pointer         harris    = FunctionType::New();
	OutputPointSetType::Pointer   pointList = OutputPointSetType::New();
	OutputPointType               CoordPoint;
	
        reader->SetFileName( inputFilename  );
 	
	harris->SetInput( 0,reader->GetOutput() );
        harris->SetSigmaD( SigmaD );
        harris->SetSigmaI( SigmaI );
	harris->SetAlpha( AlphaValue );
	harris->SetThreshold( Threshold );
	harris->SetOutput(pointList);

        harris->Update();


	std::ofstream file;
	file.open(outputFilename);

	unsigned long  NbPoints  = pointList->GetNumberOfPoints();
	file << "NbPoints : " << NbPoints <<std::endl;
	
	for (unsigned long i = 0 ; i < NbPoints ; i++)
	   {
	   pointList->GetPoint(i,&CoordPoint);
           file << i <<" / " << NbPoints << " : " ;
	   file << CoordPoint[0]<<" , "<< CoordPoint[1] << std::endl;
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

