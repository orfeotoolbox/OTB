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
#include "otbMultiChannelsPolarimetricSynthesisFilter.h"


int otbMultiChannelsPolarimetricSynthesisFilter( int argc, char * argv[] )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        const char * outputFilename = argv[2];
           
        double  PsiI = strtod(argv[3],NULL);
        double  TauI = strtod(argv[4],NULL);
        double  PsiR = strtod(argv[5],NULL);
        double  TauR = strtod(argv[6],NULL);
	
        std::cout<<"PsiI: "<<PsiI<<std::endl;
        std::cout<<"TauI: "<<TauI<<std::endl;
        std::cout<<"PsiR: "<<PsiR<<std::endl;
        std::cout<<"TauR: "<<TauR<<std::endl;
        
        PsiI=45;
        TauI=15;
        PsiR=95;
        TauR=5;
        
        typedef std::complex <double>                        InputPixelType;
        typedef double    	                        OutputPixelType;
        const   unsigned int                            Dimension = 2;

        typedef otb::VectorImage< InputPixelType,  Dimension >  InputImageType;
        typedef otb::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;

        typedef otb::MultiChannelsPolarimetricSynthesisFilter< InputImageType,OutputImageType >   FilterType;
	

        FilterType::Pointer polarimetricSynthesis = FilterType::New();
        
	polarimetricSynthesis->SetPsiI( PsiI );
	polarimetricSynthesis->SetTauI( TauI );
        polarimetricSynthesis->SetPsiR( PsiR );
	polarimetricSynthesis->SetTauR( TauR );
         
        
        ReaderType::Pointer reader = ReaderType::New();
        WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        writer->SetFileName( outputFilename );
        
        polarimetricSynthesis->SetInput( reader->GetOutput() );
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


