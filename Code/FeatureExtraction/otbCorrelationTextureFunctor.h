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
#ifndef __otbCorrelationTextureFunctor_h
#define __otbCorrelationTextureFunctor_h

#include "otbTextureFunctorBase.h"

namespace otb
{
namespace Functor
{
/** \class CorrelationTextureFunctor
 *  \brief This functor calculates the correlation image texture according to Haralick descriptors.
 *
 *  Computes correlation using joint histogram (neighborhood and offset neighborhood).
 *  The formula is:
 *  $ \frac{\sum_{i}\sum_{j}i.j.p(i,j)-\mu_{x}\mu_{y}} {\sigma_{x}\sigma_{y}} $
 *  Where $\mu_{x}$, $\mu_{y}$, $\sigma_{x}$ and $\sigma_{y}$ are the mean and standard deviation of $p_{x}$ and $p_{y}$.
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \sa TextureFunctorBase
 *  \ingroup Functor
 *  \ingroup Statistics
 */


template <class TIterInput1, class TIterInput2, class TOutput>
class ITK_EXPORT CorrelationTextureFunctor : 
public TextureFunctorBase<TIterInput1, TIterInput2, TOutput>
{
public:
  CorrelationTextureFunctor(){};
  virtual ~CorrelationTextureFunctor(){};

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
    double sumProb = 0.;
    for (unsigned r = 0; r<this->GetHisto().size(); r++)
      {
	for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	  { 
	    double p =  static_cast<double>(this->GetHisto()[r][s])*areaInv;
	    sumProb += p;
	    double pixProd = ( (static_cast<double>(r)+0.5)*this->GetOffsetBinLength() ) * ( (static_cast<double>(s)+0.5)*this->GetNeighBinLength() );
	    out += pixProd * p;
	  }
    }
    
    double meanPOff = sumProb/static_cast<double>(this->GetHisto().size());
    double meanPNeigh = sumProb/static_cast<double>(this->GetHisto()[0].size());
   
    // Standard deviation of p for offset region
    double stdPOff = 0.;
    for (unsigned r = 0; r<this->GetHisto().size(); r++)
      {
	double sumTemp = 0.;
	for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	  {
	    sumTemp += this->GetHisto()[r][s]*areaInv;
	  }
	stdPOff +=  vcl_pow( (meanPOff-sumTemp), 2);
      }
    stdPOff /= this->GetHisto().size();
    stdPOff = vcl_sqrt(stdPOff);

    // Standard deviation of p for neighborhood region
    double stdPNeigh = 0.;
    for (unsigned r = 0; r<this->GetHisto()[0].size(); r++)
      {
	double sumTemp = 0.;
	for (unsigned s = 0; s<this->GetHisto().size(); s++)
	  {
	    sumTemp += this->GetHisto()[s][r]*areaInv;
	  }
	stdPNeigh +=  vcl_pow( (meanPNeigh-sumTemp), 2);
      }
    stdPNeigh /= this->GetHisto()[0].size();
    stdPNeigh = vcl_sqrt(stdPNeigh);
    

    if(stdPOff*stdPNeigh != 0)
     	out = (out - meanPOff*meanPNeigh) / (stdPOff*stdPNeigh);
    
    return out;  
  }
  
};
 
 
} // namespace Functor
} // namespace otb

#endif

