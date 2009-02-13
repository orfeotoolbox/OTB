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
#ifndef __otbAngularSecondMomentumTextureFunctor_h
#define __otbAngularSecondMomentumTextureFunctor_h

#include "otbTextureFunctorBase.h"

namespace otb
{
namespace Functor
{
/** \class AngularSecondMomentumTextureFunctor
 *  \brief This functor calculates the cluster shade image texture.
 *
 *  Computes angular second moment using joint histogram (neighborhood and offset neighborhood) .
 *  The formula is:
 *  $ sum_{i}\sum_{j}p(i,j)^2 $
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \sa TextureFunctorBase
 *  \ingroup Functor
 *  \ingroup Statistics
 */
template <class TIterInput1, class TIterInput2, class TOutput>
class ITK_EXPORT AngularSecondMomentumTextureFunctor : 
public TextureFunctorBase<TIterInput1, TIterInput2, TOutput>
{
public:
  AngularSecondMomentumTextureFunctor(){};
  virtual ~AngularSecondMomentumTextureFunctor(){};

  typedef TIterInput1                           IterType1;
  typedef TIterInput2                           IterType2;
  typedef TOutput                               OutputType;
  typedef typename IterType1::InternalPixelType InternalPixelType;
  typedef typename IterType1::ImageType         ImageType;
  typedef itk::Neighborhood<InternalPixelType,::itk::GetImageDimension<ImageType>::ImageDimension>    NeighborhoodType;
 

  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    this->ComputeJointHistogram(neigh, neighOff);
  
    double out = 0.;

    double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);
    double areaInv = 1/area;
      for (unsigned r = 0; r<this->GetHisto().size(); r++)
	{
	  for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	    {
	      double p = static_cast<double>(this->GetHisto()[r][s]) * areaInv;
	      out += vcl_pow( p, 2 );
	    }
	}
      return out;  
  }

};

} // namespace Functor
} // namespace otb

#endif

