#ifndef  _otb_DataNode_h
#define  _otb_DataNode_h

#include "itkPoint.h"
#include "itkPolyLineParametricPath.h"
#include "otbPolygon.h"

#include <iostream>

namespace otb
{

  enum NodeType{ROOT,DOCUMENT,FOLDER,FEATURE_POINT,FEATURE_LINE,FEATURE_POLYGON};


  /** \class DataNode
   *  \brief
   *
   *
   */
  template <class TPrecision = double, unsigned VDimension = 2> class DataNode
    : public itk::Object
    {
      public:   
      typedef DataNode Self;
      typedef itk::Object Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;

      itkNewMacro(Self);
      itkTypeMacro(DataNode,Object);

      /// Template parameters typedefs
      typedef TPrecision PrecisionType;
      itkStaticConstMacro(Dimension, unsigned int, VDimension);
      

      typedef itk::Point<PrecisionType,VDimension>       PointType;
      typedef itk::PolyLineParametricPath<VDimension>    LineType;
      typedef typename LineType::Pointer                 LinePointerType;
      typedef otb::Polygon<PrecisionType>                PolygonType;
      typedef typename PolygonType::Pointer              PolygonPointerType;                       
 
      typedef std::map<std::string,std::string>          FieldMapType;
      typedef std::pair<std::string,std::string>         FieldType;

      itkGetMacro(NodeType,NodeType);
      itkGetStringMacro(NodeId);
      itkSetStringMacro(NodeId);

      void SetNodeType(NodeType type);
      
      PointType&         GetPoint();
      LinePointerType    GetLine();
      PolygonPointerType GetPolygon();


      void SetPoint(PointType point);
      void SetLine(LinePointerType line);
      void SetPolygon(PolygonPointerType polygon);

      bool IsDocument();
      bool IsRoot();
      bool IsFolder();
      bool IsPointFeature();
      bool IsLineFeature();
      bool IsPolygonFeature();
      
      void SetField(std::string key, std::string value);
      std::string GetField(std::string key);
      void RemoveField(std::string key);
      bool HasField(std::string key);
      FieldType GetNthField(unsigned int index);
      unsigned int GetNumberOfFields();
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

      typedef struct {
	bool      valid;
	PointType point;
	LinePointerType line;
	PolygonPointerType polygon;
      } DataType;


      NodeType             m_NodeType;  
      std::string          m_NodeId;
      DataType             m_Data;
      FieldMapType         m_FieldMap;

    };



} // end namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDataNode.txx"
#endif

#endif
