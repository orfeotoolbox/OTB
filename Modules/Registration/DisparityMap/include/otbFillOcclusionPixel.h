/*
 * Copyright (C) 2017-2018 CS Systemes d'Information (CS SI)
 * Copyright (C) 2017 Damia Belazouz

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

#ifndef otbFillOcclusionPixel_h
#define otbFillOcclusionPixel_h
#include <utility> 
#include <iterator>

#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"
#include <itkNumericTraits.h>
#include <itkConstNeighborhoodIterator.h>


namespace otb
{
namespace Functor
{
/** \class FillPixelFilterOperator
 *
 * \brief Functor to fill the occluded pixel in a disparity map. The filter used is a bilateral filter.
 *
 * \ingroup OTBDisparityMap
 */
template < class TInput, class TOutput >
class FillPixelFilterOperator
{
public:
  typedef typename TInput::OffsetType      OffsetType;
  typedef std::pair<int,double> pairCord;
  typedef typename TInput::PixelType               PixelType;


  static bool comparePair(pairCord i,pairCord j) { 
  if( i.first < j.first ) return i.first <= j.first;
    if( j.first < i.first ) return false;
    return (i.first < j.first ); }

  
  TOutput operator() (  const TInput & input) const
    { 

    PixelType input_center_pixel = input.GetCenterPixel() ;
    unsigned int Wsize = input.Size();
    const double sSpace = 9 ;
    const double sColor = 255*0.1;

    std::vector< std::pair<int,double> > duo(Wsize);   
  
    PixelType I(3);
    if(input_center_pixel[4] < 0.0000000001)
      {
      double Wt=0. ;
      for (unsigned int j  = 0; j< Wsize; j++)
        {
        const auto & Offset_j = input.GetOffset(j);
        I[0] = input_center_pixel[1]- input.GetPixel(j)[1];
        I[1] = input_center_pixel[2]- input.GetPixel(j)[2];
        I[2] = input_center_pixel[3]- input.GetPixel(j)[3];

        const double W = std::exp( -(Offset_j[0]*Offset_j[0] + Offset_j[1]*Offset_j[1])/(sSpace*sSpace) 
        -(I.GetSquaredNorm())/(sColor*sColor));
             
        duo[j].first = input.GetPixel(j)[0];               
        duo[j].second =  W; 
        Wt += W ; 
        }
      Wt = Wt/2;    
      std::sort (duo.begin(), duo.end(), comparePair);

      double Wtemp=0. ; 
      for (auto const& d : duo) 
        {
        Wtemp += d.first;
        if (Wtemp >= Wt)  return TOutput(d.first);
        }
      assert(!"unreachable code");
      }
    else
      {
      return TOutput(input_center_pixel[0]) ;
      }  
    } // end operator

}; // end of functor class  FillPixelFilterOperator

}  // end of fonctor 



/** \class FillPixelFilter
 *
 * \brief Fill the occluded pixel in a disparity map. The filter used is a bilateral filter.
 *
 * \ingroup OTBDisparityMap
 */
template < class TInputImage, class TOutputImage >
class ITK_EXPORT FillPixelFilter
 : public UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::FillPixelFilterOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >
           
{
public:
  /** Standard class typedefs */
  typedef FillPixelFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::FillPixelFilterOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
   
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(FillPixelFilter, ImageToImageFilter);

protected:
  FillPixelFilter()=default ;
  ~FillPixelFilter() override  =default ;
  void GenerateOutputInformation(void) override;

  FillPixelFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class FillPixelFilter


template < class TInputImage, class TOutputImage >
void FillPixelFilter < TInputImage, TOutputImage >
::GenerateOutputInformation(void){
 Superclass::GenerateOutputInformation(); 
 this->GetOutput()->SetNumberOfComponentsPerPixel(1);
}

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif




