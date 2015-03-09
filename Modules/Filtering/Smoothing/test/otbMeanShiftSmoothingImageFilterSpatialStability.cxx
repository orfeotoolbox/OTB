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
#include "otbImageFileReader.h"
#include "otbMeanShiftSmoothingImageFilter.h"


#include "otbDifferenceImageFilter.h"
#include "otbMultiChannelExtractROI.h"

#include <iomanip>


int otbMeanShiftSmoothingImageFilterSpatialStability(int argc, char * argv[])
{
  if (argc != 10)
    {
    std::cerr << "Usage: " << argv[0] <<
    " infname spatialdifffname spectraldifffname  spatialBandwidth rangeBandwidth threshold maxiterationnumber startx starty sizex sizey"
              << std::endl;
    return EXIT_FAILURE;
    }

  const char *       infname                   = argv[1];
  const double       spatialBandwidth          = atof(argv[2]);
  const double       rangeBandwidth            = atof(argv[3]);
  const double       threshold                 = atof(argv[4]);
  const unsigned int maxiterationnumber        = atoi(argv[5]);
  const unsigned int startX                    = atoi(argv[6]);
  const unsigned int startY                    = atoi(argv[7]);
  const unsigned int sizeX                    = atoi(argv[8]);
  const unsigned int sizeY                    = atoi(argv[9]);


  /* maxit - threshold */

  const unsigned int Dimension = 2;
  typedef float                                            PixelType;
  typedef otb::VectorImage<PixelType, Dimension>           ImageType;
  typedef otb::ImageFileReader<ImageType>                  ReaderType;
  typedef otb::MeanShiftSmoothingImageFilter<ImageType, ImageType> FilterType;
  typedef FilterType::OutputSpatialImageType               SpatialImageType;
  typedef SpatialImageType::InternalPixelType                      SpatialPixelType;
  typedef otb::MultiChannelExtractROI<PixelType,PixelType>  ExtractROIFilterType;
  typedef otb::MultiChannelExtractROI<SpatialPixelType,SpatialPixelType>  SpatialExtractROIFilterType;

  typedef otb::DifferenceImageFilter<ImageType,ImageType>  SubtractFilterType;
  typedef otb::DifferenceImageFilter<SpatialImageType,SpatialImageType>  SpatialSubtractFilterType;

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

  // GlobalShift ensure spatial stability
  ImageType::IndexType globalShift;
  globalShift[0]=startX;
  globalShift[1]=startY;
  MSfilterROI->SetGlobalShift(globalShift);

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
  filterSubSpatial->SetValidInput(filterROISpatial1->GetOutput());
  filterSubSpatial->SetTestInput(filterROISpatial2->GetOutput());
  filterSubSpatial->Update();

  SpatialImageType::PixelType spatialOutputDiff = filterSubSpatial->GetTotalDifference();

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
   filterSubSpectral->SetValidInput(filterROISpectral1->GetOutput());
   filterSubSpectral->SetTestInput(filterROISpectral2->GetOutput());
   filterSubSpectral->Update();

   ImageType::PixelType spectralOutputDiff = filterSubSpectral->GetTotalDifference();

   bool spatialUnstable = false;
   bool spectralUnstable = false;


   for(unsigned int i = 0; i < spatialOutputDiff.Size(); ++i)
     {
     if(spatialOutputDiff[i] > 0)
       {
       spatialUnstable = true;
       }
     }

   for(unsigned int i = 0; i < spectralOutputDiff.Size(); ++i)
     {
     if(spectralOutputDiff[i] > 0)
       {
       spectralUnstable = true;
       }
     }

   std::cerr<<std::setprecision(10);

   if(spatialUnstable)
     {
     std::cerr<<"Spatial output is unstable (total difference = "<<spatialOutputDiff<<")"<<std::endl;
     }

   if(spectralUnstable)
     {
     std::cerr<<"Spectral output is unstable (total difference = "<<spectralOutputDiff<<")"<<std::endl;
     }


   if(spatialUnstable || spectralUnstable)
     {
     return EXIT_FAILURE;
     }

  return EXIT_SUCCESS;
}
