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
// The following example illustrates how to segment very large images
// using the \doxygen{otb}{StreamingVectorizedSegmentation}. This filter is
// templated over the segmentation filter that will be used to segment each tile
// of the input image. In this example we will use the \doxygen{otb}{MeanShiftVectorImageFilter}.
// The labeled output image of each tile is then vectorized (using a filter based on GDALPolygonize)
// and stored into a \doxygen{otb}{ogr}{Layer} within the \doxygen{otb}{ogr}{DataSource}
// set as input. Finally a fusion filter, \doxygen{otb}{OGRDataSourceStreamStitchingFilter}, is used to merge polygons
// at tile border.
//
// Let's take a look at the code.
// First we include all the needed headers
// Software Guide : EndLatex
#include <iostream>

// Software Guide : BeginCodeSnippet
#include "otbStreamingImageToOGRDataSourceSegmentationFilter.h"
#include "otbMeanShiftVectorImageFilter.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbOGRDataSourceStreamStitchingFilter.h"
// Software Guide : EndCodeSnippet

#include "otbVectorImage.h"
#include "otbImageFileReader.h"


int main(int argc, char *argv[])
{
  if (argc != 13)
    {
      std::cerr << "Usage: " << argv[0];
      std::cerr << " inputImage maskImage outputVec layerName TileDimension"
               << "spatialRadius rangeRadius minObjectSize filterSmallObj minSize"
               << "SimplifyFlag Tolerance" << std::endl;
      return EXIT_FAILURE;
    }

  const char * imageName                    = argv[1];
  const char * maskName                     = argv[2];
  const char * dataSourceName               = argv[3];
  const char * layerName                    = argv[4];
  const unsigned int tileSize               = atoi(argv[5]);
  const unsigned int spatialRadius          = atoi(argv[6]);
  const double rangeRadius                  = atof(argv[7]);
  const unsigned int minimumObjectSize      = atoi(argv[8]);
  const bool filterSmallObj                 = atoi(argv[9]);
  const unsigned int minSize                = atoi(argv[10]);
  const bool simplify                       = atoi(argv[11]);
  const double tolerance                    = atof(argv[12]);
  const std::string fieldName("DN");

  // Software Guide : BeginLatex
  //
  // We now declare the image and pixel types used as input of the MeanShiftFilter.
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef float InputPixelType;
  typedef unsigned int LabelPixelType;

  const unsigned int Dimension = 2;
  typedef otb::VectorImage<InputPixelType,  Dimension>          ImageType;
  typedef otb::Image<LabelPixelType,  Dimension>          LabelImageType;
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  //
  // Then the mean shift segmentation filter is declared using the Image type declared previsouly.
  // The StreamingVectorizedSegmentation is templated over the mean shift filter, the input image type and the output vector data type
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  //typedef otb::MeanShiftSmoothingImageFilter<ImageType, ImageType> MeanShiftImageFilterType;
  typedef otb::MeanShiftVectorImageFilter <ImageType, ImageType, LabelImageType>  SegmentationFilterType;
  typedef otb::StreamingImageToOGRDataSourceSegmentationFilter<ImageType, SegmentationFilterType> StreamingVectorizedSegmentationType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Finaly we define a Reader on the input image and a mask reader.
  // All pixels in the mask with a value of 0 will not be considered suitable for vectorization.
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ImageType>           ReaderType;
  typedef otb::ImageFileReader<LabelImageType>      MaskReaderType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now we have declared all type needed for the pipeline, we instantiate the different filters,
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer        reader = ReaderType::New();
  MaskReaderType::Pointer    maskReader = MaskReaderType::New();
  StreamingVectorizedSegmentationType::Pointer filter = StreamingVectorizedSegmentationType::New();
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  //
  // The instanciation of the DataSource is slightly different as usual.
  // In fact the \code{New()} method on a \doxygen{otb}{ogr}{DataSource} can be called with or without parameters.
  // Without parameters, the \code{New()} method instanciate a "Memory" DataSource, which means all the data are stored in memory.
  // This is not useful in case of large scale segmentation as it will result in millions of polygons kept in memory ...
  // However the \code{New()} method can also take a filename (\code{std::String}) parameter. Then either the file already exists
  // and the corresponding ogr driver is used to open the file, or it doesn't exists and then it is created.
  // Here we used a non existing filename to create a new file in writing mode.
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  otb::ogr::DataSource::Pointer ogrDS = otb::ogr::DataSource::New(dataSourceName, otb::ogr::DataSource::Modes::Update_LayerCreateOnly);
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  //
  // Now we set the parameters to the segmentation filter.The \doxygen{otb}{MeanShiftVectorImageFilter}
  // required three parameters, the spatial radius, the range radius and the minimum object size.
  // We use the \code{GetSegmentationFilter()} method on the \doxygen{otb}{StreamingVectorizedSegmentation}
  // to get a pointer to the segmentation filter.
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  filter->GetSegmentationFilter()->SetSpatialRadius(spatialRadius);
  filter->GetSegmentationFilter()->SetRangeRadius(rangeRadius);
  filter->GetSegmentationFilter()->SetMinimumRegionSize(minimumObjectSize);
  // Software Guide : EndCodeSnippet
  
  
  // Software Guide : BeginLatex
  //
  // Then we set parameters to the \doxygen{otb}{StreamingVectorizedSegmentation} filter.
  // These parameters are :
  // \begin{itemize}
  //  \item tile size : use \code{SetTileDimensionTiledStreaming()} for square tile or \code{SetNumberOfLinesStrippedStreaming()}
  // for Strip.
  // \item layer name : name of the layer that will be created in the input \doxygen{otb}{ogr}{DataSource}.
  // \item field name : name of the field that will contained the label values. (default is "DN").
  // \item start label : first label. Each polygons have a unique label (incremented by one).
  // \item option to filter small polygons (default to false).
  // \item minimum object size : in case filter small polygons option is True
  // \item simplify option : simplification of polygon vertex (default to false).This can reduced very efficiently the size
  // of the output file with no real impact on the results.
  // \item simplification tolerance
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  filter->GetStreamer()->SetTileDimensionTiledStreaming(tileSize);
  filter->SetLayerName(layerName);
  filter->SetFieldName(fieldName);
  filter->SetStartLabel(1);
  filter->SetFilterSmallObject(filterSmallObj);
  filter->SetMinimumObjectSize(minSize);
  filter->SetSimplify(simplify);
  filter->SetSimplificationTolerance(tolerance);
  // Software Guide : EndCodeSnippet
  
  
  // Software Guide : BeginLatex
  //
  // Finally we connect the pipeline
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  reader->SetFileName(imageName);
  reader->UpdateOutputInformation();
  
  maskReader->SetFileName(maskName);
  maskReader->UpdateOutputInformation();
  
  filter->SetInput(reader->GetOutput());
  filter->SetInputMask(maskReader->GetOutput());
  filter->SetOGRDataSource(ogrDS);
  // Software Guide : EndCodeSnippet
  
  
  // Software Guide : BeginLatex
  //
  // And call the \code{Initialize()} method (needed to create the output layer in the datasource)
  // before calling the \code{Update()} method.
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  filter->Initialize();
  filter->Update();
  // Software Guide : EndCodeSnippet
  
  
  // Software Guide : BeginLatex
  //
  // The segmentation is done, but as it works tile by tile, we need to fusion polygons at tile border.
  // We use the \doxygen{otb}{OGRDataSourceStreamStitchingFilter}. This filter uses a simple fusion strategy.
  // Polygons that have the largest intersection over a tile are fusioned. Each polygon can be fusioned
  // only once per tile border (row and column).
  // Let's look at the code for fusioning.
  // As usual we declared and instanciate the \doxygen{otb}{OGRDataSourceStreamStitchingFilter}.
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  typedef otb::OGRDataSourceStreamStitchingFilter<ImageType>   FusionFilterType;
  FusionFilterType::Pointer fusionFilter = FusionFilterType::New();
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  // Next we set the input image and the input \doxygen{otb}{ogr}{DataSource}.
  // The image is internally used in the filter to compute coordinates of streaming tiles.
  // The DataSource is the one containing the segmentation results.
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  fusionFilter->SetInput(reader->GetOutput());
  fusionFilter->SetOGRDataSource(ogrDS);
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  // We set the name of the layer containing segmentation results which is the same that we used
  // for the \doxygen{otb}{StreamingVectorizedSegmentation} filter. We also set the size of the
  // tile used, which may be different from the one we set in the \doxygen{otb}{StreamingVectorizedSegmentation} filter
  // but can be retrieved using the \code{GetStreamSize()} method.
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  fusionFilter->SetStreamSize(filter->GetStreamSize());
  fusionFilter->SetLayerName(layerName);
  // Software Guide : EndCodeSnippet
  
  // Software Guide : BeginLatex
  // Finally we call the \code{GenerateData()} method to launch the processing.
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  fusionFilter->GenerateData();
  // Software Guide : EndCodeSnippet


  return EXIT_SUCCESS;
}
