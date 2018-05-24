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

#ifndef otbWeightsGuidedFilter_h
#define otbWeightsGuidedFilter_h


//#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"
#include "otbBinaryFunctorNeighborhoodVectorImageFilter.h"
#include <itkConstNeighborhoodIterator.h>



namespace otb
{

namespace Functor
{
template<class TInput_I1, class TInput_I2 , class TOutput> 
class Weights
{
public:
  Weights(){}
  ~Weights() {}


  int  GetNumberOfComponentsPerPixel() const
    {
    return 2;   
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

    int i = 0;

  TOutput operator() ( TInput_I1 input_it1 , TInput_I2 input_it2 )
    { 

    unsigned int Nband =  input_it1.GetPixel(0).Size(); 

    typename TInput_I1::RadiusType r_Box = input_it1.GetRadius();  
    unsigned int r_Mean = r_Box[0]-1;
    unsigned int size_Mean = 2*r_Mean+1;

    unsigned int epsilon = 0.0001 ; 

    TOutput output(1); 
    output.Fill(0);


    //cov k=1

  /*
    Matrice_cov[0] = input_it1.GetPixel(1)[3] + epsilon;
    Matrice_cov[1] = input_it1.GetPixel(1)[6];
    Matrice_cov[2] = input_it1.GetPixel(1)[7];
    Matrice_cov[3] = Matrice_cov[1];
    Matrice_cov[4] = input_it1.GetPixel(1)[4] + epsilon;
    Matrice_cov[5] = input_it1.GetPixel(1)[8];
    Matrice_cov[6] = Matrice_cov[2];
    Matrice_cov[7] = Matrice_cov[5];
    Matrice_cov[8] = input_it1.GetPixel(1)[5] + epsilon;
  */  

    typedef itk::Matrix<double, 3, 3> MatrixType;
    MatrixType M;
    M(0,0) = input_it1.GetPixel(1)[3] + epsilon ; //var_rr
    M(0,1) = input_it1.GetPixel(1)[6]; // var rg
    M(0,2) = input_it1.GetPixel(1)[7]; // var rb
    M(1,0) = M(0,1); //var rg
    M(1,1) = input_it1.GetPixel(1)[4] + epsilon; //var gg
    M(1,2) = input_it1.GetPixel(1)[8]; // var gb
    M(2,0) = M(0,2); //var rb
    M(2,1) = M(1,2); //var gb
    M(2,2) = input_it1.GetPixel(1)[5] + epsilon; //var bb

    MatrixType Matrice_cov_inv( M.GetInverse() );

    //calcul ak
    //r
    std::vector<double> elem1;
    elem1.resize(14,0);
    for(unsigned int i=0; i < 15 ; ++i)
      {
      elem1[i] = input_it1.GetPixel(1)[24+i] - input_it1.GetPixel(1)[0] * input_it1.GetPixel(1)[9+i];
      }


    //g
    std::vector<double> elem2;
    elem2.resize(14,0);
    for(unsigned int i=0; i < 15 ; ++i)
      {
      elem2[i] = input_it1.GetPixel(1)[39+i] - input_it1.GetPixel(1)[1] * input_it1.GetPixel(1)[9+i];
      }



    //b
    std::vector<double> elem3;
    elem3.resize(14,0);
    for(unsigned int i=0; i < 15 ; ++i)
      {
      elem3[i] = input_it1.GetPixel(1)[54+i] - input_it1.GetPixel(1)[2] * input_it1.GetPixel(1)[9+i];
      }

      std::vector<double> ak;
      ak.resize(2,0);

      ak[0] = Matrice_cov_inv(0,0)*elem1[1] + Matrice_cov_inv(0,1)*elem2[2] * Matrice_cov_inv(0,2)*elem3[3];
      ak[1] = Matrice_cov_inv(1,0)*elem1[1] + Matrice_cov_inv(1,1)*elem2[2] * Matrice_cov_inv(1,2)*elem3[3];
      ak[2] = Matrice_cov_inv(2,0)*elem1[1] + Matrice_cov_inv(1,1)*elem2[2] * Matrice_cov_inv(2,2)*elem3[3];




    return output ; 

    }

      protected:
      unsigned char                   m_RadiusMin;
      unsigned char                   m_RadiusMax;

}; //end class

} // end Functor


  // class LocalMeanGuidanceImageFilter
  template <class TInputImage1, class TInputImage2, class TOutputImage>
  class ITK_EXPORT WeightsGuidedFilter :
    public otb::BinaryFunctorNeighborhoodVectorImageFilter<
        TInputImage1, TInputImage2, TOutputImage,
        Functor::Weights<
            typename itk::ConstNeighborhoodIterator<TInputImage1>,
            typename itk::ConstNeighborhoodIterator<TInputImage2>,
            typename TOutputImage::PixelType> >
  {
  public:
  /** Standard class typedefs. */
  typedef WeightsGuidedFilter Self;
  typedef typename otb::BinaryFunctorNeighborhoodVectorImageFilter<
      TInputImage1,
      TInputImage2,
      TOutputImage,
      Functor::Weights<
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
  WeightsGuidedFilter() {}
  ~WeightsGuidedFilter() override {}

  void GenerateOutputInformation(void) override
    {
    Superclass::GenerateOutputInformation();
    this->GetOutput()->SetNumberOfComponentsPerPixel(1);
    } 

  private:
  WeightsGuidedFilter(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented


  }; // end of WeightsGuidedFilter class

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif