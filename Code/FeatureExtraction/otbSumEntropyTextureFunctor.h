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
#ifndef __otbSumEntropyTextureFunctor_h
#define __otbSumEntropyTextureFunctor_h

#include "otbTextureFunctorBase.h"

namespace otb
{
namespace Functor
{
/** \class SumEntropyTextureFunctor
 *  \brief This functor calculates the inverse difference moment of an image
 *
 *   Computes joint histogram (neighborhood and offset neighborhood) 
 *   which bins are computing using Scott formula.
 *   Computes the probabiltiy p for each pair of pixel.
 *   InverseSumMoment  is the sum 1/(1+(pi-poff)²)*p over the neighborhood.
 *   TIterInput is an ietrator, TOutput is a PixelType.
 *
 *  \ingroup Functor
 *  \ingroup 
 *  \ingroup Statistics
 */
template <class TIterInput1, class TIterInput2, class TOutput>
class ITK_EXPORT SumEntropyTextureFunctor : 
public TextureFunctorBase<TIterInput1, TIterInput2, TOutput>
{
public:
  SumEntropyTextureFunctor(){};
  ~SumEntropyTextureFunctor(){};

  typedef TIterInput1                           IterType1;
  typedef TIterInput2                           IterType2;
  typedef TOutput                               OutputType;
  typedef typename IterType1::OffsetType        OffsetType;
  typedef typename IterType1::RadiusType        RadiusType;
  typedef typename IterType1::InternalPixelType InternalPixelType;
  typedef typename IterType1::ImageType         ImageType;
  typedef itk::Neighborhood<InternalPixelType,::itk::GetImageDimension<ImageType>::ImageDimension>    NeighborhoodType;
  typedef std::vector<double>                   DoubleVectorType;
  typedef std::vector<int>                      IntVectorType;
  typedef std::vector<IntVectorType>            IntVectorVectorType;


  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    DoubleVectorType binsLength = this->StatComputation(neigh, neighOff);

    RadiusType radius = neigh.GetRadius();
    double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);
    double areaInv = 1/area;
    OffsetType offset;
    offset.Fill(0);
    OffsetType offsetOff;
    OffsetType offsetOffInit;

    offsetOffInit[0] = -radius[0]+this->GetOffset()[0]-1;
    offsetOffInit[1] = -radius[1]+this->GetOffset()[1]-1;

    int histoIdX = 0;
    int histoIdY = 0;
    double out = 0.;

    IntVectorType histoTemp;
    IntVectorVectorType histo;
    if (binsLength[0] != 0)
      histoTemp = IntVectorType( vcl_floor( static_cast<double>(this->GetMaxi()-this->GetMini())/binsLength[0])+1., 0);
    else
      histoTemp = IntVectorType( 1, 0 );

    if (binsLength[1] != 0)
        histo = IntVectorVectorType( vcl_floor(static_cast<double>(this->GetMaxiOff()-this->GetMiniOff())/binsLength[1])+1., histoTemp );
    else
      histo = IntVectorVectorType( 1, histoTemp );

    offsetOff = offsetOffInit;
    for ( int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++ )
	{
	  offsetOff[0]++;
	  offsetOff[1] = offsetOffInit[1];
	  offset[0] = l;
	  for ( int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); k++)
	    {
	      offsetOff[1]++;
	      offset[1] = k;
	      histoIdX = 0;
	      histoIdY = 0;
	      if ( binsLength[1] != 0)
		histoIdX = static_cast<int>(vcl_floor( (static_cast<double>(neighOff[offsetOff])-this->GetMiniOff()) / static_cast<double>(binsLength[1]) ));
	      if ( binsLength[0] !=0 )
		histoIdY = static_cast<int>(vcl_floor( (static_cast<double>(neigh[offset])-this->GetMini()) /static_cast<double>( binsLength[0]) ));
	      
	      histo[histoIdX][histoIdY]++;
	      
	    }
	}
  
    // loop over bin neighborhood values
    for (unsigned sB = 0; sB<histo[0].size(); sB++)
      { 
	double nCeil = (static_cast<double>(sB)+0.5)*binsLength[0];
	double nCeil2 = (static_cast<double>(sB)+histo[0].size()+0.5)*binsLength[0];
	double Px_y = 0.;
	double Px_y2 = 0.;
	for (unsigned r = 0; r<histo.size(); r++)
	  {
	    double rVal = (static_cast<double>(r)+0.5)*binsLength[1];
	    for (unsigned s = 0; s<histo[r].size(); s++)
	      { 
		double sVal = (static_cast<double>(s)+0.5)*binsLength[0];
		if( vcl_abs(rVal + sVal - nCeil) < vcl_abs(binsLength[0]) )
		  {
		    Px_y +=  static_cast<double>(histo[r][s])*areaInv;
		  }
		if( vcl_abs(rVal + sVal - nCeil2) < vcl_abs(binsLength[0]) )
		  {
		    Px_y2 +=  static_cast<double>(histo[r][s])*areaInv;
		  }
	      }
	  }
	if(Px_y != 0.)
	  out += Px_y * vcl_log(Px_y);
	if(Px_y2 != 0.)
	  out += Px_y2 * vcl_log(Px_y2);
      }

      
    if(out != 0)
      out = -out;

  
    return out;  
  }
  
};
 
 
} // namespace Functor
} // namespace otb

#endif

