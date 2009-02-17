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
#ifndef __otbEntropyTextureFunctor_h
#define __otbEntropyTextureFunctor_h

#include "otbTextureFunctorBase.h"

namespace otb
{
namespace Functor
{
/** \class EntropyTextureFunctor
 *  \brief This functor calculates the local entropy of an image
 *
 *  Computes joint histogram (neighborhood and offset neighborhood).
 *  This formula is:
 *  \f[ -\sum_{i}p_{x-y}(i)\log{(p_{x-y}(i))}  \f]
 *
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \ingroup Functor
 *  \ingroup Statistics
 */

template <class TIterInput1, class TIterInput2, class TOutput>
class ITK_EXPORT EntropyTextureFunctor :
public TextureFunctorBase<TIterInput1, TIterInput2, TOutput>
{
public:
  EntropyTextureFunctor(){};
  virtual ~EntropyTextureFunctor(){};

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
      for (unsigned r = 0; r<this->GetHisto().size(); r++)
	{
	  for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	    {
	      double p = static_cast<double>(this->GetHisto()[r][s]) * areaInv;
	      if (p != 0)
		out += (p * vcl_log(p));
	    }
	}
      if (out != 0.)
	out = -(out);

      return out;
  }
};

} // namespace Functor
} // namespace otb

#endif

