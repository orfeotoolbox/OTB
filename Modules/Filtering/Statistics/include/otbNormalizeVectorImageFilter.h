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

#ifndef otbNormalizeVectorImageFilter_h
#define otbNormalizeVectorImageFilter_h

#include <vnl/vnl_math.h>

#include <itkMacro.h>
#include <otbVectorImage.h>
#include <otbUnaryFunctorVectorImageFilter.h>

#include <otbStreamingStatisticsVectorImageFilter.h>

namespace otb
{
namespace Functor
{
/** \brief NormalizeVectorImageFunctor
 * \brief this functor performs affine transformation on VariableLengthVectors
 */
template < class TInput, class TOutput >
class NormalizeVectorImageFunctor
{
public:
  NormalizeVectorImageFunctor() { }
  virtual ~NormalizeVectorImageFunctor () { }

  typedef typename itk::NumericTraits< TInput >::RealType RealVectorType;
  typedef typename  itk::NumericTraits< typename RealVectorType::ValueType >::RealType RealType;

  TOutput operator() ( const TInput & input )
  {
    unsigned int length = input.Size();
    TOutput output ( length );
    for ( unsigned int i = 0; i < length; ++i )
    {
      output[i] = static_cast<typename TOutput::ValueType>(
                    ( static_cast< RealType >( input[i] ) - m_Mean[i] )
                      / m_StdDev[i] );
    }
    return output;
  }

  template < class T >
  void SetMean ( const itk::VariableLengthVector<T> & m )
  {
    m_Mean.SetSize( m.Size() );
    for ( unsigned int i = 0; i < m_Mean.Size(); ++i )
      m_Mean[i] = static_cast< RealType >( m[i] );
  }

  RealVectorType GetMean() const
  {
    return this->m_Mean;
  }

  template < class T>
  void SetStdDev ( const itk::VariableLengthVector<T> & sigma )
  {
    m_StdDev.SetSize( sigma.Size() );
    for ( unsigned int i = 0; i < m_StdDev.Size(); ++i )
    {
      m_StdDev[i] = static_cast< RealType >( sigma[i] );
      if ( m_StdDev[i] == itk::NumericTraits< RealType >::Zero )
      {
        throw itk::ExceptionObject(__FILE__, __LINE__,
                "Cannot divide by zero !", ITK_LOCATION);
      }
    }
  }

  template < class T >
  void SetVariance ( const itk::VariableLengthVector<T> & var )
  {
    m_StdDev.SetSize( var.Size() );
    for ( unsigned int i = 0; i < m_StdDev.Size(); ++i )
    {
      m_StdDev[i] = vcl_sqrt( static_cast< RealType >( var[i] ) );
      if ( m_StdDev[i] == itk::NumericTraits< RealType >::Zero )
      {
        throw itk::ExceptionObject(__FILE__, __LINE__,
                "Cannot divide by zero !", ITK_LOCATION);
      }
    }
  }

  RealVectorType GetStdDev() const
  {
    return this->m_StdDev;
  }

protected:

  RealVectorType m_Mean;
  RealVectorType m_StdDev;
}; // end of class NormalizeVectorImageFunctor

} // end of namespace Functor


/** \class NormalizeVectorImageFilter
 * \brief Normalize an VectorImage by setting its mean to zero and possibly variance to one (band by band).
 *
 * NormalizeVectorImageFilter shifts and scales an image so that the pixels in the image
 * have a zero mean and unit variance.
 *
 * This filter uses StreamingStatisticsVectorImageFilter to compute the mean and variance of
 * the input and then applies the dedicated functor.
 *
 * \sa StreamingStatisticsVectorImageFilter
 *
 * \ingroup OTBStatistics
 */
template < class TInputImage, class TOutputImage >
class ITK_EXPORT NormalizeVectorImageFilter
  : public UnaryFunctorVectorImageFilter< TInputImage, TOutputImage,
            Functor::NormalizeVectorImageFunctor<
              typename TInputImage::PixelType,
              typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs */
  typedef NormalizeVectorImageFilter Self;
  typedef UnaryFunctorVectorImageFilter< TInputImage, TOutputImage,
            Functor::NormalizeVectorImageFunctor<
              typename TInputImage::PixelType,
              typename TOutputImage::PixelType > > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(NormalizeVectorImageFilter, ImageToImageFilter);

  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  typedef typename itk::NumericTraits< typename TInputImage::PixelType >::RealType RealVectorType;
  typedef typename  itk::NumericTraits< typename RealVectorType::ValueType >::RealType RealType;

  typedef StreamingStatisticsVectorImageFilter< InputImageType > CovarianceEstimatorFilterType;
  typedef typename CovarianceEstimatorFilterType::Pointer CovarianceEstimatorFilterPointerType;

  itkGetConstMacro(CovarianceEstimator, CovarianceEstimatorFilterType*);

  template < class T >
  void SetMean ( const itk::VariableLengthVector<T> & m )
  {
    this->GetFunctor().SetMean( m );
    m_IsGivenMean = true;
    m_UseMean = true;
    this->Modified();
  }

  template < class T >
  void SetStdDev ( const itk::VariableLengthVector<T> & sigma )
  {
    this->GetFunctor().SetStdDev( sigma );
    m_IsGivenStdDev = true;
    m_UseStdDev = true;
    this->Modified();
  }

  template < class T >
  void SetVariance ( const itk::VariableLengthVector<T> & var )
  {
    this->GetFunctor().SetVariance( var );
    m_IsGivenStdDev = true;
    m_UseStdDev = true;
    this->Modified();
  }


  itkSetMacro(UseMean, bool);
  itkSetMacro(UseStdDev, bool);

protected:
  NormalizeVectorImageFilter ();
  ~NormalizeVectorImageFilter() override { }

  void GenerateOutputInformation() override;


private:
  NormalizeVectorImageFilter ( const Self & );
  void operator=( const Self & );

  bool m_IsGivenMean;
  bool m_IsGivenStdDev;

  bool m_UseMean;
  bool m_UseStdDev;

  CovarianceEstimatorFilterPointerType m_CovarianceEstimator;

}; // end of class NormalizeVectorImageFilter

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNormalizeVectorImageFilter.hxx"
#endif

#endif // otbNormalizeVectorImageFilter_h

