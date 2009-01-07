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
#ifndef __otbSHPVectorDataIO_txx
#define __otbSHPVectorDataIO_txx

#include "otbSHPVectorDataIO.h"

#include <itksys/SystemTools.hxx>
#include "itkExceptionObject.h"
#include "itkByteSwapper.h"
#include "otbMacro.h"
#include "otbSystem.h"
#include "otbDataNode.h"
#include "itkPreOrderTreeIterator.h"
#include "otbMetaDataKey.h"
#include "otbFileName.h"

namespace otb
{
  template<class TData>
  SHPVectorDataIO<TData>
  ::SHPVectorDataIO()
  {
    // OGR factory registration
    OGRRegisterAll();
    m_DataSource = NULL;

  }
  template<class TData>
  SHPVectorDataIO<TData>::~SHPVectorDataIO()
  {
    if(m_DataSource != NULL)
      {
	OGRDataSource::DestroyDataSource(m_DataSource);
      }
  }
  template<class TData>
  bool
  SHPVectorDataIO<TData>::CanReadFile( const char* filename )
  {


    OGRDataSource * poDS = OGRSFDriverRegistrar::Open(filename, FALSE);
    if(poDS == NULL)
      {
	return false;
      }

    OGRDataSource::DestroyDataSource(poDS);
    return true;
  }


  // Used to print information about this object
  template<class TData>
  void
  SHPVectorDataIO<TData>::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

  // Read vector data
  template<class TData>
  void
  SHPVectorDataIO<TData>
  ::Read(VectorDataPointerType data)
  {
    // Destroy previous opened data source
    if(m_DataSource != NULL)
      {
	OGRDataSource::DestroyDataSource(m_DataSource);
      }

    m_DataSource = OGRSFDriverRegistrar::Open(this->m_FileName.c_str(), FALSE);

    if(m_DataSource == NULL)
      {
	itkExceptionMacro(<<"Failed to open data file "<<this->m_FileName);
      }

    otbMsgDebugMacro( <<"Driver to read: OGR");
    otbMsgDebugMacro( <<"Reading  file: "<< this->m_FileName);

    // Reading layers
    otbMsgDebugMacro(<<"Number of layers: "<<m_DataSource->GetLayerCount());

    // Retrieving root node
    DataTreePointerType tree = data->GetDataTree();
    DataNodePointerType root = tree->GetRoot()->Get();

    OGRSpatialReference * oSRS =NULL;
    //We take the assumption that the spatial reference is common to all layers
    oSRS = m_DataSource->GetLayer(0)->GetSpatialRef();

    if (oSRS != NULL)
    {
      char * projectionRefChar;
      oSRS->exportToWkt(&projectionRefChar);
      std::string projectionRef = projectionRefChar;
      OGRFree(projectionRefChar);
      itk::MetaDataDictionary & dict = data->GetMetaDataDictionary();
      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef );
    }
    else
    {
      otbMsgDevMacro(<< "Can't retrieve the OGRSpatialReference from the shapefile");
    }



    std::string projectionRefWkt = data->GetProjectionRef();

//     if (projectionRefWkt.compare(this->m_TargetProjection) != 0)
//     {
//       itkGenericExceptionMacro(<<"OTB is currently not able to reproject shapefiles from: \n"
//           << projectionRefWkt << "\n to \n" << this->m_TargetProjection);
//     }


    bool projectionInformationAvailable = !projectionRefWkt.empty();

    if (projectionInformationAvailable)
    {
      otbMsgDevMacro(<< "Projection information : " << projectionRefWkt);
      otbMsgDevMacro(<< " - Origin : " << this->m_Origin);
      otbMsgDevMacro(<< " - Spacing : " << this->m_Spacing);
    }
    else
    {
      this->m_Origin.Fill(0.0);
      this->m_Spacing.Fill(1.0);
      otbMsgDevMacro(<< "Projection information unavailable => spacing set to 1 and origin to 0");
    }
    data->SetOrigin(this->m_Origin);
    data->SetSpacing(this->m_Spacing);

    // For each layer
    for(int layerIndex = 0;layerIndex<m_DataSource->GetLayerCount();++layerIndex)
      {
	/** retrieving layer and property */
	OGRLayer * layer = m_DataSource->GetLayer(layerIndex);
	OGRFeatureDefn * dfn = layer->GetLayerDefn();

	/** Create the document node */
	DataNodePointerType document = DataNodeType::New();
	document->SetNodeType(DOCUMENT);
	document->SetNodeId(dfn->GetName());

	/** Retrieving the fields types */
	for(int fieldIndex = 0; fieldIndex<dfn->GetFieldCount();++fieldIndex)
	  {
	    OGRFieldDefn * field  = dfn->GetFieldDefn(fieldIndex);
	    document->SetField(field->GetNameRef(),OGRFieldDefn::GetFieldTypeName(field->GetType()));
	    // std::cout<<"Document "<<document->GetNodeId()<<": Adding field "<<field->GetNameRef()<<" "<<OGRFieldDefn::GetFieldTypeName(field->GetType())<<std::endl;
	  }

	/** Adding the layer to the data tree */
	tree->Add(document,root);

	/** Temporary pointer to store the feature */
	OGRFeature * feature;

	layer->ResetReading();

	while((feature = layer->GetNextFeature())!=NULL)
	  {
	    DataNodePointerType folder = DataNodeType::New();
	    folder->SetNodeType(FOLDER);
	    folder->SetNodeId(feature->GetDefnRef()->GetName());

	    // Reading fields
	    for(int fieldIndex = 0; fieldIndex<dfn->GetFieldCount();++fieldIndex)
	      {
		OGRFieldDefn * field  = dfn->GetFieldDefn(fieldIndex);
		folder->SetField(field->GetNameRef(),feature->GetFieldAsString(fieldIndex));
		//  std::cout<<"Folder "<<folder->GetNodeId()<<": Adding field "<<field->GetNameRef()<<" "<<feature->GetFieldAsString(fieldIndex)<<std::endl;
	      }


	    tree->Add(folder,document);

	    /** Temporary geometry container */
	    OGRGeometry * geometry = feature->GetGeometryRef();


	    if(geometry != NULL)
	      {
		switch(geometry->getGeometryType())
		  {
		  case wkbPoint:
		    {
		      tree->Add(ConvertGeometryToPointNode(geometry),folder);
		      break;
		    }
		  case wkbPoint25D:
		    {
		      tree->Add(ConvertGeometryToPointNode(geometry),folder);
		      break;
		    }
		  case wkbLineString:
		    {
		      tree->Add(ConvertGeometryToLineNode(geometry),folder);
		      break;
		    }
		  case wkbLineString25D:
		    {
		      tree->Add(ConvertGeometryToLineNode(geometry),folder);
		      break;
		    }
		  case wkbPolygon:
		    {
		      tree->Add(ConvertGeometryToPolygonNode(geometry),folder);
		      break;
		    }
		  case wkbPolygon25D:
		    {
		      tree->Add(ConvertGeometryToPolygonNode(geometry),folder);
		      break;
		    }
		  case wkbMultiPoint:
		    {
		      DataNodePointerType multi = DataNodeType::New();
		      multi->SetNodeType(FEATURE_MULTIPOINT);
		      tree->Add(multi,folder);

		      OGRMultiPoint * ogrMulti = (OGRMultiPoint *) geometry;

		      for(int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
			{
			  tree->Add(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)),multi);
			}
		      break;
		    }
		  case wkbMultiPoint25D:
		    {
		      DataNodePointerType multi = DataNodeType::New();
		      multi->SetNodeType(FEATURE_MULTIPOINT);
		      tree->Add(multi,folder);

		      OGRMultiPoint * ogrMulti = (OGRMultiPoint *) geometry;

		      for(int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
			{
			  tree->Add(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)),multi);
			}
		      break;
		    }
		  case wkbMultiLineString:
		    {
		      DataNodePointerType multi = DataNodeType::New();
		      multi->SetNodeType(FEATURE_MULTILINE);
		      tree->Add(multi,folder);

		      OGRMultiLineString * ogrMulti = (OGRMultiLineString *) geometry;

		      for(int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
			{
			  tree->Add(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)),multi);
			}
		      break;
		    }
		  case wkbMultiLineString25D:
		    {
		      DataNodePointerType multi = DataNodeType::New();
		      multi->SetNodeType(FEATURE_MULTILINE);
		      tree->Add(multi,folder);

		      OGRMultiLineString * ogrMulti = (OGRMultiLineString *) geometry;

		      for(int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
			{
			  tree->Add(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)),multi);
			}
		      break;
		    }
		  case wkbMultiPolygon:
		    {
		      DataNodePointerType multi = DataNodeType::New();
		      multi->SetNodeType(FEATURE_MULTIPOLYGON);
		      tree->Add(multi,folder);

		      OGRMultiPolygon * ogrMulti = (OGRMultiPolygon *) geometry;

		      for(int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
			{
			  tree->Add(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)),multi);
			}
		      break;
		    }
		  case wkbMultiPolygon25D:
		    {
		      DataNodePointerType multi = DataNodeType::New();
		      multi->SetNodeType(FEATURE_MULTIPOLYGON);
		      tree->Add(multi,folder);

		      OGRMultiPolygon * ogrMulti = (OGRMultiPolygon *) geometry;

		      for(int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
			{
			  tree->Add(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)),multi);
			}
		      break;
		    }
		  case wkbGeometryCollection:
		    {
		      DataNodePointerType multi = DataNodeType::New();
		      multi->SetNodeType(FEATURE_COLLECTION);
		      tree->Add(multi,folder);

		      OGRGeometryCollection * ogrMulti = (OGRGeometryCollection *) geometry;

		      for(int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
			{
			  switch(ogrMulti->getGeometryRef(geoIndex)->getGeometryType())
			    {
			    case wkbPoint:
			      {
				tree->Add(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)),multi);
				break;
			      }
			    case wkbPoint25D:
			      {
				tree->Add(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)),multi);
				break;
			      }
			    case wkbLineString:
			      {
				tree->Add(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)),multi);
				break;
			      }
			    case wkbLineString25D:
			      {
				tree->Add(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)),multi);
				break;
			      }
			    case wkbPolygon:
			      {
				tree->Add(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)),multi);
				break;
			      }
			    case wkbPolygon25D:
			      {
				tree->Add(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)),multi);
				break;
			      }
			    default:
			      {
				std::cout<<"Geometry type not found: "<<ogrMulti->getGeometryRef(geoIndex)->getGeometryType()<<std::endl;
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
		      tree->Add(multi,folder);

		      OGRGeometryCollection * ogrMulti = (OGRGeometryCollection *) geometry;

		      for(int geoIndex = 0;geoIndex<ogrMulti->getNumGeometries();++geoIndex)
			{
			  switch(ogrMulti->getGeometryRef(geoIndex)->getGeometryType())
			    {
			    case wkbPoint:
			      {
				tree->Add(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)),multi);
				break;
			      }
			    case wkbPoint25D:
			      {
				tree->Add(ConvertGeometryToPointNode(ogrMulti->getGeometryRef(geoIndex)),multi);
				break;
			      }
			    case wkbLineString:
			      {
				tree->Add(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)),multi);
				break;
			      }
			    case wkbLineString25D:
			      {
				tree->Add(ConvertGeometryToLineNode(ogrMulti->getGeometryRef(geoIndex)),multi);
				break;
			      }
			    case wkbPolygon:
			      {
				tree->Add(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)),multi);
				break;
			      }
			    case wkbPolygon25D:
			      {
				tree->Add(ConvertGeometryToPolygonNode(ogrMulti->getGeometryRef(geoIndex)),multi);
				break;
			      }
			    default:
			      {
				std::cout<<"Geometry type not found: "<<ogrMulti->getGeometryRef(geoIndex)->getGeometryType()<<std::endl;
				break;
			      }
			    }
			}
		      break;
		    }
		  default:
		    {
		      std::cout<<"Geometry not handled: "<<geometry->getGeometryName()<<std::endl;
		      break;
		    }
		  }

	      }
	  }
      }
  }


  template<class TData>
  typename SHPVectorDataIO<TData>
  ::DataNodePointerType
  SHPVectorDataIO<TData>
  ::ConvertGeometryToPointNode(const OGRGeometry * ogrGeometry) const
  {
    OGRPoint * ogrPoint = (OGRPoint *) ogrGeometry;

    if(ogrPoint == NULL)
    {
      itkGenericExceptionMacro(<<"Failed to convert OGRGeometry to OGRPoint");
    }
    SpacingType spacing = this->m_Spacing;
    OriginType origin = this->m_Origin;

    PointType otbPoint;
    otbPoint.Fill(0);
    otbPoint[0] = static_cast<typename DataNodeType::PrecisionType>((ogrPoint->getX()-origin[0])/spacing[0]);
    otbPoint[1] = static_cast<typename DataNodeType::PrecisionType>((ogrPoint->getY()-origin[1])/spacing[1]);

    if(DataNodeType::Dimension > 2)
      {
        otbPoint[2]=static_cast<typename DataNodeType::PrecisionType>((ogrPoint->getZ()-origin[2])/spacing[2]);
      }

    DataNodePointerType node = DataNodeType::New();
    node->SetPoint(otbPoint);
    return node;
  }

  template<class TData>
  typename SHPVectorDataIO<TData>
  ::DataNodePointerType
  SHPVectorDataIO<TData>
  ::ConvertGeometryToLineNode(const OGRGeometry * ogrGeometry) const
  {
    OGRLineString * ogrLine = (OGRLineString *)ogrGeometry;

    if(ogrLine == NULL)
    {
      itkGenericExceptionMacro(<<"Failed to convert OGRGeometry to OGRLine");
    }
    SpacingType spacing = this->m_Spacing;
    OriginType origin = this->m_Origin;

    LinePointerType line = LineType::New();

    OGRPoint * ogrTmpPoint = new OGRPoint();

    for(int pIndex = 0;pIndex<ogrLine->getNumPoints();++pIndex)
      {

	ogrLine->getPoint(pIndex,ogrTmpPoint);

	typename LineType::VertexType vertex;

        vertex[0] = (ogrTmpPoint->getX()-origin[0])/spacing[0];
        vertex[1] = (ogrTmpPoint->getY()-origin[1])/spacing[1];

	if(DataNodeType::Dimension > 2)
	  {
            vertex[2]= (ogrTmpPoint->getZ()-origin[2])/spacing[2];
	  }

	line->AddVertex(vertex);
      }
    delete ogrTmpPoint;

    DataNodePointerType node = DataNodeType::New();
    node->SetLine(line);

    return node;
  }

  template<class TData>
  typename SHPVectorDataIO<TData>
  ::DataNodePointerType
  SHPVectorDataIO<TData>
  ::ConvertGeometryToPolygonNode(const OGRGeometry * ogrGeometry) const
  {
    OGRPolygon * ogrPolygon = (OGRPolygon *)ogrGeometry;

    if(ogrPolygon == NULL)
    {
      itkGenericExceptionMacro(<<"Failed to convert OGRGeometry to OGRPolygon");
    }
    SpacingType spacing = this->m_Spacing;
    OriginType origin = this->m_Origin;

    OGRPoint * ogrTmpPoint = new OGRPoint();

    OGRLinearRing *  ogrRing = ogrPolygon->getExteriorRing();

    PolygonPointerType extRing = PolygonType::New();

    for(int pIndex = 0;pIndex<ogrRing->getNumPoints();++pIndex)
      {
	ogrRing->getPoint(pIndex,ogrTmpPoint);
	typename PolygonType::VertexType vertex;
        vertex[0] = (ogrTmpPoint->getX()-origin[0])/spacing[0];
        vertex[1] = (ogrTmpPoint->getY()-origin[1])/spacing[1];

	if(DataNodeType::Dimension > 2)
	  {
            vertex[2]= (ogrTmpPoint->getZ()-origin[2])/spacing[2];
	  }

	extRing->AddVertex(vertex);
      }

    PolygonListPointerType intRings = PolygonListType::New();

    for(int intRingIndex = 0;intRingIndex<ogrPolygon->getNumInteriorRings();++intRingIndex)
      {
	PolygonPointerType ring = PolygonType::New();
	ogrRing = ogrPolygon->getInteriorRing(intRingIndex);
	for(int pIndex = 0;pIndex<ogrRing->getNumPoints();++pIndex)
	  {
	    ogrRing->getPoint(pIndex,ogrTmpPoint);
	    typename PolygonType::VertexType vertex;

            vertex[0] = (ogrTmpPoint->getX()-origin[0])/spacing[0];
            vertex[1] = (ogrTmpPoint->getY()-origin[1])/spacing[1];
	    if(DataNodeType::Dimension > 2)
	      {
                vertex[2]= (ogrTmpPoint->getZ()-origin[2])/spacing[2];
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

  template<class TData>
  bool SHPVectorDataIO<TData>::CanWriteFile( const char* filename )
  {
    std::string lFileName(filename);
    if( System::IsADirName(lFileName) == true )
      {
	return false;
      }
    const std::string Extension = System::GetExtension(filename);
    if( (Extension == "shp") || (Extension == "SHP") )
      {
	return true;
      }
    else
      {
	return false;
      }
  }

  template<class TData>
  void SHPVectorDataIO<TData>::Write(const VectorDataConstPointerType data)
  {
    //  // try to create an ogr driver
    OGRSFDriver * ogrDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("ESRI Shapefile");

    if(ogrDriver == NULL)
      {
	itkExceptionMacro(<<"No OGR driver found to write file "<<this->m_FileName);
      }

    // free an existing previous data source, if any
    if(m_DataSource != NULL)
      {
	OGRDataSource::DestroyDataSource(m_DataSource);
      }

   //if file exist, OGR can't overwrite: remove it  first
    otb::FileName filename(this->m_FileName.c_str());
    if (filename.exists())
    {
      filename.remove();
    }

    // m_DataSource = OGRSFDriverRegistrar::Open(this->m_FileName.c_str(), TRUE);
    m_DataSource = ogrDriver->CreateDataSource(this->m_FileName.c_str(),NULL);


    // check the created data source
    if(m_DataSource == NULL)
      {
	itkExceptionMacro(<<"Failed to create OGR data source for file "<<this->m_FileName<<". Since OGR can not overwrite existing file, be sure that this file does not already exist");
      }

    // Retrieve data required for georeferencing
    OriginType origin;
    SpacingType spacing;
    origin.Fill(0.0);
    spacing.Fill(1.0);

    std::string projectionRefWkt = data->GetProjectionRef();
    bool projectionInformationAvailable = !projectionRefWkt.empty();

    if (projectionInformationAvailable)
    {
      origin = data->GetOrigin();
      spacing = data->GetSpacing();
      otbMsgDevMacro(<< "Projection information : " << projectionRefWkt);
      otbMsgDevMacro(<< " - Origin : " << origin);
      otbMsgDevMacro(<< " - Spacing : " << spacing);
    }
    else
    {
      otbMsgDevMacro(<< "Projection information unavailable => spacing set to 1 and origin to 0");
    }

    // Retrieving root node
    DataTreeConstPointerType tree = data->GetDataTree();
    if (tree->GetRoot() == NULL)
    {
      itkExceptionMacro(<<"Data tree is empty: Root == NULL");
    }
    DataNodePointerType root = tree->GetRoot()->Get();

    typedef itk::PreOrderTreeIterator<DataTreeType> TreeIteratorType;

    OGRSpatialReference * oSRS =NULL;
    OGRLayer * ogrCurrentLayer = NULL;
    std::vector<OGRFeature *> ogrFeatures;
    OGRGeometryCollection * ogrCollection = NULL;
    // OGRGeometry * ogrCurrentGeometry = NULL;

    TreeIteratorType it(tree);
    it.GoToBegin();

    while(!it.IsAtEnd())
      {
	switch(it.Get()->GetNodeType())
	  {
	  case ROOT:
	    {
	      break;
	    }
	  case DOCUMENT:
	    {
	      if(ogrCurrentLayer!=NULL && ogrFeatures.size()>0)
		{
		  std::vector<OGRFeature*>::iterator fIt = ogrFeatures.begin();

		  while(fIt!=ogrFeatures.end())
		    {
		      if(ogrCurrentLayer->CreateFeature(*fIt) != OGRERR_NONE)
			{
			  itkExceptionMacro(<<"Failed to create ogr feature in file "<<this->m_FileName);
			}
		      OGRFeature::DestroyFeature(*fIt);
		      ++fIt;
		    }
		}
	      ogrFeatures.clear();
              //TODO georeference here from OGRSpatialReference http://www.gdal.org/ogr/classOGRDataSource.html
              if (projectionInformationAvailable)
              {
                oSRS = new OGRSpatialReference(projectionRefWkt.c_str());
              }

              ogrCurrentLayer = m_DataSource->CreateLayer(it.Get()->GetNodeId(),oSRS,wkbUnknown,NULL);
	      if(ogrCurrentLayer == NULL)
	      {
		itkExceptionMacro(<<"Failed to create layer "<<it.Get()->GetNodeId());
	      }
	      break;
	    }
	    case FOLDER:
		{
		  if(ogrCurrentLayer!=NULL && ogrCollection != NULL && !ogrFeatures.empty())
		    {
		      ogrFeatures.back()->SetGeometry(ogrCollection);
		      delete ogrCollection;
		      ogrCollection = NULL;
		    }

		  ogrFeatures.push_back(OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn()));
		  ogrFeatures.back()->SetField("Name",it.Get()->GetNodeId());

		  break;
		}
	    case FEATURE_POINT:
	      {
		OGRPoint ogrPoint;
                ogrPoint.setX(it.Get()->GetPoint()[0] * spacing[0] + origin[0]);
                ogrPoint.setY(it.Get()->GetPoint()[1] * spacing[1] + origin[1]);


		if(DataNodeType::Dimension>2)
		  {
                    ogrPoint.setZ(it.Get()->GetPoint()[2] * spacing[2] + origin[2]);
		  }

		if(ogrCollection == NULL)
		  {
		    ogrFeatures.back()->GetDefnRef()->SetGeomType(wkbPoint);
		    ogrFeatures.back()->SetGeometry(&ogrPoint);
		  }
		else
		  {
		    ogrCollection->addGeometry(&ogrPoint);
		  }

 		break;
	      }
	    case FEATURE_LINE:
 	      {
		OGRLineString ogrLine;
		VertexListConstPointerType vertexList = it.Get()->GetLine()->GetVertexList();

		typename VertexListType::ConstIterator vIt = vertexList->Begin();

		while(vIt != vertexList->End())
		  {
		    OGRPoint ogrPoint;
                    ogrPoint.setX(vIt.Value()[0] * spacing[0] + origin[0]);
                    ogrPoint.setY(vIt.Value()[1] * spacing[1] + origin[1]);
		    if(DataNodeType::Dimension>2)
		      {
                        ogrPoint.setZ(vIt.Value()[2] * spacing[2] + origin[2]);
		      }
		    ogrLine.addPoint(&ogrPoint);
		    ++vIt;
		  }

		if(ogrCollection == NULL)
		  {
		    ogrFeatures.back()->GetDefnRef()->SetGeomType(wkbLineString);
		    ogrFeatures.back()->SetGeometry(&ogrLine);
		  }
		else
		  {
		    ogrCollection->addGeometry(&ogrLine);
		  }

		break;
	      }
	    case FEATURE_POLYGON:
	      {
		OGRPolygon * ogrPolygon = new OGRPolygon();
		OGRLinearRing * ogrExternalRing = new OGRLinearRing();
		VertexListConstPointerType vertexList = it.Get()->GetPolygonExteriorRing()->GetVertexList();

		typename VertexListType::ConstIterator vIt = vertexList->Begin();

		while(vIt != vertexList->End())
		  {
		    OGRPoint ogrPoint;
                    ogrPoint.setX(vIt.Value()[0] * spacing[0] + origin[0]);
                    ogrPoint.setY(vIt.Value()[1] * spacing[1] + origin[1]);
		    if(DataNodeType::Dimension>2)
		      {
                        ogrPoint.setZ(vIt.Value()[2] * spacing[2] + origin[2]);
		      }

		    ogrExternalRing->addPoint(&ogrPoint);
		    ++vIt;
		  }
		ogrPolygon->addRing(ogrExternalRing);
		delete ogrExternalRing;

		// Retrieving internal rings as well
		for(typename PolygonListType::Iterator pIt = it.Get()->GetPolygonInteriorRings()->Begin();
		    pIt!=it.Get()->GetPolygonInteriorRings()->End();++pIt)
		  {
		    OGRLinearRing * ogrInternalRing = new OGRLinearRing();
		    vertexList = pIt.Get()->GetVertexList();
		    vIt = vertexList->Begin();

		    while(vIt != vertexList->End())
		      {
			OGRPoint ogrPoint;
                        ogrPoint.setX(vIt.Value()[0] * spacing[0] + origin[0]);
                        ogrPoint.setY(vIt.Value()[1] * spacing[1] + origin[1]);
			if(DataNodeType::Dimension>2)
			  {
                            ogrPoint.setZ(vIt.Value()[2] * spacing[2] + origin[2]);
			  }
			ogrInternalRing->addPoint(&ogrPoint);
			++vIt;
		      }
		    ogrPolygon->addRing(ogrInternalRing);
		    delete ogrInternalRing;
		  }
		if(ogrCollection == NULL)
		  {
		    ogrFeatures.back()->GetDefnRef()->SetGeomType(wkbPolygon);
		    ogrFeatures.back()->SetGeometry(ogrPolygon);
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
		if(ogrCollection != NULL || ogrFeatures.empty())
		  {
		    itkExceptionMacro(<<"Problem while creating multipoint.");
		  }
		ogrCollection = new OGRMultiPoint();
		ogrFeatures.back()->GetDefnRef()->SetGeomType(wkbMultiPoint);
		break;
	      }
	    case FEATURE_MULTILINE:
	      {
		if(ogrCollection != NULL || ogrFeatures.empty())
		  {
		    itkExceptionMacro(<<"Problem while creating multiline.");
		  }
		ogrCollection = new OGRMultiLineString();
		ogrFeatures.back()->GetDefnRef()->SetGeomType(wkbMultiLineString);
		break;
	      }
	    case FEATURE_MULTIPOLYGON:
	      {
		if(ogrCollection != NULL || ogrFeatures.empty())
		  {
		    itkExceptionMacro(<<"Problem while creating multipolygon.");
		  }
		ogrCollection = new OGRMultiPolygon();
		ogrFeatures.back()->GetDefnRef()->SetGeomType(wkbMultiPolygon);
		break;
	      }
	    case FEATURE_COLLECTION:
	      {
		if(ogrCollection != NULL || ogrFeatures.empty())
		  {
		    itkExceptionMacro(<<"Problem while creating collection.");
		  }
		ogrCollection = new OGRMultiPoint();
		ogrFeatures.back()->GetDefnRef()->SetGeomType(wkbGeometryCollection);
		break;
	      }
	  }
	++it;
      }
  if(ogrCurrentLayer!=NULL && ogrCollection != NULL && !ogrFeatures.empty())
  {
    ogrFeatures.back()->SetGeometry(ogrCollection);
    delete ogrCollection;
    ogrCollection = NULL;
  }

  if(ogrCurrentLayer!=NULL && ogrFeatures.size()>0)
      {
	std::vector<OGRFeature*>::iterator fIt = ogrFeatures.begin();

	while(fIt!=ogrFeatures.end())
	  {
	    if(ogrCurrentLayer->CreateFeature(*fIt) != OGRERR_NONE)
	      {
		itkExceptionMacro(<<"Failed to create ogr feature in file "<<this->m_FileName);
	      }
	    OGRFeature::DestroyFeature(*fIt);
	    ++fIt;
	  }
      }
    ogrFeatures.clear();

    otbMsgDevMacro( <<" SHPVectorDataIO::Write()  ");
  }

  } // end namespace otb

#endif
