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
#ifndef __otbVarianceTextureFunctor_h
#define __otbVarianceTextureFunctor_h

#include "otbMeanTextureFunctor.h"

namespace otb
{
namespace Functor
{
/** \class VarianceTextureFunctor
 *  \brief This functor calculates the cluster shade image texture.
 *
 *  Computes variance using joint histogram (neighborhood and offset neighborhood) .
 *  The formula is:
 *  $ \sum_{i}{2N}(i-SAV)^2.p_{x+y}(i) $
 *  With $SAV$ is the sum average texture value.
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \sa MeanTextureFunctor
 *  \sa TextureFunctorBase
 *  \ingroup Functor
 *  \ingroup Statistics
 */

template <class TIterInput1, class TIterInput2, class TOutput>
class ITK_EXPORT VarianceTextureFunctor : 
public MeanTextureFunctor<TIterInput1, TIterInput2, TOutput>
{
public:
  VarianceTextureFunctor(){};
  ~VarianceTextureFunctor(){};

  typedef TIterInput1                           IterType1;
  typedef TIterInput2                           IterType2;
  typedef TOutput                               OutputType;
  typedef typename IterType1::InternalPixelType InternalPixelType;
  typedef typename IterType1::ImageType         ImageType;
  typedef itk::Neighborhood<InternalPixelType,::itk::GetImageDimension<ImageType>::ImageDimension>    NeighborhoodType;
  typedef MeanTextureFunctor<TIterInput1, TIterInput2, TOutput> Superclass;

  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    this->ComputeJointHistogram(neigh, neighOff);
    double mean = Superclass::ComputeOverSingleChannel(neigh, neighOff);
    double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);
    double areaInv = 1/area;
    double out = 0.;
    for (unsigned r = 0; r<this->GetHisto().size(); r++)
      {
	for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	  {
	    double p = static_cast<double>(this->GetHisto()[r][s]) * areaInv;
	    double square = vcl_pow( ( ( (static_cast<double>(s)+0.5)*this->GetNeighBinLength() ) - this->GetMean()), 2);
	    out += square*p;
	  }
      }
    
    return out;
  }
};


} // namespace Functor
} // namespace otb

#endif

