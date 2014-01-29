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
#ifndef __otbDataNode_h
#define __otbDataNode_h

#include "itkPoint.h"
#include "otbPolyLineParametricPathWithValue.h"
#include "otbPolygon.h"
#include "otbObjectList.h"
#include "otbVectorDataKeywordlist.h"

#include <iostream>

#include "ogrsf_frmts.h"

namespace otb
{
/**
 * This enumeration describes the type of nodes used to represent a tree of vector data in the Orfeo ToolBox.
 */

typedef
enum
  {
  ROOT = 0,
  DOCUMENT = 1,
  FOLDER = 2,
  FEATURE_POINT = 3,
  FEATURE_LINE = 4,
  FEATURE_POLYGON = 5,
  FEATURE_MULTIPOINT = 6,
  FEATURE_MULTILINE = 7,
  FEATURE_MULTIPOLYGON = 8,
  FEATURE_COLLECTION = 9
  } NodeType;

/** \class DataNode
 *  \brief This class represents a node of data in a vector data hierarchy.
 *
 *  A DataNode has a type represented by the NodeType enumeration. Depending on
 *  its type, it can have associated data such as Point, Line or Polygon.
 *
 *  A flag indicates if the data are valid or not.
 *
 *  A DataNode has also a dictionary of fields which can be used to store associated data.
 *  Depending on the writing vector data file format, fields will be used by the writing driver.
 *
 * The three templates indicate
 * - the precision of the point coordinates (default double)
 * - the number of dimensions of the space (default 2)
 * - the precision of the value associated with objects such as lines or polygons (default double)
 *
 * \sa VectorData
 */
template <class TPrecision = double, unsigned VDimension = 2, class TValuePrecision = double>
class DataNode
  : public itk::Object
{
public:
  /** Standard class typedef */
  typedef DataNode                      Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(DataNode, Object);

  /** Template parameters typedefs */
  typedef TPrecision      PrecisionType;
  typedef TValuePrecision ValuePrecisionType;
  itkStaticConstMacro(Dimension, unsigned int, VDimension);

  /** Internal data typedef */
  typedef itk::Point<PrecisionType, VDimension>                                PointType;
  typedef otb::PolyLineParametricPathWithValue<ValuePrecisionType, VDimension> LineType;
  typedef typename LineType::Pointer                                           LinePointerType;
  typedef typename LineType::ConstPointer                                      LineConstPointerType;
  typedef Polygon<ValuePrecisionType>                                          PolygonType;
  typedef typename PolygonType::VertexListType                                 VertexListType;
  typedef typename VertexListType::ConstPointer                                VertexListConstPointerType;
  typedef typename PolygonType::Pointer                                        PolygonPointerType;
  typedef typename PolygonType::ConstPointer                                   PolygonConstPointerType;
  typedef ObjectList<PolygonType>                                              PolygonListType;
  typedef typename PolygonListType::Pointer                                    PolygonListPointerType;
  typedef typename PolygonListType::ConstPointer                               PolygonListConstPointerType;

  /** Fields typedef */
//   typedef std::map<std::string, std::string>          FieldMapType;
//   typedef std::pair<std::string, std::string>         FieldType;

  /** Accessors */
  itkGetConstMacro(NodeType, NodeType);
  itkGetStringMacro(NodeId);
  itkSetStringMacro(NodeId);

  /**
   * Set the node type.
   * \param type node type.
   */
  void SetNodeType(NodeType type);
  /**
   * \return the node type as a printable string.
   */
  std::string GetNodeTypeAsString() const;
  /**
   * Get the point data, when valid.
   * \return The point.
   */
  PointType         GetPoint() const;
  /**
   * Get the line data, when valid.
   * \return The line.
   */
  LinePointerType    GetLine() const;
  /**
   * Get the polygon exterior ring data, when valid.
   * \return The polygon exterior ring.
   */
  PolygonPointerType GetPolygonExteriorRing() const;
  /**
   * Get the polygon interior rings data, when valid.
   * \return The polygon interior rings list.
   */
  PolygonListPointerType GetPolygonInteriorRings() const;
  /**
   * Set the point data. Node type is automatically set to FEATURE_POINT.
   * \param point the point.
   */
  void SetPoint(PointType point);
  /**
   * Set the line data. Node type is automatically set to FEATURE_LINE.
   * \param line The line.
   */
  void SetLine(LineType* line);
  /**
   * Set the polygon exterior ring. Node type is automatically set to FEATURE_POLYGON.
   * \param polygon The polygon exterior ring.
   */
  void SetPolygonExteriorRing(PolygonType* polygon);

  /**
   * Set the polygon interior rings. Node type is automatically set to FEATURE_POLYGON.
   * \param polygonList The polygon interior rings list.
   */
  void SetPolygonInteriorRings(PolygonListType* polygonList);

  /**
   * \return True if node type is DOCUMENT.
   */
  bool IsDocument() const;
  /**
   * \return True if node type is ROOT.
   */
  bool IsRoot() const;
  /**
   * \return True if node type is FOLDER.
   */
  bool IsFolder() const;
  /**
   * \return True if node type is FEATURE_POINT.
   */
  bool IsPointFeature() const;
  /**
   * \return True if node type is FEATURE_LINE.
   */
  bool IsLineFeature() const;
  /**
   * \return True if node type is FEATURE_POLYGON.
   */
  bool IsPolygonFeature() const;
  /**
   * \return True if node type is FEATURE_MULTIPOINT.
   */
  bool IsMultiPointFeature() const;
  /**
   * \return True if node type is FEATURE_MULTILINE.
   */
  bool IsMultiLineFeature() const;
  /**
   * \return True if node type is FEATURE_MULTIPOLYGON.
   */
  bool IsMultiPolygonFeature() const;
  /**
   * \return True if node type is FEATURE_COLLECTION.
   */
  bool IsCollectionFeature() const;
  /**
   * Add a field to the node.
   * \param key The name of the field.
   * \param value The value of the field.
   */
  void SetFieldAsString(const std::string& key, const std::string& value);
  /**
   * Returns the value associated with a field name.
   * \param key The name of the field.
   * \return The value of the field. A default value is returned if the key was not found.
   */
  std::string GetFieldAsString(const std::string& key) const;

  /**
   * Add a field to the node.
   * \param key The name of the field.
   * \param value The value of the field.
   */
  void SetFieldAsInt(const std::string& key, int value);
  /**
   * Returns the value associated with a field name.
   * \param key The name of the field.
   * \return The value of the field. A default value is returned if the key was not found.
   */
  int GetFieldAsInt(const std::string& key) const;

  /**
   * Add a field to the node.
   * \param key The name of the field.
   * \param value The value of the field.
   */
  void SetFieldAsDouble(const std::string& key, double value);
  /**
   * Returns the value associated with a field name.
   * \param key The name of the field.
   * \return The value of the field. A default value is returned if the key was not found.
   */
  double GetFieldAsDouble(const std::string& key) const;

  // /**
  //  * Remove the field associated with the given key, if possible.
  //  * \param key The name of the field.
  //  */
  //   void RemoveField(const std::string& key);
/**
 * \return True if the node contains the field named after the given key.
 * \param key The name of the field.
 */
  bool HasField(const std::string& key) const;

  /**
   * Copy the field list from a DataNode
   * \param datanode where to get the keywordlist to copy.
   */
  void CopyFieldList(const DataNode * dataNode);

  /**
    * \return the field list in the node.
    */
  std::vector<std::string> GetFieldList() const;

  /** \return the distance to a point */
  double EuclideanDistanceMetric(const DataNode* node);

  double EuclideanDistanceMetric(const PointType point);

   /**
    * \return true if the two datanodes intersects
    */
  bool Intersects(const DataNode* node);

   /**
    * \return true if the two datanodes intersects
    */
  bool Within(const DataNode* node);

/**
 * Clear all fields.
 */
//   void ClearFields();

protected:
  /** Constructor */
  DataNode();
  /** Destructor */
  virtual ~DataNode() {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  OGRGeometry* ConvertDataNodeToOGRGeometry(const DataNode* dataNode);

private:
  DataNode(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** typedef of the data associated with the node */
  typedef struct
  {
    bool valid;
    PointType point;
    LinePointerType line;
    PolygonPointerType exteriorRing;
    PolygonListPointerType interiorRings;
  } DataType;

  /** The node type */
  NodeType m_NodeType;

  /** The node id */
  std::string m_NodeId;

  /** The data associated with the node */
  DataType m_Data;

  /** The fields map */
//   FieldMapType         m_FieldMap;
};
} // end namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDataNode.txx"
#endif

#endif
