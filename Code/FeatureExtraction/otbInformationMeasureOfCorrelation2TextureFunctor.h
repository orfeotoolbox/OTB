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
#ifndef __otbInformationMeasureOfCorrelation2TextureFunctor_h
#define __otbInformationMeasureOfCorrelation2TextureFunctor_h

#include "otbEntropyTextureFunctor.h"

namespace otb
{
namespace Functor
{
/** \class InformationMeasureOfCorrelation2TextureFunctor
 *  \brief This functor calculates the information measure of correlation 2 image texture according to Haralick descriptors.
 *
 *  Computes  information measure of correlation 2 using joint histogram (neighborhood and offset neighborhood) .
 *  The formula is:
 *  \f$ \sqrt{|1-exp(2.(H(X,Y)_{2}-H(X,Y)))|} \f$
 *  Where \f$H(X,Y)\f$, \f$H(X)\f$, \f$H(Y)\f$ are the entropies and
 *  \f$ H(X,Y)_{2} = -\sum_{i}\sum_{j}p_{x}(i)p_{y}(j)\log{(p_{x}(i)p_{y}(j))} \f$
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \sa EntropyTextureFunctor
 *  \sa TextureFunctorBase
 *  \ingroup Functor
 *  \ingroup Statistics
 */

template <class TIterInput, class TOutput>
class ITK_EXPORT InformationMeasureOfCorrelation2TextureFunctor :
public EntropyTextureFunctor<TIterInput, TOutput>
{
public:
  InformationMeasureOfCorrelation2TextureFunctor(){};
  virtual ~InformationMeasureOfCorrelation2TextureFunctor(){};

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
    for (unsigned r = 0; r<this->GetHisto()[0].size(); r++)
      {
	double sumTemp = 0.;
	for (unsigned s = 0; s<this->GetHisto().size(); s++)
	  {
	    sumTemp += this->GetHisto()[s][r]*areaInv;
	  }
	PxVector.push_back( sumTemp );
      }

    // Computes HY
    for (unsigned r = 0; r<this->GetHisto().size(); r++)
      {
	double sumTemp = 0.;
	for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	  {
	    sumTemp += this->GetHisto()[r][s]*areaInv;
	  }
	PyVector.push_back( sumTemp );
      }

    // Computes HXY2
    double HXY2 = 0.;
    for (unsigned r = 0; r<this->GetHisto().size(); r++)
      {
	for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	  {
	    double PxPy = PyVector[r]*PxVector[s];
	    if( PxPy != 0. )
	      {
		HXY2 += PxPy * vcl_log( PxPy );
	      }

	  }
      }
    if ( HXY2 != 0. )
      HXY2 = -HXY2;

    out = vcl_sqrt( vcl_abs(1-vcl_exp( -2.*(HXY2-HXY))) );

    return out;
  }

};


} // namespace Functor
} // namespace otb

#endif

