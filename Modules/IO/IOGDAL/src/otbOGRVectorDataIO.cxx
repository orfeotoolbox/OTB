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
#include "otbOGRVectorDataIO.h"

#include "ogrsf_frmts.h"
#include "otbSystem.h"
#include "itksys/SystemTools.hxx"
#include "itkByteSwapper.h"
#include "otbMacro.h"
#include "otbDataNode.h"
#include "otbMetaDataKey.h"
#include "itkTimeProbe.h"
#include "otbOGRIOHelper.h"

namespace otb
{

OGRVectorDataIO
::OGRVectorDataIO() :
  m_DataSource(ITK_NULLPTR)
{
  // OGR factory registration
  OGRRegisterAll();
}


OGRVectorDataIO::~OGRVectorDataIO()
{
  if (m_DataSource != ITK_NULLPTR)
    {
    this->CloseInternalDataSource();
    }
}


bool
OGRVectorDataIO::CanReadFile(const char* filename) const
{
  otb::ogr::version_proxy::GDALDatasetType * poDS = ogr::version_proxy::Open(filename, true);
  
  if (poDS == ITK_NULLPTR)
    {
    std::cerr<<"Can not read file "<<filename<<" with GDALOpen"<<std::endl;
    return false;
    }
//     std::cout << poDS->GetDriver()->GetName() << std::endl;
  ogr::version_proxy::Close(poDS);
  return true;
}

// Used to print information about this object

void
OGRVectorDataIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read vector data

void
OGRVectorDataIO
::Read(itk::DataObject* datag)
{
  VectorDataPointerType data = dynamic_cast<VectorDataType*>(datag);
  // Destroy previous opened data source

  if(!data)
    {
    itkExceptionMacro(<<"Failed to dynamic cast to otb::VectorData (this should never happen)");
    }

  if (m_DataSource != ITK_NULLPTR)
    {
    this->CloseInternalDataSource();
    }

  m_DataSource = ogr::version_proxy::Open(this->m_FileName.c_str(),true);

  if (m_DataSource == ITK_NULLPTR)
    {
    itkExceptionMacro(<< "Failed to open data file " << this->m_FileName);
    }

  otbMsgDebugMacro(<< "Driver to read: OGR");
  otbMsgDebugMacro(<< "Reading  file: " << this->m_FileName);

  // Reading layers
  otbMsgDevMacro(<< "Number of layers: " << m_DataSource->GetLayerCount());

  // Retrieving root node
  DataTreePointerType tree = data->GetDataTree();
  DataNodePointerType root = tree->GetRoot()->Get();

  OGRSpatialReference * oSRS = ITK_NULLPTR;
  //We take the assumption that the spatial reference is common to all layers
  oSRS = m_DataSource->GetLayer(0)->GetSpatialRef();

  if (oSRS != ITK_NULLPTR)
    {
    char * projectionRefChar;
    oSRS->exportToWkt(&projectionRefChar);
    std::string projectionRef = projectionRefChar;
    CPLFree(projectionRefChar);
    itk::MetaDataDictionary& dict = data->GetMetaDataDictionary();
    itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef);
    }
  else
    {
    otbMsgDevMacro(<< "Can't retrieve the OGRSpatialReference from the shapefile");
    }

  std::string projectionRefWkt = data->GetProjectionRef();

  bool projectionInformationAvailable = !projectionRefWkt.empty();

  if (projectionInformationAvailable)
    {
    otbMsgDevMacro(<< "Projection information : " << projectionRefWkt);
    }
  else
    {
    otbMsgDevMacro(<< "Projection information unavailable: assuming WGS84");
    }

  // For each layer
  for (int layerIndex = 0; layerIndex < m_DataSource->GetLayerCount(); ++layerIndex)
    {
    /** retrieving layer and property */
    OGRLayer * layer = m_DataSource->GetLayer(layerIndex);
    otbMsgDevMacro(<< "Number of features: " << layer->GetFeatureCount());

    OGRFeatureDefn * dfn = layer->GetLayerDefn();

    /** Create the document node */
    DataNodePointerType document = DataNodeType::New();
    document->SetNodeType(DOCUMENT);
    document->SetNodeId(dfn->GetName());

    /** Retrieving the fields types */
//     OGRFieldDefn * field;
//     for (int fieldIndex = 0; fieldIndex<dfn->GetFieldCount(); ++fieldIndex)
//     {
//       field  = dfn->GetFieldDefn(fieldIndex);
//       document->SetField(field->GetNameRef(), OGRFieldDefn::GetFieldTypeName(field->GetType()));
//       // std::cout<<"Document "<<document->GetNodeId()<<": Adding field "<<field->GetNameRef()<<" "<<OGRFieldDefn::GetFieldTypeName(field->GetType())<<std::endl;
//     }

    /** Adding the layer to the data tree */
    tree->Add(document, root);

    /// This is not good but we do not have the choice if we want to
    /// get a hook on the internal structure
    InternalTreeNodeType * documentPtr = const_cast<InternalTreeNodeType *>(tree->GetNode(document));

    /** IO class helper to convert ogr layer*/

    OGRIOHelper::Pointer OGRConversion = OGRIOHelper::New();
    OGRConversion->ConvertOGRLayerToDataTreeNode(layer, documentPtr);

    } // end For each layer

  this->CloseInternalDataSource();
}

void OGRVectorDataIO::CloseInternalDataSource()
{
  assert(m_DataSource != NULL && "m_DataSource cannot be NULL");
  ogr::version_proxy::Close(m_DataSource);
  m_DataSource = ITK_NULLPTR;
}


bool OGRVectorDataIO::CanWriteFile(const char* filename) const
{

  std::string lFileName(filename);
  if (itksys::SystemTools::FileIsDirectory(lFileName.c_str()) == true)
    {
    return false;
    }

  return (this->GetOGRDriverName(filename) != "NOT-FOUND");
}


void OGRVectorDataIO::Write(const itk::DataObject* datag, char ** /** unused */)
{
  itk::TimeProbe chrono;
  chrono.Start();

  VectorDataConstPointerType data = dynamic_cast<const VectorDataType*>(datag);

  if(!data)
    {
    itkExceptionMacro(<<"Failed to dynamic cast to otb::VectorData (this should never happen).");
    }


  //Find first the OGR driver
  ogr::version_proxy::GDALDriverType * ogrDriver =
    ogr::version_proxy::GetDriverByName(this->GetOGRDriverName(this->m_FileName).data());

  if (ogrDriver == ITK_NULLPTR)
    {
    itkExceptionMacro(<< "No OGR driver found to write file " << this->m_FileName);
    }

  // free an existing previous data source, if any
  if (m_DataSource != ITK_NULLPTR)
    {
    ogr::version_proxy::Close(m_DataSource);
    }

  // Erase the dataSource if already exist
  ogr::version_proxy::Delete(this->m_FileName.c_str());

  // m_DataSource = OGRSFDriverRegistrar::Open(this->m_FileName.c_str(), TRUE);
  m_DataSource = ogr::version_proxy::Create(ogrDriver,this->m_FileName.c_str());

  // check the created data source
  if (m_DataSource == ITK_NULLPTR)
    {
    itkExceptionMacro(
      << "Failed to create OGR data source for file " << this->m_FileName <<
      ". Since OGR can not overwrite existing file, be sure that this file does not already exist");
    }

  // Retrieve data required for georeferencing

  std::string projectionRefWkt = data->GetProjectionRef();
  bool        projectionInformationAvailable = !projectionRefWkt.empty();

  if (projectionInformationAvailable)
    {
    otbMsgDevMacro(<< "Projection information : " << projectionRefWkt);
    }
  else
    {
    otbMsgDevMacro(<< "Projection information unavailable");
    }

  //TODO georeference here from OGRSpatialReference http://www.gdal.org/ogr/classOGRDataSource.html
  OGRSpatialReference * oSRS = ITK_NULLPTR;
  if (projectionInformationAvailable)
    {
    oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
    }

  // Retrieving root node
  DataTreeConstPointerType tree = data->GetDataTree();
  if (tree->GetRoot() == ITK_NULLPTR)
    {
    itkExceptionMacro(<< "Data tree is empty: Root == NULL");
    }
  DataNodePointerType root = tree->GetRoot()->Get();

  unsigned int layerKept;
  OGRLayer *   ogrCurrentLayer = ITK_NULLPTR;
//   OGRFeatureVectorType ogrFeatures;
  OGRGeometryCollection * ogrCollection = ITK_NULLPTR;
  // OGRGeometry * ogrCurrentGeometry = NULL;

  // Get the input tree root
  InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(tree->GetRoot());

  //Refactoring SHPIO Manuel
  OGRIOHelper::Pointer IOConversion = OGRIOHelper::New();
  layerKept = IOConversion->ProcessNodeWrite(inputRoot, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);

  otbMsgDevMacro( << "layerKept " << layerKept );
  (void)layerKept; // keep compiler happy

  otb::ogr::version_proxy::Close(m_DataSource);
  m_DataSource = ITK_NULLPTR;

  if (oSRS != ITK_NULLPTR)
    {
    OSRRelease(oSRS);
    }

  chrono.Stop();
  otbMsgDevMacro( << "OGRVectorDataIO: file saved in " << chrono.GetMean() << " seconds. (" << layerKept <<
  " elements)" );

  otbMsgDevMacro(<< " OGRVectorDataIO::Write()  ");
}


std::string
OGRVectorDataIO::GetOGRDriverName(std::string name) const
{
  std::string extension;
  std::string driverOGR;

  std::string upperName;
  upperName = name;
  std::transform(name.begin(), name.end(), upperName.begin(), (int (*)(int))toupper);

  //Test of PostGIS connection string
  if (upperName.substr(0, 3) == "PG:")
    {
    driverOGR = "PostgreSQL";
    }
  else
    {
    extension = itksys::SystemTools::GetFilenameLastExtension(upperName);
    if (extension == ".SHP") driverOGR = "ESRI Shapefile";
    else if ((extension == ".TAB")) driverOGR = "MapInfo File";
    else if (extension == ".GML") driverOGR = "GML";
    else if (extension == ".GPX") driverOGR = "GPX";
    else if (extension == ".SQLITE") driverOGR = "SQLite";
     else if (extension==".KML") driverOGR="KML";
    else driverOGR = "NOT-FOUND";
    }
  //std::cout << name << " " << driverOGR <<" "<<upperName<< " "<< upperName.substr(0, 3) << std::endl;
  return driverOGR;
}

} // end namespace otb
