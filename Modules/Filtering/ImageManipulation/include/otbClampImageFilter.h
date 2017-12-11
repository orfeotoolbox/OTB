/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbClampImageFilter_h
#define otbClampImageFilter_h

#include "otbConvertTypeFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{

/** \class ClampImageFilter
 * \brief Set image values to a user-specified value if they are below,
 * above, or between simple threshold values.
 *
 * ClampImageFilter  clamp image values to be between an upper
 * and lower value. Values lower than m_Lower values are set to lower,
 * and values greater than upper threshold are set to upper threshold
 * value.
 *
 * By default lower and upper thresholds are set to the maximum and
 * minimum bounds of the image pixel type.
 *
 * The pixels must support the operators >= and <=.
 *
 * \ingroup IntensityImageFilters Multithreaded
 *
 * \ingroup OTBImageManipulation
 */
  template <class TInputImage, class TOutputImage=TInputImage>
  class ITK_EXPORT ClampImageFilter 
  : public itk::UnaryFunctorImageFilter< TInputImage , TOutputImage ,
      Functor::ConvertTypeFunctor <typename TInputImage::PixelType ,
                                   typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef ClampImageFilter Self;
  typedef itk::UnaryFunctorImageFilter< TInputImage , TOutputImage ,
    Functor::ConvertTypeFunctor <typename TInputImage::PixelType ,
                                 typename TOutputImage::PixelType> >  Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( ClampImageFilter , itk::UnaryFunctorImageFilter );


  /** Some additional typedefs.  */
  typedef TInputImage                                  InputImageType;
  typedef typename InputImageType::RegionType          InputImageRegionType;
  typedef typename InputImageType::PixelType           InputImagePixelType;

  /** Some additional typedefs.  */
  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::PixelType OutputImagePixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
  typedef typename itk::NumericTraits< OutputInternalPixelType >::ValueType OutputPixelValueType;


  /** The values greater than or equal to the value are set to OutsideValue. */
  void ClampAbove(const OutputPixelValueType &thresh);

  /** The values less than or equal to the value are set to OutsideValue. */
  void ClampBelow(const OutputPixelValueType &thresh);

  /** The values outside the range are set to OutsideValue. */
  void ClampOutside(const OutputPixelValueType &lower, const OutputPixelValueType &upper);

  /** Set/Get methods to set the lower threshold */
  void SetLower(OutputPixelValueType val)
  {
    m_Lower = val;
    m_DLower = static_cast<double>(val);
    this->GetFunctor().SetLowest( m_Lower );
    this->Modified();
  }
  itkGetConstMacro(Lower, OutputPixelValueType);

  /** Set/Get methods to set the upper threshold */
  void SetUpper(OutputPixelValueType val)
  {
    m_Upper = val;
    m_DUpper = static_cast<double>(val);
    this->GetFunctor().SetHighest( m_Upper );
    this->Modified();
  }
  itkGetConstMacro(Upper, OutputPixelValueType);


protected:
  ClampImageFilter();
  ~ClampImageFilter() ITK_OVERRIDE {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /** ClampImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  // void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
  //                           itk::ThreadIdType threadId ) ITK_OVERRIDE;

  void GenerateOutputInformation(void) ITK_OVERRIDE
  {
    Superclass::GenerateOutputInformation();

    // typename InputImageType::IndexType index;
    // index.Fill(0);
    // InputImagePixelType px = ->GetPixel( index );

    unsigned int sizeIn = this->GetInput()->GetNumberOfComponentsPerPixel();
    // sizeIn *= itk::NumericTraits < typename itk::NumericTraits< InputImagePixelType >::ValueType > 
    //   :: GetLength();

    this->GetOutput()->SetNumberOfComponentsPerPixel( 
      this->GetFunctor().GetOutputSize ( sizeIn ) );
  }

private:
  ClampImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double m_DLower;
  double m_DUpper;

  OutputPixelValueType m_Lower;
  OutputPixelValueType m_Upper;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbClampImageFilter.txx"
#endif

#endif
