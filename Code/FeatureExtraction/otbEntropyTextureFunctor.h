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
 *   Computes joint histogram (neighborhood and offset neighborhood) 
 *   which bins are computing using Scott formula.
 *   Computes the probabiltiy p for each pair of pixel.
 *   Entropy  is the sum p.log(p) over the neighborhood.
 *   TIterInput is an ietrator, TOutput is a PixelType.
 *
 *  \ingroup Functor
 *  \ingroup Statistics
 */
template <class TIterInput1, class TIterInput2, class TOutput>
class ITK_EXPORT EntropyTextureFunctor : 
public TextureFunctorBase<TIterInput1, TIterInput2, TOutput>
{
public:
  EntropyTextureFunctor()
    {
      m_Mini = itk::NumericTraits<double>::max();
      m_MiniOff = itk::NumericTraits<double>::max();
      m_Maxi = itk::NumericTraits<double>::NonpositiveMin();
      m_MaxiOff = itk::NumericTraits<double>::NonpositiveMin();
    };
  ~EntropyTextureFunctor(){};

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


  double GetMaxi(){ return m_Maxi; };
  double GetMini(){ return m_Mini; };
  double GetMaxiOff(){ return m_MaxiOff; };
  double GetMiniOff(){ return m_MiniOff; };
  
  /** Computes the histogram bins using Scott formula, plus min/max. */
  DoubleVectorType StatComputation(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
    {
      DoubleVectorType output;
      double binLength    = 0;
      double binLengthOff =0;
      double mean    = 0.;
      double meanOff = 0.;
      RadiusType radius = neigh.GetRadius();
      m_Mini = itk::NumericTraits<double>::max();
      m_MiniOff = itk::NumericTraits<double>::max();
      m_Maxi = itk::NumericTraits<double>::NonpositiveMin();
      m_MaxiOff = itk::NumericTraits<double>::NonpositiveMin();
      double area = static_cast<double>(neigh.GetSize()[0]*neigh.GetSize()[1]);
      double areaInv = 1/area;
      double scottCoef =  3.5 /(vcl_pow(area, 1/3) );
      
      OffsetType offset;
      offset.Fill(0);
      OffsetType offsetOff;
      OffsetType offsetOffInit;
      
      offsetOffInit[0] = -radius[0]+this->GetOffset()[0]-1;
      offsetOffInit[1] = -radius[1]+this->GetOffset()[1]-1;
      
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
	      mean += static_cast<double>(neigh[offset]);
	      meanOff += static_cast<double>(neighOff[offsetOff]);
	    
	      m_Mini    = std::min(static_cast<double>(neigh[offset]),      m_Mini);
	      m_Maxi    = std::max(static_cast<double>(neigh[offset]),      m_Maxi);
	      m_MiniOff = std::min(static_cast<double>(neighOff[offsetOff]),m_MiniOff);
	      m_MaxiOff = std::max(static_cast<double>(neighOff[offsetOff]),m_MaxiOff);
	    }
	}
      mean *= areaInv;
      meanOff *= areaInv;
    
      offsetOff = offsetOffInit;
 
      for( int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++ )
	{
	  offsetOff[0]++;
	  offsetOff[1] = offsetOffInit[1];  
	  offset[0] = l;
	  for( int k = -static_cast<int>(radius[1]); k <= static_cast<int>(radius[1]); k++)
	    {
	      offsetOff[1]++;
	      offset[1] = k;
	      binLength += vcl_pow( (mean-static_cast<double>(neigh[offset])), 2);
	      binLengthOff += vcl_pow( (meanOff-static_cast<double>(neighOff[offsetOff])), 2);
	    }
	}
    
      binLength *= areaInv;
      binLength = vcl_sqrt( binLength );
      binLengthOff *= areaInv;
      binLengthOff = vcl_sqrt( binLengthOff );
    
      output.push_back( scottCoef*binLength );
      output.push_back( scottCoef*binLengthOff );

      return output;
    }


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
      histoTemp = IntVectorType( vcl_floor( static_cast<double>(m_Maxi-m_Mini)/binsLength[0])+1., 0);
    else
      histoTemp = IntVectorType( 1, 0 );
    if (binsLength[1] != 0)
        histo = IntVectorVectorType( vcl_floor(static_cast<double>(m_MaxiOff-m_MiniOff)/binsLength[1])+1., histoTemp );
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
		histoIdX = static_cast<int>(vcl_floor( (static_cast<double>(neighOff[offsetOff])-m_MiniOff) / static_cast<double>(binsLength[1]) ));
	      if ( binsLength[0] !=0 )
		histoIdY = static_cast<int>(vcl_floor( (static_cast<double>(neigh[offset])-m_Mini) /static_cast<double>( binsLength[0]) ));
	      
	      histo[histoIdX][histoIdY]++;
	      
	    }
	}

      for (unsigned r = 0; r<histo.size(); r++)
	{
	  for (unsigned s = 0; s<histo[r].size(); s++)
	    {
	      double p = histo[r][s] * areaInv;
	      if (p != 0)
		out += (p * vcl_log(p));
	    }
	}
      if (out != 0.)
	out = -(out);

      return out;
  }



private:
 /** Stores min/max neighborhood area values */
 double m_Mini;
 double m_Maxi;
 /** Stores min/max neighborhood+offset values */
 double m_MiniOff;
 double m_MaxiOff;
};




  } // namespace Functor
} // namespace otb

#endif

