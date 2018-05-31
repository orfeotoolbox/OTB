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

#ifndef otbMeanVectorImageFilter_h
#define otbMeanVectorImageFilter_h

#include "otbBinaryFunctorNeighborhoodVectorImageFilter.h"
#include <itkConstNeighborhoodIterator.h>


namespace otb
{
namespace Functor
{
/** \class MeanVectorFunctor
 *  \brief This functor generate the mean value of a vector
 *
 * \ingroup OTBImageManipulation
 */
template<class TInput1, class TInput2, class TOutput>
class MeanVectorFunctor
{
public:
  //typedef MeanFunctor<TInput, TOutputValue> MeanVectorFunctorType;

  MeanVectorFunctor(){}
  ~MeanVectorFunctor() {}




  int  GetNumberOfComponentsPerPixel() const
    {
    return m_numberOfComponents;   
    }


   unsigned char GetRadiusMin(void)
    {
     return m_RadiusMax;     
    }

  unsigned char GetRadiusMax(void)
    {
    return m_RadiusMin;
    }

  void SetRadius(const unsigned char& min, const unsigned char& max)
    {
      m_RadiusMin = min < max ? min : max;
      m_RadiusMax = max > min ? max : min;
    }

  void SetNumberOfComponent( unsigned int nb)
  {
    m_numberOfComponents = nb;
  }



  TOutput operator() (TInput1 input_weights, TInput2 input_I)
    {  
     
    unsigned int Nband_weights = input_weights.GetPixel(0).Size();
    unsigned int Nband_I = input_I.GetPixel(0).Size();


    TOutput output(Nband_weights/4); 
    output.Fill(0); 

    
    typename TInput1::RadiusType r_Box = input_weights.GetRadius(); 
    unsigned int r_Mean = r_Box[0];
    unsigned int size_Mean = 2*r_Mean+1;

      

    std::vector<double> v_mean_ai;
    v_mean_ai.resize(Nband_weights,0);

    for(unsigned int b = 0 ; b < Nband_weights ; ++b )
      {
        double mean_ak(0.);
        for(unsigned int i = 0 ; i < input_weights.Size() ; ++i)
          {
          mean_ak += input_weights.GetPixel(i)[b] ;
          }
          mean_ak /= size_Mean*size_Mean ;

          v_mean_ai[b] = mean_ak ;
       // output[b] = static_cast<typename TOutput::ValueType>(mean_ak) ;
      }

    


      for(unsigned int b = 0 ; b < Nband_weights/4 ; ++b)
        {
          double qi(0.);
          qi = v_mean_ai[4*b] * input_I.GetCenterPixel()[0] + v_mean_ai[4*b+1] * input_I.GetCenterPixel()[1] + v_mean_ai[4*b+2] * input_I.GetCenterPixel()[2] + v_mean_ai[4*b+3] ;
          
        output[b] = static_cast<typename TOutput::ValueType>(qi);
        }
  
    

    return output ;
    }

  protected:
    unsigned char                   m_RadiusMin;
    unsigned char                   m_RadiusMax;
    unsigned int                    m_numberOfComponents ;


};

} // end namespace functor



// class MeanVectorImageFilter
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



  protected:
  MeanVectorImageFilter() {}
  ~MeanVectorImageFilter() override {}

   void GenerateOutputInformation(void) override
    {
    Superclass::GenerateOutputInformation();
    unsigned int nb_comp (this->GetInput(0)->GetNumberOfComponentsPerPixel());
    this->GetOutput()->SetNumberOfComponentsPerPixel(nb_comp/4);
    this->GetFunctor().SetNumberOfComponent(nb_comp/4);
    } 



  private:
  MeanVectorImageFilter(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented


  }; // end of MeanVectorImageFilter class



} // end namespace otb

#endif
