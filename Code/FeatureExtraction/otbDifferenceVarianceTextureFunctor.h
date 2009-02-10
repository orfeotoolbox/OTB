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
#ifndef __otbDifferenceVarianceTextureFunctor_h
#define __otbDifferenceVarianceTextureFunctor_h

#include "otbTextureFunctorBase.h"

namespace otb
{
namespace Functor
{
/** \class DifferenceVarianceTextureFunctor
 *  \brief This functor calculates the inverse difference moment of an image
 *
 *   Computes joint histogram (neighborhood and offset neighborhood) 
 *   which bins are computing using Scott formula.
 *   Computes the probabiltiy p for each pair of pixel.
 *   InverseDifferenceMoment  is the sum 1/(1+(pi-poff)²)*p over the neighborhood.
 *   TIterInput is an ietrator, TOutput is a PixelType.
 *
 *  \ingroup Functor
 *  \ingroup 
 *  \ingroup Statistics
 */
template <class TIterInput1, class TIterInput2, class TOutput>
class ITK_EXPORT DifferenceVarianceTextureFunctor : 
public TextureFunctorBase<TIterInput1, TIterInput2, TOutput>
{
public:
  DifferenceVarianceTextureFunctor(){};
  ~DifferenceVarianceTextureFunctor(){};

  typedef TIterInput1                           IterType1;
  typedef TIterInput2                           IterType2;
  typedef TOutput                               OutputType;
  typedef typename IterType1::InternalPixelType InternalPixelType;
  typedef typename IterType1::ImageType         ImageType;
  typedef itk::Neighborhood<InternalPixelType,::itk::GetImageDimension<ImageType>::ImageDimension>    NeighborhoodType;


  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    this->ComputeJointHistogram(neigh, neighOff);
    double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);
    double areaInv = 1/area;
    double out = 0.;


    double MeanPx_y = 0.;
    // Computes mean Px_y
    for (unsigned sB = 0; sB<this->GetHisto()[0].size(); sB++)
      { 
	double nCeil = (static_cast<double>(sB)+0.5)*this->GetNeighBinLength();
	for (unsigned r = 0; r<this->GetHisto().size(); r++)
	  {
	    double rVal = (static_cast<double>(r)+0.5)*this->GetOffsetBinLength();
	    for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	      { 
		if( vcl_abs((static_cast<double>(s)+0.5)*this->GetNeighBinLength() - rVal - nCeil) < vcl_abs(this->GetNeighBinLength()) )
		  {
		    MeanPx_y +=  static_cast<double>(this->GetHisto()[r][s])*areaInv;
		  }
	      }
	  }
      }
    MeanPx_y /= static_cast<double>(this->GetHisto()[0].size());

    // Computes variance
    double varPx_y = 0.;
    for (unsigned sB = 0; sB<this->GetHisto()[0].size(); sB++)
      { 
	double Px_y = 0.;
	double nCeil = (static_cast<double>(sB)+0.5)*this->GetNeighBinLength();
	for (unsigned r = 0; r<this->GetHisto().size(); r++)
	  {
	    double rVal = (static_cast<double>(r)+0.5)*this->GetOffsetBinLength();
	    for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	      { 
		if( vcl_abs((static_cast<double>(s)+0.5)*this->GetNeighBinLength() - rVal - nCeil) < vcl_abs(this->GetNeighBinLength()) )
		  {
		    Px_y +=  static_cast<double>(this->GetHisto()[r][s])*areaInv;
		  }
	      }
	  }
	varPx_y += vcl_pow((Px_y - MeanPx_y), 2);
      }

    return varPx_y/this->GetHisto()[0].size();
  }
  
};
 
 
} // namespace Functor
} // namespace otb

#endif

