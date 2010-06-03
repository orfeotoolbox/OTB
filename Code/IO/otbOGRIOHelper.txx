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
#ifndef __otbOGRIOHelper_txx
#define __otbOGRIOHelper_txx

#include "itkTimeProbe.h"

#include "otbOGRIOHelper.h"

namespace otb
{

template<class TVectorData>
inline typename OGRIOHelper<TVectorData>
::DataNodePointerType
OGRIOHelper<TVectorData>
::ConvertGeometryToPointNode(const OGRGeometry * ogrGeometry) const
{
  OGRPoint * ogrPoint = (OGRPoint *) ogrGeometry;

  if (ogrPoint == NULL)
    {
    itkGenericExceptionMacro(<< "Failed to convert OGRGeometry to OGRPoint");
    }

  PointType otbPoint;
  otbPoint.Fill(0);
  otbPoint[0] = static_cast<typename DataNodeType::PrecisionType>(ogrPoint->getX());
  otbPoint[1] = static_cast<typename DataNodeType::PrecisionType>(ogrPoint->getY());

  if (DataNodeType::Dimension > 2)
    {
    if (PointType::PointDimension != 3)
      {
      itkGenericExceptionMacro(<< "OTB vector data can't contain the OGR information (2D instead of 2.5D)");
      }
    otbPoint[2] = static_cast<typename DataNodeType::PrecisionType>(ogrPoint->getZ());
    }

  DataNodePointerType node = DataNodeType::New();
  node->SetPoint(otbPoint);
  return node;
}

template<class TVectorData>
inline typename OGRIOHelper<TVectorData>
::DataNodePointerType
OGRIOHelper<TVectorData>
::ConvertGeometryToLineNode(const OGRGeometry * ogrGeometry) const
{
  OGRLineString * ogrLine = (OGRLineString *) ogrGeometry;

  if (ogrLine == NULL)
    {
    itkGenericExceptionMacro(<< "Failed to convert OGRGeometry to OGRLine");
    }

  LinePointerType line = LineType::New();

  OGRPoint * ogrTmpPoint = new OGRPoint();

  for (int pIndex = 0; pIndex < ogrLine->getNumPoints(); ++pIndex)
    {

    ogrLine->getPoint(pIndex, ogrTmpPoint);

    typename LineType::VertexType vertex;

    vertex[0] = ogrTmpPoint->getX();
    vertex[1] = ogrTmpPoint->getY();

    if (DataNodeType::Dimension > 2)
      {
      if (LineType::VertexType::PointDimension != 3)
        {
        itkGenericExceptionMacro(<< "OTB vector data can't contain the OGR information (2D instead of 2.5D)");
        }
      vertex[2] = ogrTmpPoint->getZ();
      }

    line->AddVertex(vertex);
    }
  delete ogrTmpPoint;

  DataNodePointerType node = DataNodeType::New();
  node->SetLine(line);

  return node;
}

template<class TVectorData>
inline typename OGRIOHelper<TVectorData>
::DataNodePointerType
OGRIOHelper<TVectorData>
::ConvertGeometryToPolygonNode(const OGRGeometry * ogrGeometry) const
{
  OGRPolygon * ogrPolygon = (OGRPolygon *) ogrGeometry;

  if (ogrPolygon == NULL)
    {
    itkGenericExceptionMacro(<< "Failed to convert OGRGeometry to OGRPolygon");
    }

  OGRPoint * ogrTmpPoint = new OGRPoint();

  OGRLinearRing * ogrRing = ogrPolygon->getExteriorRing();

  PolygonPointerType extRing = PolygonType::New();

  for (int pIndex = 0; pIndex < ogrRing->getNumPoints(); ++pIndex)
    {
    ogrRing->getPoint(pIndex, ogrTmpPoint);
    typename PolygonType::VertexType vertex;
    vertex[0] = ogrTmpPoint->getX();
    vertex[1] = ogrTmpPoint->getY();

    if (DataNodeType::Dimension > 2)
      {
      if (PolygonType::VertexType::PointDimension != 3)
        {
        itkGenericExceptionMacro(<< "OTB vector data can't contain the OGR information (2D instead of 2.5D)");
        }
      vertex[2] = ogrTmpPoint->getZ();
      }

    extRing->AddVertex(vertex);
    }

  PolygonListPointerType intRings = PolygonListType::New();

  for (int intRingIndex = 0; intRingIndex < ogrPolygon->getNumInteriorRings(); ++intRingIndex)
    {
    PolygonPointerType ring = PolygonType::New();
    ogrRing = ogrPolygon->getInteriorRing(intRingIndex);
    for (int pIndex = 0; pIndex < ogrRing->getNumPoints(); ++pIndex)
      {
      ogrRing->getPoint(pIndex, ogrTmpPoint);
      typename PolygonType::VertexType vertex;

      vertex[0] = ogrTmpPoint->getX();
      vertex[1] = ogrTmpPoint->getY();
      if (DataNodeType::Dimension > 2)
        {
        if (PolygonType::VertexType::PointDimension != 3)
          {
          itkGenericExceptionMacro(<< "OTB vector data can't contain the OGR information (2D instead of 2.5D)");
          }
        vertex[2] = ogrTmpPoint->getZ();
        }
      ring->AddVertex(vertex);
      }
    intRings->PushBack(ring);
    }

  delete ogrTmpPoint;

  DataNodePointerType node = DataNodeType::New();
  node->SetPolygonExteriorRing(extRing);
  node->SetPolygonInteriorRings(intRings);

  return node;
}

template<class TVectorData>
inline void OGRIOHelper<TVectorData>
::ConvertOGRLayerToDataTreeNode(OGRLayer * layer, InternalTreeNodeType * documentPtr) const
{
  /** Temporary pointer to store the feature */
  OGRFeature * feature;

  layer->ResetReading();

  unsigned int   counter = 0;
  itk::TimeProbe chrono;

  while ((feature = layer->GetNextFeature()) != NULL)
    {
    chrono.Start();

    typename InternalTreeNodeType::Pointer folderPtr = documentPtr; //TODO: clean references to folder and replace by documentPtr

    // A pointer to the current multi-geometry
    typename InternalTreeNodeType::Pointer multiPtr;

    /** Temporary geometry container */
    OGRGeometry * geometry = feature->GetGeometryRef();

    if (geometry != NULL)
      {
      otb::VectorDataKeywordlist kwl;
      for (int fieldNum = 0; fieldNum < feature->GetFieldCount(); ++fieldNum)
        {
        if (feature->IsFieldSet(fieldNum))
          {
          kwl.AddField(feature->GetFieldDefnRef(fieldNum), feature->GetRawFieldRef(fieldNum));
          }
        }

//       Functor::OGRIOHelper<VectorDataType> IOFunctuor;

      switch (geometry->getGeometryType())
        {
      case wkbPoint:
        {
        typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();

        newNode->Set(ConvertGeometryToPointNode(geometry));
        //Reach the DataNode inside the tree node
        itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
        itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                        MetaDataKey::VectorDataKeywordlistKey,
                                                        kwl);
        folderPtr->AddChild(newNode);
        break;
        }
      case wkbPoint25D:
        {
        typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
        newNode->Set(ConvertGeometryToPointNode(geometry));
        //Reach the DataNode inside the tree node
        itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
        itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                        MetaDataKey::VectorDataKeywordlistKey,
                                                        kwl);
        folderPtr->AddChild(newNode);
        break;
        }
      case wkbLineString:
        {
        typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
        newNode->Set(ConvertGeometryToLineNode(geometry));
        //Reach the DataNode inside the tree node
        itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
        itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                        MetaDataKey::VectorDataKeywordlistKey,
                                                        kwl);
        folderPtr->AddChild(newNode);
        break;
        }
      case wkbLineString25D:
        {
        typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
        newNode->Set(ConvertGeometryToLineNode(geometry));
        //Reach the DataNode inside the tree node
        itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
        itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                        MetaDataKey::VectorDataKeywordlistKey,
                                                        kwl);
        folderPtr->AddChild(newNode);
        break;
        }
      case wkbPolygon:
        {
        typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
        newNode->Set(ConvertGeometryToPolygonNode(geometry));
        //Reach the DataNode inside the tree node
        itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
        itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                        MetaDataKey::VectorDataKeywordlistKey,
                                                        kwl);
        folderPtr->AddChild(newNode);
        break;
        }
      case wkbPolygon25D:
        {
        typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
        newNode->Set(ConvertGeometryToPolygonNode(geometry));
        //Reach the DataNode inside the tree node
        itk::MetaDataDictionary& dict = newNode->Get()->GetMetaDataDictionary();
        itk::EncapsulateMetaData<VectorDataKeywordlist>(dict,
                                                        MetaDataKey::VectorDataKeywordlistKey,
                                                        kwl);
        folderPtr->AddChild(newNode);
        break;
        }
      case wkbMultiPoint:
        {

        DataNodePointerType multi = DataNodeType::New();
        multi->SetNodeType(FEATURE_MULTIPOINT);
        multiPtr = InternalTreeNodeType::New();
        multiPtr->Set(multi);
        folderPtr->AddChild(multiPtr);

        OGRMultiPoint * ogrMulti = (OGRMultiPoint *) geometry;

        for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          newNode->Set(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)));
          multiPtr->AddChild(newNode);
          }
        break;
        }
      case wkbMultiPoint25D:
        {
        DataNodePointerType multi = DataNodeType::New();
        multi->SetNodeType(FEATURE_MULTIPOINT);

        multiPtr = InternalTreeNodeType::New();
        multiPtr->Set(multi);
        folderPtr->AddChild(multiPtr);

        OGRMultiPoint * ogrMulti = (OGRMultiPoint *) geometry;

        for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          newNode->Set(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)));
          multiPtr->AddChild(newNode);
          }
        break;
        }
      case wkbMultiLineString:
        {
        DataNodePointerType multi = DataNodeType::New();
        multi->SetNodeType(FEATURE_MULTILINE);

        multiPtr = InternalTreeNodeType::New();
        multiPtr->Set(multi);
        folderPtr->AddChild(multiPtr);

        OGRMultiLineString * ogrMulti = (OGRMultiLineString *) geometry;

        for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          newNode->Set(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)));
          multiPtr->AddChild(newNode);
          }
        break;
        }
      case wkbMultiLineString25D:
        {
        DataNodePointerType multi = DataNodeType::New();
        multi->SetNodeType(FEATURE_MULTILINE);

        multiPtr = InternalTreeNodeType::New();
        multiPtr->Set(multi);
        folderPtr->AddChild(multiPtr);

        OGRMultiLineString * ogrMulti = (OGRMultiLineString *) geometry;

        for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          newNode->Set(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)));
          multiPtr->AddChild(newNode);
          }
        break;
        }
      case wkbMultiPolygon:
        {
        DataNodePointerType multi = DataNodeType::New();
        multi->SetNodeType(FEATURE_MULTIPOLYGON);

        multiPtr = InternalTreeNodeType::New();
        multiPtr->Set(multi);
        folderPtr->AddChild(multiPtr);

        OGRMultiPolygon * ogrMulti = (OGRMultiPolygon *) geometry;

        for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          newNode->Set(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)));
          multiPtr->AddChild(newNode);
          }
        break;
        }
      case wkbMultiPolygon25D:
        {
        DataNodePointerType multi = DataNodeType::New();
        multi->SetNodeType(FEATURE_MULTIPOLYGON);

        multiPtr = InternalTreeNodeType::New();
        multiPtr->Set(multi);
        folderPtr->AddChild(multiPtr);

        OGRMultiPolygon * ogrMulti = (OGRMultiPolygon *) geometry;

        for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
          {
          typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
          newNode->Set(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)));
          multiPtr->AddChild(newNode);
          }
        break;
        }
      case wkbGeometryCollection:
        {
        DataNodePointerType multi = DataNodeType::New();
        multi->SetNodeType(FEATURE_COLLECTION);

        multiPtr = InternalTreeNodeType::New();
        multiPtr->Set(multi);
        folderPtr->AddChild(multiPtr);

        OGRGeometryCollection * ogrMulti = (OGRGeometryCollection *) geometry;

        for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
          {
          switch (ogrMulti->getGeometryRef(geoIndex)->getGeometryType())
            {
          case wkbPoint:
            {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)));
            multiPtr->AddChild(newNode);
            break;
            }
          case wkbPoint25D:
            {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)));
            multiPtr->AddChild(newNode);
            break;
            }
          case wkbLineString:
            {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)));
            multiPtr->AddChild(newNode);
            break;
            }
          case wkbLineString25D:
            {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)));
            multiPtr->AddChild(newNode);
            break;
            }
          case wkbPolygon:
            {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)));
            multiPtr->AddChild(newNode);
            break;
            }
          case wkbPolygon25D:
            {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)));
            multiPtr->AddChild(newNode);
            break;
            }
          default:
            {
            std::cout << "Geometry type not found: " << ogrMulti->getGeometryRef(geoIndex)->getGeometryType() <<
            std::endl;
            break;
            }
            }
          }
        break;
        }
      case wkbGeometryCollection25D:
        {
        DataNodePointerType multi = DataNodeType::New();
        multi->SetNodeType(FEATURE_COLLECTION);

        multiPtr = InternalTreeNodeType::New();
        multiPtr->Set(multi);
        folderPtr->AddChild(multiPtr);

        OGRGeometryCollection * ogrMulti = (OGRGeometryCollection *) geometry;

        for (int geoIndex = 0; geoIndex < ogrMulti->getNumGeometries(); ++geoIndex)
          {
          switch (ogrMulti->getGeometryRef(geoIndex)->getGeometryType())
            {
          case wkbPoint:
            {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)));
            multiPtr->AddChild(newNode);
            break;
            }
          case wkbPoint25D:
            {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)));
            multiPtr->AddChild(newNode);
            break;
            }
          case wkbLineString:
            {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)));
            multiPtr->AddChild(newNode);
            break;
            }
          case wkbLineString25D:
            {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)));
            multiPtr->AddChild(newNode);
            break;
            }
          case wkbPolygon:
            {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)));
            multiPtr->AddChild(newNode);
            break;
            }
          case wkbPolygon25D:
            {
            typename InternalTreeNodeType::Pointer newNode = InternalTreeNodeType::New();
            newNode->Set(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)));
            multiPtr->AddChild(newNode);
            break;
            }
          default:
            {
            std::cout << "Geometry type not found: " << ogrMulti->getGeometryRef(geoIndex)->getGeometryType() <<
            std::endl;
            break;
            }
            }
          }
        break;
        }
      default:
        {
        std::cout << "Geometry not handled: " << geometry->getGeometryName() << std::endl;
        break;
        }
        }

      }
    OGRFeature::DestroyFeature(feature);
    chrono.Stop();
    ++counter;
    } //end While feature
  otbMsgDevMacro(
    << layer->GetFeatureCount() << " features read, average insertion time " << chrono.GetMeanTime() << " s");
}

template<class TVectorData>
unsigned int OGRIOHelper<TVectorData>
::ProcessNodeWrite(InternalTreeNodeType * source, OGRDataSource * m_DataSource, OGRGeometryCollection * ogrCollection,
                   OGRLayer * ogrCurrentLayer, OGRSpatialReference * oSRS)
{
  //unsigned int m_Kept = 0;
  // Get the children list from the input node
  ChildrenListType children = source->GetChildrenList();
//   std::cout << "Found " <<  children.size() << " children" << std::endl;

  // For each child
  for (typename ChildrenListType::iterator it = children.begin(); it != children.end(); ++it)
    {
    DataNodePointerType dataNode = (*it)->Get();
//     std::cout << "Processing " << dataNode->GetNodeType() << " with Id: " << dataNode->GetNodeId() << std::endl;
    otbGenericMsgDebugMacro(<< "Type of node " << dataNode->GetNodeType() << " id" << dataNode->GetNodeId());
    ++m_Kept;
    switch (dataNode->GetNodeType())
      {
    case ROOT:
      {
      break;
      }
    case DOCUMENT:
      {
      ogrCurrentLayer = m_DataSource->CreateLayer(dataNode->GetNodeId(), oSRS, wkbUnknown, NULL);
      if (ogrCurrentLayer == NULL)
        {
        //itkExceptionMacro(<<"Failed to create layer "<<dataNode->GetNodeId());
        std::cout << "Failed to create layer " << dataNode->GetNodeId() << std::endl;
        }
      ProcessNodeWrite(*it, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);
      break;
      }
    case FOLDER:
      {

      ProcessNodeWrite(*it, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);
      break;
      }
    case FEATURE_POINT:
      {
      //Build the ogrObject
      OGRPoint ogrPoint;
      ogrPoint.setX(dataNode->GetPoint()[0]);
      ogrPoint.setY(dataNode->GetPoint()[1]);

      if (DataNodeType::Dimension > 2)
        {
        ogrPoint.setZ(dataNode->GetPoint()[2]);
        }

      //Save it in the structure
      if (ogrCollection == NULL)
        {
        OGRFeature *ogrFeature;

        ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
        ogrFeature->SetField("Name", dataNode->GetNodeId());
        ogrFeature->SetGeometry(&ogrPoint);

        if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
          {
          itkExceptionMacro(<< "Failed to create feature in shapefile.");
//             std::cout << "Failed to create feature in shapefile."
          }

        OGRFeature::DestroyFeature(ogrFeature);
        }
      else
        {
        ogrCollection->addGeometry(&ogrPoint);
        }

      break;
      }
    case FEATURE_LINE:
      {
      //Build the ogrObject
      OGRLineString              ogrLine;
      VertexListConstPointerType vertexList = dataNode->GetLine()->GetVertexList();

      typename VertexListType::ConstIterator vIt = vertexList->Begin();

      while (vIt != vertexList->End())
        {
        OGRPoint ogrPoint;
        ogrPoint.setX(vIt.Value()[0]);
        ogrPoint.setY(vIt.Value()[1]);
        if (DataNodeType::Dimension > 2)
          {
          ogrPoint.setZ(vIt.Value()[2]);
          }
        ogrLine.addPoint(&ogrPoint);
        ++vIt;
        }

      //Save it in the structure
      if (ogrCollection == NULL)
        {
        OGRFeature *ogrFeature;

        ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
        ogrFeature->SetField("Name", dataNode->GetNodeId());
        ogrFeature->SetGeometry(&ogrLine);

        if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
          {
          itkExceptionMacro(<< "Failed to create feature in shapefile.");
          }

        OGRFeature::DestroyFeature(ogrFeature);

        }
      else
        {
        ogrCollection->addGeometry(&ogrLine);
        }

      break;
      }
    case FEATURE_POLYGON:
      {
      
      //Build the ogrObject
      OGRPolygon *               ogrPolygon = new OGRPolygon();
      OGRLinearRing *            ogrExternalRing = new OGRLinearRing();
      VertexListConstPointerType vertexList = dataNode->GetPolygonExteriorRing()->GetVertexList();

      typename VertexListType::ConstIterator vIt = vertexList->Begin();

      while (vIt != vertexList->End())
        {
        OGRPoint ogrPoint;
	std::cout <<"OgrPoint " <<vIt.Value()[0] << ","<< vIt.Value()[1] << std::endl;
        ogrPoint.setX(vIt.Value()[0]);
        ogrPoint.setY(vIt.Value()[1]);
        if (DataNodeType::Dimension > 2)
          {
          ogrPoint.setZ(vIt.Value()[2]);
          }

        ogrExternalRing->addPoint(&ogrPoint);
        ++vIt;
        }
      ogrPolygon->addRing(ogrExternalRing);
      delete ogrExternalRing;

      // Retrieving internal rings as well
      for (typename PolygonListType::Iterator pIt = dataNode->GetPolygonInteriorRings()->Begin();
           pIt != dataNode->GetPolygonInteriorRings()->End(); ++pIt)
        {
        OGRLinearRing * ogrInternalRing = new OGRLinearRing();
        vertexList = pIt.Get()->GetVertexList();
        vIt = vertexList->Begin();

        while (vIt != vertexList->End())
          {
          OGRPoint ogrPoint;
          ogrPoint.setX(vIt.Value()[0]);
          ogrPoint.setY(vIt.Value()[1]);
          if (DataNodeType::Dimension > 2)
            {
            ogrPoint.setZ(vIt.Value()[2]);
            }
          ogrInternalRing->addPoint(&ogrPoint);
          ++vIt;
          }
        ogrPolygon->addRing(ogrInternalRing);
        delete ogrInternalRing;
        }

      //Save it in the structure
      if (ogrCollection == NULL)
        {
        OGRFeature *ogrFeature;

        ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
        ogrFeature->SetField("Name", dataNode->GetNodeId());
        ogrFeature->SetGeometry(ogrPolygon);

        if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
          {
          itkExceptionMacro(<< "Failed to create feature in shapefile.");
          }

        OGRFeature::DestroyFeature(ogrFeature);

        }
      else
        {
        ogrCollection->addGeometry(ogrPolygon);
        }

      delete ogrPolygon;
      break;
      }
    case FEATURE_MULTIPOINT:
      {
      if (ogrCollection != NULL)
        {
        itkExceptionMacro(<< "Problem while creating multipoint.");
        }
      //ogrCollection = new OGRMultiPoint();

      OGRFeature *ogrFeature;

      ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
      ogrFeature->SetField("Name", dataNode->GetNodeId());
      ogrFeature->GetDefnRef()->SetGeomType(wkbMultiPoint);
      ogrFeature->SetGeometry(ogrCollection);

      if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
        {
        itkExceptionMacro(<< "Failed to create feature in shapefile.");
        }

      ProcessNodeWrite(*it, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);
      break;
      }
    case FEATURE_MULTILINE:
      {
      if (ogrCollection != NULL)
        {
        itkExceptionMacro(<< "Problem while creating multiline.");
        }
      // Comment this line :  the ogrCollection has not to be
      // recreated since it is created once when we process the node ROOT
      //ogrCollection = new OGRMultiLineString();

      OGRFeature *ogrFeature;

      ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
      ogrFeature->SetField("Name", dataNode->GetNodeId());
      ogrFeature->GetDefnRef()->SetGeomType(wkbMultiLineString);
      ogrFeature->SetGeometry(ogrCollection);

      if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
        {
        itkExceptionMacro(<< "Failed to create feature in shapefile.");
        }      
      ProcessNodeWrite(*it, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);
      break;
      }
    case FEATURE_MULTIPOLYGON:
      {
      if (ogrCollection != NULL)
        {
        itkExceptionMacro(<< "Problem while creating multipolygon.");
        }
      // Comment this line :  the ogrCollection has not to be
      // recreated since it is created once when we process the node ROOT
      //ogrCollection = new OGRMultiPolygon();

      OGRFeature *ogrFeature;

      ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
      ogrFeature->SetField("Name", dataNode->GetNodeId());
      ogrFeature->GetDefnRef()->SetGeomType(wkbMultiPolygon);
      ogrFeature->SetGeometry(ogrCollection);

      if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
        {
        itkExceptionMacro(<< "Failed to create feature in shapefile.");
        }
      ProcessNodeWrite(*it, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);
      break;
      }
    case FEATURE_COLLECTION:
      {
      if (ogrCollection != NULL)
        {
        itkExceptionMacro(<< "Problem while creating collection.");
        }
      // Comment this line :  the ogrCollection has not to be
      // recreated since it is created once when we process the node ROOT
      //ogrCollection = new OGRMultiPolygon();
      

      OGRFeature *ogrFeature;

      ogrFeature = OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn());
      ogrFeature->SetField("Name", dataNode->GetNodeId());
      ogrFeature->GetDefnRef()->SetGeomType(wkbGeometryCollection);
      ogrFeature->SetGeometry(ogrCollection);

      if (ogrCurrentLayer->CreateFeature(ogrFeature) != OGRERR_NONE)
        {
        itkExceptionMacro(<< "Failed to create feature in shapefile.");
        }

      ProcessNodeWrite(*it, m_DataSource, ogrCollection, ogrCurrentLayer, oSRS);
      break;
      }
      }
    }

  return m_Kept;
}
/*
template<class TLabelObject, class TPolygon>
inline typename OGRIOHelper<TLabelObject,TPolygon>
::PolygonPointerType
OGRIOHelper<TLabelObject,TPolygon>
::operator()(const LabelObjectType * labelObject)
{

}
*/

} // end namespace otb

#endif
