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
#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbPolarimetricSynthesisFilter.h"


int otbPolarimetricSynthesisFilter( int argc, char * argv[] )
{
  try 
    { 
        const char * inputFilename1  = argv[1];
        const char * inputFilename2  = argv[2];
        const char * inputFilename3  = argv[3];
        const char * inputFilename4  = argv[4];
                                
        const char * outputFilename = argv[5];
           
        double  PsiI = strtod(argv[6],NULL);
        //double  TauI = strtod(argv[7],NULL);
	double  KhiI = strtod(argv[7],NULL);
        double  PsiR = strtod(argv[8],NULL);
        //double  TauR = strtod(argv[9],NULL);
	double  KhiR = strtod(argv[9],NULL);
	
        typedef std::complex <double>                   InputPixelType;
        typedef double    	                        OutputPixelType;
        const   unsigned int                            Dimension = 2;

        typedef otb::Image< InputPixelType,  Dimension >  InputImageType;
        typedef otb::Image< OutputPixelType, Dimension >  OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >   ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >   WriterType;

        typedef otb::PolarimetricSynthesisFilter<  InputImageType,InputImageType,InputImageType,InputImageType,OutputImageType >   FilterType;
	

        FilterType::Pointer polarimetricSynthesis = FilterType::New();
        
	polarimetricSynthesis->SetPsiI( PsiI );
	//polarimetricSynthesis->SetTauI( TauI );
	polarimetricSynthesis->SetKhiI( KhiI );
        polarimetricSynthesis->SetPsiR( PsiR );
	//polarimetricSynthesis->SetTauR( TauR );
	polarimetricSynthesis->SetKhiR( KhiR );
      
        ReaderType::Pointer reader1 = ReaderType::New();
        ReaderType::Pointer reader2 = ReaderType::New();
        ReaderType::Pointer reader3 = ReaderType::New();
        ReaderType::Pointer reader4 = ReaderType::New();                        
        WriterType::Pointer writer = WriterType::New();


        reader1->SetFileName( inputFilename1 );
        reader2->SetFileName( inputFilename2 );
        reader3->SetFileName( inputFilename3 );
        reader4->SetFileName( inputFilename4 );                        
        writer->SetFileName( outputFilename );
             
        polarimetricSynthesis->SetInputHH( reader1->GetOutput() );
        polarimetricSynthesis->SetInputHV( reader2->GetOutput() );
        polarimetricSynthesis->SetInputVH( reader3->GetOutput() );
        polarimetricSynthesis->SetInputVV( reader4->GetOutput() );                        
        writer->SetInput( polarimetricSynthesis->GetOutput() );
        
        writer->Update(); 

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
  // Software Guide : EndCodeSnippet

//#endif
  return EXIT_SUCCESS;
}


