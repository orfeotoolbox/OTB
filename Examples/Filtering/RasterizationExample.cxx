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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {w002n44e.shp}
//    OUTPUTS: {ArcachonRasterizedCoastline.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// The \doxygen{otb}{VectorDataToMapFilter} allows performing
// rasterization of a given vector data as a binary mask. This example
// will demonstrate how to use this filter to perform rasterization of
// the SRTM water body masks available here:
// \url{http://dds.cr.usgs.gov/srtm/version2_1/SWBD/}.
//
// First step to use this filter is to include the appropriate headers:
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbVectorData.h"
#include "otbImage.h"
#include "otbVectorDataToMapFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImageFileWriter.h"
#include "otbVectorDataProjectionFilter.h"
#include "itkRGBAPixel.h"
#include "itkChangeLabelImageFilter.h"
#include "otbVectorDataFileReader.h"

int main(int argc, char * argv[])
{
  // Software Guide : BeginLatex
  //
  // Then, we need to define the appropriate VectorData and Image
  // type.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef unsigned char                 PixelType;
  typedef otb::Image<PixelType, 2>      ImageType;
  typedef otb::VectorData<>             VectorDataType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Using these typedefs, we can define and instantiate the
  // \doxygen{otb}{VectorDataToMapFilter}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorDataToMapFilter<VectorDataType,
    ImageType>            VectorDataToMapFilterType;
  VectorDataToMapFilterType::Pointer vectorDataRendering
    = VectorDataToMapFilterType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We will also define a \doxygen{otb}{VectorDataFileReader} to read
  // the VectorData, as well as a
  // \doxygen{otb}{VectorDataProjectionFilter} to reproject our data
  // in a given map projection.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
  reader->SetFileName(argv[1]);

  typedef otb::VectorDataProjectionFilter<VectorDataType,
    VectorDataType> ProjectionFilterType;
  ProjectionFilterType::Pointer projection = ProjectionFilterType::New();
  projection->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet

  std::string projectionRefWkt = "PROJCS[\"WGS 84 / UTM zone 30N\", GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563, AUTHORITY[\"EPSG\",\"7030\"]], AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]], UNIT[\"degree\", 0.01745329251994328, AUTHORITY[\"EPSG\",\"9122\"]], AUTHORITY[\"EPSG\",\"4326\"]], UNIT[\"metre\", 1, AUTHORITY[\"EPSG\",\"9001\"]], PROJECTION[\"Transverse_Mercator\"], PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", -3], PARAMETER[\"scale_factor\", 0.9996], PARAMETER[\"false_easting\", 500000], PARAMETER[\"false_northing\", 0], AUTHORITY[\"EPSG\",\"32630\"], AXIS[\"Easting\", EAST], AXIS[\"Northing\", NORTH]]";

  // Software Guide : BeginLatex
  //
  // Next step is to specify the map projection in which to reproject
  // our vector.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  projection->SetOutputProjectionRef(projectionRefWkt);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Since the input vector can be pretty big, we will perform an
  // extract of the region of interest using the
  // \doxygen{otb}{VectorDataExtractROI}.
  //
  // The first step is to define the region of interest.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::RemoteSensingRegion<double> RegionType;
  ImageType::SizeType size;
  size[0] = 500;
  size[1] = 500;

  ImageType::PointType origin;
  origin[0] = 633602; //UL easting
  origin[1] = 4961679; //UL northing

  ImageType::SpacingType spacing;
  spacing[0] = 56;
  spacing[1] = -56;

  RegionType region;
  RegionType::SizeType sizeInUnit;
  sizeInUnit[0] = size[0] * spacing[0];
  sizeInUnit[1] = size[1] * spacing[1];
  region.SetSize(sizeInUnit);
  region.SetOrigin(origin);
  region.SetRegionProjection(projectionRefWkt);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Then, we define and set-up the
  // \doxygen{otb}{VectorDataExtractROI} filter using the region.
  //
  // Software Guide : EndLatex

  // Software Guide :: BeginCodeSnippet

  typedef otb::VectorDataExtractROI<VectorDataType> ExtractROIType;
  ExtractROIType::Pointer extractROI = ExtractROIType::New();
  extractROI->SetRegion(region);
  extractROI->SetInput(projection->GetOutput());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now, we can plug the the ROI filter to the
  // \doxygen{otb}{VectorDataToMapFilter}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  vectorDataRendering->SetInput(extractROI->GetOutput());
  vectorDataRendering->SetSize(size);
  vectorDataRendering->SetOrigin(origin);
  vectorDataRendering->SetSpacing(spacing);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Since we are interested in binary rendering, we need to set the
  // appropriate rendering style.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  vectorDataRendering->SetRenderingStyleType(VectorDataToMapFilterType::Binary);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The rendering filter will return a binary image with label 0 when
  // outside the rasterized vector features and 255 when inside. To
  // get a fancier rendering we will substitute a blue color to the
  // foreground value and green to the background value.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::RGBAPixel<unsigned char> RGBAPixelType;
  typedef otb::Image<RGBAPixelType, 2>   RGBAImageType;
  typedef itk::ChangeLabelImageFilter<ImageType,
    RGBAImageType> ChangeLabelImageFilterType;

  ChangeLabelImageFilterType::Pointer
    changeLabelFilter = ChangeLabelImageFilterType::New();

  RGBAPixelType green, blue;
  green.SetAlpha(255);
  green.SetGreen(255);
  blue.SetAlpha(255);
  blue.SetBlue(255);

  changeLabelFilter->SetChange(0, blue);
  changeLabelFilter->SetChange(255, green);
  changeLabelFilter->SetInput(vectorDataRendering->GetOutput());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Last step is to write the image to the disk using a
  // \doxygen{otb}{ImageFileWriter}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<RGBAImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(changeLabelFilter->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.4\textwidth]{ArcachonRasterizedCoastline.eps}
  // \itkcaption[Rasterized SRTM water bodies near Arcachon, France.]{Rasterized SRTM water bodies near Arcachon, France.}
  // \label{fig:RasterizationFilterOutput}
  // \end{figure}
  //
  //  Figure \ref{fig:RasterizationFilterOutput} illustrates the use
  //  of the rasterization filter on SRTM water bodies mask near
  //  Arcachon in France. Ocean appears in blue while land appears in green.
  //
  //  Software Guide : EndLatex


  return EXIT_SUCCESS;
}
