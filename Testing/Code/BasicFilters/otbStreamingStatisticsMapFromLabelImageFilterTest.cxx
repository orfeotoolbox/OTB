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

#include "otbVectorImage.h"
#include "otbImage.h"

#include <itkImageRegionIteratorWithIndex.h>
#include "otbImageFileWriter.h"

#include "otbStreamingStatisticsMapFromLabelImageFilter.h"


template<class InternalVectorPixelType>
int generic_StreamingStatisticsMapFromLabelImageFilterTest(int argc, char * argv[])
{
  typedef unsigned int LabelPixelType;

  const unsigned int Dimension = 2;
  const unsigned int nbComponents = 3;

  typedef typename otb::VectorImage<InternalVectorPixelType, Dimension> VectorImageType;
  typedef typename VectorImageType::PixelType VectorPixelType;
  typedef typename VectorImageType::Pointer VectorPointerType;

  typedef typename otb::Image<LabelPixelType, Dimension> LabelImageType;
  typedef typename LabelImageType::Pointer LabelImagePointerType;

  typedef typename itk::ImageRegionIteratorWithIndex<VectorImageType> VIteratorType;
  typedef typename itk::ImageRegionIteratorWithIndex<LabelImageType> IteratorType;

  typedef typename otb::StreamingStatisticsMapFromLabelImageFilter<VectorImageType, LabelImageType>
      StreamingStatisticsMapFromLabelImageFilterType;
  typedef typename StreamingStatisticsMapFromLabelImageFilterType::Pointer
      StreamingStatisticsMapFromLabelImageFilterPointerType;
  typedef typename StreamingStatisticsMapFromLabelImageFilterType::MeanValueMapType MeanValueMapType;
  typedef typename StreamingStatisticsMapFromLabelImageFilterType::LabelPopulationMapType LabelPopulationMapType;

  VectorPointerType supportImage = VectorImageType::New();
  LabelImagePointerType labelImage = LabelImageType::New();

  typedef otb::ImageFileWriter<VectorImageType> WriterType;
  typedef typename WriterType::Pointer WriterPointerType;

  typedef otb::ImageFileWriter<LabelImageType> LWriterType;
  typedef typename LWriterType::Pointer LWriterPointerType;

  VectorPixelType redColor(nbComponents), greenColor(nbComponents), blueColor(nbComponents);
  redColor[0] = 255, redColor[1] = 0, redColor[2] = 0;
  greenColor[0] = 0, greenColor[1] = 255, greenColor[2] = 0;
  blueColor[0] = 0, blueColor[1] = 0, blueColor[2] = 255;

  LabelPixelType redLabel = 10, greenLabel = 20, blueLabel = 30;

  typename VectorImageType::SizeType size;
  typename VectorImageType::IndexType start;
  typename VectorImageType::RegionType region;
  size[0] = atoi(argv[2]); // size along X
  size[1] = atoi(argv[3]); // size along Y
  start[0] = 0; // first index on X
  start[1] = 0; // first index on Y
  region.SetIndex(start);
  region.SetSize(size);

  supportImage->SetNumberOfComponentsPerPixel(nbComponents);
  supportImage->SetRegions(region);
  supportImage->Allocate();
  supportImage->FillBuffer(blueColor);

  labelImage->SetRegions(region);
  labelImage->Allocate();
  labelImage->FillBuffer(blueLabel);

  typename VectorImageType::SizeType sizeSelectRed, sizeSelectGreen;
  typename VectorImageType::IndexType startSelectRed, startSelectGreen;
  typename VectorImageType::RegionType regionSelectRed, regionSelectGreen;
  sizeSelectRed[0] = size[0] / 4.0; // size along X
  sizeSelectRed[1] = size[1] / 4.0; // size along Y
  sizeSelectGreen[0] = size[0] / 2.0;; // size along X
  sizeSelectGreen[1] = size[1] / 2.0;; // size along Y

  startSelectRed[0] = (size[0] - sizeSelectRed[0] - sizeSelectGreen[0]) / 3.0; // first index on X
  startSelectRed[1] = (size[1] - sizeSelectRed[1] - sizeSelectGreen[1]) / 3.0; // first index on Y
  startSelectGreen[0] = size[0] - startSelectRed[0] - sizeSelectGreen[0]; // first index on X
  startSelectGreen[1] = size[1] - startSelectRed[1] - sizeSelectGreen[1]; // first index on X

  regionSelectRed.SetIndex(startSelectRed);
  regionSelectRed.SetSize(sizeSelectRed);
  regionSelectGreen.SetIndex(startSelectGreen);
  regionSelectGreen.SetSize(sizeSelectGreen);

  std::cout << "size = " << size << std::endl;
  std::cout << "sizeSelectRed = " << sizeSelectRed << std::endl;
  std::cout << "sizeSelectGreen = " << sizeSelectGreen << std::endl;
  std::cout << "startSelectRed = " << startSelectRed << std::endl;
  std::cout << "startSelectGreen = " << startSelectGreen << std::endl << std::endl;

  VIteratorType supportImageRedIterator(supportImage, regionSelectRed);
  IteratorType labelImageRedIterator(labelImage, regionSelectRed);
  labelImageRedIterator.GoToBegin();
  for(supportImageRedIterator.GoToBegin() ; !supportImageRedIterator.IsAtEnd() ; ++supportImageRedIterator)
  {
    supportImageRedIterator.Set(redColor);
    labelImageRedIterator.Set(redLabel);
    ++labelImageRedIterator;
  }

  VIteratorType supportImageGreenIterator(supportImage, regionSelectGreen);
  IteratorType labelImageGreenIterator(labelImage, regionSelectGreen);
  labelImageGreenIterator.GoToBegin();
  for(supportImageGreenIterator.GoToBegin() ; !supportImageGreenIterator.IsAtEnd() ; ++supportImageGreenIterator)
  {
    supportImageGreenIterator.Set(greenColor);
    labelImageGreenIterator.Set(greenLabel);
    ++labelImageGreenIterator;
  }

  //Optional writing of the generated images
  if (atoi(argv[4]) != 0)
    {
    WriterPointerType writer = WriterType::New();
    writer->SetFileName(argv[5]);
    writer->SetInput(supportImage);
    writer->Update();

    LWriterPointerType lwriter = LWriterType::New();
    lwriter->SetFileName(argv[6]);
    lwriter->SetInput(labelImage);
    lwriter->Update();
    }

  unsigned int nbPixels = size[0] * size[1];
  unsigned int nbRedPixBL = sizeSelectRed[0] * sizeSelectRed[1];
  unsigned int nbGreenPixBL = sizeSelectGreen[0] * sizeSelectGreen[1];
  unsigned int nbBluePixBL = nbPixels - nbRedPixBL - nbGreenPixBL;

  std::cout << "nbPixels = " << nbPixels << std::endl;
  std::cout << "nbRedPixBL = " << nbRedPixBL << std::endl;
  std::cout << "nbGreenPixBL = " << nbGreenPixBL << std::endl;
  std::cout << "nbBluePixBL = " << nbBluePixBL << std::endl << std::endl;

  StreamingStatisticsMapFromLabelImageFilterPointerType m_StatisticsMapFromLabelImageFilter;
  m_StatisticsMapFromLabelImageFilter = StreamingStatisticsMapFromLabelImageFilterType::New();
  m_StatisticsMapFromLabelImageFilter->SetInput(supportImage);
  m_StatisticsMapFromLabelImageFilter->SetInputLabelImage(labelImage);
  m_StatisticsMapFromLabelImageFilter->Update();

  LabelPopulationMapType labelPopulationMapBL;
  labelPopulationMapBL[redLabel] = nbRedPixBL;
  labelPopulationMapBL[greenLabel] = nbGreenPixBL;
  labelPopulationMapBL[blueLabel] = nbBluePixBL;
  LabelPopulationMapType labelPopulationMap = m_StatisticsMapFromLabelImageFilter->GetLabelPopulationMap();

  MeanValueMapType labelToMeanIntensityMapBL;
  labelToMeanIntensityMapBL[redLabel] = redColor;
  labelToMeanIntensityMapBL[greenLabel] = greenColor;
  labelToMeanIntensityMapBL[blueLabel] = blueColor;
  MeanValueMapType labelToMeanIntensityMap = m_StatisticsMapFromLabelImageFilter->GetMeanValueMap();


  if ( (labelPopulationMap != labelPopulationMapBL) || (labelToMeanIntensityMap != labelToMeanIntensityMapBL) )
    {
    if (labelPopulationMap != labelPopulationMapBL)
      {
      typename LabelPopulationMapType::iterator itlabelPopulationMap;
      std::cout << std::endl;
      std::cout << "ERROR with m_StatisticsMapFromLabelImageFilter->GetLabelPopulationMap()" << std::endl;
      for (itlabelPopulationMap = labelPopulationMap.begin(); itlabelPopulationMap != labelPopulationMap.end(); ++itlabelPopulationMap)
        {
        std::cout << "    labelPopulationMap[" << itlabelPopulationMap->first << "] = "
            << labelPopulationMap[itlabelPopulationMap->first] << std::endl;
        std::cout << "    labelPopulationMapBL[" << itlabelPopulationMap->first << "] = "
            << labelPopulationMapBL[itlabelPopulationMap->first] << std::endl << std::endl;
        }
      }

    if (labelToMeanIntensityMap != labelToMeanIntensityMapBL)
      {
      typename MeanValueMapType::iterator itMeanValueMap;
      std::cout << std::endl;
      std::cout << "ERROR with m_StatisticsMapFromLabelImageFilter->GetMeanValueMap()" << std::endl;
      for (itMeanValueMap = labelToMeanIntensityMap.begin(); itMeanValueMap != labelToMeanIntensityMap.end(); ++itMeanValueMap)
        {
        std::cout << "    labelToMeanIntensityMap[" << itMeanValueMap->first << "] = "
            << labelToMeanIntensityMap[itMeanValueMap->first] << std::endl;
        std::cout << "    labelToMeanIntensityMapBL[" << itMeanValueMap->first << "] = "
            << labelToMeanIntensityMapBL[itMeanValueMap->first] << std::endl << std::endl;
        }
      }

    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}



int otbStreamingStatisticsMapFromLabelImageFilterTest(int argc, char * argv[])
{
  if (strcmp(argv[1], "UINT8") == 0)
    {
    return generic_StreamingStatisticsMapFromLabelImageFilterTest<unsigned char> (argc, argv);
    }

  if (strcmp(argv[1], "UINT16") == 0)
    {
    return generic_StreamingStatisticsMapFromLabelImageFilterTest<unsigned short> (argc, argv);
    }

  if (strcmp(argv[1], "INT16") == 0)
    {
    return generic_StreamingStatisticsMapFromLabelImageFilterTest<short> (argc, argv);
    }

  if (strcmp(argv[1], "UINT32") == 0)
    {
    return generic_StreamingStatisticsMapFromLabelImageFilterTest<unsigned int> (argc, argv);
    }

  if (strcmp(argv[1], "INT32") == 0)
    {
    return generic_StreamingStatisticsMapFromLabelImageFilterTest<int> (argc, argv);
    }

  if (strcmp(argv[1], "FLOAT") == 0)
    {
    return generic_StreamingStatisticsMapFromLabelImageFilterTest<float> (argc, argv);
    }

  if (strcmp(argv[1], "DOUBLE") == 0)
    {
    return generic_StreamingStatisticsMapFromLabelImageFilterTest<double> (argc, argv);
    }

  return EXIT_SUCCESS;
}
