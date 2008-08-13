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
        
        const char * inputFilename1  = argv[1];
        const char * inputFilename2  = argv[2];
        const char * inputFilename3  = argv[3];
        const char * inputFilename4  = argv[4];
                                
        const char * outputFilename = argv[5];
           
        double  PsiI = strtod(argv[6],NULL);
        double  TauI = strtod(argv[7],NULL);
        double  PsiR = strtod(argv[8],NULL);
        double  TauR = strtod(argv[9],NULL);
    

        typedef std::complex <double>                           InputPixelType;
        typedef double    	                                OutputPixelType;
        const   unsigned int                                    Dimension = 2;
        typedef otb::Image< InputPixelType,  Dimension >        InputImageType;
        typedef otb::Image< OutputPixelType, Dimension >        OutputImageType;

        typedef otb::VectorImage< InputPixelType,  Dimension >  InputVectorImageType;

        typedef otb::ImageFileReader< InputImageType  >         ReaderType;
        typedef otb::ImageFileWriter< OutputImageType >         WriterType;

        typedef otb::MultiChannelsPolarimetricSynthesisFilter< InputVectorImageType,OutputImageType >   FilterType;

        FilterType::Pointer polarimetricSynthesis = FilterType::New();
        
	polarimetricSynthesis->SetPsiI( PsiI );
	polarimetricSynthesis->SetTauI( TauI );
        polarimetricSynthesis->SetPsiR( PsiR );
	polarimetricSynthesis->SetTauR( TauR );
  
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
        
        reader1->Update();
        reader2->Update();
        reader3->Update();
        reader4->Update();

        InputVectorImageType::Pointer outputPtr = InputVectorImageType::New();        

        InputVectorImageType::IndexType start;
        start[0]=0;
        start[1]=0;

	InputVectorImageType::SizeType size = reader1->GetOutput()->GetLargestPossibleRegion().GetSize();

        InputVectorImageType::RegionType region;
        region.SetSize(size);
        region.SetIndex(start);
        outputPtr->SetRegions(region);
        outputPtr->SetNumberOfComponentsPerPixel(4);
        outputPtr->Allocate();
        InputVectorImageType::PixelType pix0;
        pix0.SetSize(4);
        InputPixelType complexpix;
        complexpix.imag()=0;
        complexpix.real()=0;
        pix0.Fill(complexpix);
        
        outputPtr->FillBuffer(pix0);

        InputVectorImageType::Pointer vectorImage = InputVectorImageType::New();
        
        // Copy of the pictures in a vector
        itk::ImageRegionConstIterator<InputImageType> inputIt1(reader1->GetOutput(), reader1->GetOutput()->GetLargestPossibleRegion());
        itk::ImageRegionConstIterator<InputImageType> inputIt2(reader2->GetOutput(), reader2->GetOutput()->GetLargestPossibleRegion());
        itk::ImageRegionConstIterator<InputImageType> inputIt3(reader3->GetOutput(), reader3->GetOutput()->GetLargestPossibleRegion());
        itk::ImageRegionConstIterator<InputImageType> inputIt4(reader4->GetOutput(), reader4->GetOutput()->GetLargestPossibleRegion());
        itk::ImageRegionIterator<InputVectorImageType> outputIt(outputPtr, outputPtr->GetLargestPossibleRegion());

        inputIt1.GoToBegin();
        inputIt2.GoToBegin();
        inputIt3.GoToBegin();
        inputIt4.GoToBegin();
        outputIt.GoToBegin();

        while( !inputIt1.IsAtEnd() ) 
          {   
          InputVectorImageType::PixelType pix;
          pix.SetSize(4);
          pix[0]=inputIt1.Get();
          pix[1]=inputIt2.Get();
          pix[2]=inputIt3.Get();
          pix[3]=inputIt4.Get();
         
          outputIt.Set( pix );
          ++inputIt1;
          ++inputIt2;
          ++inputIt3;
          ++inputIt4;
          ++outputIt;
          }
        
        polarimetricSynthesis->SetInput(outputPtr);
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


