/*
 * Copyright (C) 2017-2018 CS Systemes d'Information (CS SI)
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


#ifndef otbMinimumVectorImageFilter_h
#define otbMinimumVectorImageFilter_h

#include "otbUnaryFunctorVectorImageFilter.h"

namespace otb
{

namespace Functor
{
/** \class MinOperator
 *  \brief Functor to extract the index of the minimum value in a volume
 *
 * \ingroup OTBDisparityMap
 */
template<class TInput, class TOutput>
class MinOperator
{
public:

  unsigned int m_Size ;
  int m_coef ;

  TOutput operator() ( TInput input ) const
    { 
    const auto pos = std::min_element(&input[0], &input[m_Size]);
    const auto index = std::distance(pos, &input[0]);
    return TOutput(index*m_coef) ;
    }
}; //end class

} // end Functor


/** \class MinimumVectorImageFilter
 *  \brief Get the index of the minimum value in a volume
 *
 * \ingroup OTBDisparityMap
 */
template <class TInputImage, class TOutputImage>
  class ITK_EXPORT MinimumVectorImageFilter :
    public otb::UnaryFunctorVectorImageFilter<
        TInputImage, TOutputImage,
        Functor::MinOperator<
            typename TInputImage::PixelType,
            typename TOutputImage::PixelType >>
  {
  public:
  /** Standard class typedefs. */
  typedef MinimumVectorImageFilter Self;
  typedef typename otb::UnaryFunctorVectorImageFilter<
      TInputImage,
      TOutputImage,      
      Functor::MinOperator<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  enum class Side {left, right};

  void SetSide(Side s)
  {
    m_Side = s;
  }
  Side GetSide() const
  {
    return m_Side;
  }
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);


  protected:  
  Side m_Side;
  MinimumVectorImageFilter() =default ;
  ~MinimumVectorImageFilter() override =default ;

  void GenerateOutputInformation(void) override
    {
    Superclass::GenerateOutputInformation();
    this->GetOutput()->SetNumberOfComponentsPerPixel(1);
    this->GetFunctor().m_Size = this->GetInput()->GetNumberOfComponentsPerPixel() ;    
    switch(m_Side)
      {
      case Side::left:
        {     
        this->GetFunctor().m_coef = 1 ;
        }
      break;
      case Side::right:
        {
        this->GetFunctor().m_coef = -1 ;
        }
      break;
      }
    }

  private:
  MinimumVectorImageFilter(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented


  }; // end of MinimumVectorImageFilter class

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif