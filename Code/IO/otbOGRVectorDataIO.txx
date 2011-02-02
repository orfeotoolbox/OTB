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
#ifndef __otbOGRVectorDataIO_txx
#define __otbOGRVectorDataIO_txx

#include "otbOGRVectorDataIO.h"

#include <itksys/SystemTools.hxx>
#include "itkExceptionObject.h"
#include "itkByteSwapper.h"
#include "otbMacro.h"
#include "otbSystem.h"
#include "otbDataNode.h"
#include "otbMetaDataKey.h"
#include "itkTimeProbe.h"
#include "otbVectorDataKeywordlist.h"

namespace otb
{
template<class TData>
OGRVectorDataIO<TData>
::OGRVectorDataIO() :
  m_DataSource(NULL)
{
  // OGR factory registration
  OGRRegisterAll();
}

template<class TData>
OGRVectorDataIO<TData>::~OGRVectorDataIO()
{
  if (m_DataSource != NULL)
    {
    OGRDataSource::DestroyDataSource(m_DataSource);
    }
}

template<class TData>
bool
OGRVectorDataIO<TData>::CanReadFile(const char* filename) const
{
  OGRDataSource * poDS = OGRSFDriverRegistrar::Open(filename, FALSE);
  if (poDS == NULL)
    {
    return false;
    }
//     std::cout << poDS->GetDriver()->GetName() << std::endl;
  OGRDataSource::DestroyDataSource(poDS);
  return true;
}

// Used to print information about this object
template<class TData>
void
OGRVectorDataIO<TData>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read vector data
template<class TData>
void
OGRVectorDataIO<TData>
::Read(VectorDataPointerType data)
{
  // Destroy previous opened data source
  if (m_DataSource != NULL)
    {
    OGRDataSource::DestroyDataSource(m_DataSource);
    }

  m_DataSource = OGRSFDriverRegistrar::Open(this->m_FileName.c_str(), FALSE);

  if (m_DataSource == NULL)
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

  OGRSpatialReference * oSRS = NULL;
  //We take the assumption that the spatial reference is common to all layers
  oSRS = m_DataSource->GetLayer(0)->GetSpatialRef();

  if (oSRS != NULL)
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
    OGRIOHelper<VectorDataType> OGRConversion;
    OGRConversion.ConvertOGRLayerToDataTreeNode(layer, documentPtr);

    } // end For each layer

  OGRDataSource::DestroyDataSource(m_DataSource);
  m_DataSource = NULL;
}

template<class TData>
bool OGRVectorDataIO<TData>::CanWriteFile(const char* filename) const
{

  std::string lFileName(filename);
  if (System::IsADirName(lFileName) == true)
    {
    return false;
    }

  return (this->GetOGRDriverName(filename) != "NOT-FOUND");
}

template<class TData>
void OGRVectorDataIO<TData>::Write(const VectorDataConstPointerType data, char ** papszOptions)
{
  itk::TimeProbe chrono;
  chrono.Start();

  //Find first the OGR driver
  OGRSFDriver * ogrDriver =
    OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(this->GetOGRDriverName(this->m_FileName).data());

  if (ogrDriver == NULL)
    {
    itkExceptionMacro(<< "No OGR driver found to write file " << this->m_FileName);
    }

  // free an existing previous data source, if any
  if (m_DataSource != NULL)
    {
    OGRDataSource::DestroyDataSource(m_DataSource);
    }

  // Erase the dataSource if already exist
  //TODO investigate the possibility of giving the option OVERWRITE=YES to the CreateDataSource method
  OGRDataSource * poDS = OGRSFDriverRegistrar::Open(this->m_FileName.c_str(), TRUE);
  if (poDS != NULL)
    {
    //Erase the data if possible
    if (poDS->GetDriver()->TestCapability(ODrCDeleteDataSource))
      {
      //Delete datasource
      poDS->GetDriver()->DeleteDataSource(this->m_FileName.c_str());
      }
    }
  OGRDataSource::DestroyDataSource(poDS);

  // m_DataSource = OGRSFDriverRegistrar::Open(this->m_FileName.c_str(), TRUE);
  m_DataSource = ogrDriver->CreateDataSource(this->m_FileName.c_str(), papszOptions);

  // check the created data source
  if (m_DataSource == NULL)
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
  OGRSpatialReference * oSRS = NULL;
  if (projectionInformationAvailable)
    {
    oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
    }

  // Retrieving root node
  DataTreeConstPointerType tree = data->GetDataTree();
  if (tree->GetRoot() == NULL)
    {
    itkExceptionMacro(<< "Data tree is empty: Root == NULL");
    }
  DataNodePointerType root = tree->GetRoot()->Get();

  unsigned int layerKept = 0;
  OGRLayer *   ogrCurrentLayer = NULL;
//   OGRFeatureVectorType ogrFeatures;
  OGRGeometryCollection * ogrCollection = NULL;
  // OGRGeometry * ogrCurrentGeometry = NULL;

  // Get the input tree root
  InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(tree->GetRoot());

  //Refactoring SHPIO Manuel
  OGRIOHelper<VectorDataType> IOConversion;
  layerKept = IOConversion.ProcessNodeWrite(inputRoot, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);

  OGRDataSource::DestroyDataSource(m_DataSource);
  m_DataSource = NULL;

  if (oSRS != NULL)
    {
    OSRRelease(oSRS);
    }

  chrono.Stop();
  std::cout << "OGRVectorDataIO: file saved in " << chrono.GetMeanTime() << " seconds. (" << layerKept <<
  " elements)" << std::endl;

  otbMsgDevMacro(<< " OGRVectorDataIO::Write()  ");
}

template<class TData>
std::string
OGRVectorDataIO<TData>::GetOGRDriverName(std::string name) const
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
    extension = System::GetExtension(upperName);
    if (extension == "SHP") driverOGR = "ESRI Shapefile";
    else if ((extension == "TAB")) driverOGR = "MapInfo File";
    else if (extension == "GML") driverOGR = "GML";
    else if (extension == "GPX") driverOGR = "GPX";
//    else if (extension=="KML")
//      driverOGR="KML";
    else driverOGR = "NOT-FOUND";
    }
  //std::cout << name << " " << driverOGR <<" "<<upperName<< " "<< upperName.substr(0, 3) << std::endl;
  return driverOGR;
}

} // end namespace otb

#endif
