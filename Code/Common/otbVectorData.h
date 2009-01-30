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
#ifndef __otbVectorData_h
#define __otbVectorData_h

#include "itkTreeContainer.h"
#include "itkDataObject.h"
#include "otbDataNode.h"
#include "itkVector.h"
#include "itkPoint.h"

namespace otb
{
/** \class VectorData
 * \brief this class represents a hierarchy of vector data.
 *
 * This class contains an itk::TreeContainer templated with otb::DataNode.
 * It represents a tree structure of vector data. These data can be read from
 * shapefiles or kml files for instance.
 *
 * The internal tree can be walked with itk::TreeIteratorBase subclasses.
 *
 * The three templates indicate
 * - the precision of the points coordinates (default double)
 * - the number of dimensions of the space (default 2)
 * - the precision of the value associated with objects such as line or polygons (default double)
 *
 * \sa DataNode
 * \sa VectorDataFileReader
 * \sa VectorDataFileWriter
 *
 */
template <class TPrecision = double, unsigned int VDimension =2, class TValuePrecision = double>
class VectorData
      : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef VectorData Self;
  typedef itk::DataObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;



  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(VectorData,DataObject);
  itkStaticConstMacro(Dimension, unsigned int, VDimension);

  /** Template parameters typedef */
  typedef TPrecision PrecisionType;
  typedef TValuePrecision ValuePrecisionType;
  typedef otb::DataNode<TPrecision,VDimension,TValuePrecision> DataNodeType;
  typedef typename DataNodeType::Pointer DataNodePointerType;
  typedef itk::TreeContainer<DataNodePointerType> DataTreeType;
  typedef typename DataTreeType::Pointer DataTreePointerType;

  itkGetObjectMacro(DataTree,DataTreeType);
  itkGetConstObjectMacro(DataTree,DataTreeType);

  virtual void SetProjectionRef(std::string projectionRef);
  virtual std::string GetProjectionRef() const;

  /** Clear the vector data  */
  virtual bool Clear();

  /** Return the number of element in the tree */
  virtual int Size() const;

protected:
  /** Constructor */
  VectorData();
  /** Destructor */
  virtual ~VectorData() {};
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  VectorData(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


  /** Data tree */
  DataTreePointerType m_DataTree;

};
}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorData.txx"
#endif

#endif

