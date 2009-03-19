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
#ifndef __otbRemoteSensingRegion_h
#define __otbRemoteSensingRegion_h

#include <algorithm>
#include "itkObjectFactory.h"

#include "itkContinuousIndex.h"
#include "itkRegion.h"
#include "otbImageKeywordlist.h"

namespace otb
{

/** \class RemoteSensingRegion
 * \brief An RemoteSensingRegion represents a structured region of data.
 *
 * RemoteSensingRegion is an class that represents some structured portion or
 * piece of an Image. The RemoteSensingRegion is represented with an index and
 * a size in each of the n-dimensions of the image. (The index is the
 * corner of the image, the size is the lengths of the image in each of
 * the topological directions.)
 *
 * To be flexible enough, the region also contain information about the projection
 * in which this information is given. It can be a cartographic projection, but also
 * a sensor model projection (hence making it useful also with non-orthorectified
 * images).
 *
 * \sa VectorDataExtractROI
 */

template <class TType>
  class ITK_EXPORT RemoteSensingRegion : public itk::Region
{
public:
  /** Standard class typedefs. */
  typedef otb::RemoteSensingRegion<TType>           Self;
  typedef itk::Region                              Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** Standard part of all itk objects. */
  itkTypeMacro(RemoteSensingRegion, itk:Region);

  /** Typedef Support*/
  typedef TType                                   Type;


  /** Index typedef support. An index is used to access pixel values. */
  typedef itk::ContinuousIndex<Type>           IndexType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef itk::ContinuousIndex<Type>  SizeType;
  typedef itk::Size<2> StandardSizeType;

  virtual typename Superclass::RegionType GetRegionType() const
  {return Superclass::ITK_STRUCTURED_REGION;}

  /** Constructor. RemoteSensingRegion is a lightweight object that is not reference
   * counted, so the constructor is public. */
  RemoteSensingRegion(unsigned int dimension)
    {
    m_InputProjectionRef = "";
    m_Size.Fill(0.);
    m_Index.Fill(0.);
    }

  /** Constructor. RemoteSensingRegion is a lightweight object that is not reference
   * counted, so the constructor is public.  Default dimension is 2. */
  RemoteSensingRegion()
    {
      m_InputProjectionRef = "";
      m_Size.Fill(0.);
      m_Index.Fill(0.);
    }

  /** Destructor. RemoteSensingRegion is a lightweight object that is not reference
   * counted, so the destructor is public. */
  virtual ~RemoteSensingRegion(){};


  /** operator=. RemoteSensingRegion is a lightweight object that is not reference
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

  void SetSize(const StandardSizeType &size)
    {
      m_Size[0] = size[0];
      m_Size[1] = size[1];
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


    ImageKeywordlist GetKeywordList() const
    {
      return m_KeywordList;
    }

    void SetKeywordList(ImageKeywordlist kwl)
    {
      this->m_KeywordList = kwl;
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

  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    os << indent << "RemoteSensingRegion" << std::endl;
    os << indent << "Index:" << this->m_Index << std::endl;
    os << indent << "Size:" << this->m_Size << std::endl;
    os << indent << "Projection:" << this->m_InputProjectionRef << std::endl;
  }



private:

  IndexType m_Index;
  SizeType  m_Size;

  std::string  m_InputProjectionRef;
  ImageKeywordlist m_KeywordList; //if we want to specify the region in term of sensor geometry
};
//extern std::ostream & operator<<(std::ostream &os, const RemoteSensingRegion &region);

template<class TType>
    std::ostream & operator<<(std::ostream &os, const RemoteSensingRegion<TType> &region)
{
  region.Print(os);
  return os;
}

} // end namespace otb

#endif
