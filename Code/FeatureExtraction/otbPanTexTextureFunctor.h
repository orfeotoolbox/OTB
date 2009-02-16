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
#ifndef __otbPanTexTextureFunctor_h
#define __otbPanTexTextureFunctor_h

#include "otbContrastTextureFunctor.h"

namespace otb
{
namespace Functor
{
/** \class PanTexTextureFunctor
 *  \brief This functor calculates the contrast image texture according to Haralick descriptors.
 *
 *  Computes contrast using joint histogram (neighborhood and offset neighborhood).
 *  The formula is:
 *  $ \sum_{n}n^2.p_{x-y}(n)^2 $
 *  TIterInput is an iterator, TOutput is a PixelType.
 *
 *  \sa TextureFunctorBase
 *  \ingroup Functor
 *  \ingroup Statistics
 */

template <class TIterInput, class TOutput>
class ITK_EXPORT PanTexTextureFunctor : 
public ContrastTextureFunctor<TIterInput, TOutput>
{
public:
  PanTexTextureFunctor()
    {
      OffsetType off;
      off[0] = 0;
      off[1] = 1;
      m_OffsetList.push_back(off); //(0,1)
      off[1] = 2;
      m_OffsetList.push_back(off); //(0,2)
      off[0] = 1;
      off[1] = -2;
      m_OffsetList.push_back(off); //(1,-2)
      off[1] = -1;
      m_OffsetList.push_back(off); //(1,-1)
      off[1] = 0;
      m_OffsetList.push_back(off); //(1,0)
      off[1] = 1;
      m_OffsetList.push_back(off); //(1,1)
      off[1] = 2;
      m_OffsetList.push_back(off); //(1,2)
      off[0] = 2;
      off[1] = -1;
      m_OffsetList.push_back(off); //(2,-1)
      off[1] = 0;
      m_OffsetList.push_back(off); //(2,0)
      off[1] = 1;
      m_OffsetList.push_back(off); //(2,1)
    };

  virtual ~PanTexTextureFunctor(){};

  typedef TIterInput                            IterType;
  typedef TOutput                               OutputType;
  typedef typename IterType::InternalPixelType  InternalPixelType;
  typedef typename IterType::ImageType          ImageType;
  typedef typename IterType::OffsetType         OffsetType;
  typedef itk::Neighborhood<InternalPixelType,::itk::GetImageDimension<ImageType>::ImageDimension>    NeighborhoodType;
  typedef ContrastTextureFunctor<IterType, OutputType> Superclass;


  virtual double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
    { 
      // Loop over each offset
      double out = itk::NumericTraits<double>::max();
      Superclass contrast;
      for(unsigned int k=0; k<m_OffsetList.size(); k++)
	{
	  contrast.SetOffset( m_OffsetList[k] );
	  double res = contrast.ComputeOverSingleChannel(neigh, neighOff);
	  if(res<out)
	    out=res;
	}

    
    return out;  
  }

 private:
  std::vector<OffsetType> m_OffsetList;
  
};
 
 
} // namespace Functor
} // namespace otb

#endif

