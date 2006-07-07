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
#include "otbThresholdImageToPointSetFilter.h"
#include "itkPointSet.h"

int otbThresholdImageToPointSetTest( int argc, char * argv[] )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        unsigned int  Number = 1;
       
        typedef unsigned char                                        InputPixelType;
        const   unsigned int        	                             Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension>              InputImageType;
        typedef otb::ImageFileReader< InputImageType  >              ReaderType;
	typedef itk::PointSet<InputPixelType,Dimension>              PointSetType;
	   
	typedef otb::ThresholdImageToPointSetFilter<InputImageType,PointSetType>  FunctionType;
  
        InputImageType::RegionType   region;
        InputImageType::SizeType     size;
        InputImageType::IndexType    start;
 
        start.Fill( 0 );
        size[0] = 50;
        size[1] = 50;

        ReaderType::Pointer   reader         = ReaderType::New();
	FunctionType::Pointer function       = FunctionType::New();
        PointSetType::Pointer pointList      = PointSetType::New();	
	
        reader->SetFileName( inputFilename  );
	
	InputImageType::Pointer image = reader->GetOutput();

        region.SetIndex( start );
        region.SetSize( size );	
	image->SetRegions(region);
	image->Update();

	function->SetInput( 0, image );
        function->SetOutput(pointList);
	function->Update();
		
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

