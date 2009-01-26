/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAcosImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-10-07 14:09:10 $
  Version:   $Revision: 1.24 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSimplePointCountStrategy_h
#define __otbSimplePointCountStrategy_h 

#include "otbMath.h"


namespace otb
{
  
/** \class Count
 * \brief Compute the density of a neighboorhood centerred in a pixel
 *
 * This filter is templated over the pixel type of the input image
 * and the pixel type of the output image. 
 *
 * The filter will walk over all the pixels in the input image, and for
 * each one of them it will do the following: 

 */

  
template< class TPointSet, class TRadiusType , class TIndexType>
class Count
{
public:
  Count() {};
  ~Count() {};
  
  inline float operator()( const TPointSet * pointSet , const  TRadiusType & size , const TIndexType & index  )
    {
      
      int accu = 0;
      float surface = 3.14*size*size;

      typedef typename TPointSet::PointsContainer::ConstIterator         iteratorType;
      
      iteratorType it = pointSet->GetPoints()->Begin();
      
      while( it != pointSet->GetPoints()->End())
	{
	  float distX2 =( index[0]-it.Value()[0])*( index[0]-it.Value()[0]);
	  float distY2 =( index[1]-it.Value()[1])*( index[1]-it.Value()[1]);
	  float dist = vcl_sqrt(distX2 + distY2);
	  
	  if(dist <= size)
	    accu++;
	  
	  ++it;
	}
            
      return static_cast<float>(accu)/surface;
    }
}; 


} // end namespace otb


#endif
