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

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {osmfile.otb}
//    OUTPUTS: {openStreetMap-Toulouse.png}
//    ${GeneratedFolder} 1.4835345 43.55968261 13
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {osmfile.otb}
//    OUTPUTS: {openStreetMap-Singapore.png}
//    ${GeneratedFolder} 103.780646 1.297643 13
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// First, we need to include several headers. There will be a bit of
// manual work going on here.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkRGBPixel.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbTileMapImageIO.h"
#include "otbInverseSensorModel.h"
#include "otbExtractROI.h"
#include "otbImageFileWriter.h"
#include "ossim/projection/ossimTileMapModel.h"
// Software Guide : EndCodeSnippet

int main( int argc, char* argv[] )
{

  if(argc!=7)
  {
    std::cout << argv[0] <<" <inputFilename> <outputFilename> "
        << "<cacheDirectory> <lon> <lat> <depth>"
        << std::endl;

    return EXIT_FAILURE;
  }


  // Software Guide : BeginLatex
  //
  // We retrieve the input parameters:
  // \begin{itemize}
  // \item the input filename is a simple text file specifying the access
  // modality to open street map data;
  // \item the output file is the image where you want to save the result;
  // \item the cache directory is necessary to keep the data retrieved from
  // the internet. It can also be reused to minimize network access;
  // \item longitude of the center of the scene;
  // \item latitude of the center of the scene;
  // \item depth which is inversely related to the resolution: when you increase
  // the depth by one, you divide the resolution by two.
  // \end{itemize}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  char * inputFilename = argv[1];
  char * outputFilename = argv[2];
  char * cacheDirectory = argv[3];
  double lon = atof(argv[4]);
  double lat = atof(argv[5]);
  int depth = atoi(argv[6]);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We now instantiate the reader. As some parameters need to be given to the
  // IO which is an \doxygen{otb}{TileMapImageIO}, we need to manually create it:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Image<RGBPixelType, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::TileMapImageIO ImageIOType;

  ImageIOType::Pointer tileIO = ImageIOType::New();
  ReaderType::Pointer readerTile = ReaderType::New();
  tileIO->SetDepth(depth);
  tileIO->SetCacheDirectory(cacheDirectory);
  readerTile->SetImageIO(tileIO);
  readerTile->SetFileName(inputFilename);
  readerTile->UpdateOutputInformation();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now, we potentially have an image of several Peta-Bytes covering the whole world
  // in the reader
  // that's why we don't want to do an update before extracting a specific
  // area.
  //
  // The coordinates are refered with an origin at the North Pole and the
  // change date meridian in Mercator projection. So we need to translate the latitude
  // and the longitude in this funny coordinate system:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::InverseSensorModel<double>  ModelType;
  ModelType::Pointer   model= ModelType::New();

  model->SetImageGeometry(readerTile->GetOutput()->GetImageKeywordlist());
  dynamic_cast<ossimTileMapModel*>(model->GetOssimModel())->setDepth(depth);
  if(!model)
  {
    std::cerr << "Unable to create a model" << std::endl;
    return 1;
  }


  typedef itk::Point <double, 2> PointType;
  PointType lonLatPoint;
  lonLatPoint[0]=lon;
  lonLatPoint[1]=lat;

  PointType tilePoint;
  tilePoint=model->TransformPoint(lonLatPoint);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // This enables us to use the \doxygen{otb}{ExtractROI} to retrieve only
  // the area of interest and to avoid crashing our memory-limited computer.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  long int startX=static_cast<long int>(tilePoint[0]);
  long int startY=static_cast<long int>(tilePoint[1]);
  long int sizeX = 500;
  long int sizeY = 500;

  std::cerr << startX <<", "<< startY << std::endl;
  std::cerr << sizeX <<", "<< sizeY << std::endl;

  typedef otb::ExtractROI< RGBPixelType,  RGBPixelType >  ExtractROIFilterType;
  ExtractROIFilterType::Pointer extractROIOsmFilter = ExtractROIFilterType::New();
  extractROIOsmFilter->SetStartX(startX-sizeX/2);
  extractROIOsmFilter->SetStartY(startY-sizeY/2);
  extractROIOsmFilter->SetSizeX( sizeX );
  extractROIOsmFilter->SetSizeY( sizeY );

  extractROIOsmFilter->SetInput(readerTile->GetOutput());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Finally, we just plug this to the writer to save our nice map of
  // the area:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(extractROIOsmFilter->GetOutput());
  writer->Update();
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:TILEMAPIMAGEIOEXAMPLE} shows the output images created from
  // open street map data.
  //
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.45\textwidth]{openStreetMap-Toulouse.eps}
  // \includegraphics[width=0.45\textwidth]{openStreetMap-Singapore.eps}
  // \itkcaption[Open street map]{Map created from open street map showing the OTB headquaters}
  // \label{fig:TILEMAPIMAGEIOEXAMPLE}
  // \end{figure}
  //
  // If your street is missing, go and improve the map.
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;

}
