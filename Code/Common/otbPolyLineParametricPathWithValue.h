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
#ifndef __otbPolyLineParametricPathWithValue_h
#define __otbPolyLineParametricPathWithValue_h

#include "itkPolyLineParametricPath.h"
#include "itkMetaDataObject.h"
#include "otbRemoteSensingRegion.h"
#include "otbMacro.h"

namespace otb
{
/** \class PolyLineParametricPathWithValue
 *  \brief This class implement a PolyLineParametricPath for which a value can be set.
 * The value is stored in the itk::MetaDataDictionary.
 * The precision of the value can be set.
 *
 * \sa itk::PolyLineParametricPath.
 */
template <class TValue, unsigned int VDimension = 2>
class ITK_EXPORT PolyLineParametricPathWithValue
  : public itk::PolyLineParametricPath<VDimension>
{
public:
  /** Standard typedefs */
  typedef PolyLineParametricPathWithValue         Self;
  typedef itk::PolyLineParametricPath<VDimension> Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(PolyLineParametricPath, MySuperclass);

  /** Template parameters typedefs */
  typedef TValue ValueType;

  /** Derived typedefs */
  typedef typename Superclass::VertexType          VertexType;
  typedef typename Superclass::VertexListType      VertexListType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename VertexListType::ConstIterator   VertexListConstIteratorType;

//   typedef itk::ImageRegion<2>                       RegionType;
  typedef otb::RemoteSensingRegion<double> RegionType;
  typedef typename RegionType::SizeType    SizeType;
  typedef typename RegionType::IndexType   IndexType;

  itkGetMacro(Key, std::string);

  void SetValue(ValueType value)
  {
    itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();
    itk::EncapsulateMetaData<ValueType>(dict, m_Key, value);
  }

  ValueType GetValue(void) const
  {
    ValueType                      resp(0);
    const itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();
    if (dict.HasKey(m_Key))
      {
      itk::ExposeMetaData<ValueType>(dict, m_Key, resp);
      }
    else
      {
      itkGenericExceptionMacro(<< "Key " << m_Key << " not found in metadata dictionary !");
      }
    return resp;
  }

  /**
   * Return the path length (perimeter).
   * \return The length.
   */
  virtual double GetLength() const;

  virtual void  AddVertex(const ContinuousIndexType& vertex);

  /**
 * Compute the path bounding region.
 * \return The region.
 */
  virtual RegionType GetBoundingRegion(void) const;

protected:
  /** Constructor */
  PolyLineParametricPathWithValue();
  /** Destructor */
  virtual ~PolyLineParametricPathWithValue() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void ComputeLength() const;
  virtual void ComputeBoundingRegion() const;
  virtual void Modified() const;

private:
  PolyLineParametricPathWithValue(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  std::string        m_Key;
  mutable double     m_Length;
  mutable bool       m_LengthIsValid;
  mutable RegionType m_BoundingRegion;
  mutable bool       m_BoundingRegionIsValid;

};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPolyLineParametricPathWithValue.txx"
#endif

#endif
