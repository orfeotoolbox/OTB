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


// Software Guide : BeginLatex
//
// This example illustrates the details of the \doxygen{otb}{RoadExtractionFilter}. 
// This filter, described in the previous section,  is a composite filter that includes 
// all the steps below. Individual filters can be replaced to design a road detector
// targeted at SAR images for example.
//
// Software Guide : EndLatex 

#include "otbPolyLineParametricPathWithValue.h"
#include "otbSpectralAngleDistanceImageFilter.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "otbNeighborhoodScalarProductFilter.h"
#include "otbRemoveIsolatedByDirectionFilter.h"
#include "otbRemoveWrongDirectionFilter.h"
#include "otbNonMaxRemovalByDirectionFilter.h"
#include "otbVectorizationPathListFilter.h"
#include "otbSimplifyPathListFilter.h"
#include "otbBreakAngularPathListFilter.h"
#include "otbRemoveTortuousPathListFilter.h"
#include "otbLinkPathListFilter.h"
#include "otbLikehoodPathListFilter.h"
#include "otbDrawPathListFilter.h"
#include "otbLikehoodPathListFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkSqrtImageFilter.h"

#include "itkCovariantVector.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbVectorRescaleIntensityImageFilter.h"


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {qb_RoadExtract.tif}
//    OUTPUTS: {ExtractRoadByStepsExampleOutput.png}, {qb_ExtractRoad_pretty.png}
//    337 557 432 859 0.00005 1.0
//  Software Guide : EndCommandLineArgs

int main( int argc, char * argv[] )
{
  
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef itk::CovariantVector<PixelType,Dimension> VectorPixelType;
  typedef otb::Image<PixelType, Dimension> InternalImageType;
  typedef otb::VectorImage<PixelType, Dimension> MultiSpectralImageType;
  typedef otb::Image<unsigned char, Dimension> OutputImageType;
  typedef otb::Image<VectorPixelType, Dimension > VectorImageType;

  typedef otb::PolyLineParametricPathWithValue< double, Dimension >       PathType;
  
  typedef otb::ImageFileWriter<OutputImageType> FileWriterType;
  typedef otb::ImageFileReader< MultiSpectralImageType > MultispectralReaderType;

  MultispectralReaderType::Pointer multispectralReader =  MultispectralReaderType::New();
  multispectralReader->SetFileName(argv[1]);

  // Create an 3 band image for the software guide 
  typedef otb::VectorImage<unsigned char, Dimension> OutputVectorImageType;
  typedef otb::ImageFileWriter<OutputVectorImageType> VectorWriterType;
  typedef otb::VectorRescaleIntensityImageFilter<MultiSpectralImageType,OutputVectorImageType> VectorRescalerType;
  typedef otb::MultiChannelExtractROI<unsigned char,unsigned char> ChannelExtractorType;
    
  multispectralReader->GenerateOutputInformation();
  
  OutputVectorImageType::PixelType minimum,maximum;
  minimum.SetSize(multispectralReader->GetOutput()->GetNumberOfComponentsPerPixel());
  maximum.SetSize(multispectralReader->GetOutput()->GetNumberOfComponentsPerPixel());
  minimum.Fill(0);
  maximum.Fill(255);
  
  VectorRescalerType::Pointer vr = VectorRescalerType::New();
  vr->SetInput(multispectralReader->GetOutput());
  vr->SetOutputMinimum(minimum);
  vr->SetOutputMaximum(maximum);
  vr->SetClampThreshold(0.01);
  
  ChannelExtractorType::Pointer selecter = ChannelExtractorType::New();
  selecter->SetInput(vr->GetOutput());
  selecter->SetExtractionRegion(multispectralReader->GetOutput()->GetLargestPossibleRegion());
  selecter->SetChannel(1);
  selecter->SetChannel(2);
  selecter->SetChannel(3);
  
  VectorWriterType::Pointer vectWriter = VectorWriterType::New();
  vectWriter->SetFileName(argv[3]);
  vectWriter->SetInput(selecter->GetOutput());
  vectWriter->Update();

  MultiSpectralImageType::PixelType pixelRef;
  pixelRef.SetSize(4);
  pixelRef[0]=atoi(argv[4]);
  pixelRef[1]=atoi(argv[5]);
  pixelRef[2]=atoi(argv[6]); 
  pixelRef[3]=atoi(argv[7]);
  
  double resolution = 0.6; //to get directly from metadata
  double alpha = atof(argv[9]);
  
  //  Software Guide : BeginLatex
  //
  //  The spectral angle is used to compute a grayscale image from the 
  //  multispectral original image using \doxygen{otb::SpectralAngleDistanceImageFilter}. The spectral angle is illustrated on
  // Figure~\ref{fig:RoadExtractionSpectralAngleDiagram}. Pixels corresponding to roads are in 
  //  darker color.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.40\textwidth]{RoadExtractionSpectralAngleDiagram.eps}
  // \itkcaption[Spectral Angle]{Illustration of the spectral angle for one pixel of a three-band image. One of the vector is the reference pixel and the other is the current pixel.}
  // \label{fig:RoadExtractionSpectralAngleDiagram}
  // \end{figure}
  //
  //
  //
  //  Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef otb::SpectralAngleDistanceImageFilter<MultiSpectralImageType,InternalImageType> SAFilterType;
  SAFilterType::Pointer saFilter = SAFilterType::New();
  saFilter->SetReferencePixel(pixelRef);
  saFilter->SetInput(multispectralReader->GetOutput());
  
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  A square root is applied to the spectral angle image in order to enhance contrast between 
  //  darker pixels (which are pixels of interest) with \doxygen{itk::SqrtImageFilter}.
  //
  //  Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef itk::SqrtImageFilter<InternalImageType,InternalImageType> SqrtFilterType;
  SqrtFilterType::Pointer sqrtFilter = SqrtFilterType::New();
  sqrtFilter->SetInput(saFilter->GetOutput());

  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Use the Gaussian gradient filter compute the gradient direction and intensity
  // (\doxygen{itk::GradientRecursiveGaussianImageFilter}).
  //
  //  Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  double sigma = alpha*(1.2/resolution+1);
  typedef itk::GradientRecursiveGaussianImageFilter< InternalImageType, VectorImageType > GradientFilterType;
  GradientFilterType::Pointer gradientFilter = GradientFilterType::New();
  gradientFilter->SetSigma(sigma);
  gradientFilter->SetInput(sqrtFilter->GetOutput());

  // Software Guide : EndCodeSnippet
  
  //  Software Guide : BeginLatex
  //
  //  Compute the scalar product of the neighboring pixels and keep the
  //  minimum value and the direction with \doxygen{otb::NeighborhoodScalarProductFilter}. 
  // This is the line detector described 
  //  in \cite{Lacroix1998}.
  //
  //  Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  typedef otb::NeighborhoodScalarProductFilter<VectorImageType,InternalImageType,InternalImageType> NeighborhoodScalarProductType;
  NeighborhoodScalarProductType::Pointer scalarFilter = NeighborhoodScalarProductType::New();
  scalarFilter->SetInput(gradientFilter->GetOutput());

  // Software Guide : EndCodeSnippet
  
  //  Software Guide : BeginLatex
  //
  //  The resulting image is passed to the \doxygen{otb::RemoveIsolatedByDirectionFilter} 
  // filter to remove pixels 
  //  with no neighbor having the same direction.
  //
  //  Software Guide : EndLatex 


  // Software Guide : BeginCodeSnippet
  typedef otb::RemoveIsolatedByDirectionFilter<InternalImageType,InternalImageType,InternalImageType> RemoveIsolatedByDirectionType;
  RemoveIsolatedByDirectionType::Pointer removeIsolatedByDirectionFilter = RemoveIsolatedByDirectionType::New();
  removeIsolatedByDirectionFilter->SetInput(scalarFilter->GetOutput());
  removeIsolatedByDirectionFilter->SetInputDirection(scalarFilter->GetOutputDirection());

  // Software Guide : EndCodeSnippet
  
  //  Software Guide : BeginLatex
  //
  //  We remove pixels having a direction corresponding to bright lines 
  //  as we know that after the spectral angle, roads are in darker color
  //  with the \doxygen{otb::RemoveWrongDirectionFilter} filter.
  //
  //  Software Guide : EndLatex  
  
  // Software Guide : BeginCodeSnippet
  typedef otb::RemoveWrongDirectionFilter<InternalImageType,InternalImageType,InternalImageType> RemoveWrongDirectionType;
  RemoveWrongDirectionType::Pointer removeWrongDirectionFilter = RemoveWrongDirectionType::New();
  removeWrongDirectionFilter->SetInput(removeIsolatedByDirectionFilter->GetOutput());
  removeWrongDirectionFilter->SetInputDirection(scalarFilter->GetOutputDirection());

  // Software Guide : EndCodeSnippet
  
  //  Software Guide : BeginLatex
  //
  //  We remove pixels which are not maximum on the direction 
  //  perpendicular to the road direction with the \doxygen{otb::NonMaxRemovalByDirectionFilter}.
  //
  //  Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef otb::NonMaxRemovalByDirectionFilter<InternalImageType,InternalImageType,InternalImageType> NonMaxRemovalByDirectionType;
  NonMaxRemovalByDirectionType::Pointer nonMaxRemovalByDirectionFilter = NonMaxRemovalByDirectionType::New();
  nonMaxRemovalByDirectionFilter->SetInput(removeWrongDirectionFilter->GetOutput());
  nonMaxRemovalByDirectionFilter->SetInputDirection(scalarFilter->GetOutputDirection());

  // Software Guide : EndCodeSnippet
  
  //  Software Guide : BeginLatex
  //
  //  Extracted road are vectorized into polylines with \doxygen{otb::VectorizationPathListFilter}.
  //
  //  Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef otb::VectorizationPathListFilter<InternalImageType,InternalImageType,PathType> VectorizationFilterType;
  VectorizationFilterType::Pointer vectorizationFilter = VectorizationFilterType::New();
  vectorizationFilter->SetInput(nonMaxRemovalByDirectionFilter->GetOutput());
  vectorizationFilter->SetInputDirection(scalarFilter->GetOutputDirection());
  vectorizationFilter->SetAmplitudeThreshold(atof(argv[8]));
  // Software Guide : EndCodeSnippet
      
  
  //  Software Guide : BeginLatex
  //
  //  However, this vectorization is too simple and need to be refined 
  //  to be usable. First, we remove all aligned points to make one segment with 
  // \doxygen{otb::SimplifyPathListFilter}.
  //  Then we break the polylines which have sharp angles as they are probably
  //  not road with \doxygen{otb::BreakAngularPathListFilter}. 
  // Finally we remove path which are too short with \doxygen{otb::RemoveTortuousPathListFilter}.
  //
  //  Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef otb::SimplifyPathListFilter<PathType> SimplifyPathType;
  SimplifyPathType::Pointer simplifyPathListFilter = SimplifyPathType::New();
  simplifyPathListFilter->SetTolerance(1.0);
  simplifyPathListFilter->SetInput(vectorizationFilter->GetOutput());
    
  typedef otb::BreakAngularPathListFilter<PathType> BreakAngularPathType;
  BreakAngularPathType::Pointer breakAngularPathListFilter = BreakAngularPathType::New();
  breakAngularPathListFilter->SetMaxAngle(M_PI/8.);
  breakAngularPathListFilter->SetInput(simplifyPathListFilter->GetOutput());
  
  typedef otb::RemoveTortuousPathListFilter<PathType> RemoveTortuousPathType;
  RemoveTortuousPathType::Pointer removeTortuousPathListFilter = RemoveTortuousPathType::New();
  removeTortuousPathListFilter->SetMeanDistanceThreshold(1.0);
  removeTortuousPathListFilter->SetInput(breakAngularPathListFilter->GetOutput());
  // Software Guide : EndCodeSnippet
    
  //  Software Guide : BeginLatex
  //
  //  Polylines within a certain range are linked (\doxygen{otb::LinkPathListFilter}) to 
  //  try to fill gaps due to occultations by vehicules, trees, etc. before simplifying 
  //  polylines (\doxygen{otb::SimplifyPathListFilter}) and 
  //  removing the shortest ones with \doxygen{otb::RemoveTortuousPathListFilter}.
  //
  //  Software Guide : EndLatex
  
  
  // Software Guide : BeginCodeSnippet
  typedef otb::LinkPathListFilter<PathType> LinkPathType;
  LinkPathType::Pointer linkPathListFilter = LinkPathType::New();
  linkPathListFilter->SetDistanceThreshold(25.0/resolution);//research area of 25 m
  linkPathListFilter->SetAngularThreshold(M_PI/8);
  linkPathListFilter->SetInput(removeTortuousPathListFilter->GetOutput());
  
  SimplifyPathType::Pointer simplifyPathListFilter2 = SimplifyPathType::New();
  simplifyPathListFilter2->SetTolerance(1.0);
  simplifyPathListFilter2->SetInput(linkPathListFilter->GetOutput());
  
  RemoveTortuousPathType::Pointer removeTortuousPathListFilter2 = RemoveTortuousPathType::New();
  removeTortuousPathListFilter2->SetMeanDistanceThreshold(10.0);
  removeTortuousPathListFilter2->SetInput(simplifyPathListFilter2->GetOutput());
  // Software Guide : EndCodeSnippet
  
  
  //  Software Guide : BeginLatex
  //
  //  A value can be associated with each polyline according to pixel values 
  // under the polyline with \doxygen{otb::LikehoodPathListFilter}. A higher value 
  // will mean a higher likelihood to be 
  // a road. Polylines are drawn on the original image with \doxygen{otb::DrawPathListFilter}.
  //
  //  Software Guide : EndLatex
  
  //NB: check if PathListWithValue is correct according to conventions
  // Software Guide : BeginCodeSnippet
  typedef otb::LikehoodPathListFilter<PathType,InternalImageType> PathListToPathListWithValueType;
  PathListToPathListWithValueType::Pointer pathListConverter = PathListToPathListWithValueType::New();
  pathListConverter->SetInput(removeTortuousPathListFilter2->GetOutput());
  pathListConverter->SetInputImage(nonMaxRemovalByDirectionFilter->GetOutput());
  
  typedef otb::MultiToMonoChannelExtractROI<PixelType,PixelType> ChannelExtractionFilterType;
  ChannelExtractionFilterType::Pointer channelExtractor = ChannelExtractionFilterType::New();
  channelExtractor->SetInput(multispectralReader->GetOutput());
  
  typedef itk::RescaleIntensityImageFilter<InternalImageType,InternalImageType> RescalerType;
  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetOutputMaximum(255);
  rescaler->SetOutputMinimum(0);
  rescaler->SetInput(channelExtractor->GetOutput());

  typedef otb::DrawPathListFilter<InternalImageType, PathType, OutputImageType> DrawPathType;
  DrawPathType::Pointer drawPathListFilter = DrawPathType::New();
  drawPathListFilter->SetInput(rescaler->GetOutput());
  drawPathListFilter->SetInputPath(pathListConverter->GetOutput());
  drawPathListFilter->SetPathValue(255);
  // Software Guide : EndCodeSnippet
  
  
  //  Software Guide : BeginLatex
  //
  //  The filter is executed by invoking the \code{Update()} method. If the
  //  filter is part of a larger image processing pipeline, calling
  //  \code{Update()} on a downstream filter will also trigger update of this
  //  filter.
  //
  //  Extracted roads are drawn on the original image and output to a file.
  //
  //  Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet
  FileWriterType::Pointer writer = FileWriterType::New();
  writer->SetInput( drawPathListFilter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:ROADEXTRACTIONBYSTEPS} shows the result of applying
  // the road extraction by steps to a fusionned Quickbird image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{qb_ExtractRoad_pretty.eps}
  // \includegraphics[width=0.25\textwidth]{ExtractRoadByStepsExampleOutput.eps}
  // \itkcaption[Road extraction filter application]{Result of applying
  // the road extraction by steps pipeline to a fusionned Quickbird
  // image. From left to right : original image, extracted road with their
  // likehood values.}  
  // \label{fig:ROADEXTRACTIONBYSTEPS} 
  // \end{figure}
  //
  // Software Guide : EndLatex
  
  return EXIT_SUCCESS;
}
