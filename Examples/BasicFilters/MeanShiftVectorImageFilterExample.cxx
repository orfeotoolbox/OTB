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

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {}
//    OUTPUTS: {MSFilteredOutput.png}, {MSClusteredOutput.png}, {MSLabelledOutput.png}, {MSBoundariesOutput.png}
// 5 50 20
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  This example demonstrates the use of the
//  \doxygen{otb}{MeanShiftVectorImageFilter} class which implements
//  filtering and clustering using the mean shift algorithm
//  \cite{Comaniciu2002}. 
//
//  Software Guide : EndLatex

#include "itkExceptionObject.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

//  Software Guide : BeginLatex
//
//  Two implementations for the filter exist, one for scalar images
//  (\doxygen{otb}{MeanShiftImageFilter}). And another one (the one
//  presented in this example) for vector images. They have the same
//  interface, so they can be used in the same way. We start by
//  including the needed header file.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMeanShiftVectorImageFilter.h"
// Software Guide : EndCodeSnippet
int main(int argc, char * argv[])
{
   if(argc != 10)
    {
      std::cerr<<"Usage: "<<argv[0]<<" infname filteredfname clusteredfname labeledclusteredfname clusterboundariesfname spatialRadius rangeRadius minregionsize scale"<<std::endl;
      return EXIT_FAILURE;
    }

  const char *       infname                = argv[1];
  const char *       filteredfname          = argv[2];
  const char *       clusteredfname         = argv[3];
  const char *       labeledclusteredfname  = argv[4];
  const char *       clusterboundariesfname = argv[5];
  const unsigned int spatialRadius          = atoi(argv[6]);
  const double       rangeRadius            = atof(argv[7]);
  const unsigned int minRegionSize          = atoi(argv[8]);
  const double       scale                  = atoi(argv[9]);

  const unsigned int Dimension = 2;
  typedef float PixelType;
  typedef otb::VectorImage<PixelType,Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;
  typedef otb::MeanShiftVectorImageFilter<ImageType,ImageType> FilterType;
  typedef FilterType::LabeledOutputType LabeledImageType;
  typedef otb::StreamingImageFileWriter<LabeledImageType> LabeledWriterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer1 = WriterType::New();
  WriterType::Pointer writer2 = WriterType::New();
  LabeledWriterType::Pointer writer3 = LabeledWriterType::New();
  LabeledWriterType::Pointer writer4 = LabeledWriterType::New();


  reader->SetFileName(infname);
  writer1->SetFileName(filteredfname);
  writer2->SetFileName(clusteredfname);
  writer3->SetFileName(labeledclusteredfname);
  writer4->SetFileName(clusterboundariesfname);

  filter->SetSpatialRadius(spatialRadius);
  filter->SetRangeRadius(rangeRadius);
  filter->SetMinimumRegionSize(minRegionSize);
  filter->SetScale(scale);

  filter->SetInput(reader->GetOutput());
  writer1->SetInput(filter->GetOutput());
  writer2->SetInput(filter->GetClusteredOutput());
  writer3->SetInput(filter->GetLabeledClusteredOutput());
  writer4->SetInput(filter->GetClusterBoundariesOutput());


  writer1->Update();
  writer2->Update();
  writer3->Update();
  writer4->Update();

  return EXIT_SUCCESS;
}


