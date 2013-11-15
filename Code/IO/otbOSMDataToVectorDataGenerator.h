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

// store the nodes (points) coordinates
#include <map>
#include <string>
#include <fstream>

#include "otbVectorData.h"
#include "otbVectorDataSource.h"

#include "otbCurlHelper.h"

namespace otb {

/** \class OSMDataToVectorDataGenerator
 *
 *  \brief Request OSM Server using the OSM API 0.6 to get the
 *         xml file storing the vector elements available.
 *
 *  This class request the OSM server using a bbox. The bbox
 *  parameters can be set using the methods Set{North, South, East,
 *  West}. Thus, only the vector elements that are within this bbox
 *  are returned in the Xml file.
 *
 *  The class parse the XML file for certain keys by default, the
 *  class allow adding key to be searched in the XML file. Use
 *  AddKey(const std::string& key) to add it to the search list.
 *  Here is a link to the OSM wiki where all the keys are listed :
 *  http://wiki.openstreetmap.org/wiki/Map_Features.
 *
 *  Adding a field and its integer value is possible through the
 *  methods SetClassKey(const std::string&) and SetClassKeyValue()
 *  The field is retrieved by GetFieldAsInt(), thus must be int-compatible
 *
 *  Getting output VectorData is possible via two methods :
 *
 *    1: Get all the features of key in a vector data via
 *       GetVectorDataByName(const std::string& key)
 *       For instance : GetVectorDataByName("highway")
 *
 *    2: Get only the features  for a key that match a given value via
 *       GetVectorDataByName(const std::string& key, const std::string& value)
 *       For instance : GetVectorDataByName("highway", "motorway")
 *
 *
 *  All the keys and their relative values are stored in a map. To get
 *  this map use the method  GetKeysMap().
 *
 *  Note that if one of the server limits are crossed
 *  node/way/relation or area requested, an exception is raised.
 *
 */
class  ITK_EXPORT OSMDataToVectorDataGenerator :
    public VectorDataSource< otb::VectorData<> >
{
public:
  /** Standard class typedefs */
  typedef OSMDataToVectorDataGenerator             Self;
  typedef VectorDataSource< otb::VectorData<> >    Superclass;
  typedef itk::SmartPointer< Self >                Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  typedef Superclass::OutputVectorDataType         VectorDataType;
  typedef VectorDataType::DataNodeType             DataNodeType;
  typedef DataNodeType::LineType                   LineType;
  typedef DataNodeType::PolygonType                PolygonType;
  typedef LineType::VertexType                     VertexType;

  // typedefs for objects to store results
  typedef std::map<int, VertexType>                ContainerType;

  // Vector to store <Key, value>
  typedef std::pair <std::string, std::string>     ElementPairType;

  // Coordinates of the nodes
  typedef std::vector<VertexType >                    PointTypeList;
  typedef std::pair<ElementPairType, PointTypeList>   VectorDataElementType;
  typedef std::vector<VectorDataElementType>          VectorDataElementListType;

  // Map to store all the keys found and their relative types
  typedef std::vector<std::string>                    StringVectorType;
  typedef std::map<std::string, StringVectorType>     KeyMapType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(OSMDataToVectorDataGenerator, VectorDataSource);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Set/Get FileName */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

  /** Macro to Set/GEt the extent */
  itkSetMacro(North, double);
  itkSetMacro(South, double);
  itkSetMacro(East, double);
  itkSetMacro(West, double);

  itkGetMacro(North, double);
  itkGetMacro(South, double);
  itkGetMacro(East, double);
  itkGetMacro(West, double);

  /** Field name containing the class identifier */
  itkGetConstMacro(ClassKey, std::string);
  itkSetMacro(ClassKey, std::string);

  /** Field name containing the class identifier */
  itkGetConstMacro(ClassKeyValue, int);
  itkSetMacro(ClassKeyValue, int);

  /* flag to choose between requesting the url using curl or a file
   in the disk */
  itkSetMacro(UseUrl, bool);

  /** Add a key to search into the list */
  void AddKey(const std::string& key)
  {
    if(!this->IsKeyPresent(key))
      m_KeyList.push_back(key);
  }

  /** Method to get the map where are stored the keys and their value
    * found when parsing the osm file.
    */
  const KeyMapType GetKeysMap() const
  {
    return m_KeysMap;
  }

  /** Method to get a VectorData where are stored elements
    * corresponding the key
    */
  const VectorDataType* GetVectorDataByName(const std::string& key);

  /** Method to get a VectorData where are stored elements
    * corresponding the key and the value
    */
  const VectorDataType* GetVectorDataByName(const std::string& key, const std::string& value);

protected:
  /** Generate Data method : lauch the process */
  virtual void GenerateData();

  /**
    * Parse the xml file get from OSM to compose the requested vector
    * data
    */
  void ParseXmlFile();

  /**
    * Compose the vector data
    */
  void ProcessVectorData(const std::string& key, const std::string& value);

  /**
    * internal convenient method to serach if a key was already added
    */
  bool IsKeyPresent(const std::string& key);


  OSMDataToVectorDataGenerator();
  virtual ~OSMDataToVectorDataGenerator();

private:
  OSMDataToVectorDataGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  // Private method to add the key, type to the map
  void AddKeyTypeToMap(const std::string& key, const std::string& value);

  // Extent of the region to get from OSM
  double                      m_North;
  double                      m_South;
  double                      m_East;
  double                      m_West;

  /** the url in OSM API format */
  std::string                 m_Url;
  bool                        m_UseUrl;

  /** List to store keys to search */
  std::vector<std::string >   m_KeyList;

  /** Variable used in to store the result of the parsing  */
  std::string                 m_FileName;
  ContainerType               m_GeoPointContainer;
  VectorDataElementListType   m_VectorDataElementList;

  /** Curl object to get the XML file */
  CurlHelper::Pointer         m_Curl;

  /** Output VectorData requested with the method GetVectorDataByName */
  VectorDataType::Pointer     m_OutputVectorData;

  /** container to store the keys and their types found */
  KeyMapType                  m_KeysMap;

  /** Class key and its value */
  std::string                 m_ClassKey;
  int                         m_ClassKeyValue;

}; // end of class

} // end of namespace otb
