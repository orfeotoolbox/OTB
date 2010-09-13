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

#include "otbGenericRSResampleImageFilter.h"
#include "otbVectorImage.h"
#include "itkVector.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

#include <ogr_spatialref.h>

int otbGenericRSResampleImageFilter(int argc, char* argv[])
{
  // Images definition
  const unsigned int Dimension = 2;
  typedef double                                      PixelType;
  typedef otb::VectorImage<PixelType, Dimension>      ImageType;
  typedef ImageType::SizeType                         SizeType;
  
  typedef itk::Vector<PixelType, 2>                   DeformationValueType;
  typedef otb::Image<DeformationValueType, Dimension> DeformationFieldType;
  
  typedef otb::GenericRSResampleImageFilter<ImageType, 
    ImageType>                                        ImageResamplerType;
  typedef ImageResamplerType::OriginType              OriginType;
  typedef ImageResamplerType::SpacingType             SpacingType;

  
  // SmartPointer instanciation
  ImageResamplerType::Pointer resampler = ImageResamplerType::New();
  
  // Check if it's a unit test.
  if (argc > 1)
    {
    const char * infname = argv[1];
    const char * outfname = argv[6];
    unsigned int isize    = atoi(argv[2]);
    double iGridSpacing    = atof(argv[3]);
    int    useInRpc          = atoi(argv[4]);
    int    useOutRpc          = atoi(argv[5]);
    
    typedef otb::ImageFileReader<ImageType>              ReaderType;
    ReaderType::Pointer         reader    = ReaderType::New();

    // Read the input image
    reader->SetFileName(infname);
    reader->UpdateOutputInformation();
  
    // Fill the output size with the user selection
    SizeType      size;
    size.Fill(isize);

    // Set the origin & the spacing of the output
    OriginType  origin;
    origin[0] = 367340;
    origin[1] = 4.83467e+06;
  
    SpacingType  spacing;
    spacing[0] = 0.6;
    spacing[1] = -0.6;
  
    // Build the ouput projection ref : UTM ref
    OGRSpatialReference    oSRS;
    oSRS.SetProjCS("UTM");
    oSRS.SetUTM(31, true);
    char * utmRef = NULL;
    oSRS.exportToWkt(&utmRef);

    // Deformation Field spacing
    SpacingType  gridSpacing;
    gridSpacing[0] = iGridSpacing;
    gridSpacing[1] = -iGridSpacing;
  
    // Set the Resampler Parameters
    resampler->SetInput(reader->GetOutput());
    resampler->SetDeformationFieldSpacing(gridSpacing); 
    resampler->SetOutputOrigin(origin);
    resampler->SetOutputSize(size);
    resampler->SetOutputSpacing(spacing);
    resampler->SetOutputProjectionRef(utmRef);
    resampler->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
    resampler->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
    if (useInRpc)
      {
      resampler->SetInputRpcGridSize(20);
      resampler->EstimateInputRpcModelOn();
      }
    
    if (useOutRpc)
      {
      resampler->SetOutputRpcGridSize(20);
      resampler->EstimateOutputRpcModelOn();
      }
    
    // Write the resampled image
    typedef otb::StreamingImageFileWriter<ImageType>    WriterType;
    WriterType::Pointer writer= WriterType::New();
    writer->SetTilingStreamDivisions(4);
    writer->SetFileName(outfname);
    writer->SetInput(resampler->GetOutput());
    writer->Update();
    }
  else
    {
    std::cout <<"Unit Test " << std::endl;
    }
  
  return 0;
}
