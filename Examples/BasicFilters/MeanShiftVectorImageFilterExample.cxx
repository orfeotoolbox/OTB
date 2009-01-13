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
//  \cite{Comaniciu2002}.  // For a given pixel, the mean shift will
//  build a set of neighboring pixels within a given spatial radius
//  and a color range. The spatial and color center of this set is
//  then computed and the algorithm iterates with this new spatial and
//  color center. The Mean shift can be used for edge-preserving
//  smoothing, or for clustering.
//
//  Software Guide : EndLatex

#include "itkExceptionObject.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

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
   if(argc != 12)
    {
      std::cerr<<"Usage: "<<argv[0]<<" infname filteredfname clusteredfname labeledclusteredfname clusterboundariesfname filteredPretty clusteredPretty spatialRadius rangeRadius minregionsize scale"<<std::endl;
      return EXIT_FAILURE;
    }

  const char *       infname                = argv[1];
  const char *       filteredfname          = argv[2];
  const char *       clusteredfname         = argv[3];
  const char *       labeledclusteredfname  = argv[4];
  const char *       clusterboundariesfname = argv[5];
  const char *       filteredPretty         = argv[6];
  const char *       clusteredPretty        = argv[7];
  const unsigned int spatialRadius          = atoi(argv[8]);
  const double       rangeRadius            = atof(argv[9]);
  const unsigned int minRegionSize          = atoi(argv[10]);
  const double       scale                  = atoi(argv[11]);

  //  Software Guide : BeginLatex
//
//  We start by the classical \code{typedef}s needed for reading and
//  writing the images. The EDISON code upon which OTB's
//  implementation is based, uses float pixel precision, so we choose
//  to work with a float pixel type.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef float PixelType;
  typedef otb::VectorImage<PixelType,Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;
// Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
//
//  We can now define the type for the filter, and also the type for
//  the labeled output image, which is needed for the corresponding writer:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  
  typedef otb::MeanShiftVectorImageFilter<ImageType,ImageType> FilterType;
  typedef FilterType::LabeledOutputType LabeledImageType;
  typedef otb::StreamingImageFileWriter<LabeledImageType> LabeledWriterType;

  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
//
//  We instantiate the filter, the reader, and 4 writers (2 for the
//  filetered and clustered images and 2 more for the labeled and
//  boundaries images):
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer1 = WriterType::New();
  WriterType::Pointer writer2 = WriterType::New();
  LabeledWriterType::Pointer writer3 = LabeledWriterType::New();
  LabeledWriterType::Pointer writer4 = LabeledWriterType::New();

  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
//
//  We set the file names for the reader and the writers:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  
  reader->SetFileName(infname);
  writer1->SetFileName(filteredfname);
  writer2->SetFileName(clusteredfname);
  writer3->SetFileName(labeledclusteredfname);
  writer4->SetFileName(clusterboundariesfname);

  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
//
//  We can now set the parameters for the filter. There are 3 main
//  parameters: the spatial radius used for defining the neighborhood,
//  the range radius used for defining the interval in the color space
//  and the minimum size for the regions to be kept after clustering.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet    
  filter->SetSpatialRadius(spatialRadius);
  filter->SetRangeRadius(rangeRadius);
  filter->SetMinimumRegionSize(minRegionSize);
  // Software Guide : EndCodeSnippet
  //  Software Guide : BeginLatex
//
//  OTB's implementation adds another parameter -- the scale -- which
//  allows to stretch the image before processing. This is useful when
//  pixel values are in the \[0-1\] range.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet      
  filter->SetScale(scale);
    // Software Guide : EndCodeSnippet
  //  Software Guide : BeginLatex
//
//  We can now plug the pipeline and run it.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet        

  filter->SetInput(reader->GetOutput());
  writer1->SetInput(filter->GetOutput());
  writer2->SetInput(filter->GetClusteredOutput());
  writer3->SetInput(filter->GetLabeledClusteredOutput());
  writer4->SetInput(filter->GetClusterBoundariesOutput());


  writer1->Update();
  writer2->Update();
  writer3->Update();
  writer4->Update();
  // Software Guide : EndCodeSnippet

  typedef itk::RescaleIntensityImageFilter< ImageType,
                                   LabeledImageType> RescalerType;

  RescalerType::Pointer rescaler = Rescaler::New();
  rescaler->SetOutputMinimum( itk::NumericTraits<LabeledImageType>::Zero() );
  rescaler->SetOutputMaximum( itk::NumericTraits<LabeledImageType>::max() );

  witer3->SetFileName( filteredPretty );
  rescaler->SetInput( filter->GetOutput() );
  writer3->SetInput( rescaler->GetOutput() );
  writer3->Update();

  witer3->SetFileName( clusteredPretty );
  rescaler->SetInput( filter->GetClusteredOutput() );
  writer3->SetInput( rescaler->GetOutput() );
  writer3->Update();

  
  return EXIT_SUCCESS;
}

