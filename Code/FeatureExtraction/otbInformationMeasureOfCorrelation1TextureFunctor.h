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
#ifndef __otbInformationMeasureOfCorrelation1TextureFunctor_h
#define __otbInformationMeasureOfCorrelation1TextureFunctor_h

#include "otbEntropyTextureFunctor.h"

namespace otb
{
namespace Functor
{
/** \class InformationMeasureOfCorrelation1TextureFunctor
 *  \brief This functor calculates the information measure of correlation 1 image texture according to Haralick descriptors.
 *
 *  Computes  information measure of correlation 1 using joint histogram (neighborhood and offset neighborhood) .
 *  The formula is:
 *  \f$ \frac{H(X,Y)-H(X,Y)_{1}}{max(H(X),H(Y))} \f$
 *  Where \f$H(X,Y)\f$, \f$H(X)\f$, \f$H(Y)\f$ are the entropies and
 *  \f$ H(X,Y)_{1} = -\sum_{i}\sum_{j}p(i,j)\log{(p_{x}(i)_{y}(j))} \f$
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \sa EntropyTextureFunctor
 *  \sa TextureFunctorBase
 *  \ingroup Functor
 *  \ingroup Statistics
 */

template <class TIterInput, class TOutput>
class ITK_EXPORT InformationMeasureOfCorrelation1TextureFunctor :
public EntropyTextureFunctor<TIterInput, TOutput>
{
public:
  InformationMeasureOfCorrelation1TextureFunctor(){};
  virtual ~InformationMeasureOfCorrelation1TextureFunctor(){};

  typedef TIterInput                            IterType;
  typedef TOutput                               OutputType;
  typedef typename IterType::InternalPixelType InternalPixelType;
  typedef typename IterType::ImageType         ImageType;
  typedef itk::Neighborhood<InternalPixelType,::itk::GetImageDimension<ImageType>::ImageDimension>    NeighborhoodType;
  typedef EntropyTextureFunctor<TIterInput, TOutput> Superclass;

  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    double HXY = Superclass::ComputeOverSingleChannel(neigh, neighOff);

    double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);
    double areaInv = 1/area;
    double out = 0.;
    // Stores marginal proba values
    std::vector<double> PxVector;
    std::vector<double> PyVector;

    // Computes HX
    double HX = 0.;
    for (unsigned r = 0; r<this->GetHisto()[0].size(); r++)
      {
	double sumTemp = 0.;
	for (unsigned s = 0; s<this->GetHisto().size(); s++)
	  {
	    sumTemp += this->GetHisto()[s][r]*areaInv;
	  }
	PxVector.push_back( sumTemp );
	if(sumTemp != 0. )
	  HX +=  sumTemp * vcl_log( sumTemp );
      }
    if ( HX != 0. )
      HX = -HX;


    // Computes HY
    double HY = 0.;
    for (unsigned r = 0; r<this->GetHisto().size(); r++)
      {
	double sumTemp = 0.;
	for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	  {
	    sumTemp += this->GetHisto()[r][s]*areaInv;
	  }
	PyVector.push_back( sumTemp );
	if(sumTemp != 0. )
	  HY +=  sumTemp * vcl_log( sumTemp );
      }
    if ( HY != 0. )
      HY = -HY;


    // Computes HXY1
    double HXY1 = 0.;
    for (unsigned r = 0; r<this->GetHisto().size(); r++)
      {
	for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	  {
	    if( PyVector[r]*PxVector[s] != 0. )
	      {
		double p = this->GetHisto()[r][s]*areaInv;
		HXY1 += p * vcl_log( PyVector[r]*PxVector[s] );
	      }

	  }
      }
    if ( HXY1 != 0. )
      HXY1 = -HXY1;

    if( std::max(HX, HY) != 0.)
      out = (HXY-HXY1) / std::max(HX, HY);

    return out;
  }

};


} // namespace Functor
} // namespace otb

#endif

