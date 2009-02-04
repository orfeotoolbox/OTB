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
#ifndef __otbTextureFunctorBase_h
#define __otbTextureFunctorBase_h

#include "otbMath.h"
#include "itkNeighborhood.h"

namespace otb
{
namespace Functor
{
/** \class TextureFunctorBase
 *  \brief This functor is the basic class for texture computation.
 *
 *   Implements the offset setting and the computation loop over each channel
 *   TIterInput1 and TIterInput2  are an iterators. The first one for the neighborhood, the second for the considered offset.
 *   TOutput is a PixelType.
 *
 *  \ingroup Functor
 *  \ingroup Statistics
 */

template <class TIterInput1, class TIterInput2, class TOutput>
class TextureFunctorBase
{
public:
  TextureFunctorBase()
  {
    m_Offset.Fill(1);
  };
  ~TextureFunctorBase() {};

  typedef TIterInput1                           IterType1;
  typedef TIterInput2                           IterType2;
  typedef TOutput                               OutputType;
  typedef typename IterType1::OffsetType        OffsetType;
  typedef typename IterType1::RadiusType        RadiusType;
  typedef typename OutputType::ValueType        OutputPixelType;
  typedef typename IterType1::InternalPixelType InternalPixelType;
  typedef typename IterType1::ImageType         ImageType;
  typedef itk::Neighborhood<InternalPixelType,::itk::GetImageDimension<ImageType>::ImageDimension>    NeighborhoodType;

  void SetOffset(OffsetType off)
  {
    m_Offset=off;
  };
  OffsetType GetOffset()
  {
    return m_Offset;
  };


  inline TOutput operator()(const IterType1 &it, const IterType2 &itOff)
    { 
      RadiusType radius = it.GetRadius();
      RadiusType radiusOff = itOff.GetRadius();
      OutputType outPix;
      outPix.SetSize( it.GetCenterPixel().GetSize() );
      outPix.Fill(0);
      OffsetType offset;
      offset.Fill(0);

      for ( unsigned int i=0; i<it.GetCenterPixel().GetSize(); i++ )
	{
	  NeighborhoodType inNeigh;
	  inNeigh.SetRadius(it.GetRadius());
	  NeighborhoodType offNeigh;
	  offNeigh.SetRadius(itOff.GetRadius());
	  
	  for ( int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++ )
	    {
	      offset[0] = l;
	      for ( int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); k++)
		{
		  offset[1] = k;
		  inNeigh[offset] = it.GetPixel(offset)[i];
		}
	    }
	  offset.Fill(0);
	  for ( int l = -static_cast<int>(radiusOff[0]); l <= static_cast<int>(radiusOff[0]); l++ )
	    {
	      offset[0] = l;
	      for ( int k = -static_cast<int>(radiusOff[1]); k <= static_cast<int>(radiusOff[1]); k++)
		{
		  offset[1] = k;
		  offNeigh[offset] = itOff.GetPixel(offset)[i];
		}
	    }
	  outPix[i] = static_cast<OutputPixelType>( this->ComputeOverSingleChannel(inNeigh, offNeigh) );
	}
      return outPix;
    }

  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff) = 0;
 
 private:
  OffsetType m_Offset;

};




} // namespace Functor
} // namespace otb

#endif

