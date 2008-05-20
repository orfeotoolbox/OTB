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
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbVectorImage.h"
#include "otbImage.h"

#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"

//int main( int argc, char * argv[] )
int otbSimpleRcsPanSharpeningFusionImageFilter( int argc, char * argv[] )
{
  try 
    { 
      const char * panchro = argv[1];
      const char * multispect = argv[2];
      const char * output = argv[3];
      
      const unsigned int Dimension = 2;
      typedef double PixelType;
      
      
      typedef otb::VectorImage<PixelType,Dimension>  VectorImageType;
      typedef otb::Image<PixelType,Dimension>        PanchroImageType;
      typedef otb::ImageFileReader<VectorImageType>  VectorReaderType;
      typedef otb::ImageFileReader<PanchroImageType> ImageReaderType;
      typedef otb::StreamingImageFileWriter<VectorImageType>  VectorImageWriterType;
      typedef otb::SimpleRcsPanSharpeningFusionImageFilter
          <PanchroImageType, VectorImageType,  VectorImageType> FilterType;
      
      VectorReaderType::Pointer multiSpectReader = VectorReaderType::New();
      ImageReaderType::Pointer  panchroReader = ImageReaderType::New();
      FilterType::Pointer       filter = FilterType::New();
      VectorImageWriterType::Pointer  writer = VectorImageWriterType::New();
      
      multiSpectReader->SetFileName(multispect);
      panchroReader->SetFileName(panchro);

      PanchroImageType::SizeType radius;
      radius[0]=3;
      radius[1]=3;
      itk::Array< double > filterCoeffs;
      filterCoeffs.SetSize(radius[0]*radius[1]);
      filterCoeffs.Fill(1);
      
      filter->SetXsInput(multiSpectReader->GetOutput());
      filter->SetPanInput(panchroReader->GetOutput());
      filter->SetRadius(radius);
      filter->SetFilter(filterCoeffs);
      writer->SetInput( filter->GetOutput() );
      writer->SetFileName(output);
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
  
  
  return EXIT_SUCCESS;
}


