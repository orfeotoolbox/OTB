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
#ifndef __otbMulti3DMapToDEMFilter_txx
#define __otbMulti3DMapToDEMFilter_txx

#include "otbMulti3DMapToDEMFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbImageToGenericRSOutputParameters.h"
#include "otbInverseSensorModel.h"

namespace otb
{

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::Multi3DMapToDEMFilter()
{
  // Set the number of inputs (1 image one optional mask)
  this->SetNumberOfInputs(2);
  this->SetNumberOfRequiredInputs(1);
  //this->m_MapKeywordLists.resize(1);
  m_DEMGridStep = 10.0;
  // Set the outputs
  this->SetNumberOfOutputs(1);
  this->SetNthOutput(0, TOutputDEMImage::New());
  // Default DEM reconstruction parameters
  m_MapSplitterList = SplitterListType::New();

  m_NoDataValue = -32768;
  m_ElevationMin = -100;
  m_ElevationMax = 500;
  m_CellFusionMode = otb::CellFusionMode::MAX;
  m_OutputParametersFrom3DMap = -2;
  m_IsGeographic=true;

  m_Margin[0]=10;
  m_Margin[1]=10;

}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::~Multi3DMapToDEMFilter()
{
}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
void Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::SetNumberOf3DMaps(unsigned int nb)
{
  if (nb > 0)
    {
    this->SetNumberOfInputs(2 * nb);
    }
}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
unsigned int Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::GetNumberOf3DMaps()
{
  return this->GetNumberOfInputs() / 2;
}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
void Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::Set3DMapInput(unsigned int index,
                                                                                 const T3DImage * map)
{
  if ((2 * (index + 1)) > this->GetNumberOfInputs())
    {
    itkExceptionMacro(<<"Index is greater than the number of images");
    }
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(2 * index, const_cast<T3DImage *> (map));
}


template<class T3DImage, class TMaskImage, class TOutputDEMImage>
void Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::SetMaskInput(unsigned int index,
                                                                                const TMaskImage * mask)
{
  if ((2 * (index + 1)) > this->GetNumberOfInputs())
    {
    itkExceptionMacro(<<"Index is greater than the number of images");
    }
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(2 * index + 1, const_cast<TMaskImage *> (mask));
}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
const T3DImage *
Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::Get3DMapInput(unsigned int index) const
{
  if ((2 * (index + 1)) > this->GetNumberOfInputs())
    {
    return NULL;
    }
  return static_cast<const T3DImage *> (this->itk::ProcessObject::GetInput(2 * index));
}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
const TMaskImage *
Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::GetMaskInput(unsigned int index) const
{
  if ((2 * (index + 1)) > this->GetNumberOfInputs())
    {
    return NULL;
    }
  return static_cast<const TMaskImage *> (this->itk::ProcessObject::GetInput(2 * index + 1));
}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
const TOutputDEMImage *
Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::GetDEMOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<const TOutputDEMImage *> (this->itk::ProcessObject::GetOutput(0));
}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
TOutputDEMImage *
Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::GetDEMOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<TOutputDEMImage *> (this->itk::ProcessObject::GetOutput(0));
}


template<class T3DImage, class TMaskImage, class TOutputDEMImage>
void Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::SetOutputParametersFromImage()
{
  int index = m_OutputParametersFrom3DMap;
  if (static_cast<unsigned int> ((2 * (index + 1))) > this->GetNumberOfInputs())
    {
    itkExceptionMacro(<< "input at position "<<index<<" is unavailable");
    }

  unsigned int numberOfInputs = this->GetNumberOfInputs() / 2;
  unsigned int indexStart = 0;
  unsigned int indexEnd = numberOfInputs - 1;

  if (index != -1)
    {
    indexStart = index;
    indexEnd = index;
    }

  //compute DEM extent union of 3D map extent

  TOutputDEMImage * outputPtr = this->GetDEMOutput();

  // Set-up a transform to use the DEMHandler
  typedef otb::GenericRSTransform<> RSTransform2DType;

  // DEM BBox
  itk::NumericTraits<DEMPixelType>::max();
  double box_xmin = itk::NumericTraits<DEMPixelType>::max();
  double box_xmax = itk::NumericTraits<DEMPixelType>::NonpositiveMin();
  double box_ymin = itk::NumericTraits<DEMPixelType>::max();
  double box_ymax = itk::NumericTraits<DEMPixelType>::NonpositiveMin();

  for (unsigned int k = indexStart; k <= indexEnd; ++k)
    {
    T3DImage *imgPtr = const_cast<T3DImage *> (this->Get3DMapInput(k));

    RSTransform2DType::Pointer mapToGroundTransform = RSTransform2DType::New();
    ImageKeywordListType imageKWL = imgPtr->GetImageKeywordlist();
    mapToGroundTransform->SetInputKeywordList(imageKWL);

    /*if(!m_ProjectionRef.empty())
     {
     mapToGroundTransform->SetOutputProjectionRef(m_ProjectionRef);
     }*/
    mapToGroundTransform->InstanciateTransform();

    typename InputMapType::SizeType inputSize = imgPtr->GetLargestPossibleRegion().GetSize();

    typename InputMapType::PointType tmpPoint;
    tmpPoint = imgPtr->GetOrigin();
    RSTransform2DType::OutputPointType ul = mapToGroundTransform->TransformPoint(tmpPoint);

    tmpPoint[0] = (imgPtr->GetOrigin())[0] + (imgPtr->GetSpacing())[0] * static_cast<double> (inputSize[0]);
    tmpPoint[1] = (imgPtr->GetOrigin())[1];
    RSTransform2DType::OutputPointType ur = mapToGroundTransform->TransformPoint(tmpPoint);

    tmpPoint[0] = (imgPtr->GetOrigin())[0] + (imgPtr->GetSpacing())[0] * static_cast<double> (inputSize[0]);
    tmpPoint[1] = (imgPtr->GetOrigin())[1] + (imgPtr->GetSpacing())[1] * static_cast<double> (inputSize[1]);
    RSTransform2DType::OutputPointType lr = mapToGroundTransform->TransformPoint(tmpPoint);

    tmpPoint[0] = (imgPtr->GetOrigin())[0];
    tmpPoint[1] = (imgPtr->GetOrigin())[1] + (imgPtr->GetSpacing())[1] * static_cast<double> (inputSize[1]);
    RSTransform2DType::OutputPointType ll = mapToGroundTransform->TransformPoint(tmpPoint);

    double xmin = std::min(std::min(std::min(ul[0], ur[0]), lr[0]), ll[0]);
    double xmax = std::max(std::max(std::max(ul[0], ur[0]), lr[0]), ll[0]);
    double ymin = std::min(std::min(std::min(ul[1], ur[1]), lr[1]), ll[1]);
    double ymax = std::max(std::max(std::max(ul[1], ur[1]), lr[1]), ll[1]);

    box_xmin = std::min(box_xmin, xmin);
    box_xmax = std::max(box_xmax, xmax);
    box_ymin = std::min(box_ymin, ymin);
    box_ymax = std::max(box_ymax, ymax);

   /* if (imageKWL.GetSize() > 0)
      {
      itk::EncapsulateMetaData<ImageKeywordListType>(outputPtr->GetMetaDataDictionary(),
                                                     MetaDataKey::OSSIMKeywordlistKey, imageKWL);
      }*/

    }

  // Choose origin
  typename TOutputDEMImage::PointType outOrigin;
  outOrigin[0] = box_xmin;
  outOrigin[1] = box_ymax;
  outputPtr->SetOrigin(outOrigin);

  // Compute step :
  // TODO : use a clean RS transform instead
  typename TOutputDEMImage::SpacingType outSpacing;
  //std::cout<<" GrisStep "<<m_DEMGridStep<<std::endl;
  outSpacing[0] = 57.295779513 * m_DEMGridStep / (6378137.0 * vcl_cos((box_ymin + box_ymax) * 0.5 * 0.01745329251));
  outSpacing[1] = -57.295779513 * m_DEMGridStep / 6378137.0;
  outputPtr->SetSpacing(outSpacing);
  // Compute output size
  typename TOutputDEMImage::RegionType outRegion;
  outRegion.SetIndex(0, 0);
  outRegion.SetIndex(1, 0);
  outRegion.SetSize(0, static_cast<unsigned int> ((box_xmax - box_xmin) / vcl_abs(outSpacing[0]) + 1));
  //TODO JGT check the size
  //outRegion.SetSize(1, static_cast<unsigned int> ((box_ymax - box_ymin) / vcl_abs(outSpacing[1])+1));
  outRegion.SetSize(1, static_cast<unsigned int> ((box_ymax - box_ymin) / vcl_abs(outSpacing[1])));
  outputPtr->SetLargestPossibleRegion(outRegion);
  outputPtr->SetNumberOfComponentsPerPixel(1);


  itk::MetaDataDictionary & dictOutput =outputPtr->GetMetaDataDictionary();
   itk::EncapsulateMetaData<std::string> (dictOutput, MetaDataKey::ProjectionRefKey,
                                             static_cast<std::string>(otb::GeoInformationConversion::ToWKT(4326)));

  //test if WGS 84 -> true -> nothing to do

  //false project

  bool isWGS84 =!(m_ProjectionRef.compare( static_cast<std::string>(otb::GeoInformationConversion::ToWKT(4326))));
 if(!m_ProjectionRef.empty() && !isWGS84)
   {

   typename OutputParametersEstimatorType::Pointer genericRSEstimator = OutputParametersEstimatorType::New();

   genericRSEstimator->SetInput(outputPtr);
  // genericRSEstimator->SetInputProjectionRef( static_cast<std::string>(otb::GeoInformationConversion::ToWKT(4326)));
   genericRSEstimator->SetOutputProjectionRef(m_ProjectionRef);
   genericRSEstimator->Compute();
   outputPtr->SetSpacing(genericRSEstimator->GetOutputSpacing());
   outputPtr->SetOrigin(genericRSEstimator->GetOutputOrigin());

    // Compute output size
   typename TOutputDEMImage::RegionType outRegion;
     outRegion.SetIndex(0, 0);
     outRegion.SetIndex(1, 0);
     outRegion.SetSize(0, genericRSEstimator->GetOutputSize()[0]);
     //TODO JGT check the size
     //outRegion.SetSize(1, static_cast<unsigned int> ((box_ymax - box_ymin) / vcl_abs(outSpacing[1])+1));
     outRegion.SetSize(1, genericRSEstimator->GetOutputSize()[1]);
     outputPtr->SetLargestPossibleRegion(outRegion);
     outputPtr->SetNumberOfComponentsPerPixel(1);


  itk::MetaDataDictionary & dict =outputPtr->GetMetaDataDictionary();

  itk::EncapsulateMetaData<std::string> (dict, MetaDataKey::ProjectionRefKey,
                                             m_ProjectionRef);

   }
  this->Modified();
}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
void Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::GenerateOutputInformation()
{
  //
  TOutputDEMImage * outputPtr = this->GetDEMOutput();
  //
  if (this->m_OutputParametersFrom3DMap == -2)
    {
    outputPtr->SetOrigin(m_OutputOrigin);
    outputPtr->SetSpacing(m_OutputSpacing);

    typename TOutputDEMImage::RegionType outRegion;
    outRegion.SetIndex(m_OutputStartIndex);
    outRegion.SetSize(m_OutputSize);
    outputPtr->SetLargestPossibleRegion(outRegion);
    outputPtr->SetNumberOfComponentsPerPixel(1);

    if (!m_ProjectionRef.empty())
      {
      // fill up the metadata information for ProjectionRef
      itk::MetaDataDictionary& dict = this->GetOutput()->GetMetaDataDictionary();
      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, m_ProjectionRef);
      }

    }
  else
    {
    this->SetOutputParametersFromImage();
    }

  if (!m_ProjectionRef.empty())
    {
    OGRSpatialReference oSRS;
    char *wkt = const_cast<char *> (m_ProjectionRef.c_str());
    oSRS.importFromWkt(&wkt);
    m_IsGeographic = oSRS.IsGeographic(); // TODO check if this test is valid for all projection systems
    }


}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
void Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::GenerateInputRequestedRegion()
{
  const TOutputDEMImage * outputDEM = this->GetDEMOutput();

  typename TOutputDEMImage::RegionType outRegion = outputDEM->GetRequestedRegion();
  typename TOutputDEMImage::PointType outOrigin = outputDEM->GetOrigin();
  typename TOutputDEMImage::SpacingType outSpacing = outputDEM->GetSpacing();

  // up left at elevation min
  TDPointType corners[8];
  corners[0][0] = outOrigin[0] + outSpacing[0] * outRegion.GetIndex(0);
  corners[0][1] = outOrigin[1] + outSpacing[1] * outRegion.GetIndex(1);
  corners[0][2] = m_ElevationMin;
  // up left at elevation max
  corners[1][0] = corners[0][0];
  corners[1][1] = corners[0][1];
  corners[1][2] = m_ElevationMax;
  // up right at elevation min
  corners[2][0] = outOrigin[0] + outSpacing[0] * (outRegion.GetIndex(0) + outRegion.GetSize(0));
  corners[2][1] = outOrigin[1] + outSpacing[1] * outRegion.GetIndex(1);
  corners[2][2] = m_ElevationMin;
  // up right at elevation max
  corners[3][0] = corners[2][0];
  corners[3][1] = corners[2][1];
  corners[3][2] = m_ElevationMax;
  // low right at elevation min
  corners[4][0] = outOrigin[0] + outSpacing[0] * (outRegion.GetIndex(0) + outRegion.GetSize(0));
  corners[4][1] = outOrigin[1] + outSpacing[1] * (outRegion.GetIndex(1) + outRegion.GetSize(1));
  corners[4][2] = m_ElevationMin;
  // low right at elevation max
  corners[5][0] = corners[4][0];
  corners[5][1] = corners[4][1];
  corners[5][2] = m_ElevationMax;
  // low left at elevation min
  corners[6][0] = outOrigin[0] + outSpacing[0] * outRegion.GetIndex(0);
  corners[6][1] = outOrigin[1] + outSpacing[1] * (outRegion.GetIndex(1) + outRegion.GetSize(1));
  corners[6][2] = m_ElevationMin;
  // low left at elevation max
  corners[7][0] = corners[6][0];
  corners[7][1] = corners[6][1];
  corners[7][2] = m_ElevationMax;

  for (unsigned int k = 0; k < this->GetNumberOf3DMaps(); ++k)
    {

    //set requested to largest and check that mask has the same size
    T3DImage *imgPtr = const_cast<T3DImage *> (this->Get3DMapInput(k));

    RSTransformType::Pointer groundToSensorTransform = RSTransformType::New();
    //groundToSensorTransform->SetInputKeywordList(outputDEM->GetImageKeywordlist());
    //groundToSensorTransform->SetInputOrigin(outputDEM->GetOrigin());
    //groundToSensorTransform->SetInputSpacing(outputDEM->GetSpacing());
    groundToSensorTransform->SetInputProjectionRef(m_ProjectionRef);

    groundToSensorTransform->SetOutputKeywordList(imgPtr->GetImageKeywordlist());
    groundToSensorTransform->SetOutputOrigin(imgPtr->GetOrigin());
    groundToSensorTransform->SetOutputSpacing(imgPtr->GetSpacing());
    groundToSensorTransform->InstanciateTransform();

    typename T3DImage::RegionType mapRegion = imgPtr->GetLargestPossibleRegion();

    itk::ContinuousIndex<double, 2> mapContiIndex;
    IndexType maxMapIndex;
    IndexType minMapIndex;
    maxMapIndex[0] = static_cast<long int> (mapRegion.GetIndex(0) + mapRegion.GetSize(0));
    maxMapIndex[1] = static_cast<long int> (mapRegion.GetIndex(1) + mapRegion.GetSize(1));
    minMapIndex[0] = static_cast<long int> (mapRegion.GetIndex(0));
    minMapIndex[1] = static_cast<long int> (mapRegion.GetIndex(1));

    IndexType minMapRequestedIndex;
    IndexType maxMapRequestedIndex;
    for (unsigned int i = 0; i < 8; i++)
      {
      TDPointType tmpSensor = groundToSensorTransform->TransformPoint(corners[i]);

      minMapRequestedIndex[0] = std::min(minMapRequestedIndex[0], static_cast<long int> (tmpSensor[0] - m_Margin[0]));
      minMapRequestedIndex[1] = std::min(minMapRequestedIndex[1], static_cast<long int> (tmpSensor[1] - m_Margin[1]));
      maxMapRequestedIndex[0] = std::max(maxMapRequestedIndex[0], static_cast<long int> (tmpSensor[0] + m_Margin[0]));
      maxMapRequestedIndex[1] = std::max(maxMapRequestedIndex[1], static_cast<long int> (tmpSensor[1] + m_Margin[1]));

      minMapRequestedIndex[0] = std::max(minMapRequestedIndex[0], minMapIndex[0]);
      minMapRequestedIndex[1] = std::max(minMapRequestedIndex[1], minMapIndex[1]);
      maxMapRequestedIndex[0] = std::min(maxMapRequestedIndex[0], maxMapIndex[0]);
      maxMapRequestedIndex[1] = std::min(maxMapRequestedIndex[1], maxMapIndex[1]);
      }

    RegionType largest = imgPtr->GetLargestPossibleRegion();
    RegionType requestedRegion = largest;

    if ((minMapRequestedIndex[0] < maxMapRequestedIndex[0]) && (minMapRequestedIndex[1] < maxMapRequestedIndex[1]))
      {
      requestedRegion.SetSize(0, maxMapRequestedIndex[0] - minMapRequestedIndex[0]);
      requestedRegion.SetSize(1, maxMapRequestedIndex[1] - minMapRequestedIndex[1]);
      requestedRegion.SetIndex(0, minMapRequestedIndex[0]);
      requestedRegion.SetIndex(1, minMapRequestedIndex[1]);

      }
    else
      {
      requestedRegion.SetSize(0, 0);
      requestedRegion.SetSize(1, 0);
      requestedRegion.SetIndex(0, minMapIndex[0]);
      requestedRegion.SetIndex(1, minMapIndex[1]);
      }

    imgPtr->SetRequestedRegion(requestedRegion);
    TMaskImage *mskPtr = const_cast<TMaskImage *> (this->GetMaskInput(k));
    if (mskPtr)
      {

      if (mskPtr->GetLargestPossibleRegion() != largest)
        {
        itkExceptionMacro(<<"mask and map at position "<<k<<" have a different largest region");
        }
      mskPtr->SetRequestedRegion(requestedRegion);
      }
     }
}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
void Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::BeforeThreadedGenerateData()
{
  const TOutputDEMImage * outputDEM = this->GetDEMOutput();

  //create splits
  // for each map we check if the input region can be splitted into threadNb
  m_NumberOfSplit.resize(this->GetNumberOf3DMaps());

  unsigned int maximumRegionsNumber = 1;

  for (unsigned int k = 0; k < this->GetNumberOf3DMaps(); ++k)
    {
    m_MapSplitterList->PushBack(SplitterType::New());
    T3DImage *imgPtr = const_cast<T3DImage *> (this->Get3DMapInput(k));

    typename T3DImage::RegionType requestedRegion = imgPtr->GetRequestedRegion();

    typename T3DImage::SizeType requestedSize=requestedRegion.GetSize();
    unsigned int regionsNumber =0;
    if(requestedSize[0]*requestedSize[1]!=0)
      {
    regionsNumber = m_MapSplitterList->GetNthElement(k)->GetNumberOfSplits(requestedRegion,
                                                                                        this->GetNumberOfThreads());
      }
    m_NumberOfSplit[k] = regionsNumber;
    otbMsgDevMacro( "map " << k << " will be splitted into " << regionsNumber << " regions" );
    if (maximumRegionsNumber < regionsNumber) maximumRegionsNumber = regionsNumber;

    }

  m_TempDEMRegions.clear();
  m_TempDEMAccumulatorRegions.clear();
  //m_ThreadProcessed.resize(maximumRegionsNumber);

  for (unsigned int i = 0; i < maximumRegionsNumber; i++)
    {
    //m_ThreadProcessed[i] = 0;
    typename TOutputDEMImage::Pointer tmpImg = TOutputDEMImage::New();
    tmpImg->SetNumberOfComponentsPerPixel(1); //Two components for mean calculus ?
    tmpImg->SetRegions(outputDEM->GetRequestedRegion());
    tmpImg->Allocate();

    tmpImg->FillBuffer(m_NoDataValue);
    m_TempDEMRegions.push_back(tmpImg);

    typename AccumulatorImageType::Pointer tmpImg2 = AccumulatorImageType::New();
    tmpImg2->SetNumberOfComponentsPerPixel(1); //Two components for mean calculus ?
    tmpImg2->SetRegions(outputDEM->GetRequestedRegion());
    tmpImg2->Allocate();

    tmpImg2->FillBuffer(0.);
    m_TempDEMAccumulatorRegions.push_back(tmpImg2);
    }


}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
void Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::ThreadedGenerateData(
                                                                                        const RegionType & outputRegionForThread,
                                                                                        int threadId)
{
  TOutputDEMImage * outputPtr = this->GetOutput();

  typename OutputImageType::PointType pointRef;
  typename OutputImageType::PointType pointRefStep;
  typename OutputImageType::RegionType requestedRegion = outputPtr->GetRequestedRegion();

  typename TOutputDEMImage::SpacingType step = outputPtr->GetSpacing();

  //convert requested region to Long/Lat

  typename TOutputDEMImage::SizeType size = requestedRegion.GetSize();

  typename TOutputDEMImage::IndexType index = requestedRegion.GetIndex();
  outputPtr->TransformIndexToPhysicalPoint(index, pointRef);
  InputInternalPixelType regionLong1 = pointRef[0];
  InputInternalPixelType regionLat1 = pointRef[1];
  InputInternalPixelType regionLong2 = pointRef[0] + size[0] * step[0];
  InputInternalPixelType regionLat2 = pointRef[1] + size[1] * step[1];
  InputInternalPixelType minLong = std::min(regionLong1, regionLong2);
  InputInternalPixelType minLat = std::min(regionLat1, regionLat2);
  InputInternalPixelType maxLong = std::max(regionLong1, regionLong2);
  InputInternalPixelType maxLat = std::max(regionLat1, regionLat2);

  typedef otb::GenericRSTransform<> RSTransform2DType;

  TOutputDEMImage * tmpDEM = NULL;
  AccumulatorImageType *tmpAcc = NULL;
  typename TOutputDEMImage::RegionType outputRequestedRegion = outputPtr->GetRequestedRegion();

  typename T3DImage::RegionType splitRegion;

  itk::ImageRegionConstIterator<InputMapType> mapIt;
  for (unsigned int k = 0; k < this->GetNumberOf3DMaps(); ++k)
    {
    if (m_NumberOfSplit[k] > 0)
      {
      T3DImage *imgPtr = const_cast<T3DImage *> (this->Get3DMapInput(k));
      TMaskImage *mskPtr = const_cast<TMaskImage *> (this->GetMaskInput(k));
      typename T3DImage::RegionType requestedRegion = imgPtr->GetRequestedRegion();

      typename InputMapType::PointType origin;
      origin = imgPtr->GetOrigin();
      typename InputMapType::SpacingType spacing;
      spacing = imgPtr->GetSpacing();
      RSTransform2DType::Pointer groundTransform;
      if (!this->m_IsGeographic)
        {
        groundTransform = RSTransform2DType::New();
        ImageKeywordListType imageKWL = imgPtr->GetImageKeywordlist();
        //groundTransform->SetInputKeywordList(imageKWL);
        groundTransform->SetInputProjectionRef(static_cast<std::string>(otb::GeoInformationConversion::ToWKT(4326)));
        groundTransform->SetOutputProjectionRef(m_ProjectionRef);
        //groundTransform->SetInputOrigin(origin);
        //groundTransform->SetInputSpacing(spacing);
        groundTransform->InstanciateTransform();
        }

      if (static_cast<unsigned int> (threadId) < m_NumberOfSplit[k])
        {
        splitRegion = m_MapSplitterList->GetNthElement(k)->GetSplit(threadId, m_NumberOfSplit[k],
                                                                    imgPtr->GetRequestedRegion());
        }
      else
        {
        splitRegion = requestedRegion;
        otbMsgDevMacro( "map " << k << " will not be splitted " );
        }
      tmpDEM = m_TempDEMRegions[threadId];
      tmpAcc = m_TempDEMAccumulatorRegions[threadId];

      mapIt = itk::ImageRegionConstIterator<InputMapType>(imgPtr, splitRegion);
      mapIt.GoToBegin();
      itk::ImageRegionConstIterator<MaskImageType> maskIt;
      bool useMask = false;
      if (mskPtr)
        {
        useMask = true;
        maskIt = itk::ImageRegionConstIterator<MaskImageType>(mskPtr, splitRegion);
        maskIt.GoToBegin();
        }

      while (!mapIt.IsAtEnd())
        {
        // check mask value if any
        if (useMask)
          {
          if (!(maskIt.Get() > 0))
            {
            ++mapIt;
            ++maskIt;
            continue;
            }
          }

        MapPixelType position = mapIt.Get();

        //std::cout<<"position"<<position<<std::endl;
        if (!this->m_IsGeographic)
          {
          //std::cout<<"is geographic "<<std::endl;
          typename RSTransform2DType::InputPointType tmpPoint;
          tmpPoint[0] = position[0];
          tmpPoint[1] = position[1];
          RSTransform2DType::OutputPointType groundPosition = groundTransform->TransformPoint(tmpPoint);
          position[0] = groundPosition[0];
          position[1] = groundPosition[1];
          }

        //test if position is in DEM BBOX
        bool isInDEM = (position[0] >= minLong) && (position[0] < maxLong) && (position[1] >= minLat) && (position[1]
            < maxLat);

        // Is point inside DEM area ?
        typename OutputImageType::PointType point2D;
        point2D[0] = position[0];
        point2D[1] = position[1];
        itk::ContinuousIndex<double, 2> continuousIndex;

        //std::cout<<"point2D "<<point2D<<std::endl;
        outputPtr->TransformPhysicalPointToContinuousIndex(point2D, continuousIndex);
        typename OutputImageType::IndexType cellIndex;
        cellIndex[0] = static_cast<int> (vcl_floor(continuousIndex[0]));
        cellIndex[1] = static_cast<int> (vcl_floor(continuousIndex[1]));
        //std::cout<<"cellindex "<<cellIndex<<std::endl;
        //index from physical
        typename OutputImageType::IndexType physCellIndex;
        //double CellIndexLong=(position[0]-outOrigin[0])/step[0];
        //double CellIndexLat=(position[1]-outOrigin[1])/step[1];
        typename OutputImageType::IndexType cellIndex2;

        if (outputRequestedRegion.IsInside(cellIndex))
          {
          //std::cout<<"is inside "<<std::endl;
          // Add point to its corresponding cell (keep maximum)
          DEMPixelType cellHeight = static_cast<DEMPixelType> (position[2]);
          //if (cellHeight > tmpDEM->GetPixel(cellIndex) && cellHeight < static_cast<DEMPixelType>(m_ElevationMax))
          //  {
          // tmpDEM->SetPixel(cellIndex,tmpDEM->GetPixel(cellIndex)+1);

          AccumulatorPixelType accPixel = tmpAcc->GetPixel(cellIndex);
          tmpAcc->SetPixel(cellIndex, tmpAcc->GetPixel(cellIndex) + 1);

          if (accPixel == 0)
            {
            tmpDEM->SetPixel(cellIndex, cellHeight);
            }
          else
            {
            DEMPixelType cellCurrentValue = tmpDEM->GetPixel(cellIndex);

            switch (this->m_CellFusionMode)
              {
              case otb::CellFusionMode::MIN:
                {
                if (cellHeight < cellCurrentValue)
                  {
                  tmpDEM->SetPixel(cellIndex, cellHeight);
                  }
                }
                break;
              case otb::CellFusionMode::MAX:
                {
                if (cellHeight > cellCurrentValue)
                  {
                  tmpDEM->SetPixel(cellIndex, cellHeight);
                  }
                }
                break;
              case otb::CellFusionMode::MEAN:
                {
                tmpDEM->SetPixel(cellIndex, cellCurrentValue + cellHeight);
                }
                break;
              case otb::CellFusionMode::ACC:
                {
                }
                break;
              default:

                itkExceptionMacro(<< "Unexpected value cell fusion mode :"<<this->m_CellFusionMode)
      ;
                break;
              }
            }

          }

        ++mapIt;

        if (useMask) ++maskIt;

        }
      }
    }
}

template<class T3DImage, class TMaskImage, class TOutputDEMImage>
void Multi3DMapToDEMFilter<T3DImage, TMaskImage, TOutputDEMImage>::AfterThreadedGenerateData()
{

  TOutputDEMImage * outputDEM = this->GetOutput();

  //check is that case can occur
  if (m_TempDEMRegions.size() < 1)
    {
    outputDEM->FillBuffer(m_NoDataValue);
    return;
    }

  itk::ImageRegionIterator<OutputImageType> outputDEMIt(outputDEM, outputDEM->GetRequestedRegion());
  itk::ImageRegionIterator<OutputImageType> firstDEMIt(m_TempDEMRegions[0], outputDEM->GetRequestedRegion());
  itk::ImageRegionIterator<AccumulatorImageType> firstDEMAccIt(m_TempDEMAccumulatorRegions[0],
                                                               outputDEM->GetRequestedRegion());
  //we use the first accumulator as global accumulator over tmpAcc for mean calculus

  outputDEMIt.GoToBegin();
  firstDEMIt.GoToBegin();
  firstDEMAccIt.GoToBegin();
  // Copy first DEM

  while (!outputDEMIt.IsAtEnd() && !firstDEMIt.IsAtEnd() && !firstDEMAccIt.IsAtEnd())
    {

    AccumulatorPixelType accPixel = firstDEMAccIt.Get();
    //useless test tempDEm is initialized with NoDataValue
    if (accPixel == 0)
      {
      outputDEMIt.Set(m_NoDataValue);
      }
    else
      {

      DEMPixelType pixelValue = firstDEMIt.Get();

      outputDEMIt.Set(pixelValue);

      if ((this->m_CellFusionMode == otb::CellFusionMode::MEAN) && (m_TempDEMRegions.size() == 1))
        {
        outputDEMIt.Set(firstDEMIt.Get() / static_cast<DEMPixelType> (accPixel));
        }
      if (this->m_CellFusionMode == otb::CellFusionMode::ACC)
        {
        outputDEMIt.Set(static_cast<DEMPixelType> (accPixel));
        }
      }
    ++outputDEMIt;
    ++firstDEMIt;
    ++firstDEMAccIt;
    }

  // Check DEMs from other threads and keep the maximum elevation
  for (unsigned int i = 1; i < m_TempDEMRegions.size(); i++)
    {

    itk::ImageRegionIterator<OutputImageType> tmpDEMIt(m_TempDEMRegions[i], outputDEM->GetRequestedRegion());
    itk::ImageRegionIterator<AccumulatorImageType> tmpDEMAccIt(m_TempDEMAccumulatorRegions[i],
                                                               outputDEM->GetRequestedRegion());

    outputDEMIt.GoToBegin();
    tmpDEMIt.GoToBegin();
    tmpDEMAccIt.GoToBegin();
    firstDEMAccIt.GoToBegin(); //Global Accumulator
    while (!outputDEMIt.IsAtEnd() && !tmpDEMIt.IsAtEnd() && !tmpDEMAccIt.IsAtEnd() && !firstDEMAccIt.IsAtEnd())
      {

      // get the accumulator value
      AccumulatorPixelType accPixel = tmpDEMAccIt.Get();
      if (accPixel != 0)
        {

        DEMPixelType cellCurrentValue = outputDEMIt.Get();
        DEMPixelType cellHeight = tmpDEMIt.Get();
        switch (this->m_CellFusionMode)
          {
          case otb::CellFusionMode::MIN:
            {
            if ((cellHeight < cellCurrentValue) || (cellCurrentValue == m_NoDataValue))
              {
              outputDEMIt.Set(cellHeight);
              }
            }
            break;
          case otb::CellFusionMode::MAX:
            {
            if ((cellHeight > cellCurrentValue) || ((cellCurrentValue == m_NoDataValue)))
              {
              outputDEMIt.Set(cellHeight);
              }
            }
            break;
          case otb::CellFusionMode::MEAN:
            {

            outputDEMIt.Set(
                            cellCurrentValue * static_cast<DEMPixelType> (cellCurrentValue != m_NoDataValue)
                                + cellHeight);
            firstDEMAccIt.Set(firstDEMAccIt.Get() + accPixel);

            }
            break;
          case otb::CellFusionMode::ACC:
            {
            firstDEMAccIt.Set(firstDEMAccIt.Get() + accPixel);
            }
            break;

          default:
            itkExceptionMacro(<< "Unexpected value cell fusion mode :"<<this->m_CellFusionMode);
            break;
          }
        }

      if (i == (m_TempDEMRegions.size() - 1))
        {
        if (this->m_CellFusionMode == otb::CellFusionMode::MEAN)
          {
          if (static_cast<DEMPixelType> (firstDEMAccIt.Get()) != 0)
            {
            outputDEMIt.Set(outputDEMIt.Get() / static_cast<DEMPixelType> (firstDEMAccIt.Get()));
            }
          else
            {
            outputDEMIt.Set(m_NoDataValue);
            }
          }
        else
          if (this->m_CellFusionMode == otb::CellFusionMode::ACC)
            {
            outputDEMIt.Set(static_cast<DEMPixelType> (firstDEMAccIt.Get()));
            }

        }
      ++outputDEMIt;
      ++tmpDEMIt;
      ++tmpDEMAccIt;
      ++firstDEMAccIt;
      }

    }

}

}


#endif
