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
    m_SpatialThreshold = 100;
    m_SpectralThreshold = 50;
    m_RatioMaxConsiderationNumber = 5;
    m_Alpha = 1;
    this->SetNumberOfDirections(20); // set the step too
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
  void SetNumberOfDirections( unsigned int D )
    { 
      m_NumberOfDirections = D;
      m_DirectionStep = 2*M_PI/static_cast<double>(D);
    };
  void SetDirectionStep( double step ){ m_DirectionStep = step; };
  unsigned int GetSpatialThreshold(){ return m_SpatialThreshold; };
  InternalPixelType GetSpectralThreshold(){ return m_SpectralThreshold; };  
  unsigned int GetRatioMaxConsiderationNumber(){ return m_RatioMaxConsiderationNumber; }; 
  double  GetAlpha(){ return m_Alpha; }; 
  unsigned int GetNumberOfDirections(){ return m_NumberOfDirections(); };


  inline TOutputValue operator()(const TIter& it)
  {
    double length = itk::NumericTraits<double>::NonpositiveMin();
    double width = itk::NumericTraits<double>::max();
    double sum = 0.;
    std::vector<double> di(m_NumberOfDirections, 0.);
    std::vector<double> minSorted(m_RatioMaxConsiderationNumber, width);
    std::vector<double> maxSorted(m_RatioMaxConsiderationNumber, length);
    std::vector<double> sti(m_NumberOfDirections, 0.);
    std::vector<unsigned int> lengthLine(m_NumberOfDirections, 0);

    std::vector<double>::iterator itVector;
    TOutputValue out;
    out.SetSize(6);
    out.Fill(0);
    //std::vector<double> radius(2, static_cast<double>(it.GetRadius()[0]));
    //radius[1] = static_cast<double>(it.GetRadius()[0]);
 
    OffsetType off;
    off.Fill(0);
    double SpatialThresholdDouble = static_cast<double>(m_SpatialThreshold);
    double NumberOfDirectionsDouble = static_cast<double>(m_NumberOfDirections);
    double dist = 0.;
    double angle = 0.;
    double sdiVal = 0.;
    double sumWMean = 0.;

    for( unsigned int d = 0; d<m_NumberOfDirections; d++ )
      {
	// Current angle direction
	angle = m_Alpha*static_cast<double>(d);

	// last offset in the diraction respecting spatial threshold
	off[0] = vcl_floor(SpatialThresholdDouble*vcl_cos( angle ) + 0.5);
	off[1] = vcl_floor(SpatialThresholdDouble*vcl_sin( angle ) + 0.5);
	// last indices in the diration respecting spectral threshold
	OffsetType offEnd = this->FindLastOffset( it, off );
	// computes distance = dist between the 2 segment point. One of them is the center pixel -> (0,0)
  	dist = vcl_sqrt( vcl_pow(static_cast<double>(offEnd[0]), 2 ) + vcl_pow(static_cast<double>(offEnd[1]), 2 ) );

	// for length computation
	if( dist>length)
	  length = dist;

	// for width computation 
	if( dist<width)
	  width = dist;

	// for PSI computation
	sum += dist;

	// for w-mean computation
	sdiVal = this->ComputeSDi(it, offEnd);

	// for Ratio computation
	bool doo = false;
	itVector = maxSorted.begin();
	while( itVector != maxSorted.end() && doo==false )
	  {
	    if( dist>(*itVector) )
	      {
		maxSorted.insert(itVector, dist);
		maxSorted.pop_back();
		doo=true;
	      }
	    ++itVector;
	  }
	doo = false;
	itVector = minSorted.begin();
	while( itVector != minSorted.end() && doo==false )
	  {
	    if( dist<(*itVector) )
	      {
		minSorted.insert(itVector, dist);
		minSorted.pop_back();
		doo=true;
	      }
	    ++itVector;
	  }
	di[d] = dist;
	lengthLine[d] = dist;//static_cast<unsigned int>( vcl_sqrt(vcl_pow(static_cast<double>(offEnd[0]), 2) + vcl_pow(static_cast<double>(offEnd[1]), 2)) );
	sti[d] = sdiVal;
	if(sdiVal!=0.)
	  sumWMean += (m_Alpha*(dist-1)*dist/*lengthLine[n]*di[n]*/)/sdiVal;
      }

    /////// FILL OUTPUT
    // length
    out[0] = length;
    // width
    out[1] = width;
    // PSI
    out[2] = sum/NumberOfDirectionsDouble;
    // w-mean
    out[3] = sumWMean/NumberOfDirectionsDouble;
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
    out[5] = vcl_sqrt(sumPSI)/(NumberOfDirectionsDouble-1.);

   
    return out;

  }


  /** Checks spectral threshold condition 
   *  the last point in the directiuon is the first that doesn't 
   *  respect the spectral condition.
   */
  OffsetType FindLastOffset( const TIter & it, const OffsetType & stopOffset )
    {	
      bool res = true;
      OffsetType currentOff;
      currentOff.Fill(0);
      int signX = 1;
      if(stopOffset[0]<0)
	signX = -1;
      int signY = 1;
      if(stopOffset[1]<0)
	signY = -1;

      currentOff[0]=signX;
      double slop = 0.;
      if(stopOffset[0]!=0)
	slop = static_cast<double>(stopOffset[1] / static_cast<double>(stopOffset[0]) );

      bool isInside = true;
      while( isInside == true && res == true)	
	{
	  // get the nearest point : Bresenham algo
	  if(stopOffset[0]!=0)
	    currentOff[1] = static_cast<int>( vcl_floor( slop*static_cast<double>(currentOff[0]) + 0.5 ));
	  else
	    currentOff[1] += signY;

	  if( vcl_abs(it.GetPixel(currentOff)[0]-it.GetCenterPixel()[0]) > m_SpectralThreshold )
	    {
	      res = false;
	    }
	  else
	    currentOff[0]+=signX;
       
	  if( signX*currentOff[0]>signX*stopOffset[0] && stopOffset[0]!=0)
	    isInside = false;
	  else if( signY*currentOff[1]>signY*stopOffset[1] && stopOffset[1] != 0 )
	    isInside = false;
	}

      return currentOff;
    }


  /** Computes SD in the ith direction */
  double ComputeSDi( const TIter & it, const OffsetType & stopOffset)
    {
      double SDi = 0.;
      bool canGo = true;
      OffsetType currentOff;
      currentOff.Fill(0);
      unsigned int nbElt = 0;
      double mean = 0.;
      double slop = 0.;
      if(stopOffset[0]!=0)
	slop = static_cast<double>(stopOffset[1] / static_cast<double>(stopOffset[0]) );
      
      int signX = 1;
      if(stopOffset[0]<0)
	signX = -1;
      int signY = 1;
      if(stopOffset[1]<0)
	signY = -1;
      currentOff[0]=signX;

      bool isInside = true;
      // First compute mean
      while( isInside == true && canGo == true )
	{
	  // get the nearest point : Bresenham algo
	  if(stopOffset[0]!=0)
	    currentOff[1] = static_cast<int>( vcl_floor( slop*static_cast<double>(currentOff[0]) + 0.5 ));
	  else
	    currentOff[1]+=signY;
 
	  mean += static_cast<double>(it.GetPixel(currentOff)[0]);
	  nbElt++;

	  if( vcl_abs(it.GetPixel(currentOff)[0]-it.GetCenterPixel()[0]) >= m_SpectralThreshold )
	      canGo = false;
	  else
	    currentOff[0]+=signX;
 
	  if( signX*currentOff[0]>signX*stopOffset[0] && stopOffset[0]!=0)
	    isInside = false;
	  else if( signY*currentOff[1]>signY*stopOffset[1] && stopOffset[1] != 0 )
	    isInside = false;
	}
      
      mean /= static_cast<double>(nbElt);
      currentOff[0] = signX;
      currentOff[1] = 0;
      isInside = true;      

      while( isInside == true && canGo == true )
	{
	  // get the nearest point : Bresenham algo
	  if(stopOffset[0]!=0)
	    currentOff[1] = static_cast<int>( vcl_floor( slop*static_cast<double>(currentOff[0]) + 0.5 ));
	  else
	    currentOff[1]+=signY;
	  SDi += vcl_pow((static_cast<double>(it.GetPixel(currentOff)[0]) - mean), 2);
	  if( vcl_abs(it.GetPixel(currentOff)[0]-it.GetCenterPixel()[0]) >= m_SpectralThreshold )
	      canGo = false;
	  else
	    currentOff[0]+=signX;

	       
	  if( signX*currentOff[0]>signX*stopOffset[0] && stopOffset[0]!=0)
	    isInside = false;
	  else if( signY*currentOff[1]>signY*stopOffset[1] && stopOffset[1] != 0 )
	    isInside = false;
	}
      return vcl_sqrt(SDi);
    }

protected:
  /** spectral threshold conditon*/
  InternalPixelType m_SpectralThreshold;
  /** spatial threshold condition */
  unsigned int m_SpatialThreshold;
  /** Max nulber of min and considered for Ration computation */
  unsigned int m_RatioMaxConsiderationNumber;
  /** constant to adjust w-mean values */
  double m_Alpha;
  /** Number of direction considered */
  unsigned int m_NumberOfDirections;
  /** Angular step between 2 directions (between  and 2*pi). */
  double m_DirectionStep;
};

} // end namespace functor
} // end namespace otb


#endif
