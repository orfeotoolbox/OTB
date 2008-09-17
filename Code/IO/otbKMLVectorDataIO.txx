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
#ifndef __otbKMLVectorDataIO_txx
#define __otbKMLVectorDataIO_txx

#include "otbKMLVectorDataIO.h"

//#include <itksys/SystemTools.hxx>
#include "itkExceptionObject.h"
//#include "itkByteSwapper.h"
#include "otbMacro.h"
#include "otbSystem.h"
#include "otbDataNode.h"
//#include "itkPreOrderTreeIterator.h"
#include "kml/base/file.h"

namespace otb
{
  template<class TData>
  KMLVectorDataIO<TData>
  ::KMLVectorDataIO()
  {
  }
  template<class TData>
  KMLVectorDataIO<TData>::~KMLVectorDataIO()
  {
  }
  template<class TData>
  bool 
  KMLVectorDataIO<TData>::CanReadFile( const char* filename )
  {
        std::string lFileName(filename);
        if( System::IsADirName(lFileName) == true )
        {
                return false;
        }
        if( System::SetToLower(System::GetExtension(lFileName)) != "kml" )
        {
                return false;
        }
        if( kmlbase::File::Exists(lFileName) == false )
        {
                return false;
        }
        
        std::string kml;
        return (kmlbase::File::ReadFileToString(lFileName, &kml) );
  }


  // Used to print information about this object
  template<class TData>
  void 
  KMLVectorDataIO<TData>::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

  // Read vector data
  template<class TData>
  void 
  KMLVectorDataIO<TData>
  ::Read(VectorDataPointerType data)
  {
        std::string kml;
        bool r = kmlbase::File::ReadFileToString(this->m_FileName, &kml);
        if( r == false )
        {
	        itkExceptionMacro(<<"Failed to open KML data file "<<this->m_FileName);
        }

    otbMsgDebugMacro( <<"Driver to read: KML");
    otbMsgDebugMacro( <<"Reading  file: "<< this->m_FileName);

#if 0

    // Retrieving root node
    DataTreePointerType tree = data->GetDataTree();
    DataNodePointerType root = tree->GetRoot()->Get();



	/** Create the document node */
	DataNodePointerType document = DataNodeType::New();
	document->SetNodeType(DOCUMENT);
	document->SetNodeId(dfn->GetName());

//	    document->SetField(field->GetNameRef(),OGRFieldDefn::GetFieldTypeName(field->GetType()));
	    // std::cout<<"Document "<<document->GetNodeId()<<": Adding field "<<field->GetNameRef()<<" "<<OGRFieldDefn::GetFieldTypeName(field->GetType())<<std::endl;

	/** Adding the layer to the data tree */
//	tree->Add(document,root);

	/** Temporary pointer to store the feature */
	    DataNodePointerType folder = DataNodeType::New();
	    folder->SetNodeType(FOLDER);
	    folder->SetNodeId(feature->GetDefnRef()->GetName());

folder->SetField(field->GetNameRef(),feature->GetFieldAsString(fieldIndex));
		//  std::cout<<"Folder "<<folder->GetNodeId()<<": Adding field "<<field->GetNameRef()<<" "<<feature->GetFieldAsString(fieldIndex)<<std::endl;
	      }


	    tree->Add(folder,document);
#endif
  }


/*
  template<class TData>
  typename KMLVectorDataIO<TData>
  ::DataNodePointerType
  KMLVectorDataIO<TData>
  ::ConvertGeometryToPointNode(const OGRGeometry * ogrGeometry)
  {
    OGRPoint * ogrPoint = (OGRPoint *) ogrGeometry; 
  
    if(ogrPoint == NULL)
      {
	itkGenericExceptionMacro(<<"Failed to convert OGRGeometry to OGRPoint");
      }
  
    PointType otbPoint;
    otbPoint.Fill(0);
    otbPoint[0] = static_cast<typename DataNodeType::PrecisionType>(ogrPoint->getX());
    otbPoint[1] = static_cast<typename DataNodeType::PrecisionType>(ogrPoint->getY());

    if(DataNodeType::Dimension > 2)
      {
	otbPoint[2]=static_cast<typename DataNodeType::PrecisionType>(ogrPoint->getZ());
      }
  
    DataNodePointerType node = DataNodeType::New();
    node->SetPoint(otbPoint);
    return node;
  }

  template<class TData>
  typename KMLVectorDataIO<TData>
  ::DataNodePointerType
  KMLVectorDataIO<TData>
  ::ConvertGeometryToLineNode(const OGRGeometry * ogrGeometry)
  {
    OGRLineString * ogrLine = (OGRLineString *)ogrGeometry;

    if(ogrLine == NULL)
      {
	itkGenericExceptionMacro(<<"Failed to convert OGRGeometry to OGRLine");
      }


    LinePointerType line = LineType::New();
  
    OGRPoint * ogrTmpPoint = new OGRPoint();
  
    for(int pIndex = 0;pIndex<ogrLine->getNumPoints();++pIndex)
      {
      
	ogrLine->getPoint(pIndex,ogrTmpPoint);
      
	typename LineType::VertexType vertex;
      
	vertex[0] = ogrTmpPoint->getX();
	vertex[1] = ogrTmpPoint->getY();

	if(DataNodeType::Dimension > 2)
	  {
	    vertex[2]= ogrTmpPoint->getZ();
	  }
      
	line->AddVertex(vertex);
      }
    delete ogrTmpPoint;

    DataNodePointerType node = DataNodeType::New();
    node->SetLine(line);

    return node;
  }

  template<class TData>
  typename KMLVectorDataIO<TData>
  ::DataNodePointerType
  KMLVectorDataIO<TData>
  ::ConvertGeometryToPolygonNode(const OGRGeometry * ogrGeometry)
  {
    OGRPolygon * ogrPolygon = (OGRPolygon *)ogrGeometry;
  
    if(ogrPolygon == NULL)
      {
	itkGenericExceptionMacro(<<"Failed to convert OGRGeometry to OGRPolygon");
      }
  
    OGRPoint * ogrTmpPoint = new OGRPoint();
  
    OGRLinearRing *  ogrRing = ogrPolygon->getExteriorRing();

    PolygonPointerType extRing = PolygonType::New();  
      
    for(int pIndex = 0;pIndex<ogrRing->getNumPoints();++pIndex)
      {
	ogrRing->getPoint(pIndex,ogrTmpPoint);
	typename PolygonType::VertexType vertex;
	vertex[0] = ogrTmpPoint->getX();
	vertex[1] = ogrTmpPoint->getY();

	if(DataNodeType::Dimension > 2)
	  {
	    vertex[2]= ogrTmpPoint->getZ();
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

	    vertex[0] = ogrTmpPoint->getX();
	    vertex[1] = ogrTmpPoint->getY();
	    if(DataNodeType::Dimension > 2)
	      {
		vertex[2]= ogrTmpPoint->getZ();
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
*/

  template<class TData>
  bool KMLVectorDataIO<TData>::CanWriteFile( const char* filename )
  {
        std::string lFileName(filename);
        if( System::IsADirName(lFileName) == true )
        {
                return false;
        }
        if( System::SetToLower(System::GetExtension(lFileName)) != "kml" )
        {
                return false;
        }
        return true;
  }

  template<class TData>
  void KMLVectorDataIO<TData>::Write(const VectorDataConstPointerType data)
  {

/*
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
    // m_DataSource = OGRSFDriverRegistrar::Open(this->m_FileName.c_str(), TRUE);
    m_DataSource = ogrDriver->CreateDataSource(this->m_FileName.c_str(),NULL);
  
  
    // check the created data source
    if(m_DataSource == NULL)
      {
	itkExceptionMacro(<<"Failed to create OGR data source for file "<<this->m_FileName<<". Since OGR can not overwrite existing file, be sure that this file does not already exist");
      }

    // Retrieving root node
    DataTreeConstPointerType tree = data->GetDataTree();
    DataNodePointerType root = tree->GetRoot()->Get();
  
    typedef itk::PreOrderTreeIterator<DataTreeType> TreeIteratorType;

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
	      ogrCurrentLayer = m_DataSource->CreateLayer(it.Get()->GetNodeId(),NULL,wkbUnknown,NULL);
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
		ogrPoint.setX(it.Get()->GetPoint()[0]);
		ogrPoint.setY(it.Get()->GetPoint()[1]);

		if(DataNodeType::Dimension>2)
		  {
		    ogrPoint.setZ(it.Get()->GetPoint()[2]);
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
		    ogrPoint.setX(vIt.Value()[0]);
		    ogrPoint.setY(vIt.Value()[1]);
		    if(DataNodeType::Dimension>2)
		      {
			ogrPoint.setZ(vIt.Value()[2]);
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
		    ogrPoint.setX(vIt.Value()[0]);
		    ogrPoint.setY(vIt.Value()[1]);
		    if(DataNodeType::Dimension>2)
		      {
			ogrPoint.setZ(vIt.Value()[2]);
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
			ogrPoint.setX(vIt.Value()[0]);
			ogrPoint.setY(vIt.Value()[1]);
			if(DataNodeType::Dimension>2)
			  {
			    ogrPoint.setZ(vIt.Value()[2]);
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
    
    otbMsgDevMacro( <<" KMLVectorDataIO::Write()  ");
*/

  }
 
  } // end namespace otb

#endif
