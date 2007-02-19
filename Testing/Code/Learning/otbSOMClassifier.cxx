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

#include <fstream>
#include "otbVectorImage.h"
#include "otbSOMMap.h"
#include "otbSOMClassifier.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkImageToListAdaptor.h"

int otbSOMClassifier(int argc, char* argv[] )
{
  try
    { 
    if (argc != 4)
      {
      std::cout << "Usage : " << argv[0] << " inputImage modelFile outputImage" 
                << std::endl ;
      return EXIT_FAILURE;
      }

    const char * imageFilename  = argv[1];
    const char * mapFilename  = argv[2];
    const char * outputFilename = argv[3];
      
    typedef double                              InputPixelType;
    typedef int                                 LabelPixelType;
    const   unsigned int        	         Dimension = 2;

    typedef itk::RGBPixel<InputPixelType> PixelType;
    typedef itk::Statistics::EuclideanDistance<PixelType> DistanceType;
    typedef otb::SOMMap<PixelType,DistanceType,Dimension> SOMMapType;
    typedef otb::Image<PixelType,Dimension> InputImageType;
    typedef otb::ImageFileReader< InputImageType  >  ReaderType;
    typedef otb::ImageFileReader<SOMMapType> SOMReaderType;
    typedef itk::Statistics::ImageToListAdaptor< InputImageType > SampleType;
    typedef otb::SOMClassifier<SampleType,SOMMapType,LabelPixelType> ClassifierType;
    typedef otb::Image<LabelPixelType, Dimension >  OutputImageType;
    typedef itk::ImageRegionIterator< OutputImageType>  OutputIteratorType;
    typedef otb::ImageFileWriter<OutputImageType>  WriterType;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( imageFilename  );
    reader->Update();
    std::cout << "Image read" << std::endl;  

    SOMReaderType::Pointer somreader = SOMReaderType::New();
    somreader->SetFileName(mapFilename);
    somreader->Update();
    std::cout<<"SOM map read"<<std::endl;

    SampleType::Pointer sample = SampleType::New();    
    sample->SetImage(reader->GetOutput());

    ClassifierType::Pointer classifier = ClassifierType::New() ;
    classifier->SetSample(sample.GetPointer());
    classifier->SetMap(somreader->GetOutput());
    classifier->Update() ;

    OutputImageType::Pointer outputImage = OutputImageType::New();
    outputImage->SetRegions( reader->GetOutput()->GetLargestPossibleRegion());
    outputImage->Allocate();

    ClassifierType::OutputType* membershipSample = classifier->GetOutput();
    ClassifierType::OutputType::ConstIterator m_iter =  membershipSample->Begin();
    ClassifierType::OutputType::ConstIterator m_last =  membershipSample->End();

    OutputIteratorType  outIt(outputImage,outputImage->GetLargestPossibleRegion());
    outIt.GoToBegin();

    while (m_iter != m_last && !outIt.IsAtEnd())
    {
    outIt.Set(m_iter.GetClassLabel());
    ++m_iter ;
    ++outIt;
    }
	
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFilename);
    writer->SetInput(outputImage);
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
    std::cout << "Unknown exception !" << std::endl; 
    return EXIT_FAILURE;
    } 
 
  return EXIT_SUCCESS;
}







