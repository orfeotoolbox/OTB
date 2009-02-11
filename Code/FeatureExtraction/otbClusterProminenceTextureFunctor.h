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
#ifndef __otbClusterProminenceTextureFunctor_h
#define __otbClusterProminenceTextureFunctor_h

#include "otbMeanTextureFunctor.h"

namespace otb
{
namespace Functor
{
/** \class ClusterProminenceTextureFunctor
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
class ITK_EXPORT ClusterProminenceTextureFunctor : 
public MeanTextureFunctor<TIterInput1, TIterInput2, TOutput>
{
public:
  ClusterProminenceTextureFunctor(){};
  ~ClusterProminenceTextureFunctor(){};

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
	double sumTemp = 0.;
	for (unsigned s = 0; s<this->GetHisto()[r].size(); s++)
	  {
	    double p = this->GetHisto()[r][s]*areaInv;
	    double sumPixel = (static_cast<double>(s)+0.5)*this->GetNeighBinLength() + (static_cast<double>(r)+0.5)*this->GetOffsetBinLength();
	    out += vcl_pow( sumPixel - 2*mean, 4) * p;
	  }
      }

    return out;  
  }
  
};
 
 
} // namespace Functor
} // namespace otb

#endif

