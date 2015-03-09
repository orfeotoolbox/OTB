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
#ifndef __otbKMLVectorDataIO_h
#define __otbKMLVectorDataIO_h

#include <string>

#include "otbVectorDataIOBase.h"
#include "otbVectorData.h"

#include "kml/dom/kml_ptr.h"
#include "otbVectorDataProjectionFilter.h"

namespace kmldom
{
class KmlFactory;
}

namespace otb
{

/** \class KMLVectorDataIO
 *
 * \brief ImageIO object for reading/writing KML format vector data
 *
 *
 * \ingroup OTBIOKML
 */
class ITK_EXPORT KMLVectorDataIO
  : public VectorDataIOBase
{
public:

  /** Standard class typedefs. */
  typedef KMLVectorDataIO               Self;
  typedef VectorDataIOBase              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(KMLVectorDataIO, VectorDataIOBase);

  /** Byte order typedef */
  typedef Superclass::ByteOrder ByteOrder;

  /** Data typedef */
  typedef VectorData<double, 2>                           VectorDataType;
  typedef VectorDataType::DataTreeType           DataTreeType;
  typedef DataTreeType::TreeNodeType             InternalTreeNodeType;
  typedef InternalTreeNodeType::ChildrenListType ChildrenListType;
  typedef DataTreeType::Pointer                  DataTreePointerType;
  typedef DataTreeType::ConstPointer             DataTreeConstPointerType;
  typedef VectorDataType::DataNodeType           DataNodeType;
  typedef DataNodeType::Pointer                  DataNodePointerType;
  typedef DataNodeType::PointType                PointType;
  typedef DataNodeType::LineType                 LineType;
  typedef LineType::VertexListType               VertexListType;
  typedef VertexListType::ConstPointer           VertexListConstPointerType;
  typedef LineType::Pointer                      LinePointerType;
  typedef LineType::VertexType                   VertexType;
  typedef DataNodeType::PolygonType              PolygonType;
  typedef PolygonType::Pointer                   PolygonPointerType;
  typedef DataNodeType::PolygonListType          PolygonListType;
  typedef PolygonListType::Pointer               PolygonListPointerType;
  typedef VectorDataType::Pointer                VectorDataPointerType;
  typedef VectorDataType::ConstPointer           VectorDataConstPointerType;
  typedef VectorDataProjectionFilter<VectorDataType, VectorDataType> ProjectionFilterType;
  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this VectorDataIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*) const;

  /** Reads the data from disk into the data structure provided. */
  virtual void Read(itk::DataObject* data);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this VectorDataIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*) const;

  /** Writes the data to disk from the data structure provided */
  virtual void Write(const itk::DataObject* data, char ** papszOptions = NULL);

protected:
  /** Constructor.*/
  KMLVectorDataIO();
  /** Destructor.*/
  virtual ~KMLVectorDataIO();

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  static const kmldom::FeaturePtr GetRootFeature(const kmldom::ElementPtr& root);

  static void PrintIndented(const std::string& item, int depth);

  static void PrintFeature(const kmldom::FeaturePtr& feature, int depth);

  void WalkGeometry(const kmldom::GeometryPtr& geometry, DataNodePointerType father);

  void WalkFeature(const kmldom::FeaturePtr& feature, DataNodePointerType father);

  void WalkContainer(const kmldom::ContainerPtr& container, DataNodePointerType father);

  /** Conversion tools */
  DataNodePointerType ConvertGeometryToPointNode(const kmldom::GeometryPtr& geometry);
  DataNodePointerType ConvertGeometryToLineStringNode(const kmldom::GeometryPtr& geometry);
  DataNodePointerType ConvertGeometryToLinearRingNode(const kmldom::GeometryPtr& geometry);
  DataNodePointerType ConvertGeometryToPolygonNode(const kmldom::GeometryPtr& geometry);
  /** end conversion tools */

  void ProcessNodeWrite(InternalTreeNodeType * source, kmldom::KmlFactory* factory,
                        kmldom::KmlPtr kml, kmldom::DocumentPtr currentDocument, kmldom::FolderPtr currentFolder,
                        kmldom::MultiGeometryPtr currentMultiGeometry);

private:
  KMLVectorDataIO(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  DataTreePointerType m_Tree;

  unsigned int m_Kept;
};

} // end namespace otb

#endif // __otbKMLVectorDataIO_h
