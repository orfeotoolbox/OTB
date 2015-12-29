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
#ifndef __otbRemoteSensingRegion_h
#define __otbRemoteSensingRegion_h

#include <algorithm>
#include <iomanip>


#include "otbImageKeywordlist.h"
#include "itkImageRegion.h"

namespace otb
{

/** \class RemoteSensingRegion
 * \brief An RemoteSensingRegion represents a structured region of data.
 *
 * RemoteSensingRegion is an class that represents some structured portion or
 * piece of an Image. The RemoteSensingRegion is represented with an index and
 * a size in each of the n-dimensions of the image. (The index is the
 * corner of the image, the size is the lengths of the image in each of
 * the topological directions. <b>For this class, the notion of index and origin
 * are the same.</b>)
 *
 * As the size and origin can be given in various system coordinates, they contain
 * double values (through the use of an itk::ContinuousIndex).
 *
 * To be flexible enough, the region also contain information about the projection
 * in which this information is given. It can be a cartographic projection, but also
 * a sensor model projection (hence making it useful also with non-orthorectified
 * images).
 *
 * \sa VectorDataExtractROI
 *
 * \ingroup OTBImageBase
 */

template <class TType>
class ITK_EXPORT RemoteSensingRegion : public itk::Region
{
public:
  /** Standard class typedefs. */
  typedef otb::RemoteSensingRegion<TType> Self;
  typedef itk::Region                     Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Standard part of all itk objects. */
  itkTypeMacro(RemoteSensingRegion, itk : Region);

  /** Typedef Support*/
  typedef TType Type;

  /** Index typedef support. An index is used to access pixel values. */
  typedef itk::ContinuousIndex<Type> IndexType;
  typedef itk::Point<Type, 2>        PointType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef itk::ContinuousIndex<Type> SizeType;
//   typedef itk::Size<2>                         StandardSizeType;

  /** ImageRegion typedef needed by the GetImageRegion() method */
  typedef itk::ImageRegion<2> ImageRegionType;

  virtual typename Superclass::RegionType GetRegionType() const
  {return Superclass::ITK_STRUCTURED_REGION; }

  /** Constructor. RemoteSensingRegion is a lightweight object that is not reference
   * counted, so the constructor is public.  Default dimension is 2. */
  RemoteSensingRegion()
    {
    m_InputProjectionRef = "";
    m_Size.Fill(0.);
    m_Index.Fill(0.);
    }

  /** Constructor. RemoteSensingRegion is a lightweight object that is not reference
    * counted, so the constructor is public.  Default dimension is 2. */
  RemoteSensingRegion(const itk::ImageRegion<2>&region)
    {
    m_InputProjectionRef = "";
    m_Size[0] = region.GetSize()[0];
    m_Size[1] = region.GetSize()[1];
    m_Index[0] = region.GetIndex()[0];
    m_Index[1] = region.GetIndex()[1];
    }

  /** Destructor. RemoteSensingRegion is a lightweight object that is not reference
   * counted, so the destructor is public. */
  virtual ~RemoteSensingRegion(){}

  /** operator=. RemoteSensingRegion is a lightweight object that is not reference
   * counted, so operator= is public. */
  void operator =(const Self& region)
  {
    m_Index = region.m_Index;
    m_Size = region.m_Size;
    m_InputProjectionRef = region.m_InputProjectionRef;
    m_KeywordList        = region.m_KeywordList;
  }

  /** This method provides explicit conversion to itk::ImageRegion<2>,
   * so as to allow using RemoteSensingRegion to specify requested
   * region for images or images iterators.
   */
  const ImageRegionType GetImageRegion()
  {
    ImageRegionType imageRegion;
    typename ImageRegionType::IndexType irIndex;
    typename ImageRegionType::SizeType irSize;

    irIndex[0] = static_cast<unsigned long>(vcl_floor(m_Index[0]));
    irIndex[1] = static_cast<unsigned long>(vcl_floor(m_Index[1]));
    irSize[0] = static_cast<unsigned long>(vcl_ceil(m_Size[0]));
    irSize[1] = static_cast<unsigned long>(vcl_ceil(m_Size[1]));

    imageRegion.SetIndex(irIndex);
    imageRegion.SetSize(irSize);

    return imageRegion;
  }

  /** Set the index defining the corner of the region. */
  void SetOrigin(const IndexType& index)
  {
    m_Index = index;
  }

  /** Set the index defining the corner of the region. */
  void SetOrigin(const PointType& index)
  {
    m_Index[0] = index[0];
    m_Index[1] = index[1];
  }

  /** Get index defining the corner of the region. */
  const IndexType& GetOrigin() const
  {
    return m_Index;
  }

  /** Set the index defining the corner of the region. */
  void SetIndex(const IndexType& index)
  {
    m_Index = index;
  }

  /** Get index defining the corner of the region. */
  const IndexType& GetIndex() const
  {
    return m_Index;
  }

  /** Set the size of the region. This plus the index determines the
   * rectangular shape, or extent, of the region. */
  void SetSize(const SizeType& size)
  {
    m_Size = size;
  }

//   void SetSize(const StandardSizeType &size)
//     {
//       m_Size[0] = size[0];
//       m_Size[1] = size[1];
//     }

  /** Get the size of the region. */
  const SizeType& GetSize() const
  {
    return m_Size;
  }

  /** Convenience methods to get the size of the image in a particular
   * coordinate direction i. Do not try to access image sizes beyond the
   * the ImageDimension. */
  Type GetSize(unsigned int i) const
  {
    return m_Size[i];
  }
  Type GetOrigin(unsigned int i) const
  {
    return m_Index[i];
  }
  void SetSize(const unsigned int i,  Type size)
  {
    m_Size[i] = size;
  }
  void SetOrigin(const unsigned int i,  Type idx)
  {
    m_Index[i] = idx;
  }

  const ImageKeywordlist& GetKeywordList() const
  {
    return m_KeywordList;
  }

  void SetKeywordList(const ImageKeywordlist& kwl)
  {
    this->m_KeywordList = kwl;
  }

  /** Compare two regions. */
  bool
  operator ==(const Self& region) const
  {
    bool same = 1;
    same = (m_Index == region.m_Index);
    same = same && (m_Size == region.m_Size);
    return same;
  }

  /** Compare two regions. */
  bool
  operator !=(const Self& region) const
  {
    bool same = 1;
    same = (m_Index == region.m_Index);
    same = same && (m_Size == region.m_Size);
    return !same;
  }

  /** Test if an index is inside */
  bool
  IsInside(const IndexType& index) const
  {
    for (unsigned int i = 0; i < IndexType::IndexDimension; ++i)
      {
      if ((index[i] < m_Index[i])
          && (index[i] < m_Index[i] + m_Size[i]))
        {
        return false;
        }
      if ((index[i] >= m_Index[i])
          && (index[i] >= m_Index[i] + m_Size[i]))
        {
        return false;
        }
      }
    return true;
  }

  /**Get/Set InputProjectionRef  std::string*/
  void SetRegionProjection(const std::string& projection)
  {m_InputProjectionRef = projection; }

  std::string
  GetRegionProjection()
  {
    return m_InputProjectionRef;
  }

  /**
   * Crop
   */
  bool
  Crop(const Self& region)
  {
    Type crop;
    bool cropPossible = true;

    // Can we crop?
    for (unsigned int i = 0; i < IndexType::IndexDimension && cropPossible; ++i)
      {

      if (
        ((region.GetOrigin()[i] <= m_Index[i])
         && (region.GetOrigin()[i] <= m_Index[i] + static_cast<Type>(m_Size[i]))
         && ((region.GetOrigin()[i] + static_cast<Type>(region.GetSize()[i])) <=  m_Index[i])
         && ((region.GetOrigin()[i] +
              static_cast<Type>(region.GetSize()[i])) <= m_Index[i] + static_cast<Type>(m_Size[i]))
        )
        ||
        ((region.GetOrigin()[i] >= m_Index[i])
         && (region.GetOrigin()[i] >= m_Index[i] + static_cast<Type>(m_Size[i]))
         && ((region.GetOrigin()[i] + static_cast<Type>(region.GetSize()[i])) >=  m_Index[i])
         && ((region.GetOrigin()[i] +
              static_cast<Type>(region.GetSize()[i])) >= m_Index[i] + static_cast<Type>(m_Size[i]))
        )
        )
        {
        return false;
        }

      }

    // we can crop, so crop
    for (unsigned int i = 0; i <  IndexType::IndexDimension; ++i)
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
    os << std::setprecision(15);
    os << indent << "RemoteSensingRegion" << std::endl;
    os << indent << "Index:" << this->m_Index << std::endl;
    os << indent << "Size:" << this->m_Size << std::endl;
    os << indent << "Projection:" << this->m_InputProjectionRef << std::endl;
    os << indent << "Keywordlist: " << this->m_KeywordList << std::endl;
  }

private:

  IndexType m_Index;
  SizeType  m_Size;

  std::string      m_InputProjectionRef;
  ImageKeywordlist m_KeywordList; //if we want to specify the region in term of sensor geometry
};
//extern std::ostream & operator<<(std::ostream &os, const RemoteSensingRegion &region);

template<class TType>
std::ostream & operator <<(std::ostream& os, const RemoteSensingRegion<TType>& region)
{
  region.Print(os);
  return os;
}

template<class ImageType, class RemoteSensingRegionType>
typename ImageType::RegionType
TransformPhysicalRegionToIndexRegion(const RemoteSensingRegionType& region, const ImageType* image)
{
  typename ImageType::RegionType outputRegion;
  typename ImageType::RegionType::IndexType index;
  typename ImageType::RegionType::IndexType index2;

  typename ImageType::PointType point;
  point[0] = region.GetIndex()[0];
  point[1] = region.GetIndex()[1];
  image->TransformPhysicalPointToIndex(point, index);

  point[0] = region.GetIndex()[0] + region.GetSize()[0];
  point[1] = region.GetIndex()[1] + region.GetSize()[1];
  image->TransformPhysicalPointToIndex(point, index2);

  typename ImageType::RegionType::SizeType size;
  size[0] = std::abs(index2[0] - index[0]);
  size[1] = std::abs(index2[1] - index[1]);

  index[0] = std::min(index2[0], index[0]);
  index[1] = std::min(index2[1], index[1]);

  outputRegion.SetIndex(index);
  outputRegion.SetSize(size);
  return outputRegion;
}


} // end namespace otb

#endif
