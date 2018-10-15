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

#include "otbBinaryFunctorNeighborhoodVectorImageFilter.h"
#include <itkConstNeighborhoodIterator.h>
#include <itkDataObject.h>


namespace otb
{


namespace Functor
{
/** \class Weights
 *  \brief Functor to perform the calculation of the coefficients of the guided filter 
 *
 *  The functor is templated by the type of inputs images (guidance image and its cost volume) and the output image,
 *  and is using two neighborhood iterators as inputs.
 *
 * \ingroup OTBDisparityMap
 */
template<class TInput1, class TInput2, class TOutput> 
class Weights
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

  unsigned int m_bandNumberInput1;
  unsigned int m_bandNumberInput2;
  unsigned int m_WSize ;

  TOutput operator() ( TInput1 input_I , TInput2 input_C ) const
    {

    TOutput output(m_numberOfComponents); 
    output.Fill(0); 

    //mean rr, gg, bb
    std::vector<double> v_mean;
    v_mean.resize(m_bandNumberInput1,0);
    //variance rr, gg, bb
    std::vector<double> v_var;
    v_var.resize(2*m_bandNumberInput1,0);

    std::vector<double> v_pmean;
    v_pmean.resize(m_bandNumberInput2,0) ;

    std::vector<double> v_multr;
    v_multr.resize(m_bandNumberInput2,0) ;

    std::vector<double> v_multg;
    v_multg.resize(m_bandNumberInput2,0) ;

    std::vector<double> v_multb;
    v_multb.resize(m_bandNumberInput2,0) ;

    //mean rr gg bb and variance rr gg bb
    for(unsigned int bandI = 0; bandI < m_bandNumberInput1 ; ++bandI)
      {
      double mean(0.);  
      double var_tmp(0.);
      double var(0.);           
      for(unsigned int i = 0 ; i < input_I.Size() ; ++i)
        {            
        mean += input_I.GetPixel(i)[bandI];
        var_tmp += input_I.GetPixel(i)[bandI] * input_I.GetPixel(i)[bandI] ;      
        }
      mean /= m_WSize*m_WSize;
      var = var_tmp/(m_WSize*m_WSize) - mean*mean ;
      v_mean[bandI] = mean ; 
      v_var[bandI] = var ;
      }

    //variance rg, rb
    for(unsigned int bandI = 1 ; bandI < m_bandNumberInput1 ; ++bandI)
      {
      double var_tmp(0.);
      double var(0.);
      for(unsigned int i = 0 ; i < input_I.Size() ; ++i)
        {
        var_tmp += input_I.GetPixel(i)[0] * input_I.GetPixel(i)[bandI] ;
        }
        var = var_tmp/(m_WSize*m_WSize) - v_mean[0] * v_mean[bandI] ; 
        v_var[m_bandNumberInput1+bandI-1] = var ;
      }

    //variance gb
    double var_tmp(0.);
    double var(0.);
    for(unsigned int i = 0 ; i < input_I.Size() ; ++i)
      {
      var_tmp += input_I.GetPixel(i)[1] * input_I.GetPixel(i)[2];
      }
    var = var_tmp/(m_WSize*m_WSize) - v_mean[1]*v_mean[2] ;
    v_var[2*m_bandNumberInput1-1] = var ;

    //mean of each slice of the cost volume + sum(Ir(i)*p(i)) + sum(Ig(i)*p(i)) + sum(Ib(i)*p(i))
    for(unsigned int bandC = 0 ; bandC < m_bandNumberInput2 ; ++bandC)
      {
      double mean_p(0.);
      double multr(0.);
      double multg(0.);
      double multb(0.);

      if(input_C.GetCenterPixel()[bandC] < 0.0000001 )
      {        
      }
      else
      {
       for (unsigned int i = 0; i < input_C.Size() ; ++i)
        {
        mean_p += input_C.GetPixel(i)[bandC];   
        multr += input_I.GetPixel(i)[0] * input_C.GetPixel(i)[bandC];
        multg += input_I.GetPixel(i)[1] * input_C.GetPixel(i)[bandC];
        multb += input_I.GetPixel(i)[2] * input_C.GetPixel(i)[bandC];
        }
        mean_p /= m_WSize*m_WSize;
        multr /= m_WSize*m_WSize;
        multg /= m_WSize*m_WSize;
        multb /= m_WSize*m_WSize;
        v_pmean[bandC] = mean_p;
        v_multr[bandC] = multr ;
        v_multg[bandC] = multg ;
        v_multb[bandC] = multb ;
        }     
      }      

    //covariance matrix
    typedef itk::Matrix<double, 3, 3> MatrixType;
    MatrixType M;
    double epsilon = 6.5025;

    //ai
    //r
    std::vector<double> elem1;
    elem1.resize(m_bandNumberInput2,0);
    //g
    std::vector<double> elem2;
    elem2.resize(m_bandNumberInput2,0);
    //b
    std::vector<double> elem3;
    elem3.resize(m_bandNumberInput2,0);

    for(unsigned int bandC = 0 ; bandC < m_bandNumberInput2 ; ++bandC)
      {
      if(m_bandNumberInput1>1)
        {
        std::vector<double> ak;
        ak.resize(m_bandNumberInput1,0); 
        // Covariance matrix
        M(0,0) = v_var[0] + epsilon;
        M(0,1) = v_var[3] ;
        M(0,2) = v_var[4] ;
        M(1,0) = v_var[3]  ;
        M(1,1) = v_var[1] + epsilon ;
        M(1,2) = v_var[5] ;
        M(2,0) = v_var[4] ;
        M(2,1) = v_var[5];
        M(2,2) = v_var[2] + epsilon ;
        // Inverse of (covariance Matrix + epsilon+I3)
        MatrixType Matrice_cov_inv( M.GetInverse() ); 
        elem1[bandC] = v_multr[bandC] - v_mean[0] * v_pmean[bandC] ;
        elem2[bandC] = v_multg[bandC] - v_mean[1] * v_pmean[bandC] ;
        elem3[bandC] = v_multb[bandC] - v_mean[2] * v_pmean[bandC] ;  

        //ak
        ak[0] = Matrice_cov_inv(0,0)*elem1[bandC] + Matrice_cov_inv(0,1)*elem2[bandC] + Matrice_cov_inv(0,2)*elem3[bandC];
        ak[1] = Matrice_cov_inv(1,0)*elem1[bandC] + Matrice_cov_inv(1,1)*elem2[bandC] + Matrice_cov_inv(1,2)*elem3[bandC];
        ak[2] = Matrice_cov_inv(2,0)*elem1[bandC] + Matrice_cov_inv(2,1)*elem2[bandC] + Matrice_cov_inv(2,2)*elem3[bandC]; 
        //bk
        double bk(0.);
        bk = v_pmean[bandC] - (ak[0]*v_mean[0]  + ak[1]*v_mean[1] + ak[2]*v_mean[2]) ;
        //output
        output[4*bandC] = static_cast<typename TOutput::ValueType>(ak[0]);
        output[4*bandC+1] = static_cast<typename TOutput::ValueType>(ak[1]);
        output[4*bandC+2] = static_cast<typename TOutput::ValueType>(ak[2]);
        output[4*bandC+3] = static_cast<typename TOutput::ValueType>(bk);
        }
      else
        {
        double ak(0.) ;
        double bk(0.) ;
        elem1[bandC] = v_multr[bandC] - v_mean[0] * v_pmean[bandC] ;          
        ak = elem1[bandC] / (v_var[m_bandNumberInput1-1]+epsilon) ;
        bk = v_pmean[bandC] - ak*v_mean[0] ;
        output[(m_bandNumberInput1+1)*bandC] = static_cast<typename TOutput::ValueType>(ak);
        output[(m_bandNumberInput1+1)*bandC+1] = static_cast<typename TOutput::ValueType>(bk);
        }
      }
    return output ; 
    }
  private:
    unsigned char                   m_RadiusMin;
    unsigned char                   m_RadiusMax;
    unsigned int                    m_numberOfComponents ;
}; //end class

} // end Functor


/** \class WeightsGuidedFilter
 *  \brief Calculation of the coefficients of the guided filter 
 * 
 * The output of this filter is a volume which contains the coefficients of the guided filter in each pixel of the input image.
 *
 * \ingroup OTBDisparityMap
 */
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
WeightsGuidedFilter() =default ;
  
~WeightsGuidedFilter() override =default ;


void GenerateOutputInformation(void) override
    {
    Superclass::GenerateOutputInformation();
    this->GetFunctor().m_bandNumberInput1 = this->GetInput(0)->GetNumberOfComponentsPerPixel() ;  
    this->GetFunctor().m_bandNumberInput2 = this->GetInput(1)->GetNumberOfComponentsPerPixel() ; 
    assert((this->GetFunctor().m_bandNumberInput1 + 1)  % (this->GetFunctor().m_bandNumberInput2) == 0) ;
    this->GetOutput()->SetNumberOfComponentsPerPixel((this->GetFunctor().m_bandNumberInput1+1)*this->GetFunctor().m_bandNumberInput2 );
    this->GetFunctor().SetNumberOfComponent((this->GetFunctor().m_bandNumberInput1+1)*this->GetFunctor().m_bandNumberInput2 );  
    this->GetFunctor().m_WSize = (this->GetFunctor().GetRadiusMax())*2+1 ;
    } 

private:
  WeightsGuidedFilter(const Self &) = delete;
  void operator =(const Self&) = delete; 

}; // end of WeightsGuidedFilter class

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif
