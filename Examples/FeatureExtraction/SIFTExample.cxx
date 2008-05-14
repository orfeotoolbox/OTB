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
//    INPUTS: {ROISpot5.png}
//    OUTPUTS: {ROISpot5SIFT0.png}
//    "SIFT0.txt" 2 3 0 0
//  Software Guide : EndCommandLineArgs
//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROISpot5.png}
//    OUTPUTS: {ROISpot5SIFT1.png}
//    "SIFT1.txt" 2 3 1 0
//  Software Guide : EndCommandLineArgs
//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROISpot5.png}
//    OUTPUTS: {ROISpot5SIFT2.png}
//    "SIFT2.txt" 2 3 2 0
//  Software Guide : EndCommandLineArgs
//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {QB_SuburbSIFT5.png}
//    "SIFT2.txt" 2 3 5 0
//  Software Guide : EndCommandLineArgs
//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_SuburbRotated10.png}
//    OUTPUTS: {QB_SuburbSIFT5Rotated10.png}
//    "SIFT2.txt" 2 3 5 0
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{ImageToSIFTKeyPointSetFilter}.
// 
// The first step required to use this filter is to include its header file. 
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "otbImageToSIFTKeyPointSetFilter.h"
// Software Guide : EndCodeSnippet
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"

#include <iostream>
#include <fstream>


int main(int argc, char * argv[])
{
  if (argc != 8)
    {
    std::cerr << "Usage: " << argv[0] ;
    std::cerr << " InputImage OutputImage OutputSIFTFile octaves scales threshold ratio" << std::endl;
    return 1;
    }
  const char * infname = argv[1];
  const char * outfname = argv[3];
  const char * outputImageFilename = argv[2];

  const unsigned int octaves = atoi(argv[4]);
  const unsigned int scales = atoi(argv[5]);
  float threshold = atof(argv[6]);
  float ratio = atof(argv[7]);

  
  typedef float RealType;
  const unsigned int Dimension =2;

  typedef otb::Image<RealType,Dimension> ImageType;
  typedef itk::VariableLengthVector<RealType> RealVectorType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef itk::PointSet<RealVectorType,Dimension> PointSetType;
  typedef otb::ImageToSIFTKeyPointSetFilter<ImageType,PointSetType> ImageToSIFTKeyPointSetFilterType;

  // PointSet iterator types
  typedef PointSetType::PointsContainer PointsContainerType;
  typedef PointsContainerType::Iterator PointsIteratorType;
  typedef PointSetType::PointDataContainer PointDataContainerType;
  typedef PointDataContainerType::Iterator PointDataIteratorType;
  
  // Instantiating object
  ReaderType::Pointer reader = ReaderType::New();
  ImageToSIFTKeyPointSetFilterType::Pointer filter = ImageToSIFTKeyPointSetFilterType::New();
  
  reader->SetFileName(infname);

    
  filter->SetInput(0,reader->GetOutput());
  filter->SetOctavesNumber(octaves);
  filter->SetScalesNumber(scales);
  
  filter->SetDoGThreshold(threshold);
  filter->SetEdgeThreshold(ratio);

  filter->Update();

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:SIFT} shows the result of applying the SIFT
  // point detector to a small patch extracted from a Spot 5 image
  // using different threshold values.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.20\textwidth]{ROISpot5.eps}
  // \includegraphics[width=0.20\textwidth]{ROISpot5SIFT0.eps}
  // \includegraphics[width=0.20\textwidth]{ROISpot5SIFT1.eps}
  // \includegraphics[width=0.20\textwidth]{ROISpot5SIFT2.eps}
  // \itkcaption[SIFT Application]{Result of applying the
  // \doxygen{otb}{ImageToSIFTKeyPointSetFilter} to a Spot 5
  // image. Left to right: original image and SIFT with thresholds 0,
  // 1 and 2 respectively.} 
  // \label{fig:SIFT}
  // \end{figure}
    // Figure~\ref{fig:SIFT} shows the result of applying the SIFT
  // point detector to a small patch extracted from a Spot 5 image
  // using different threshold values.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.25\textwidth]{QB_SuburbSIFT5.eps}
  // \includegraphics[width=0.25\textwidth]{QB_SuburbSIFT5Rotated10.eps}
  // \itkcaption[SIFT Application]{Result of applying the
  // \doxygen{otb}{ImageToSIFTKeyPointSetFilter} to a high resolution image
  // image. Left to right: original image and SIFT on the original
  // anda rotated image respectively.} 
  // \label{fig:SIFT}
  // \end{figure}
  //  Software Guide : EndLatex
  
  //
  //Building the output image for visualization
  ImageType::OffsetType t = {{ 0, 1}};
  ImageType::OffsetType b = {{ 0,-1}};
  ImageType::OffsetType r = {{ 1, 0}};
  ImageType::OffsetType l = {{-1, 0}};
  
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Image<RGBPixelType, 2> OutputImageType;

  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  
  OutputImageType::Pointer outputImage = OutputImageType::New();
  outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  outputImage->Allocate();

  itk::ImageRegionIterator<OutputImageType> iterOutput(outputImage,
						       reader->GetOutput()->GetLargestPossibleRegion());

  for (iterOutput.GoToBegin(); !iterOutput.IsAtEnd(); ++iterOutput)
    {
      ImageType::IndexType index = iterOutput.GetIndex();
      ImageType::PixelType grayPix = reader->GetOutput()->GetPixel(index);
      OutputImageType::PixelType rgbPixel;
      rgbPixel.SetRed( static_cast<unsigned char>(grayPix) );
      rgbPixel.SetGreen( static_cast<unsigned char>(grayPix) );
      rgbPixel.SetBlue( static_cast<unsigned char>(grayPix) );
      
      iterOutput.Set(rgbPixel);
    }
  
  PointsIteratorType pIt = filter->GetOutput()->GetPoints()->Begin();
  ImageType::SpacingType spacing = reader->GetOutput()->GetSpacing();
  ImageType::PointType origin = reader->GetOutput()->GetOrigin();
  OutputImageType::SizeType size = outputImage->GetLargestPossibleRegion().GetSize();


  while( pIt!=filter->GetOutput()->GetPoints()->End() )
    {
      ImageType::IndexType index;

      index[0] = (unsigned int)
	(vcl_floor
	 ((double)((pIt.Value()[0]-origin[0])/spacing[0]+0.5)));
      
      index[1] = (unsigned int)
	(vcl_floor
	 ((double)((pIt.Value()[1]-origin[1])/spacing[1]+0.5)));
      
      OutputImageType::PixelType keyPixel;
      keyPixel.SetRed(0);
      keyPixel.SetGreen(255);
      keyPixel.SetBlue(0);

      if (
	static_cast<unsigned int>(index[1]) <
	static_cast<unsigned int>(size[1])
	&& static_cast<unsigned int>(index[0]) <
	static_cast<unsigned int>(size[0])
	&& static_cast<unsigned int>(index[1]) >=
	static_cast<unsigned int>(0)
	&& static_cast<unsigned int>(index[0]) >=
	static_cast<unsigned int>(0))
	{
	outputImage->SetPixel(index,keyPixel);
      
       if (static_cast<unsigned int>(index[1]) < static_cast<unsigned int>(size[1]-1) )
 	outputImage->SetPixel(index+t,keyPixel);
       if (index[1] > 0)
 	outputImage->SetPixel(index+b,keyPixel);


        if (static_cast<unsigned int>(index[0]) < static_cast<unsigned int>(size[0]-1) )
	 
 	 outputImage->SetPixel(index+r,keyPixel);
	 
        if (index[0] > 0)
  	outputImage->SetPixel(index+l,keyPixel);
      }
       ++pIt;
    }

  std::ofstream outfile(outfname);
  outfile << filter;
  outfile.close();

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(outputImage);
  writer->SetFileName(outputImageFilename);
  writer->Update();
  
  return EXIT_SUCCESS;
}
