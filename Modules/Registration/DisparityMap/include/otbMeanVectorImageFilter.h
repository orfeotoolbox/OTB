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


#ifndef otbMeanVectorImageFilter_h
#define otbMeanVectorImageFilter_h

#include "otbBinaryFunctorNeighborhoodVectorImageFilter.h"
#include <itkConstNeighborhoodIterator.h>


namespace otb
{
namespace Functor
{
/**\class MeanVectorFunctor
 * \brief This functor performs the multiplication between the local mean of the first image and the pixel value of the second image. 
 * It is templated by the type of inputs images and the output image, and is using two neighborhood iterators as inputs. 
 *
 * \ingroup OTBDisparityMap
 */
template<class TInput1, class TInput2, class TOutput>
class MeanVectorFunctor
{
public:
  int  GetNumberOfComponentsPerPixel() const
    {
    return m_numberOfComponents;   
    }

  unsigned char GetRadiusMin() const
    {
     return m_RadiusMin;     
    }

  unsigned char GetRadiusMax() const
    {
    return m_RadiusMax;
    }

  void SetRadius(std::uint8_t min, std::uint8_t max) 
    {
    std::tie(m_RadiusMin, m_RadiusMax) = std::minmax(min, max);
    }

  void SetNumberOfComponent( unsigned int nb)
    {
    m_numberOfComponents = nb;
    }

  typedef typename TInput2::PixelType               PixelType;

  unsigned int m_bandNumberInput1 ;
  unsigned int m_bandNumberInput2 ;
  unsigned int m_bandNumberOutput ;
  unsigned int m_WSize ;
  


  TOutput operator() (TInput1 input_weights, TInput2 input_I) const
    {      
    PixelType I_pixel = input_I.GetCenterPixel();   


    TOutput output(m_bandNumberOutput); 
    output.Fill(0);     
    std::vector<double> v_mean_ai(m_bandNumberInput1); 

    for(unsigned int b = 0 ; b < m_bandNumberInput1 ; ++b )
      {
        double mean_ak(0.);
        for(unsigned int i = 0 ; i < input_weights.Size() ; ++i)
          {
          mean_ak += input_weights.GetPixel(i)[b] ;
          }
          mean_ak /= m_WSize*m_WSize ;
          v_mean_ai[b] = mean_ak ;


      }   

    for(unsigned int b = 0 ; b < m_bandNumberOutput ; ++b)
      {
      double qi(0.);
      if(m_bandNumberInput2>1)
        {
        qi = v_mean_ai[(m_bandNumberInput2+1)*b] * I_pixel[0] + v_mean_ai[(m_bandNumberInput2+1)*b+1] * I_pixel[1] + v_mean_ai[(m_bandNumberInput2+1)*b+2] * I_pixel[2] + v_mean_ai[(m_bandNumberInput2+1)*b+3] ;
        }
      else
        {
        qi = v_mean_ai[(m_bandNumberInput2+1)*b] * I_pixel[0] + v_mean_ai[(m_bandNumberInput2+1)*b+1] ;
        }         
      output[b] = static_cast<typename TOutput::ValueType>(qi);
      }
    return output ;
    }

  private:
    unsigned char                   m_RadiusMin;
    unsigned char                   m_RadiusMax;
    unsigned int                    m_numberOfComponents ;

};

} // end namespace functor


/**\class MeanVectorFunctor
 * \brief Performs the multiplication between the local mean of the first image and the pixel value of the second image. 
 * It is templated by the type of inputs images and the output image, and is using two neighborhood iterators as inputs. 
 *
 * \ingroup OTBDisparityMap
 */
template <class TInputImage1, class TInputImage2,  class TOutputImage>
class ITK_EXPORT MeanVectorImageFilter :
    public otb::BinaryFunctorNeighborhoodVectorImageFilter<
        TInputImage1,TInputImage2, TOutputImage,
        Functor::MeanVectorFunctor<
            typename itk::ConstNeighborhoodIterator<TInputImage1>,
            typename itk::ConstNeighborhoodIterator<TInputImage2>,
            typename TOutputImage::PixelType> >
  {
public:
  /** Standard class typedefs. */
  typedef MeanVectorImageFilter Self;
  typedef typename otb::BinaryFunctorNeighborhoodVectorImageFilter<
      TInputImage1,
      TInputImage2,
      TOutputImage,
      Functor::MeanVectorFunctor<
          typename itk::ConstNeighborhoodIterator<TInputImage1>,
          typename itk::ConstNeighborhoodIterator<TInputImage2>,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::ConstNeighborhoodIterator<TInputImage1> NeighborhoodIteratorType;
  typedef typename NeighborhoodIteratorType::RadiusType   RadiusType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  int GetRadiusMax()
    {
     return this->GetFunctor().GetRadiusMax();     
    }

protected:
  MeanVectorImageFilter()=default ;
  ~MeanVectorImageFilter() override =default ;

  virtual void GenerateInputRequestedRegion() override
    {
    // this->GetOutput()->GetRequestedRegion();
      // call the superclass' implementation of this method
    Superclass::GenerateInputRequestedRegion();

    // get pointers to the input and output
    typename Superclass::InputImagePointer inputPtr1 = const_cast< TInputImage1 * >( this->GetInput() );
    typename Superclass::InputImagePointer inputPtr2 = const_cast< TInputImage2 * >( this->GetInput() );

    if ( !inputPtr1 || !inputPtr2 )
      {
      return;
      }

    // get a copy of the input requested region (should equal the output
    // requested region)
    typename TInputImage1::RegionType inputRequestedRegion1;
    inputRequestedRegion1 = inputPtr1->GetRequestedRegion();
    typename TInputImage2::RegionType inputRequestedRegion2;
    inputRequestedRegion2 = inputPtr2->GetRequestedRegion();

    // pad the input requested region by the operator radius
    inputRequestedRegion1.PadByRadius(this->GetRadiusMax());
    inputRequestedRegion2.PadByRadius(this->GetRadiusMax());

    // crop the input requested region at the input's largest possible region
    if ( inputRequestedRegion1.Crop( inputPtr1->GetLargestPossibleRegion() ))
      {
      inputPtr1->SetRequestedRegion(inputRequestedRegion1);
      return;
      }
    else
      {
      // Couldn't crop the region (requested region is outside the largest
      // possible region).  Throw an exception.
      // store what we tried to request (prior to trying to crop)
      inputPtr1->SetRequestedRegion(inputRequestedRegion1);
      // build an exception
      itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
      std::ostringstream          msg;
      msg << static_cast< const char * >( this->GetNameOfClass() )
          << "::GenerateInputRequestedRegion()";
      e.SetLocation( msg.str().c_str() );
      e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
      e.SetDataObject(inputPtr1);
      throw e;
      }

    if ( inputRequestedRegion2.Crop( inputPtr2->GetLargestPossibleRegion() ))
      {
      inputPtr2->SetRequestedRegion(inputRequestedRegion2);
      return;
      }
    else
      {
      // Couldn't crop the region (requested region is outside the largest
      // possible region).  Throw an exception.
      // store what we tried to request (prior to trying to crop)
      inputPtr2->SetRequestedRegion(inputRequestedRegion2);
      // build an exception
      itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
      std::ostringstream          msg;
      msg << static_cast< const char * >( this->GetNameOfClass() )
          << "::GenerateInputRequestedRegion()";
      e.SetLocation( msg.str().c_str() );
      e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
      e.SetDataObject(inputPtr2);
      throw e;
      }
    } 

  void GenerateOutputInformation(void) override
    {
    Superclass::GenerateOutputInformation();
    this->GetFunctor().m_bandNumberInput1 = this->GetInput(0)->GetNumberOfComponentsPerPixel() ;  
    this->GetFunctor().m_bandNumberInput2 = this->GetInput(1)->GetNumberOfComponentsPerPixel() ; 
    assert(this->GetFunctor().m_bandNumberInput1  % (this->GetFunctor().m_bandNumberInput2+1) == 0) ;
    this->GetFunctor().m_bandNumberOutput = this->GetFunctor().m_bandNumberInput1/(this->GetFunctor().m_bandNumberInput2+1) ;

    this->GetOutput()->SetNumberOfComponentsPerPixel( this->GetFunctor().m_bandNumberOutput );
    this->GetFunctor().SetNumberOfComponent( this->GetFunctor().m_bandNumberOutput );

    this->GetFunctor().m_WSize = (this->GetFunctor().GetRadiusMax())*2+1;

    } 

private:
  MeanVectorImageFilter(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented

}; // end of MeanVectorImageFilter class

} // end namespace otb

#endif

