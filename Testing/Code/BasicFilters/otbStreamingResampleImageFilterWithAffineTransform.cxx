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

#include "otbStreamingResampleImageFilter.h"
#include "otbVectorImage.h"
#include "itkVector.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

// Transform
#include "itkTransform.h"
#include "itkAffineTransform.h"


int otbStreamingResampleImageFilterWithAffineTransform(int argc, char* argv[])
{
  // Images definition
  const unsigned int Dimension = 2;
  typedef double                                                  InternalPixelType;
  typedef otb::VectorImage<InternalPixelType, Dimension>          ImageType;
  typedef otb::StreamingResampleImageFilter<ImageType, ImageType> ImageResamplerType;

  // Instantiate a Resampler
  ImageResamplerType::Pointer resampler = ImageResamplerType::New();

  if (argc== 4)
    {
    const char * infname = argv[1];
    const char * outfname = argv[3];
    unsigned int isize    = atoi(argv[2]);
  
    typedef itk::Transform<
        InternalPixelType, Dimension, Dimension>     TransformType;

    typedef itk::AffineTransform<
        InternalPixelType, Dimension>              AffineTransformType;
    typedef otb::ImageFileReader<ImageType>         ReaderType;

    // Instantiate an affine transformation Pointer
    AffineTransformType::Pointer affineTransform    = AffineTransformType::New();

    // Instanciate reader
    ReaderType::Pointer reader = ReaderType::New();

    // Get the image file
    reader->SetFileName(infname);
    reader->UpdateOutputInformation();
  
    /** Set the options. */
    ImageResamplerType::OriginType centerOfRotation;
    centerOfRotation[ 0 ] = -3.0; centerOfRotation[ 1 ] = -3.0;
    affineTransform->SetCenter( centerOfRotation );
  
    /** Create and set parameters. */
    AffineTransformType::ParametersType parameters( affineTransform->GetNumberOfParameters() );
    parameters[ 0 ] =   1.1;
    parameters[ 1 ] =   0.1;
    parameters[ 2 ] =  -0.2;
    parameters[ 3 ] =   0.9;
    parameters[ 4 ] =  10.3;
    parameters[ 5 ] = -33.8;
    affineTransform->SetParameters( parameters );
  
    // Get the size specified by the user
    ImageType::SizeType   size;
    size.Fill(isize);

    // Default value builder
    ImageType::PixelType defaultValue;
    itk::NumericTraits<ImageType::PixelType>::SetLength(defaultValue, reader->GetOutput()->GetNumberOfComponentsPerPixel());

    /** Set the OptResampler Parameters*/
    resampler->SetInput(reader->GetOutput());
    resampler->SetOutputParametersFromImage(reader->GetOutput());
    resampler->SetTransform(affineTransform);
    resampler->SetDisplacementFieldSpacing(5.);
    resampler->SetOutputSize(size);
    resampler->SetEdgePaddingValue(defaultValue);
    
    // Write the resampled image
    typedef otb::ImageFileWriter<ImageType>    WriterType;
    WriterType::Pointer writer= WriterType::New();
    writer->SetNumberOfDivisionsTiledStreaming(4);
    writer->SetFileName(outfname);
    writer->SetInput(resampler->GetOutput());
    writer->Update();
    }
  else
    {
    std::cout <<" Unit Test"<< std::endl;
    }


  return EXIT_SUCCESS;
}
