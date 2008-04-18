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
#ifndef  _otb_DataNode_h
#define  _otb_DataNode_h

#include "itkPoint.h"
#include "itkPolyLineParametricPath.h"
#include "otbPolygon.h"
#include "otbObjectList.h"

#include <iostream>

namespace otb
{
/**
 * This enumeration describes the type fo nodes used to represent a tree of vector data in the Orfeo ToolBox. 
 */
typedef enum{ROOT,DOCUMENT,FOLDER,FEATURE_POINT,FEATURE_LINE,FEATURE_POLYGON,FEATURE_MULTIPOINT,FEATURE_MULTILINE,FEATURE_MULTIPOLYGON,FEATURE_COLLECTION} NodeType;


/** \class DataNode
 *  \brief This class represent a node of data in a vector data hierarchy.
 *   A DataNode has a type represented by the NodeType enumeration. Depending of 
 *  its type, it can have associated data such as Point, Line or Polygon.
 *
 *  A flag indicates if the data are valid or not. 
 *
 *  A DataNode has also a dictionnary of fields which can be used to store associated data.
 *  Depending of the writing vector data file format, fields will be used by the writing driver.
 *
 * \sa VectorData
 */
template <class TPrecision = double, unsigned VDimension = 2> 
class DataNode
: public itk::Object
 {
   public:
   /** Standard class typedef */
   typedef DataNode Self;
   typedef itk::Object Superclass;
   typedef itk::SmartPointer<Self> Pointer;
   typedef itk::SmartPointer<const Self> ConstPointer;

   /** Standard macro */
   itkNewMacro(Self);
   itkTypeMacro(DataNode,Object);

   /** Template parameters typedefs */
   typedef TPrecision PrecisionType;
   itkStaticConstMacro(Dimension, unsigned int, VDimension);


   /** Internal data typedef */
   typedef itk::Point<PrecisionType,VDimension>       PointType;
   typedef itk::PolyLineParametricPath<VDimension>    LineType;
   typedef typename LineType::Pointer                 LinePointerType;
   typedef Polygon<PrecisionType>                     PolygonType;
   typedef typename PolygonType::Pointer              PolygonPointerType; 
   typedef ObjectList<PolygonType>               PolygonListType;
   typedef typename PolygonListType::Pointer          PolygonListPointerType;
   
   /** Fields typedef */
   typedef std::map<std::string,std::string>          FieldMapType;
   typedef std::pair<std::string,std::string>         FieldType;

   /** Accessors */
   itkGetMacro(NodeType,NodeType);
   itkGetStringMacro(NodeId);
   itkSetStringMacro(NodeId);

   /**
    * Set the node type.
    * \param The node type.
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
   PointType&         GetPoint() const;
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
    * \param The point.
    */
   void SetPoint(PointType point);
   /**
    * Set the line data. Node type is automatically set to FEATURE_LINE.
    * \param The line.
    */
   void SetLine(LinePointerType line);
   /**
    * Set the polygon exterior ring. Node type is automatically set to FEATURE_POLYGON.
    * \param The polygon exterior ring.
    */
   void SetPolygonExteriorRing(PolygonPointerType polygon);

   /**
    * Set the polygon interior rings. Node type is automatically set to FEATURE_POLYGON.
    * \param The polygon interior rings list.
    */
   void SetPolygonInteriorRings(PolygonListPointerType polygonList);

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
    * \param The value of the field.
    */
   void SetField(std::string key, std::string value);
   /**
    * Returns the value associated with a field name.
    * \param key The name of the field.
    * \return The value of the field. A default value is retuned if the key was not found.
    */
   std::string GetField(std::string key) const;
   /**
    * Remove the field associated with the given key, if possible.
    * \param key The name of the field.
    */
   void RemoveField(std::string key);
   /**
    * \return True if the node contains the field named after the given key.
    * \param key The name of the field.
    */
   bool HasField(std::string key) const;
   /**
    * \return the nth field of the node as a std::pair of (key,value).
    * \param index the index of the field to return.
    */
   FieldType GetNthField(unsigned int index) const;
   /**
    * \return the number of fields in the node.
    */
   unsigned int GetNumberOfFields() const;
   /**
    * Clear all fields.
    */
   void ClearFields();
  
   protected:
   /** Constructor */
   DataNode();
   /** Destructor */
   ~DataNode(){};
   /** PrintSelf method */
   void PrintSelf(std::ostream& os, itk::Indent indent) const;
   
   private:
   DataNode(const Self&); //purposely not implemented
   void operator=(const Self&); //purposely not implemented
   
   /** typedef of the data associated with the node */
   typedef struct {
     bool      valid;
     PointType point;
     LinePointerType line;
     PolygonPointerType  exteriorRing;
     PolygonListPointerType  interiorRings;
   } DataType;

   /** The node type */
   NodeType             m_NodeType;  
   
   /** The node id */
   std::string          m_NodeId;
   
   /** The data associated with the node */
   DataType             m_Data;
   
   /** The fields map */
   FieldMapType         m_FieldMap;
 };
} // end namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDataNode.txx"
#endif

#endif
