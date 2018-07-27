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



    TOutput output(Nband_weights/(Nband_I+1)); 
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
    


      for(unsigned int b = 0 ; b < Nband_weights/(Nband_I+1) ; ++b)
        {
          double qi(0.);
          if(Nband_I>1)
            {
            qi = v_mean_ai[(Nband_I+1)*b] * input_I.GetCenterPixel()[0] + v_mean_ai[(Nband_I+1)*b+1] * input_I.GetCenterPixel()[1] + v_mean_ai[(Nband_I+1)*b+2] * input_I.GetCenterPixel()[2] + v_mean_ai[(Nband_I+1)*b+3] ;
            }
          else
            {
            qi = v_mean_ai[(Nband_I+1)*b] * input_I.GetCenterPixel()[0] + v_mean_ai[(Nband_I+1)*b+1] ;
            }
         
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

   virtual void GenerateInputRequestedRegion() ITK_OVERRIDE
    {
    // this->GetOutput()->GetRequestedRegion();
      // call the superclass' implementation of this method
    Superclass::GenerateInputRequestedRegion();

    // get pointers to the input and output
    typename Superclass::InputImagePointer inputPtr1 = const_cast< TInputImage1 * >( this->GetInput() );
    typename Superclass::InputImagePointer inputPtr2 = const_cast< TInputImage2 * >( this->GetInput() );

    if ( !inputPtr1 )
      {
      return;
      }
    if ( !inputPtr2 )
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
    inputRequestedRegion1.PadByRadius(9);
    inputRequestedRegion2.PadByRadius(9);

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
    unsigned int nb_comp (this->GetInput(0)->GetNumberOfComponentsPerPixel());
    unsigned int nb_inI (this->GetInput(1)->GetNumberOfComponentsPerPixel());
    this->GetOutput()->SetNumberOfComponentsPerPixel(nb_comp/(nb_inI+1));
    this->GetFunctor().SetNumberOfComponent(nb_comp/(nb_inI+1));
    } 


  private:
  MeanVectorImageFilter(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented


  }; // end of MeanVectorImageFilter class



} // end namespace otb

#endif
