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

namespace otb
{
  template<class TData>
  SHPVectorDataIO<TData>
  ::SHPVectorDataIO()
  {

    //   // Byte order specification
    //   if ( itk::ByteSwapper<char>::SystemIsLittleEndian() == true)
    //   {
    //         this->m_ByteOrder = Superclass::LittleEndian;
    //   }
    //   else
    //   {
    //         this->m_ByteOrder = Superclass::BigEndian;
    //   }

    //   m_FileByteOrder = Superclass::BigEndian;

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
    //    std::string lFileName(filename);
    //         if( System::IsADirName(lFileName) == true )
    //         {
    //                 return false;
    //         }
    //         if( m_File.is_open() )
    //         {
    //                 m_File.close();
    //         }


    OGRDataSource * poDS = OGRSFDriverRegistrar::Open(filename, FALSE);
    if(poDS == NULL)
      {
	return false;
      }
	
    OGRDataSource::DestroyDataSource(poDS);
    return true;

    // Is this necessary ?
	
    //         std::fstream header_file;
    //         header_file.open( filename,  std::ios::in | std::ios::binary );
    //         if( header_file.fail() )
    //         {
    //                 otbMsgDevMacro(<<"SHPVectorDataIO::CanReadFile() failed header open ! " );
    //                 return false;
    //         }

    //         //Read header informations
    //         bool lResult = InternalReadHeaderInformation(header_file,false);
    //         header_file.close();
    //         return (lResult);
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
    //   OGRDataSource::DestroyDataSource(m_DataSource);
  }


  template<class TData>
  typename SHPVectorDataIO<TData>
  ::DataNodePointerType
  SHPVectorDataIO<TData>
  ::ConvertGeometryToPointNode(const OGRGeometry * ogrGeometry)
  {
    OGRPoint * ogrPoint = (OGRPoint *) ogrGeometry; 
  
    if(ogrPoint == NULL)
      {
	itkGenericExceptionMacro(<<"Failed to convert OGRGeometry to OGRPoint");
      }
  
    PointType otbPoint;
    otbPoint.Fill(0);
    otbPoint[0] = ogrPoint->getX();
    otbPoint[1] = ogrPoint->getY();

    if(DataNodeType::Dimension > 2)
      {
	otbPoint[2]=ogrPoint->getZ();
      }
  
    DataNodePointerType node = DataNodeType::New();
    node->SetPoint(otbPoint);
    return node;
  }

  template<class TData>
  typename SHPVectorDataIO<TData>
  ::DataNodePointerType
  SHPVectorDataIO<TData>
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
  typename SHPVectorDataIO<TData>
  ::DataNodePointerType
  SHPVectorDataIO<TData>
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


  /** Is this necessary ? */


  // template<class TData>
  // void 
  // SHPVectorDataIO<TData>
  // ::ReadVectorDataInformation()
  // {
  //         if( m_File.is_open() )
  //         {
  //                 m_File.close();
  //         }

  //         m_File.open( this->m_FileName.c_str(),  std::ios::in | std::ios::binary );
  //         if( m_File.fail() )
  //         {
  //                 itkExceptionMacro(<<"SHPVectorDataIO::ReadVectorDataInformation() failed header open ! " );
  //         }

  //         //Read header informations
  //         InternalReadHeaderInformation(m_File,true);
	
  // otbMsgDebugMacro( <<"Driver to read: SHP");
  // otbMsgDebugMacro( <<"         Read  file         : "<< this->m_FileName);
  // }

  // template<class TData>
  // bool SHPVectorDataIO<TData>::InternalReadHeaderInformation(std::fstream & file, const bool reportError)
  // {
  //   return true;
  // }

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
    // m_DataSource = OGRSFDriverRegistrar::Open(this->m_FileName.c_str(), TRUE);
    m_DataSource = ogrDriver->CreateDataSource(this->m_FileName.c_str(),NULL);
  
  
    // check the created data source
    if(m_DataSource == NULL)
      {
	itkExceptionMacro(<<"Failed to create OGR data source for file "<<this->m_FileName);
      }

    // Retrieving root node
    DataTreeConstPointerType tree = data->GetDataTree();
    DataNodePointerType root = tree->GetRoot()->Get();
  
    typedef itk::PreOrderTreeIterator<DataTreeType> TreeIteratorType;

    OGRLayer * ogrCurrentLayer = NULL;
    std::vector<OGRFeature *> ogrFeatures;
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
		  if(ogrCurrentLayer!=NULL)
		    {		     
		      ogrFeatures.push_back(OGRFeature::CreateFeature(ogrCurrentLayer->GetLayerDefn()));
		      ogrFeatures.back()->SetField("Name",it.Get()->GetNodeId());  
		    }
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
		ogrFeatures.back()->GetDefnRef()->SetGeomType(wkbPoint);
		ogrFeatures.back()->SetGeometry(&ogrPoint);

 		break;
	      }
	    case FEATURE_LINE:
 	      {
		OGRLineString ogrLine;
		VertexListConstPointerType vertexList = it.Get()->GetLine()->GetVertexList();
		ogrLine.setNumPoints(vertexList->Size());
	    
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
		ogrFeatures.back()->GetDefnRef()->SetGeomType(wkbLineString);
		ogrFeatures.back()->SetGeometry(&ogrLine);
		break;
	      }
	    case FEATURE_POLYGON:
	      {
		break;
	      }
	    case FEATURE_MULTIPOINT:
	      {
		break;
	      }
	    case FEATURE_MULTILINE:
	      {
		break;
	      }
	    case FEATURE_MULTIPOLYGON:
	      {
		break;
	      }
	    case FEATURE_COLLECTION:
	      {
		break;
	      }
	  }
	++it;
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

    /** Is this necessary ? */

    // template<class TData>
    // void SHPVectorDataIO<TData>::WriteVectorDataInformation()
    // {

    //         if ( this->m_FileName == "" )
    //         {
    //                 itkExceptionMacro(<<"A FileName must be specified.");
    //         }
    //         if( CanWriteFile(this->m_FileName.c_str()) == false)
    //         {
    //                 itkExceptionMacro(<< "The file "<<this->m_FileName.c_str()<<" is not defined as a SHP file");
    //         }
    //         // Close file from any previous vector data
    //         if ( m_File.is_open() )
    //         {
    //                 m_File.close();
    //         }
  
    //         // Open the new file for writing
    //         // Actually open the file
    //         m_File.open(this->m_FileName.c_str(),  std::ios::out | std::ios::trunc | std::ios::binary );
    //         if( m_File.fail() )
    //         {
    //                 itkExceptionMacro(<< "Cannot write requested file "<<this->m_FileName.c_str()<<".");
    //         } 


    // otbMsgDebugMacro( <<"Driver to write: SHP");
    // otbMsgDebugMacro( <<"         Write file         : "<< this->m_FileName);

    // }

  
  } // end namespace otb

#endif
