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

#ifndef otbLocalStatImageFilter_h
#define otbLocalStatImageFilter_h


//#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"
#include "otbBinaryFunctorNeighborhoodVectorImageFilter.h"
#include <itkConstNeighborhoodIterator.h>




namespace otb
{

namespace Functor
{
template<class TInput_I, class TInput_CostVolume, class TOutput> 
class LocalStatVectorImage1
{
public:
  LocalStatVectorImage1(){}
  ~LocalStatVectorImage1() {}


  int  GetNumberOfComponentsPerPixel() const
    {
    return 69;   
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




  TOutput operator() ( TInput_I input_it , TInput_CostVolume input_C )
    {

    unsigned int Nband =  input_it.GetPixel(0).Size(); //bands number
    unsigned int Nband_C =  input_C.GetPixel(0).Size(); //bands number
    
  
    typename TInput_I::RadiusType r_Box = input_it.GetRadius(); 
    unsigned int r_Mean = r_Box[0]-1;
    unsigned int size_Mean = 2*r_Mean+1;


    TOutput output(Nband*3+4*Nband_C); 

    output.Fill(0); 


    //Mean
    for(unsigned int b = 0; b < Nband; ++b)
      {
        double mean(0.);       


        for(unsigned int i = 0; i < input_it.Size() ; ++i)
          {
            mean += input_it.GetPixel(i)[b] ;  
          }
          mean /= size_Mean*size_Mean;    
          output[b] = static_cast<typename TOutput::ValueType>(mean) ;
      }
      
      //variance rr, gg, bb
   //   std::cout << "variance rr, gg, bb" << std::endl;
      for(unsigned int b = Nband; b < Nband*2; ++b)
        {
        double var(0.);
        double sum(0.);
        for(unsigned int i = 0; i < input_it.Size() ; ++i)
          {
          sum += input_it.GetPixel(i)[b-Nband] * input_it.GetPixel(i)[b-Nband] ;        
          }
        var = sum/(size_Mean*size_Mean) - output[b-Nband] * output[b-Nband] ;
        output[b] = static_cast<typename TOutput::ValueType>(var) ;
        }

     // std::cout << "variance rg, gb" << std::endl;
      for(unsigned int b = Nband*2; b < Nband*3-1; ++b)
        {
        double var(0.);
        double sum(0.);
        for(unsigned int i = 0; i < input_it.Size() ; ++i)
          {
            sum += input_it.GetPixel(i)[b-2*Nband] * input_it.GetPixel(i)[b-2*Nband+1] ;       
          }
        var = sum/(size_Mean*size_Mean) - output[b-2*Nband] * output[b-2*Nband+1] ;
        output[b] = static_cast<typename TOutput::ValueType>(var) ;
        }

      //std::cout << "variance gb" << std::endl;
        for(unsigned int b = Nband*3-1; b < Nband*3; ++b)
          {
          double var(0.);
          double sum(0.);
          for(unsigned int i = 0; i < input_it.Size() ; ++i)
            {
              sum += input_it.GetPixel(i)[b-2*Nband] * input_it.GetPixel(i)[b-2*Nband-2] ;       
            }
          var = sum/(size_Mean*size_Mean) - output[b-2*Nband] * output[b-2*Nband-2] ;
          output[b] = static_cast<typename TOutput::ValueType>(var) ;
          }


      // mean of each slice of the cost volume
        for(unsigned int b = Nband*3 ; b < Nband*3+Nband_C ; ++b)
          {
          double mean_p(0.);
          for (unsigned int i = 0; i < input_C.Size() ; ++i)
            {
            mean_p += input_C.GetPixel(i)[b-Nband*3]     ;   
            }
            mean_p /= size_Mean*size_Mean;
            output[b] = static_cast<typename TOutput::ValueType>(mean_p) ;
          }

      // sum Ir(i)*p(i)
        for(unsigned int b = Nband*3+Nband_C; b < Nband*3+2*Nband_C ; ++b)
          {
          double mult_r(0.);
          for (unsigned int i = 0; i < input_C.Size() ; ++i)
            {
            mult_r += input_it.GetPixel(i)[0] * input_C.GetPixel(i)[b-( Nband*3+Nband_C)];
            }
            mult_r/=size_Mean*size_Mean;
            output[b] = static_cast<typename TOutput::ValueType>(mult_r) ;
          }


      // sum Ig(i)*p(i)
        for(unsigned int b = Nband*3+2*Nband_C; b < Nband*3+3*Nband_C ; ++b)
          {
          double mult_g(0.);
          for (unsigned int i = 0; i < input_C.Size() ; ++i)
            {
            mult_g += input_it.GetPixel(i)[1] * input_C.GetPixel(i)[b-(Nband*3+2*Nband_C)];
            }
            mult_g/=size_Mean*size_Mean;
            output[b] = static_cast<typename TOutput::ValueType>(mult_g) ;
          }

             // sum Ib(i)*p(i)
        for(unsigned int b = Nband*3+3*Nband_C; b < Nband*3+4*Nband_C ; ++b)
          {
          double mult_b(0.);
          for (unsigned int i = 0; i < input_C.Size() ; ++i)
            {
            mult_b += input_it.GetPixel(i)[2] * input_C.GetPixel(i)[b-(Nband*3+3*Nband_C)];
            }
            mult_b/=size_Mean*size_Mean;
            output[b] = static_cast<typename TOutput::ValueType>(mult_b) ;
          }
     

    //Functor2


    // Covariance matrix
    typedef itk::Matrix<double, 3, 3> MatrixType;
    MatrixType M;
    unsigned int epsilon = 255*255*0.0001 ; 


    //Calcul ak
    //r
    std::vector<double> elem1;
    elem1.resize(Nband_C-1,0);
    //g
    std::vector<double> elem2;
    elem2.resize(Nband_C-1,0);
    //b
    std::vector<double> elem3;
    elem3.resize(Nband_C-1,0);

    std::vector<double> ak;
    ak.resize(2,0);

    std::vector<double> sum_ak;
    sum_ak.resize(2,0);


    for(unsigned int disp=0; disp < 1 ; ++disp)
      {  

      //matrice de covariance fenêtre k :
      M(0,0) = output[3] + epsilon;
      M(0,1) = output[6] ;
      M(0,2) = output[7] ;

      M(1,0) = M(0,0) ;
      M(1,1) = output[4] + epsilon ;
      M(1,2) = output[8] ;

      M(2,0) = M(0,2) ;
      M(2,1) = M(1,2); ;
      M(2,2) = output[5] + epsilon ;

      // Inverse of (covariance Matrix + epsilon+I3)
      MatrixType Matrice_cov_inv( M.GetInverse() );  

      elem1[disp] = output[24+disp] - output[0]*output[9+disp];
      elem2[disp] = output[39+disp] - output[1]*output[9+disp];
      elem3[disp] = output[54+disp] - output[2]*output[9+disp];   

      ak[0] = Matrice_cov_inv(0,0)*elem1[disp] + Matrice_cov_inv(0,1)*elem2[disp] * Matrice_cov_inv(0,2)*elem3[disp];
      ak[1] = Matrice_cov_inv(1,0)*elem1[disp] + Matrice_cov_inv(1,1)*elem2[disp] * Matrice_cov_inv(1,2)*elem3[disp];
      ak[2] = Matrice_cov_inv(2,0)*elem1[disp] + Matrice_cov_inv(1,1)*elem2[disp] * Matrice_cov_inv(2,2)*elem3[disp];

      }


  
    return output ; 

    }

  protected:
      TInput_I m_in;
      unsigned char                   m_RadiusMin;
      unsigned char                   m_RadiusMax;
}; //end class

} // end Functor


  // class LocalMeanGuidanceImageFilter
  template <class TInputImage1, class TInputImage2, class TOutputImage>
  class ITK_EXPORT LocalStatImageFilter :
    public otb::BinaryFunctorNeighborhoodVectorImageFilter<
        TInputImage1, TInputImage2, TOutputImage,
        Functor::LocalStatVectorImage1<
            typename itk::ConstNeighborhoodIterator<TInputImage1>,
            typename itk::ConstNeighborhoodIterator<TInputImage2>,
            typename TOutputImage::PixelType> >
  {
  public:
  /** Standard class typedefs. */
  typedef LocalStatImageFilter Self;
  typedef typename otb::BinaryFunctorNeighborhoodVectorImageFilter<
      TInputImage1,
      TInputImage2,
      TOutputImage,
      Functor::LocalStatVectorImage1<
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
  LocalStatImageFilter() {}
  ~LocalStatImageFilter() override {}

  void GenerateOutputInformation(void) override
    {
    Superclass::GenerateOutputInformation();
    this->GetOutput()->SetNumberOfComponentsPerPixel(69);
    } 

  private:
  LocalStatImageFilter(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented


  }; // end of LocalStatImageFilter class

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif