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


// Software Guide : BeginLatex
//
// There is some vector data sets widely available on the internet. These data
// sets can be huge, covering an entire country, with hundreds of thousands
// objects.
//
// Most of the time, you won't be interested in the whole area and would like
// to focuss only on the area corresponding to your satellite image.
//
// The \doxygen{otb}{VectorDataExtractROI} is able to extract the area corresponding
// to your satellite image, even if the image is still in sensor geometry (provided
// the sensor model is supported by OTB). Let's see how we can do that.
//
// This example demonstrates the use of the
// \doxygen{otb}{VectorDataExtractROI}.
//
// Software Guide : EndLatex

#include "otbVectorData.h"
#include "otbVectorDataExtractROI.h"

#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbImageMetadataInterfaceFactory.h"

#include "otbImage.h"
#include "otbImageFileReader.h"

int main(int argc, char* argv[])
{
  if (argc < 4)
    {
    std::cout << argv[0] <<
    " <input vector filename> <input image name> <output vector filename>  "
              <<
    std::endl;

    return EXIT_FAILURE;
    }

  const char * inVectorName = argv[1];
  const char * inImageName = argv[2];
  const char * outVectorName = argv[3];

  typedef double            Type;
  typedef otb::VectorData<> VectorDataType;

  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType> VectorDataWriterType;

  typedef   otb::RemoteSensingRegion<Type> TypedRegion;

  typedef otb::Image<unsigned char, 2>    ImageType;
  typedef otb::ImageFileReader<ImageType> ImageReaderType;
  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(inImageName);
  imageReader->UpdateOutputInformation();

  // Software Guide : BeginLatex
  //
  // After the usual declaration (you can check the source file for the details),
  // we can declare the \doxygen{otb}{VectorDataExtractROI}:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorDataExtractROI<VectorDataType> FilterType;
  FilterType::Pointer filter = FilterType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Then, we need to specify the region to extract. This region is a bit special as
  // it contains also information related to its reference system (cartographic projection
  // or sensor model projection). We retrieve all these information from the image
  // we gave as an input.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  TypedRegion            region;
  TypedRegion::SizeType  size;
  TypedRegion::IndexType index;

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

  otb::ImageMetadataInterfaceBase::Pointer imageMetadataInterface
    = otb::ImageMetadataInterfaceFactory::CreateIMI(
    imageReader->GetOutput()->GetMetaDataDictionary());
  region.SetRegionProjection(
    imageMetadataInterface->GetProjectionRef());

  region.SetKeywordList(imageReader->GetOutput()->GetImageKeywordlist());

  filter->SetRegion(region);
  // Software Guide : EndCodeSnippet

  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
  VectorDataWriterType::Pointer     writer = VectorDataWriterType::New();
  reader->SetFileName(inVectorName);
  writer->SetFileName(outVectorName);

  // Software Guide : BeginLatex
  //
  // And finally, we can plug the filter in the pipeline:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  // Software Guide : EndCodeSnippet

  writer->Update();

  return EXIT_SUCCESS;
}
