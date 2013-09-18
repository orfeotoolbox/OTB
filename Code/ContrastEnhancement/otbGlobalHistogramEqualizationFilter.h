/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbGlobalHistogramEqualizationFilter_h
#define __otbGlobalHistogramEqualizationFilter_h

#include "itkImageToImageFilter.h"
#include "itkHistogram.h"
#include "itkArray.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{


template<class TInput, class TOutput>
class ITK_EXPORT GlobalHistogramEqualizationFilter :
    public itk::ImageToImageFilter <TInput, TOutput>
{
public:
  /** Standard class typedefs. */
  typedef GlobalHistogramEqualizationFilter          Self;
  typedef itk::ImageToImageFilter<TInput, TOutput>   Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  typedef typename TInput::InternalPixelType   InternalPixelType;
  typedef typename itk::NumericTraits< InternalPixelType >::RealType MeasurementType;
  typedef itk::Statistics::Histogram< MeasurementType, 1 > HistogramType;
  typedef typename HistogramType::Pointer HistogramPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Self, Superclass);

  typedef typename TInput::RegionType          RegionType;
  typedef typename TInput::Pointer            InputImagePointer;
  typedef typename TInput::PixelType           PixelType;

  itkGetConstMacro(MinimumRange, InternalPixelType)
  itkSetMacro(MinimumRange, InternalPixelType);

  void SetHistogram(HistogramPointerType histogram)
  {
    m_Histogram = histogram;
  }

  HistogramPointerType GetHistogram()
  {
    return m_Histogram;
  }

  virtual void BeforeThreadedGenerateData();

  virtual void ThreadedGenerateData(const RegionType&
                                     outputRegion, int threadId);


protected:
  GlobalHistogramEqualizationFilter()
    {
//    m_LookupArray = ArrayType::New();
    this->SetNumberOfRequiredInputs( 1 );
    }
  virtual ~GlobalHistogramEqualizationFilter(){}
  void PrintSelf(std::ostream& os, itk::Indent indent) const { }

  /** Generate data method. */
//  void GenerateData();

  /** Adaptive histogram equalization requires more input that it
   * outputs. It needs request an input region that is padded by the
   * radius.
  * \sa ProcessObject::GenerateInputRequestedRegion() */
  //void GenerateInputRequestedRegion();


private:
  GlobalHistogramEqualizationFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  HistogramPointerType  m_Histogram;
  InternalPixelType     m_MinimumRange;
  unsigned int          m_NumberOfBins;

 typedef itk::Array<unsigned long> ArrayType;

  ArrayType m_LookupArray;

  //bool         m_NoDataFlag;
  //NoDataType   m_NoData;
  //int          m_NumberOfBins;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGlobalHistogramEqualizationFilter.txx"
#endif

#endif
