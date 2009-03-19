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


// Software Guide : BeginLatex
//
// There is some vector data sets widely available on the internet. These data
// sets can be huge, covering an entire country, with hundreds of thousands
// objects.
//
// Most of the time, you won't be interested in the whole area and would like
// to focuss only on the area corresponding to your satellite image.
//
//
// This example demonstrates the use of the
// \doxygen{otb}{VectorDataExtractROI}.
//
// Software Guide : EndLatex

#include "otbVectorData.h"
#include "otbVectorDataExtractROI.h"
#include "otbCartographicRegion.h"

#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

#include "otbImage.h"
#include "otbImageFileReader.h"

int main( int argc, char* argv[] )
{
  if (argc < 4  )
  {
    std::cout << argv[0] <<" <input vector filename> <input image name> <output vector filename>  "  << std::endl;

    return EXIT_FAILURE;
  }

  const char * inVectorName = argv[1];
  const char * inImageName = argv[2];
  const char * outVectorName = argv[3];

  typedef double                                             Type;
  typedef otb::VectorData<>                                  VectorDataType;
  typedef otb::VectorDataExtractROI< VectorDataType >        FilterType;
  typedef otb::VectorDataFileReader<VectorDataType>          VectorDataFileReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>          VectorDataWriterType;

  typedef   otb::CartographicRegion<Type>                    TypedRegion;

  typedef otb::Image<unsigned char, 2>                       ImageType;
  typedef otb::ImageFileReader<ImageType>                    ImageReaderType;
  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(inImageName);
  imageReader->UpdateOutputInformation();


  FilterType::Pointer               filter = FilterType::New();
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
  VectorDataWriterType::Pointer     writer = VectorDataWriterType::New();

  TypedRegion region;
  TypedRegion::SizeType                                     size;
  TypedRegion::IndexType                                    index;

  size[0]  = imageReader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]
      * imageReader->GetOutput()->GetSpacing()[0];
  size[1]  = imageReader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]
      * imageReader->GetOutput()->GetSpacing()[1];
  index[0] = imageReader->GetOutput()->GetOrigin()[0]
      - 0.5 * imageReader->GetOutput()->GetSpacing()[0];
  index[1] = imageReader->GetOutput()->GetOrigin()[1]
      - 0.5 * imageReader->GetOutput()->GetSpacing()[1];
  region.SetSize(size);
  region.SetOrigin(index);

  otb::ImageMetadataInterface::Pointer imageMetadataInterface = otb::ImageMetadataInterface::New();
  region.SetRegionProjection(imageMetadataInterface->GetProjectionRef(imageReader->GetOutput()->GetMetaDataDictionary()));

  reader->SetFileName(inVectorName);

  filter->SetInput(reader->GetOutput());
  filter->SetRegion(region);

  writer->SetFileName(outVectorName);
  writer->SetInput(filter->GetOutput());
  writer->Update();


  return EXIT_SUCCESS;
}
