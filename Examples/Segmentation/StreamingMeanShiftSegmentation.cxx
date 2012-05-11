/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


// Software Guide : BeginLatex
//
// The following example illustrates how to segment images
// using the \doxygen{otb}{StreamingVectorizedSegmentation}.
// The \doxygen{otb}{MeanShiftImageFilter} is used to segment each tile of the image.
// The labeled output image of each tile is then polygonized and stored into a \doxygen{otb}{VectorData}
// The method used for polygonize is GDALPolygonize.
//
// First we include the segmentation filter (Here it is the MeanShiftFilter) and
// the StreamingVectorizedSegmentation header.
// Software Guide : EndLatex
#include <iostream>

// Software Guide : BeginCodeSnippet
#include "otbMeanShiftImageFilter.h"
#include "otbStreamingVectorizedSegmentationOGR.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorData.h"

#include "otbOGRDataSourceWrapper.h"

int main(int argc, char *argv[])
{
  if (argc < 3)
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr <<
    " inputImage outputVectorData nbLinePerStream "
              << std::endl;
    return 1;
    }

  // Software Guide : BeginLatex
  //
  // We now declare the image and pixel types to use as input of the MeanShiftFilter.
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef float InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType,  Dimension>          ImageType;
  typedef otb::VectorData<double, 2>                      VectorDataType;
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  //
  // Next we declare the output VectorData type that will contain all the polygons.
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef otb::VectorData<double, 2>                      VectorDataType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Then the mean shift segmentation filter is declared using the Image type declared previsouly.
  // The StreamingVectorizedSegmentation is templated over the mean shift filter, the input image type and the output vector data type
  // Software Guide : EndLatex
  
  // Typedefs
  // Software Guide : BeginCodeSnippet
  typedef otb::MeanShiftImageFilter<ImageType, ImageType> MeanShiftImageFilterType;
  typedef otb::StreamingVectorizedSegmentationOGR<ImageType, MeanShiftImageFilterType> StreamingVectorizedSegmentationType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Finaly we define a Reader on the input image and a writer for the VectorData.
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ImageType>                      ReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>            WriterType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now we have declared all type needed for the pipeline, we instantiate the different filters,
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer             reader = ReaderType::New();
  StreamingVectorizedSegmentationType::Pointer filter = StreamingVectorizedSegmentationType::New();
  //WriterType::Pointer             writer = WriterType::New();
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  //
  // And we connect the pipeline and set parameters for the mean shift filter.
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  reader->SetFileName(argv[1]);
  reader->GenerateOutputInformation();
  filter->SetInput(reader->GetOutput());

  otb::ogr::DataSource::Pointer ogrDS = otb::ogr::DataSource::New(argv[2], otb::ogr::DataSource::Modes::write);

  filter->SetOGRDataSource(ogrDS);


  //filter->GetStreamer()->SetNumberOfLinesStrippedStreaming(atoi(argv[3]));
  filter->GetStreamer()->SetAutomaticTiledStreaming();
  
  const std::string fieldName("DN");
  filter->SetFieldName(fieldName);
  filter->SetLayerName("Layer");
  filter->SetStartLabel(1);
  filter->SetUse8Connected(false);
  
  filter->GetSegmentationFilter()->SetSpatialRadius(10);
  filter->GetSegmentationFilter()->SetRangeRadius(15);
  filter->GetSegmentationFilter()->SetMinimumRegionSize(400);
  
  filter->Initialize();
  
  filter->Update();


  //writer->SetFileName(argv[2]);
  //writer->SetInput(filter->GetOutputVectorData());
  //writer->Update();
  // Software Guide : EndCodeSnippet


  return EXIT_SUCCESS;
}
