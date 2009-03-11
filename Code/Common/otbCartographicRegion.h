/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageIORegion.h,v $
  Language:  C++
  Date:      $Date: 2007-08-22 17:47:20 $
  Version:   $Revision: 1.19 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbCartographicRegion_h
#define __otbCartographicRegion_h

#include <algorithm>
#include "itkObjectFactory.h"

#include "itkContinuousIndex.h"


namespace otb
{

/** \class CartographicRegion
 * \brief An CartographicRegion represents a structured region of data.
 *
 * CartographicRegion is an class that represents some structured portion or
 * piece of an Image. The CartographicRegion is represented with an index and
 * a size in each of the n-dimensions of the image. (The index is the
 * corner of the image, the size is the lengths of the image in each of
 * the topological directions.) 
 *
 * \sa Index
 * \sa Size

 */

template <class TType>
  class ITK_EXPORT CartographicRegion
{
public:
  /** Standard class typedefs. */
  typedef otb::CartographicRegion<TType>           Self;


  /** Standard part of all itk objects. */
  //itkTypeMacro(CartographicRegion, itk:Region);

  /** Typedef Support*/
  typedef TType                                   Type;
  

  /** Index typedef support. An index is used to access pixel values. */
  typedef itk::ContinuousIndex<Type>           IndexType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef itk::ContinuousIndex<Type>  SizeType;


  /** Constructor. CartographicRegion is a lightweight object that is not reference
   * counted, so the constructor is public. */
  CartographicRegion(unsigned int dimension)
    {
    m_InputProjectionRef = "";  
    m_Size.Fill(0.);
    m_Index.Fill(0.);
    }
  
  /** Constructor. CartographicRegion is a lightweight object that is not reference
   * counted, so the constructor is public.  Default dimension is 2. */
  CartographicRegion()
    {
      m_InputProjectionRef = "";
      m_Size.Fill(0.);
      m_Index.Fill(0.);
    }
  
  /** Destructor. CartographicRegion is a lightweight object that is not reference
   * counted, so the destructor is public. */
  virtual ~CartographicRegion(){};


  /** operator=. CartographicRegion is a lightweight object that is not reference
   * counted, so operator= is public. */
  void operator=(const Self& region) 
    {
    m_Index = region.m_Index;  
    m_Size = region.m_Size;
    m_InputProjectionRef = region.m_InputProjectionRef;
    }

  /** Set the index defining the corner of the region. */
  void SetOrigin(const IndexType &index) 
    {
    m_Index = index;
    }

  /** Get index defining the corner of the region. */
  const IndexType& GetOrigin() const
    {
    return m_Index;
    }
  
  /** Set the size of the region. This plus the index determines the
   * rectangular shape, or extent, of the region. */
  void SetSize(const SizeType &size)
    {
    m_Size = size;
    }

  /** Get the size of the region. */
  const SizeType& GetSize() const
    {
    return m_Size;
    }

  /** Convenience methods to get the size of the image in a particular
   * coordinate direction i. Do not try to access image sizes beyond the
   * the ImageDimension. */
  Type GetSize( unsigned int i) const
    {
    return m_Size[i];
    }
  Type GetOrigin( unsigned int i) const
    {
    return m_Index[i];
    }
  void SetSize(const unsigned int i,  Type size)
    {
    m_Size[i] = size;
    }
  void SetOrigin(const  unsigned int i,  Type idx)
    {
    m_Index[i] = idx;
    }

  /** Compare two regions. */
  bool
  operator==(const Self &region) const
    {
    bool same = 1;
    same = (m_Index == region.m_Index);
    same = same && (m_Size == region.m_Size);
    return same;
    }

  /** Compare two regions. */
  bool
  operator!=(const Self &region) const
    {
    bool same = 1;
    same = (m_Index == region.m_Index);
    same = same && (m_Size == region.m_Size);
    return !same;
    }
 
  /** Test if an index is inside */
  bool
  IsInside(const IndexType &index) const
    {
    for(unsigned int i=0; i<IndexType::IndexDimension; i++)
      {
      if( index[i] < m_Index[i] ) 
        {
        return false;
        }
      if( index[i] >= m_Index[i] + m_Size[i] ) 
        {
        return false;
        }
      }
    return true;
    }
 

  /**Get/Set InputProjectionRef  std::string*/
  void SetRegionProjection(std::string  projection)
    {m_InputProjectionRef = projection;}

  std::string
    GetRegionProjection()
    {
      return m_InputProjectionRef;
    }
    
  /** 
   * Crop 
   */
  bool
    Crop(const Self & region)
    {
      Type crop;
      bool cropPossible = true;

      // Can we crop?
      for (unsigned int i = 0; i < IndexType::IndexDimension && cropPossible; i++)
        {
          // Is left edge of current region to the right of the right edge
          // of the region to crop with? (if so, we cannot crop)
          if (m_Index[i] >= region.GetOrigin()[i]
              + static_cast<Type>(region.GetSize()[i]))
            {
              cropPossible = false;
            }
          // If right edge of the current region to the left of the left
          // edge of the region to crop with? (if so, we cannot crop)
          if (m_Index[i] + static_cast<Type>(m_Size[i]) <= region.GetOrigin()[i])
            {
              cropPossible = false;
            }
        }

      // if we cannot crop, return without changing anythin
      if (!cropPossible)
        {
          return cropPossible;
        }

      // we can crop, so crop
      for (unsigned int i=0; i <  IndexType::IndexDimension; i++)
        {
          // first check the start index
          if (m_Index[i] < region.GetOrigin()[i])
            {
              // how much do we need to adjust
              crop = region.GetOrigin()[i] - m_Index[i];

              // adjust the start index and the size of the current region
              m_Index[i] += crop;
              m_Size[i] -= static_cast<Type>(crop);
            }
          // now check the final size
          if (m_Index[i] + static_cast<Type>(m_Size[i])
              > region.GetOrigin()[i] + static_cast<Type>(region.GetSize()[i]))
            {
              // how much do we need to adjust
              crop = m_Index[i] + static_cast<Type>(m_Size[i])
                - region.GetOrigin()[i] - static_cast<Type>(region.GetSize()[i]);

              // adjust the size
              m_Size[i] -= static_cast<Type>(crop);
            }
        }

      return cropPossible;
    }
  

protected:

private:

  IndexType m_Index;
  SizeType  m_Size;
  
  std::string  m_InputProjectionRef;
};
//extern std::ostream & operator<<(std::ostream &os, const CartographicRegion &region); 
} // end namespace otb

#endif
