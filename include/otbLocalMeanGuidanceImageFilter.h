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

#ifndef otbLocalMeanGuidanceImageFilter_h
#define otbLocalMeanGuidanceImageFilter_h

//#include "otbUnaryFunctorVectorImageFilter.h"
#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"
#include <itkConstNeighborhoodIterator.h>



namespace otb
{

namespace Functor
{
template<class TInput, class TOutput>
class LocalMeanVectorImage
{
public:
  LocalMeanVectorImage(){}
  virtual ~LocalMeanVectorImage() {}

  TOutput operator() ( TInput input_it )
    {

    unsigned int Nband =  input_it.GetPixel(0).Size(); //bands number

    typename TInput::RadiusType r_Box = input_it.GetRadius();  
    unsigned int r_Mean = r_Box[0]-1;
    unsigned int size_Mean = 2*r_Mean+1;

    TOutput output(Nband*2); 
    output.Fill(0); 

    //Median
    for(unsigned int b = 0; b < Nband/2; ++b)
      {
        double mean(0.);       


        for(unsigned int i = 0; i < input_it.Size() ; ++i)
          {
            mean += input_it.GetPixel(i)[b]     ;   

          }
          mean /= size_Mean*size_Mean;    
          output[b] = static_cast<typename TOutput::ValueType>(mean) ;
      }

      //variance
    for(unsigned int b =  Nband/2; b < Nband; ++b)
      {
        double sum2(0.);
        double sum1(0.);
        double var(0.); 

        for(unsigned int i = 0; i < input_it.Size() ; ++i)
          {
         
            sum2 += input_it.GetPixel(i)[b]* input_it.GetPixel(i)[b]    ; 
            sum1 +=  input_it.GetPixel(i)[b] ;
            var =  (sum2 - ( sum1 * output[b-Nband/2])) ;
          }
          var = var / (size_Mean-1);
          output[b-Nband/2] = static_cast<typename TOutput::ValueType>(var) ;
          

      }





    return output ;

    }
}; //end class

} // end Functor


  // class LocalMeanGuidanceImageFilter
  template <class TInputImage, class TOutputImage>
  class ITK_EXPORT LocalMeanGuidanceImageFilter :
    public otb::UnaryFunctorNeighborhoodVectorImageFilter<
        TInputImage, TOutputImage,
        Functor::LocalMeanVectorImage<
            typename itk::ConstNeighborhoodIterator<TInputImage>,
           typename TOutputImage::PixelType> >
  {
  public:
  /** Standard class typedefs. */
  typedef LocalMeanGuidanceImageFilter Self;
  typedef typename otb::UnaryFunctorNeighborhoodVectorImageFilter<
      TInputImage,
      TOutputImage,
      Functor::LocalMeanVectorImage<
          typename itk::ConstNeighborhoodIterator<TInputImage>,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::ConstNeighborhoodIterator<TInputImage> NeighborhoodIteratorType;
  typedef typename NeighborhoodIteratorType::RadiusType   RadiusType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);



  protected:
  LocalMeanGuidanceImageFilter() {}
  ~LocalMeanGuidanceImageFilter() override {}


  private:
  LocalMeanGuidanceImageFilter(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented


  }; // end of LocalMeanGuidanceImageFilter class

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif