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

#ifndef __otbVectorDataProperties_h
#define __otbVectorDataProperties_h

#include "itkObjectFactory.h"

// #include "itkNumericTraits.h"

#include "otbVectorData.h"
namespace otb
{
/** \class VectorDataProperties
 *   \brief Compute public properties of the vectordata
 *
 * \sa VectorData
 */

template <class TVectorData>
class ITK_EXPORT VectorDataProperties : public itk::DataObject
{
public:
  /** Standard class typedefs. */
  typedef VectorDataProperties          Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Convenient typedefs */
  typedef TVectorData                           VectorDataType;
  typedef typename VectorDataType::Pointer      VectorDataPointerType;
  typedef typename VectorDataType::ConstPointer InputVectorDataConstPointer;
  typedef typename VectorDataType::DataNodeType DataNodeType;
  typedef typename DataNodeType::Pointer        DataNodePointerType;
  typedef typename DataNodeType::PolygonType    PolygonType;

  typedef typename VectorDataType::DataTreeType::TreeNodeType InternalTreeNodeType;
  typedef typename InternalTreeNodeType::ChildrenListType     ChildrenListType;

  typedef typename PolygonType::RegionType RegionType;
  typedef typename RegionType::Pointer     RegionPointerType;
  typedef typename RegionType::IndexType   IndexType;
  typedef typename RegionType::SizeType    SizeType;
  typedef typename RegionType::Type        Type;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataProperties, LightObject);

  /** Number of dimensions. */
  itkStaticConstMacro(VectorDataDimension, unsigned int,
                      TVectorData::Dimension);

  /** Get/Set the VectorData. */
//      itkGetObjectMacro(VectorDataObject, VectorDataType);
  void SetVectorDataObject(const VectorDataType * v) {m_VectorDataObject = const_cast <VectorDataType *>(v); }
//
  RegionType GetBoundingRegion() {return m_BoundingRegion; }
  void SetBoundingRegion(RegionType& region) {m_BoundingRegion = region; }

  void AddRegion(const RegionType& region);
  void ComputeBoundingRegion();
protected:
  /** Constructor */
  VectorDataProperties(){};
  /** Destructor */
  virtual ~VectorDataProperties() {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void ProcessNode(InternalTreeNodeType * source);
  bool IsBoundingRegionNull();
private:
  VectorDataProperties(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  //Pointer to the VectorData
  VectorDataType * m_VectorDataObject;
  RegionType       m_BoundingRegion;

}; // end class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataProperties.txx"
#endif

#endif
