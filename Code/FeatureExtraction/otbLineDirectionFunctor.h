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
#ifndef __otbLineDirectionFunctor_h
#define __otbLineDirectionFunctor_h

#include "otbMath.h"
#include "itkNumericTraits.h"
#include <iostream>
#include <vector>

namespace otb
{
/** \class LineDirectionFunctor
 *  \brief This functor first computes the spectral angle according to a reference pixel.
 *  \brief Then multiplies the result by a gaussian coefficient
 *  \brief And reverse the pixel values.
 */
namespace Functor
{
template<class TIter,class TOutputValue>
class LineDirectionFunctor
{
public:
  LineDirectionFunctor()
  {
    m_SpatialThreshold = 5;
    m_SpectralThreshold = 7;
    m_RatioMaxConsiderationNumber = 5;
    m_Alpha = 0.5;
  };

  ~LineDirectionFunctor() {};

  typedef typename TIter::InternalPixelType InternalPixelType;
  typedef typename TIter::SizeType          SizeType;
  typedef typename TIter::IndexType         IndexType;
  typedef typename TIter::OffsetType        OffsetType;
  typedef typename TOutputValue::ValueType  InternalOutputPixelType;
 
  void SetSpatialThreshold( unsigned int thresh ){ m_SpatialThreshold=thresh; };
  void SetSpectralThreshold( InternalPixelType thresh ){ m_SpectralThreshold=thresh; };
  void SetRatioMaxConsiderationNumber( unsigned int value ){ m_RatioMaxConsiderationNumber=value; };
  void SetAlpha( double alpha ){ m_Alpha=alpha; };
  unsigned int GetSpatialThreshold(){ return m_SpatialThreshold; };
  InternalPixelType GetSpectralThreshold(){ return m_SpectralThreshold; };  
  unsigned int GetRatioMaxConsiderationNumber(){ return m_RatioMaxConsiderationNumber; }; 
  double  GetAlpha(){ return m_Alpha; }; 

  inline TOutputValue operator()(const TIter& it)
  {
    double length = itk::NumericTraits<double>::NonpositiveMin();
    double width = itk::NumericTraits<double>::max();
    double sum = 0.;
    std::vector<double> di;
    std::vector<double> minSorted(m_RatioMaxConsiderationNumber, width);
    std::vector<double> maxSorted(m_RatioMaxConsiderationNumber, length);
    std::vector<double> sti;
    std::vector<unsigned int> lengthLine;

    std::vector<double>::iterator itVector;
    TOutputValue out;
    out.SetSize(6);
    out.Fill(0);
    SizeType radius = it.GetRadius();
 
    OffsetType off;
    off.Fill(0);
    double SpatialThresholdDouble = static_cast<double>(m_SpatialThreshold);

    double D = 0; // nulber of directions. Porbablity false : diag, vert, hor...
    for (int l = -static_cast<int>(radius[0]); l <= static_cast<int>(radius[0]); l++ )
      {
	off[0] = l;
	double ll = vcl_pow(static_cast<double>(l), 2);

	// start at 0 because of the image walking
	for (int k = 0; k <= static_cast<int>(radius[1]); k++ )
	  {
	    if( k == 0 && l < 0 )
	      {	
		double val = 0;
		double sdiVal = 1.;
		double kk = vcl_pow(static_cast<double>(k), 2);
		if( vcl_sqrt(ll + kk) < SpatialThresholdDouble )
		  {
		    off[1] = k;

		    // check the spectral threshold
		    bool isGood = this->isLineRespectSpectralThreshold(it, off);
		    if (isGood == true)
		      { 
			val = vcl_sqrt( vcl_pow(static_cast<double>(it.GetCenterPixel()/*[0]*/)-static_cast<double>(it.GetPixel(off)/*[0]*/), 2 ) 
					       + vcl_pow(static_cast<double>(it.GetCenterPixel()/*[1]*/)-static_cast<double>(it.GetPixel(off)/*[1]*/), 2 ) );
		
			// for length computation
			if( val>length)
			  length = val;
			// for width computation 
			if( val<width)
			  width = val;
			// for PSI computation
			sum += val;
			// for w-mean computation
			sdiVal = this->ComputeSDi(it, off);
			// for Ratio computation
			bool doo = false;
			itVector = maxSorted.begin();
			while( itVector != maxSorted.end() && doo==false )
			  {
			    if( val>(*itVector) )
			      {
				maxSorted.insert(itVector, val);
				maxSorted.pop_back();
				doo=true;
			      }
			    ++itVector;
			  }
			doo = false;
			itVector = minSorted.begin();
			while( itVector != minSorted.end() && doo==false )
			  {
			    if( val<(*itVector) )
			      {
				minSorted.insert(itVector, val);
				minSorted.pop_back();
				doo=true;
			      }
			    ++itVector;
			  }
			//std::cout<<"CA C FAIT OU PAS"<<std::endl;
		      }
		    di.push_back(val);
		    lengthLine.push_back(static_cast<unsigned int>(vcl_sqrt(ll + kk)-1));
		    sti.push_back(sdiVal);
		    D++;
		  } // if( vcl_sqrt(vcl_pow(i, 2)+vcl_pow(i, 2)) < m_SpatialTreshold )
	      
	      }// if( j == 0 && i<0 )
	  
	  }// for (int j = 0; j<= radius[1]; j++ )
      }

    /////// FILL OUTPUT
    // length
    out[0] = length;
    // width
    out[1] = width;
    // PSI
    out[3] = sum/D;
    // w-mean
    double sumWMean = 0.;
    for(unsigned int n=0; n<di.size(); n++)
      sumWMean += (m_Alpha*lengthLine[n]*di[n])/sti[n];
    out[4] = sumWMean/D;
    // ratio
    double sumMin = 0;
    double sumMax = 0;
    for(unsigned int t=0; t<m_RatioMaxConsiderationNumber ; t++)
      {
	sumMin += minSorted[t];
	sumMax += maxSorted[t];
      }
    if (sumMax != 0.)
      out[4] = vcl_atan(sumMin/sumMax);
    else if (sumMax == 0. && sumMin == 0.)
      out[4] = 1.;
    

    // SD
    double sumPSI = 0;
    for(unsigned int n=0; n<di.size(); n++)
      sumPSI += vcl_pow(di[n] - out[3] , 2);
    out[5] = vcl_sqrt(sumPSI)/(D-1);

   
    return out;

  }
  
  /** checks spectral threshold condition */
  bool isLineRespectSpectralThreshold( const TIter & it, const OffsetType & stopOffset )
    {
      bool res = true;
      OffsetType currentOff;
      currentOff.Fill(0);
      currentOff[0]++;
      double slop = static_cast<double>(stopOffset[1] / static_cast<double>(stopOffset[0]) );
      while( currentOff[0]<=stopOffset[0] && currentOff[1]<=stopOffset[1] && res == true)
	{
	  // get the nearest point : Bresenham algo
	  currentOff[1] = static_cast<int>( vcl_floor( slop*static_cast<double>(currentOff[0]) + 0.5 ));
	  if( vcl_abs(it.GetPixel(currentOff)-it.GetCenterPixel()) >= m_SpectralThreshold )
	    res = false;
	  currentOff[0]++;
	}
      return res;
    }
  

  /** Computes SD in the ith direction */
  double ComputeSDi( const TIter & it, const OffsetType & stopOffset)
    {
      double SDi = 0.;
      bool canGo = true;
      OffsetType currentOff;
      currentOff.Fill(0);
      currentOff[0]++;
      unsigned int nbElt = 0;
      double mean = 0;
      double slop = static_cast<double>(stopOffset[1] / static_cast<double>(stopOffset[0]) );
      // First compute mean
      while( currentOff[0]<=stopOffset[0] && currentOff[1]<=stopOffset[1] && canGo == true)
	{
	  // get the nearest point : Bresenham algo
	  currentOff[1] = static_cast<int>( vcl_floor( slop*static_cast<double>(currentOff[0]) + 0.5 ));
	  if( vcl_abs(it.GetPixel(currentOff)-it.GetCenterPixel()) >= m_SpectralThreshold )
	    {
	      mean = 0;
	      canGo = false;
	    }
	  else
	    {
	      mean += static_cast<double>(it.GetPixel(currentOff));
	    }
	  currentOff[0]++;
	  nbElt++;
	}
      
      mean /= static_cast<double>(nbElt);
      
      while( currentOff[0]<=stopOffset[0] && currentOff[1]<=stopOffset[1] && canGo == true)
	{
	  // get the nearest point : Bresenham algo
	  currentOff[1] = static_cast<int>( vcl_floor( slop*static_cast<double>(currentOff[0]) + 0.5 ));
	  if( vcl_abs(it.GetPixel(currentOff)-it.GetCenterPixel()) >= m_SpectralThreshold )
	    {
	      SDi = 0;
	      canGo = false;
	    }
	  else
	    {
	      SDi += vcl_pow((static_cast<double>(it.GetPixel(currentOff)) - mean), 2);
	    }
	  currentOff[0]++;
	}
      return vcl_sqrt(SDi);
    }

protected:
  InternalPixelType m_SpectralThreshold;
  unsigned int m_SpatialThreshold;
  unsigned int m_RatioMaxConsiderationNumber;
  double m_Alpha;
};

} // end namespace functor
} // end namespace otb


#endif
