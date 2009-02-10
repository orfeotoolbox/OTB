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

#include "otbTextureFunctorBase.h"

namespace otb
{
namespace Functor
{
/** \class SumVarianceextureFunctor
 *  \brief This functor calculates the local entropy of an image
 *
 *   Computes joint histogram (neighborhood and offset neighborhood) 
 *   which bins are computing using Scott formula.
 *   Computes the probabiltiy p for each pair of pixel.
 *   Entropy  is the sum (i-mu)².log(p) over the neighborhood.
 *   TIterInput is an ietrator, TOutput is a PixelType.
 *
 *  \ingroup Functor
 *  \ingroup Statistics
 */
template <class TIterInput1, class TIterInput2, class TOutput>
class ITK_EXPORT SumVarianceTextureFunctor : 
public TextureFunctorBase<TIterInput1, TIterInput2, TOutput>
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


  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    this->ComputeJointHistogram(neigh, neighOff);
    double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);
    double areaInv = 1/area;
    double out = 0.;
    // loop over bin neighborhood values
    double f6 = 0.;
    for (unsigned sB = 0; sB<this->GetHisto()[0].size(); sB++)
      { 
	double nCeil = (static_cast<double>(sB)+0.5)*this->GetNeighBinLength();
	for (unsigned r = 0; r<this->GetHisto().size(); r++)
	  {
	    double rVal = (static_cast<double>(r)+0.5)*this->GetOffsetBinLength();
	    for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	      { 
		double sVal = (static_cast<double>(s)+0.5)*this->GetNeighBinLength();
		// In theory don't have the abs but will deals with neighborhood and offset without the same this->GetHisto()
		// thus loop over 2*Ng don't have sense
		//if( vcl_abs(rVal + sVal - nCeil) < vcl_abs(binsLength[0]+binsLength[1]) || vcl_abs(rVal + sVal - 2*nCeil) < vcl_abs(binsLength[0]+binsLength[1]) )
		if( vcl_abs(rVal + sVal - nCeil) < vcl_abs(this->GetNeighBinLength()) || vcl_abs(rVal + sVal - 2*nCeil) < 2*vcl_abs(this->GetNeighBinLength()) )
		  {
		    double p =  static_cast<double>(this->GetHisto()[r][s])*areaInv;
		    f6 += nCeil * p;
		  }
	      }
	  }
      }
    
    // loop over bin neighborhood values
    for (unsigned sB = 0; sB<this->GetHisto()[0].size(); sB++)
      { 
	double Px_y = 0.;
	double nCeil = (static_cast<double>(sB)+0.5)*this->GetNeighBinLength();
	double coef = vcl_pow( (nCeil-f6), 2);
	for (unsigned r = 0; r<this->GetHisto().size(); r++)
	  {
	    double rVal = (static_cast<double>(r)+0.5)*this->GetOffsetBinLength();
	    for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	      { 
		double sVal = (static_cast<double>(s)+0.5)*this->GetNeighBinLength();
		if( vcl_abs(rVal + sVal - nCeil) < vcl_abs(this->GetNeighBinLength()) || vcl_abs(rVal + sVal - 2*nCeil) < vcl_abs(this->GetNeighBinLength()) )
		  {
		    Px_y +=  static_cast<double>(this->GetHisto()[r][s])*areaInv;
		  }
	      }
	  }
	
	out += coef * Px_y;
      }
    
    
    return out;
  }
};




  } // namespace Functor
} // namespace otb

#endif

