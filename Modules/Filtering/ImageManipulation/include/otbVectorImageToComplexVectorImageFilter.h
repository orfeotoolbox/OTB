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

#ifndef otVectorbImageToComplexVectorImageFilter_h
#define otVectorbImageToComplexVectorImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include <complex>
#include <limits>
namespace otb
{

namespace Functor
{
template < class InputPixelType , class OutputPixelType > class RealToComplex 
{
  public :
    typedef typename OutputPixelType::ValueType OutputComplexType;
    typedef typename OutputComplexType::value_type OutputType;
    RealToComplex() : m_Size(0) 
    {
      m_Lower = std::numeric_limits< OutputType >::lowest();
      m_Dlower = static_cast< double > ( m_Lower );
      m_Upper = std::numeric_limits< OutputType >::max();
      m_Dupper = static_cast< double > ( m_Upper );
    }
    ~RealToComplex() {}

    void SetSize(unsigned int m)
    {
      m_Size = m;
    }

    OutputPixelType operator()( InputPixelType in )
    {
      OutputPixelType out;
      out.SetSize( m_Size );
      OutputType re , im ;
      double reD , imD ;
      for (unsigned int i = 0 ; i < m_Size ; i++ )
        {
        re = static_cast < OutputType > ( in[ 2 * i ] );
        reD = static_cast < double > ( in[ 2 * i ] );
        im = static_cast < OutputType > ( in[ 2 * i + 1 ] );
        imD = static_cast < double > ( in[ 2 * i + 1 ] );
        if ( reD <= m_Dlower )
          re = m_Lower;
        else if ( m_Dupper <= reD )
          re = m_Upper;
        if ( imD <= m_Dlower )
          im = m_Lower;
        else if ( m_Dupper <= imD )
          im = m_Upper; 

        out[i] = std::complex < OutputType > ( re , im );
        }
      return out;
    }

  private:
    unsigned int m_Size;
    OutputType m_Lower , m_Upper;
    double m_Dlower , m_Dupper;

};

} // end namespace functor 



template < class TInputImage , class TOutputImage >
class VectorImageToComplexVectorImageFilter : 
public itk::UnaryFunctorImageFilter<
      TInputImage, TOutputImage,
      Functor::RealToComplex<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
{
public :
  /** Standard typedefs */
  typedef VectorImageToComplexVectorImageFilter Self;
  typedef Functor::RealToComplex< typename TInputImage::PixelType,
    typename TOutputImage::PixelType> FunctorType;

  typedef itk::UnaryFunctorImageFilter< TInputImage, 
    TOutputImage, FunctorType > Superclass;

  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro( Self );

  /** Creation through object factory macro */
  itkTypeMacro( VectorImageToComplexVectorImageFilter , ImageToImageFilter );

  void SetSize( unsigned int m )
  {
    m_Size = m;
    m_SizeFlag = true;
  }

protected :
  VectorImageToComplexVectorImageFilter(): m_Size(0) , m_SizeFlag(false) {}
  ~VectorImageToComplexVectorImageFilter() override {}

  void UpdateOutputInformation() override 
  {
    Superclass::UpdateOutputInformation();
    typename TOutputImage::Pointer output = this->GetOutput();
    if ( !m_SizeFlag )
      {
      typename TInputImage::ConstPointer input = this->GetInput();
      m_Size = input -> GetNumberOfComponentsPerPixel() / 2;  
      }
    this->GetFunctor().SetSize( m_Size );
    output->SetVectorLength( m_Size );
  };

private :
  VectorImageToComplexVectorImageFilter(const Self&) = delete ;
  void operator=(const Self&) = delete ;
  unsigned int m_Size;
  bool m_SizeFlag;
};

} // end namespace otb

#endif
