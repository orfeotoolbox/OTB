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
#ifndef __otbSHPVectorDataIO_h
#define __otbSHPVectorDataIO_h

#include "otbVectorDataIOBase.h"
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "ogrsf_frmts.h"
#include "otbOGRIOHelper.h"

namespace otb
{

/** \class SHPVectorDataIO
 *
 * \brief ImageIO object for reading and writing SHP format vector data
 *
 */
template <class TData> class ITK_EXPORT SHPVectorDataIO
      : public VectorDataIOBase<TData>
{
public:

  /** Standard class typedefs. */
  typedef SHPVectorDataIO          Self;
  typedef VectorDataIOBase<TData>  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SHPVectorDataIO, VectorDataIOBase);
  
  /** Byte order typedef */
  typedef typename Superclass::ByteOrder  ByteOrder;

  /** Data typedef */
  typedef TData VectorDataType;
  typedef typename VectorDataType::DataTreeType  DataTreeType;
  typedef typename DataTreeType::TreeNodeType    InternalTreeNodeType;
  typedef typename InternalTreeNodeType::ChildrenListType        ChildrenListType;
  typedef typename DataTreeType::Pointer         DataTreePointerType;
  typedef typename DataTreeType::ConstPointer    DataTreeConstPointerType;
  typedef typename VectorDataType::DataNodeType  DataNodeType;
  typedef typename DataNodeType::Pointer         DataNodePointerType;
  typedef typename DataNodeType::PointType       PointType;
  typedef typename DataNodeType::LineType        LineType;
  typedef typename LineType::VertexListType      VertexListType;
  typedef typename VertexListType::ConstPointer  VertexListConstPointerType;
  typedef typename LineType::Pointer             LinePointerType;
  typedef typename DataNodeType::PolygonType     PolygonType;
  typedef typename PolygonType::Pointer          PolygonPointerType;
  typedef typename DataNodeType::PolygonListType PolygonListType;
  typedef typename PolygonListType::Pointer      PolygonListPointerType;
  typedef typename VectorDataType::Pointer       VectorDataPointerType;
  typedef typename VectorDataType::ConstPointer  VectorDataConstPointerType;
  typedef typename Superclass::SpacingType       SpacingType;
  typedef typename Superclass::PointType         OriginType;


  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this VectorDataIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*) const;

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(VectorDataPointerType data);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*) const;

  /** Writes the data to disk from the memory buffer provided */
  virtual void Write(VectorDataConstPointerType data);

protected:
  /** Construtor.*/
  SHPVectorDataIO();
  /** Destructor.*/
  virtual ~SHPVectorDataIO();

  /*   virtual void InternalReadVectorDataInformation(){}; */

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  SHPVectorDataIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  std::string GetOGRDriverName(std::string name) const;
 
  OGRDataSource * m_DataSource;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSHPVectorDataIO.txx"
#endif

#endif // __otbSHPVectorDataIO_h
