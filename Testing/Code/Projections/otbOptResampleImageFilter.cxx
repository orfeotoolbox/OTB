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

#include "otbOptResampleImageFilter.h"
#include "otbVectorImage.h"
#include "itkVector.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

// Transform 
#include "itkTransform.h"
#include "itkAffineTransform.h"

int otbOptResampleImageFilter(int argc, char* argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[3];
  unsigned int isize    = atoi(argv[2]);
  
  // Images definition
  const unsigned int Dimension = 2;
  typedef double                                      PixelType;
  typedef otb::VectorImage<PixelType, Dimension>      ImageType;
  typedef itk::Vector<PixelType, 2>                   DeformationValueType;
  typedef otb::Image<DeformationValueType, Dimension> DeformationFieldType;

  typedef itk::Transform<
    PixelType, Dimension, Dimension>     TransformType;

  typedef itk::AffineTransform<
    PixelType, Dimension>              AffineTransformType;
  
  // 
  typedef otb::OptResampleImageFilter<ImageType, ImageType, DeformationFieldType> ImageResamplerType;

  typedef otb::ImageFileReader<ImageType>         ReaderType;

  // Instantiate an affine transformation Pointer
  AffineTransformType::Pointer affineTransform    = AffineTransformType::New();
  // Istantiate a Resampler
  ImageResamplerType::Pointer resampler = ImageResamplerType::New();
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
  
  /** Set the OptResampler Parameters*/
  resampler->SetInput(reader->GetOutput());
  resampler->SetTransform(affineTransform);
  resampler->SetDeformationFieldSpacing(5.); // TODO : add the spacing
                                             // it to the
                                             // command line
  resampler->SetOutputSize(size);
  
  // Write the resampled image
  typedef otb::StreamingImageFileWriter<ImageType>    WriterType;
  WriterType::Pointer writer= WriterType::New();
  writer->SetTilingStreamDivisions();
  // TODO : implement the WriteGeomFile in StreamingImageFileWriter
  //writer->WriteGeomFileOn();
  writer->SetFileName(outfname);
  writer->SetInput(resampler->GetOutput());
  writer->Update();


  return EXIT_SUCCESS;
}
