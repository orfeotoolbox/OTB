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
#ifndef __otbUrbanAreaDetectionImageFilter_h
#define __otbUrbanAreaDetectionImageFilter_h

#include "otbImage.h"
#include "otbWaterIndicesFunctor.h"
#include "otbMultiChannelRAndGAndNIRIndexImageFilter.h"
#include "itkImageToImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

#include "itkSobelEdgeDetectionImageFilter.h"
#include "otbEdgeDetectorImageFilter.h"
#include "otbEdgeDensityImageFilter.h"
#include "otbBinaryImageDensityFunction.h"
#include "otbVectorImageToIntensityImageFilter.h"

namespace otb
{

namespace Functor
{

/** \class RadiometricNonWaterNonVegetationIndexFunctor
 *  \brief This functor computes ndwi and ndvi of an image
 *  \brief in order to find urban areas and generate a grayscale image
 */
template<class TInput, class TOutput = double>
class RadiometricNonWaterNonVegetationIndexFunctor
{
public:
  typedef Functor::NDVI<double, double, double>  VegetationFunctorType;
  typedef Functor::NDWI2<double, double, double> WaterFunctorType;
  typedef TOutput                                ValueType;

  VegetationFunctorType GetVegetationFunctor(){ return m_VegetationFunctor; }
  WaterFunctorType GetWaterFunctor(){ return m_WaterFunctor; }

  void SetRedIndex(int id){ m_VegetationFunctor.SetRedIndex(id); }
  void SetGreenIndex(int id){ m_WaterFunctor.SetGIndex(id); }
  void SetNIRIndex(int id)
  {
    m_VegetationFunctor.SetNIRIndex(id);
    m_WaterFunctor.SetNIRIndex(id);
  }

  RadiometricNonWaterNonVegetationIndexFunctor(){}
  virtual ~RadiometricNonWaterNonVegetationIndexFunctor(){}

  inline TOutput operator ()(const TInput& pInPix)
  {
    TOutput lVIval, lWIval, lOutPix;

    TInput zero = pInPix;
    zero.Fill(0);
    if (pInPix != zero)
      {
      lVIval = static_cast<ValueType>(std::max(0., m_VegetationFunctor(pInPix)));
      lWIval = static_cast<ValueType>(std::max(0., m_WaterFunctor(pInPix)));

      lOutPix = vcl_sqrt(vcl_abs((1 - lVIval) * (1 - lWIval)));
      }
    else lOutPix = 0;

    return lOutPix;
  }

protected:
  VegetationFunctorType m_VegetationFunctor;
  WaterFunctorType      m_WaterFunctor;

};

/** \class RadiometricNonWaterNonVegetationDetectionFunctor
 *  \brief This functor uses computes RadiometricNonWaterNonVegetationIndexFunctor
 *  \brief and appli a threshold to produce a binary image
 */
template<class TInput, class TOutput>
class RadiometricNonWaterNonVegetationDetectionFunctor
{
public:
  typedef RadiometricNonWaterNonVegetationIndexFunctor<TInput> RadiometricNonWaterNonVegetationIndexFunctorType;

  RadiometricNonWaterNonVegetationDetectionFunctor()
  {
    m_LowerThreshold = 0.0;
    m_UpperThreshold = 1.0;
  }

  virtual ~RadiometricNonWaterNonVegetationDetectionFunctor() {}
  inline TOutput operator ()(const TInput& inPix)
  {
    // The vegetation and water areas are put to 0.
    double indexesVal = static_cast<double>(m_RadiometricNonWaterNonVegetationIndexFunctor(inPix));

    if ((indexesVal > m_LowerThreshold) && (indexesVal <= m_UpperThreshold))
      {
      return static_cast<TOutput>(1);
      }
    else
      {
      return static_cast<TOutput>(0);
      }
  }

  void SetLowerThreshold(double threshold){ m_LowerThreshold = threshold; }
  void SetUpperThreshold(double threshold){ m_UpperThreshold = threshold; }
  double GetLowerThreshold(){ return m_LowerThreshold; }
  double GetUpperThreshold(){ return m_UpperThreshold; }

  void SetRedIndex(int id){ m_RadiometricNonWaterNonVegetationIndexFunctor.SetRedIndex(id); }
  void SetGreenIndex(int id){ m_RadiometricNonWaterNonVegetationIndexFunctor.SetGreenIndex(id); }
  void SetNIRIndex(int id){ m_RadiometricNonWaterNonVegetationIndexFunctor.SetNIRIndex(id); }

protected:
  RadiometricNonWaterNonVegetationIndexFunctorType m_RadiometricNonWaterNonVegetationIndexFunctor;
  double                                           m_LowerThreshold;
  double                                           m_UpperThreshold;

};

} //end namespace functor

/** \class UrbanAreaDetectionImageFilter
 *  \brief This filter uses associated functors to produce a binary or grayscale image
 *  \brief where the white areas represents the urban areas.
 */
template <class TInputImage, class TOutputImage,
    class TFunction = Functor::RadiometricNonWaterNonVegetationIndexFunctor<
        typename TInputImage::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT UrbanAreaDetectionImageFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef UrbanAreaDetectionImageFilter                      Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UrbanAreaDetectionImageFilter, itk::ImageToImageFilter);

  /** Images typedefs */
  typedef TInputImage                         VectorImageType;
  typedef TOutputImage                        OutputImageType;
  typedef double                              SingleImagePixelType;
  typedef Image<SingleImagePixelType, 2>      SingleImageType;
  typedef SingleImageType::SizeType           SizeType;
  typedef typename OutputImageType::PixelType OutputImagePixelType;
  typedef typename VectorImageType::PixelType VectorImagePixelType;

  /** Filters typedefs */
  // NonVegetationNonWaterIndexFilter
  typedef Functor::RadiometricNonWaterNonVegetationDetectionFunctor<VectorImagePixelType,
      OutputImagePixelType> FunctorType;
  typedef MultiChannelRAndGAndNIRIndexImageFilter <VectorImageType, OutputImageType,
      FunctorType>                           UrbanAreaExtractionFilterType;
  typedef typename UrbanAreaExtractionFilterType::Pointer
  UrbanAreaExtrationFilterPointerType;
  // Erode/Dilate Filters
  typedef typename itk::BinaryBallStructuringElement<OutputImagePixelType,
      2>                 StructuringElementType;
  typedef typename itk::BinaryErodeImageFilter<OutputImageType, OutputImageType,
      StructuringElementType>  ErodeFilterType;
  typedef typename itk::BinaryDilateImageFilter<OutputImageType, OutputImageType,
      StructuringElementType> DilateFilterType;
  // Intensity
  typedef VectorImageToIntensityImageFilter<VectorImageType, SingleImageType> IntensityFilterType;
  typedef typename IntensityFilterType::Pointer                               IntensityFilterPointerType;
  // EdgeDensityFilter
  typedef BinaryImageDensityFunction<SingleImageType>
  CountFunctionType;
  typedef itk::SobelEdgeDetectionImageFilter<SingleImageType, SingleImageType> SobelType;
  typedef EdgeDetectorImageFilter<SingleImageType, SingleImageType,
      SobelType>                           SobelDetectorType;
  typedef EdgeDensityImageFilter<SingleImageType, SingleImageType, SobelDetectorType,
      CountFunctionType> EdgeDensityFilterType;
  typedef typename EdgeDensityFilterType::Pointer
  EdgeDensityFilterPointerType;
  // Threshold
  typedef itk::BinaryThresholdImageFilter<SingleImageType, OutputImageType> ThresholdFilterType;
  typedef typename ThresholdFilterType::Pointer                             ThresholdFilterPointerType;
  // Multiply
  typedef itk::MultiplyImageFilter<VectorImageType, OutputImageType, VectorImageType> MultiplyImageFilterType;
  typedef typename MultiplyImageFilterType::Pointer                                   MultiplyImageFilterPointerType;

  /** Get/Set indices */
  void SetRedIndex(int id){ m_UrbanAreaExtractionFilter->SetRedIndex(id); }
  void SetGreenIndex(int id){ m_UrbanAreaExtractionFilter->SetGreenIndex(id); }
  void SetNIRIndex(int id){ m_UrbanAreaExtractionFilter->SetNIRIndex(id); }
  unsigned int GetRedIndex(){ return m_UrbanAreaExtractionFilter->GetRedIndex(); }
  unsigned int GetGreenIndex(){ return m_UrbanAreaExtractionFilter->GetGreenIndex(); }
  unsigned int GetNIRIndex(){ return m_UrbanAreaExtractionFilter->GetNIRIndex(); }
  /** Get/Set threshold values */
  itkGetMacro(ThresholdValue, double);
  itkSetMacro(ThresholdValue, double);
  itkGetMacro(ThresholdDensity, double);
  itkSetMacro(ThresholdDensity, double);
  itkGetMacro(SobelLowerThreshold, double);
  itkSetMacro(SobelLowerThreshold, double);
  itkGetMacro(SobelUpperThreshold, double);
  itkSetMacro(SobelUpperThreshold, double);

  /** Methods */
  virtual void GenerateData();

protected:
  UrbanAreaDetectionImageFilter();
  virtual ~UrbanAreaDetectionImageFilter(){}

private:
  UrbanAreaDetectionImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Filters */
  IntensityFilterPointerType          m_IntensityFilter;
  EdgeDensityFilterType::Pointer      m_EdgeDensityFilter;
  SobelDetectorType::Pointer          m_SobelFilter;
  ThresholdFilterPointerType          m_Thresholder;
  UrbanAreaExtrationFilterPointerType m_UrbanAreaExtractionFilter;
  MultiplyImageFilterPointerType      m_MultiplyFilter;

  /** Thresholds */
  double m_ThresholdValue;
  double m_ThresholdDensity;

  double m_SobelLowerThreshold;
  double m_SobelUpperThreshold;

}; // end class RadiometricNonWaterNonVegetationDetectionImageFilter

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUrbanAreaDetectionImageFilter.txx"
#endif

#endif
