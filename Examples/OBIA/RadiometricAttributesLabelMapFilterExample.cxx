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
//    INPUTS: {qb_RoadExtract.tif}
//    OUTPUTS: {OBIARadiometricAttribute1.png}, {qb_ExtractRoad_Radiometry_pretty.jpg}
//    STATS::Band1::Mean 0 0.5 16 16 50 1.0
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  This example shows the basic approach to perform object based analysis on a image.
//  The input image is firstly segmented using the \doxygen{otb}{MeanShiftImageFilter}
//  Then each segmented region is converted to a Map of labeled objects.
//  Afterwards the \doxygen{otb}{otbMultiChannelRAndNIRIndexImageFilter} computes
//  radiometric attributes for each object. In this example the NDVI is computed.
//  The computed feature is passed to the \doxygen{otb}{BandsStatisticsAttributesLabelMapFilter}
//  which computes statistics over the resulting band.
//  Therefore, region's statistics over each band can be access by concatening
//  STATS, the band number and the statistical attribute separated by colons. In this example
//  the mean of the first band (which contains the NDVI) is access over all the regions
//  with the attribute: 'STATS::Band1::Mean'.
//
//  Software Guide : EndLatex

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbMeanShiftVectorImageFilter.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "otbShapeAttributesLabelMapFilter.h"
#include "otbBandsStatisticsAttributesLabelMapFilter.h"
#include "itkLabelMapToBinaryImageFilter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbAttributesMapOpeningLabelMapFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbMultiChannelRAndNIRIndexImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"

int main(int argc, char * argv[])
{
  if (argc != 11)
    {
    std::cerr << "Usage: " << argv[0] <<
    " reffname outfname outprettyfname attribute_name ";
    std::cerr <<
    "lowerThan tresh spatialRadius rangeRadius minregionsize scale" <<
    std::endl;
    return EXIT_FAILURE;
    }

  const char * reffname = argv[1];
  const char * outfname = argv[2];
  const char * outprettyfname = argv[3];
  const char * attr     = argv[4];
  bool       lowerThan      = atoi(argv[5]);
  double       thresh         = atof(argv[6]);

  const unsigned int spatialRadius          = atoi(argv[7]);
  const double       rangeRadius            = atof(argv[8]);
  const unsigned int minRegionSize          = atoi(argv[9]);
  const double       scale                  = atoi(argv[10]);

  const unsigned int Dimension = 2;

  // Labeled image type
  typedef unsigned short                              LabelType;
  typedef unsigned char                              MaskPixelType;
  typedef double                                      PixelType;
  typedef otb::Image<LabelType, Dimension>            LabeledImageType;
  typedef otb::Image<MaskPixelType, Dimension>        MaskImageType;
  typedef otb::Image<PixelType, Dimension>            ImageType;
  typedef otb::VectorImage<PixelType, Dimension>      VectorImageType;
  typedef otb::VectorImage<unsigned char, Dimension>  OutputVectorImageType;
  typedef otb::ImageFileReader<LabeledImageType>      LabeledReaderType;
  typedef otb::ImageFileReader<ImageType>             ReaderType;
  typedef otb::ImageFileReader<VectorImageType>       VectorReaderType;
  typedef otb::ImageFileWriter<MaskImageType>         WriterType;
  typedef otb::ImageFileWriter<OutputVectorImageType> VectorWriterType;
  typedef otb::VectorRescaleIntensityImageFilter
  <VectorImageType, OutputVectorImageType> VectorRescalerType;
  typedef otb::MultiChannelExtractROI<unsigned char,
      unsigned char> ChannelExtractorType;
  // Label map typedef
  typedef otb::AttributesMapLabelObject<LabelType, Dimension,
      double>
  LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>
  LabelMapType;
  typedef itk::LabelImageToLabelMapFilter<LabeledImageType,
      LabelMapType>
  LabelMapFilterType;
  typedef otb::ShapeAttributesLabelMapFilter<LabelMapType>
  ShapeLabelMapFilterType;
  typedef otb::BandsStatisticsAttributesLabelMapFilter<LabelMapType,
      VectorImageType>
  RadiometricLabelMapFilterType;
  typedef otb::AttributesMapOpeningLabelMapFilter<LabelMapType>
  OpeningLabelMapFilterType;
  typedef itk::LabelMapToBinaryImageFilter<LabelMapType,
      MaskImageType>
  LabelMapToBinaryImageFilterType;
  typedef otb::MultiChannelRAndNIRIndexImageFilter<VectorImageType,
      ImageType> NDVIImageFilterType;
  typedef otb::ImageToVectorImageCastFilter<ImageType, VectorImageType>
  ImageToVectorImageCastFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(reffname);

  LabeledReaderType::Pointer lreader = LabeledReaderType::New();
  lreader->SetFileName(reffname);

  VectorReaderType::Pointer vreader = VectorReaderType::New();
  vreader->SetFileName(reffname);
  vreader->Update();
  //  Software Guide : BeginLatex
  //
  // Firstly, segment the input image by using the Mean Shift algorithm (see \ref{sec:MeanShift} for deeper
  // explanations).
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MeanShiftVectorImageFilter
  <VectorImageType, VectorImageType, LabeledImageType> FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetSpatialRadius(spatialRadius);
  filter->SetRangeRadius(rangeRadius);
  filter->SetMinimumRegionSize(minRegionSize);
  filter->SetScale(scale);
  // Software Guide : EndCodeSnippet

  // For non regression tests, set the number of threads to 1
  // because MeanShift results depends on the number of threads
  filter->SetNumberOfThreads(1);

  //  Software Guide : BeginLatex
  //
  // The \doxygen{otb}{MeanShiftImageFilter} type is instantiated using the image
  // types.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(vreader->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The \doxygen{itk}{LabelImageToLabelMapFilter} type is instantiated using the output
  // of the \doxygen{otb}{MeanShiftImageFilter}. This filter produces a labeled image
  // where each segmented region has a unique label.
  //
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
  labelMapFilter->SetInput(filter->GetLabeledClusteredOutput());
  labelMapFilter->SetBackgroundValue(itk::NumericTraits<LabelType>::min());

  ShapeLabelMapFilterType::Pointer shapeLabelMapFilter =
    ShapeLabelMapFilterType::New();
  shapeLabelMapFilter->SetInput(labelMapFilter->GetOutput());
  // Software Guide : EndCodeSnippet
  //  Software Guide : BeginLatex
  //
  // Instantiate the  \doxygen{otb}{RadiometricLabelMapFilterType} to
  // compute statistics of the feature image on each label object.
  //
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  RadiometricLabelMapFilterType::Pointer radiometricLabelMapFilter
    = RadiometricLabelMapFilterType::New();
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  //  Feature image could be one of the following image:
  //  \begin{itemize}
  //  \item GEMI
  //  \item NDVI
  //  \item IR
  //  \item IC
  //  \item IB
  //  \item NDWI2
  //  \item Intensity
  //  \end{itemize}
  //
  //  Input image must be convert to the desired coefficient.
  //  In our case, statistics are computed on the NDVI coefficient on each label object.
  //
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  NDVIImageFilterType:: Pointer ndviImageFilter = NDVIImageFilterType::New();

  ndviImageFilter->SetRedIndex(3);
  ndviImageFilter->SetNIRIndex(4);
  ndviImageFilter->SetInput(vreader->GetOutput());

  ImageToVectorImageCastFilterType::Pointer ndviVectorImageFilter =
      ImageToVectorImageCastFilterType::New();

  ndviVectorImageFilter->SetInput(ndviImageFilter->GetOutput());

  radiometricLabelMapFilter->SetInput(shapeLabelMapFilter->GetOutput());
  radiometricLabelMapFilter->SetFeatureImage(ndviVectorImageFilter->GetOutput());
  // Software Guide : EndCodeSnippet
  //  Software Guide : BeginLatex
  //
  // The \doxygen{otb}{AttributesMapOpeningLabelMapFilter} will perform the selection.
  // There are three parameters. \code{AttributeName} specifies the radiometric attribute, \code{Lambda}
  // controls the thresholding of the input and \code{ReverseOrdering} make this filter to remove the
  // object with an attribute value greater than \code{Lambda} instead.
  //
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  OpeningLabelMapFilterType::Pointer opening = OpeningLabelMapFilterType::New();
  opening->SetInput(radiometricLabelMapFilter->GetOutput());
  opening->SetAttributeName(attr);
  opening->SetLambda(thresh);
  opening->SetReverseOrdering(lowerThan);
  opening->Update();
  // Software Guide : EndCodeSnippet
  //  Software Guide : BeginLatex
  //
  //  Then, Label objects selected are transform in a Label Image using the
  //  \doxygen{itk}{LabelMapToLabelImageFilter}.
  //
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  LabelMapToBinaryImageFilterType::Pointer labelMap2LabeledImage
    = LabelMapToBinaryImageFilterType::New();
  labelMap2LabeledImage->SetInput(opening->GetOutput());
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // And finally, we declare the writer and call its \code{Update()} method to
  // trigger the full pipeline execution.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(outfname);
  writer->SetInput(labelMap2LabeledImage->GetOutput());
  writer->Update();
  // Software Guide : EndCodeSnippet

  OutputVectorImageType::PixelType minimum, maximum;
  minimum.SetSize(vreader->GetOutput()->GetNumberOfComponentsPerPixel());
  maximum.SetSize(vreader->GetOutput()->GetNumberOfComponentsPerPixel());
  minimum.Fill(0);
  maximum.Fill(255);

  VectorRescalerType::Pointer vr = VectorRescalerType::New();
  vr->SetInput(filter->GetClusteredOutput());
  vr->SetOutputMinimum(minimum);
  vr->SetOutputMaximum(maximum);
  vr->SetClampThreshold(0.01);

  ChannelExtractorType::Pointer selecter = ChannelExtractorType::New();
  selecter->SetInput(vr->GetOutput());
  selecter->SetExtractionRegion(vreader->GetOutput()->GetLargestPossibleRegion());
  selecter->SetChannel(3);
  selecter->SetChannel(2);
  selecter->SetChannel(1);

  VectorWriterType::Pointer vectWriter = VectorWriterType::New();
  vectWriter->SetFileName(outprettyfname);
  vectWriter->SetInput(selecter->GetOutput());
  vectWriter->Update();

  return EXIT_SUCCESS;
}

// Software Guide : BeginLatex
//
// Figure~\ref{fig:RADIOMETRIC_LABEL_MAP_FILTER} shows the result of applying
// the object selection based on radiometric attributes.
// \begin{figure} [htbp]
// \center
// \includegraphics[width=0.44\textwidth]{qb_ExtractRoad_Radiometry_pretty.eps}
// \includegraphics[width=0.44\textwidth]{OBIARadiometricAttribute1.eps}
// \itkcaption[Object based extraction based on ]{Vegetation mask resulting from processing.}
// \label{fig:RADIOMETRIC_LABEL_MAP_FILTER}
// \end{figure}
//
// Software Guide : EndLatex
