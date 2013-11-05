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
#include "itkMacro.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMeanShiftSmoothingImageFilter.h"


#include "itkSubtractImageFilter.h"
#include "otbMultiChannelExtractROI.h"


int otbMeanShiftSmoothingImageFilterSpatialStability(int argc, char * argv[])
{
  if (argc != 12)
    {
    std::cerr << "Usage: " << argv[0] <<
    " infname spatialdifffname spectraldifffname  spatialBandwidth rangeBandwidth threshold maxiterationnumber startx starty sizex sizey"
              << std::endl;
    return EXIT_FAILURE;
    }

  const char *       infname                   = argv[1];
  const char *       spatialdifffname              = argv[2];
  const char *       spectraldifffname             = argv[3];
  const double       spatialBandwidth          = atof(argv[4]);
  const double       rangeBandwidth            = atof(argv[5]);
  const double       threshold                 = atof(argv[6]);
  const unsigned int maxiterationnumber        = atoi(argv[7]);
  const unsigned int startX                    = atoi(argv[8]);
  const unsigned int startY                    = atoi(argv[9]);
  const unsigned int sizeX                    = atoi(argv[10]);
  const unsigned int sizeY                    = atoi(argv[11]);


  /* maxit - threshold */

  const unsigned int Dimension = 2;
  typedef float                                            PixelType;
  typedef double                                           KernelType;
  typedef otb::VectorImage<PixelType, Dimension>           ImageType;
  typedef otb::ImageFileReader<ImageType>                  ReaderType;
  typedef otb::ImageFileWriter<ImageType>                  WriterType;
  typedef otb::MeanShiftSmoothingImageFilter<ImageType, ImageType> FilterType;
  typedef FilterType::OutputIterationImageType             IterationImageType;
  typedef otb::ImageFileWriter<IterationImageType>         IterationWriterType;
  typedef FilterType::OutputSpatialImageType               SpatialImageType;
  typedef SpatialImageType::InternalPixelType                      SpatialPixelType;
  typedef otb::ImageFileWriter<SpatialImageType>           SpatialWriterType;
  typedef FilterType::OutputLabelImageType                 LabelImageType;
  typedef otb::ImageFileWriter<LabelImageType>             LabelWriterType;
  typedef otb::MultiChannelExtractROI<PixelType,PixelType>  ExtractROIFilterType;
  typedef otb::MultiChannelExtractROI<SpatialPixelType,SpatialPixelType>  SpatialExtractROIFilterType;

  typedef itk::SubtractImageFilter<ImageType,ImageType,ImageType>  SubtractFilterType;
  typedef itk::SubtractImageFilter<SpatialImageType,SpatialImageType,SpatialImageType>  SpatialSubtractFilterType;

  // Instantiating object

  FilterType::Pointer MSfilter = FilterType::New();
  FilterType::Pointer MSfilterROI = FilterType::New();

  ExtractROIFilterType::Pointer filterROI = ExtractROIFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(infname);

  //first pipeline

  // Set filter parameters
  MSfilter->SetSpatialBandwidth(spatialBandwidth);
  MSfilter->SetRangeBandwidth(rangeBandwidth);
  MSfilter->SetThreshold(threshold);
  MSfilter->SetMaxIterationNumber(maxiterationnumber);
  MSfilter->SetInput(reader->GetOutput());
  MSfilter->SetModeSearch(false);
  MSfilter->Update();

  //second pipeline
  filterROI->SetInput(reader->GetOutput());
  filterROI->SetStartX(startX);
  filterROI->SetStartY(startY);
  filterROI->SetSizeX(sizeX);
  filterROI->SetSizeY(sizeY);
  filterROI->UpdateOutputInformation();

  MSfilterROI->SetSpatialBandwidth(spatialBandwidth);
  MSfilterROI->SetRangeBandwidth(rangeBandwidth);
  MSfilterROI->SetThreshold(threshold);
  MSfilterROI->SetMaxIterationNumber(maxiterationnumber);
  MSfilterROI->SetInput(filterROI->GetOutput());
  MSfilterROI->SetModeSearch(false);

  //compare output

  const unsigned int border=maxiterationnumber*spatialBandwidth+1;
  const unsigned int startXROI2=border;
  const unsigned int startXROI=border+startX;
  const unsigned int startYROI2=border;
  const unsigned int startYROI=border+startY;

  const unsigned int sizeXROI=sizeX-2*border;
  const unsigned int sizeYROI=sizeY-2*border;

  SpatialExtractROIFilterType::Pointer filterROISpatial1 = SpatialExtractROIFilterType::New();
  filterROISpatial1->SetInput(MSfilter->GetSpatialOutput());
  filterROISpatial1->SetStartX(startXROI);
  filterROISpatial1->SetStartY(startYROI);
  filterROISpatial1->SetSizeX(sizeXROI);
  filterROISpatial1->SetSizeY(sizeYROI);


  SpatialExtractROIFilterType::Pointer filterROISpatial2 = SpatialExtractROIFilterType::New();
  filterROISpatial2->SetInput(MSfilterROI->GetSpatialOutput());
  filterROISpatial2->SetStartX(startXROI2);
  filterROISpatial2->SetStartY(startYROI2);
  filterROISpatial2->SetSizeX(sizeXROI);
  filterROISpatial2->SetSizeY(sizeYROI);

  SpatialSubtractFilterType::Pointer filterSubSpatial = SpatialSubtractFilterType::New();
  filterSubSpatial->SetInput1(filterROISpatial1->GetOutput());
  filterSubSpatial->SetInput2(filterROISpatial2->GetOutput());

  ExtractROIFilterType::Pointer filterROISpectral1 = ExtractROIFilterType::New();
  filterROISpectral1->SetInput(MSfilter->GetRangeOutput());
  filterROISpectral1->SetStartX(startXROI);
  filterROISpectral1->SetStartY(startYROI);
  filterROISpectral1->SetSizeX(sizeXROI);
  filterROISpectral1->SetSizeY(sizeYROI);


  ExtractROIFilterType::Pointer filterROISpectral2 = ExtractROIFilterType::New();
  filterROISpectral2->SetInput(MSfilterROI->GetRangeOutput());
  filterROISpectral2->SetStartX(startXROI2);
  filterROISpectral2->SetStartY(startYROI2);
  filterROISpectral2->SetSizeX(sizeXROI);
  filterROISpectral2->SetSizeY(sizeYROI);

   SubtractFilterType::Pointer filterSubSpectral = SubtractFilterType::New();
   filterSubSpectral->SetInput1(filterROISpectral1->GetOutput());
   filterSubSpectral->SetInput2(filterROISpectral2->GetOutput());

  SpatialWriterType::Pointer writerSpatial = SpatialWriterType::New();
  WriterType::Pointer writerSpectral = WriterType::New();

  writerSpatial->SetFileName(spatialdifffname);
  writerSpectral->SetFileName(spectraldifffname);

  writerSpatial->SetInput(filterSubSpatial->GetOutput());
  writerSpectral->SetInput(filterSubSpectral->GetOutput());


  writerSpatial->Update();
  writerSpectral->Update();

  return EXIT_SUCCESS;
}
