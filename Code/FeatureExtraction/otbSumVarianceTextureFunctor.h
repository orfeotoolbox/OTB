/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSumVarianceTextureFunctor_h
#define __otbSumVarianceTextureFunctor_h

#include "otbSumAverageTextureFunctor.h"

namespace otb
{
namespace Functor
{
/** \class SumVarianceextureFunctor
 *  \brief This functor calculates the sum variance image texture according to Haralick descriptiors.
 *
 *  Computes sum variance using joint histogram (neighborhood and offset neighborhood).
 *  The formula is:
 *  $ \sum_{i}{2N}(i-SAV)^2.p_{x+y}(i) $
 *  With $SAV$ is the sum average texture value.
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \sa SumAverageTextureFunctor
 *  \sa TextureFunctorBase
 *  \ingroup Functor
 *  \ingroup Statistics
 */
template <class TIterInput1, class TIterInput2, class TOutput>
class ITK_EXPORT SumVarianceTextureFunctor : 
public SumAverageTextureFunctor<TIterInput1, TIterInput2, TOutput>
{
public:
  SumVarianceTextureFunctor(){};
  ~SumVarianceTextureFunctor(){};

  typedef TIterInput1                           IterType1;
  typedef TIterInput2                           IterType2;
  typedef TOutput                               OutputType;
  typedef typename IterType1::InternalPixelType InternalPixelType;
  typedef typename IterType1::ImageType         ImageType;
  typedef itk::Neighborhood<InternalPixelType,::itk::GetImageDimension<ImageType>::ImageDimension>    NeighborhoodType;
  typedef SumAverageTextureFunctor<TIterInput1, TIterInput2, TOutput> SuperClass;

  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    double f6 = SuperClass::ComputeOverSingleChannel(neigh, neighOff);
   
    double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);
    double areaInv = 1/area;
    double out = 0.;
    for (unsigned sB = 0; sB<this->GetHisto()[0].size(); sB++)
      { 
	double nCeil = (static_cast<double>(sB)+0.5)*this->GetNeighBinLength();
	double nCeil2 = (static_cast<double>(sB)+this->GetHisto()[0].size()+0.5)*this->GetNeighBinLength();
	double Px_y = 0.;
	double Px_y2 = 0.;
	for (unsigned r = 0; r<this->GetHisto().size(); r++)
	  {
	    double rVal = (static_cast<double>(r)+0.5)*this->GetOffsetBinLength();
	    for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	      { 
		double sVal = (static_cast<double>(s)+0.5)*this->GetNeighBinLength();
		if( vcl_abs(rVal + sVal - nCeil) < vcl_abs(this->GetNeighBinLength()) )
		  {
		    Px_y +=  static_cast<double>(this->GetHisto()[r][s])*areaInv;
		  }
		if( vcl_abs(rVal + sVal - nCeil2) < vcl_abs(this->GetNeighBinLength()) )
		  {
		    Px_y2 +=  static_cast<double>(this->GetHisto()[r][s])*areaInv;
		  }
	      }
	  }
	out += vcl_pow((nCeil-f6), 2)*Px_y + vcl_pow((nCeil2-f6), 2)*Px_y2;
      }
  
    return out;
  }
};




  } // namespace Functor
} // namespace otb

#endif

