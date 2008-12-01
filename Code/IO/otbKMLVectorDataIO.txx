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

#include "itkExceptionObject.h"
#include "otbMacro.h"
#include "otbSystem.h"
#include "otbDataNode.h"
#include "itkPreOrderTreeIterator.h"

#include "kml/dom.h"
#include "kml/dom/kml22.h"
#include "kml/base/file.h"


using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::KmlPtr;
using kmldom::KmlFactory;
using kmldom::DocumentPtr;
using kmldom::FolderPtr;
using kmldom::ContainerPtr;
using kmldom::GeometryPtr;
using kmldom::MultiGeometryPtr;
using kmldom::PlacemarkPtr;
using kmldom::CoordinatesPtr;
using kmldom::PointPtr;
using kmldom::LineStringPtr;
using kmldom::LinearRingPtr;
using kmldom::PolygonPtr;
using kmldom::ModelPtr;
using kmldom::OuterBoundaryIsPtr;
using kmldom::InnerBoundaryIsPtr;



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

  // Get the features of the kml file, read into the root
  template<class TData>
      const FeaturePtr 
      KMLVectorDataIO<TData>::GetRootFeature(const ElementPtr& root) 
  {
    const KmlPtr kml = kmldom::AsKml(root);
    if (kml && kml->has_feature()) {
      return kml->get_feature();
    }
    return kmldom::AsFeature(root);
  }
  
  // Print the selected feature
  template<class TData>
      void
      KMLVectorDataIO<TData>::PrintIndented(std::string item, int depth) 
  {
    while (depth--) 
    {
      std::cout << "  ";
    }
    std::cout << item <<std::endl;
  }
  
  template<class TData>
      void 
      KMLVectorDataIO<TData>::WalkFeature(const FeaturePtr& feature, DataNodePointerType father)
  {

    DataNodePointerType node = NULL;
        
    if (feature) 
    {
      if (const ContainerPtr container = kmldom::AsContainer(feature)) 
      {
        WalkContainer(container,father);
      } 
      else if (const PlacemarkPtr placemark = kmldom::AsPlacemark(feature)) 
      {
        WalkGeometry(placemark->get_geometry(),father);
      }
        
        // The Read() method is not exhaustive it is possible to add the read of the "link", 
        // the style (iconStyle, LineStyle...). Then into the containers we also can find the fields :
        // <visibility> <description> <LookAt>... cf. code.google.com/apis/kml/documentation/kmlelementssinmaps.html 
    }
    return;
  }

  template<class TData>
      void
      KMLVectorDataIO<TData>::WalkContainer(const ContainerPtr& container, DataNodePointerType father)
  {
  
    DataNodePointerType node = NULL;
  
    for (size_t i = 0; i < container->get_feature_array_size(); ++i) 
    {
      FeaturePtr feature = container->get_feature_array_at(i);
      switch (feature->Type()) 
      {
        case kmldom::Type_Document:
        {
          DataNodePointerType document = DataNodeType::New();
          document->SetNodeType(DOCUMENT);        
          document->SetNodeId(feature->get_id());
          if (feature->has_name()) {
            document->SetField("name",feature->get_name());
          }
          m_Tree->Add(document,father);
          WalkFeature(feature,document);              
          break;
        } 
        case kmldom::Type_Folder:
        {
          DataNodePointerType folder = DataNodeType::New();
          folder->SetNodeType(FOLDER);
          folder->SetNodeId(feature->get_id());                      
          if (feature->has_name()) {
            folder->SetField("name",feature->get_name());
          }
          m_Tree->Add(folder,father);
          WalkFeature(feature,folder);
          break;
        }
        case kmldom::Type_GroundOverlay:
        {
          WalkFeature(feature,father);
          break;
        }
        case kmldom::Type_NetworkLink:
        {
          WalkFeature(feature,father);
          break;
        }
        case kmldom::Type_PhotoOverlay:
        {
          WalkFeature(feature,father);                      
          break;
        }
        case kmldom::Type_Placemark:
        {
	      // We just ignore placemarks                   
          WalkFeature(feature,father);
          break;
        }
        case kmldom::Type_ScreenOverlay:
        {
          WalkFeature(feature,father);                      
          break;
        }
        default:
          break;
      }

    }
    return;
  }  

  // Walk through a geometry and create a GeometryNode
  template<class TData>  
      void
      KMLVectorDataIO<TData>::WalkGeometry(const GeometryPtr& geometry, DataNodePointerType father) 
  {
  
    // Creation of a node
    DataNodePointerType node = NULL;

    if (!geometry) 
    {
      return;
    }
    // Create a Node with the Geometry type.
    switch(geometry->Type())
    {
      case kmldom::Type_Point:
      {
        node = ConvertGeometryToPointNode(geometry);
        m_Tree->Add(node,father); 
        break;
      }
      case kmldom::Type_LineString:
      {
        node = ConvertGeometryToLineStringNode(geometry);
        m_Tree->Add(node,father);                
        break;
      }
      case kmldom::Type_LinearRing:
      {
        node = ConvertGeometryToLinearRingNode(geometry);
        m_Tree->Add(node,father);                
        break;
      }
      case kmldom::Type_Polygon:
      {
        node = ConvertGeometryToPolygonNode(geometry);
        m_Tree->Add(node,father);
        break;
      }
      case kmldom::Type_MultiGeometry:
      {
	  // this case is treated downer
        break;
      }
      case kmldom::Type_Model:
      {
        break;
      }
      default:  // KML has 6 types of Geometry.
        break;
    }

    // Recurse into <MultiGeometry>.
    if (const MultiGeometryPtr multigeometry = kmldom::AsMultiGeometry(geometry)) 
    {
      DataNodePointerType multi = DataNodeType::New();
      multi->SetNodeType(FEATURE_COLLECTION);
      m_Tree->Add(multi,father);            
      for (size_t i = 0; i < multigeometry->get_geometry_array_size(); ++i)
      {
        WalkGeometry(multigeometry->get_geometry_array_at(i),multi);
      }           
    }
    return;
  }
  
  template<class TData>
      typename KMLVectorDataIO<TData>
  ::DataNodePointerType
      KMLVectorDataIO<TData>
  ::ConvertGeometryToPointNode(const GeometryPtr& geometry)
  {

    if(geometry == NULL)
    {       
      itkGenericExceptionMacro(<<"Failed to convert GeometryPtr to PointNode");
    }
    
    const PointPtr pt = kmldom::AsPoint(geometry);
      
    PointType otbPoint;
    otbPoint.Fill(0);
    otbPoint[0] = static_cast<typename DataNodeType::PrecisionType>(pt->get_coordinates()->get_coordinates_array_at(0).get_longitude());
    otbPoint[1] = static_cast<typename DataNodeType::PrecisionType>(pt->get_coordinates()->get_coordinates_array_at(0).get_latitude());

    if(DataNodeType::Dimension > 2)
    {
      otbPoint[2]=static_cast<typename DataNodeType::PrecisionType>(pt->get_coordinates()->get_coordinates_array_at(0).get_altitude());
    }   

    DataNodePointerType node = DataNodeType::New();
    node->SetPoint(otbPoint);
    return node;
  }

  template<class TData>
    typename KMLVectorDataIO<TData>
  ::DataNodePointerType
      KMLVectorDataIO<TData>
  ::ConvertGeometryToLineStringNode(const GeometryPtr& geometry)
  {

    if(geometry == NULL)
    {
      itkGenericExceptionMacro(<<"Failed to convert GeometryPtr to LineNode");
    }

    const LineStringPtr ls = kmldom::AsLineString(geometry);
    const CoordinatesPtr coords = ls->get_coordinates();
    int array_size = coords->get_coordinates_array_size();

    LinePointerType line = LineType::New();
    for(int i=0;i<array_size;i++)
    {
      typename LineType::VertexType vertex;
      vertex[0] = coords->get_coordinates_array_at(i).get_longitude();
      vertex[1] = coords->get_coordinates_array_at(i).get_latitude();

      if(DataNodeType::Dimension > 2)
      {
        vertex[2]= coords->get_coordinates_array_at(i).get_altitude();
      }
      line->AddVertex(vertex);
    }

    DataNodePointerType node = DataNodeType::New();
    node->SetLine(line);

    return node;
  }
  
  template<class TData>
      typename KMLVectorDataIO<TData>
  ::DataNodePointerType
      KMLVectorDataIO<TData>
  ::ConvertGeometryToLinearRingNode(const GeometryPtr& geometry)
  {

    if(geometry == NULL)
    {
      itkGenericExceptionMacro(<<"Failed to convert GeometryPtr to LineNode");
    }

    const LinearRingPtr lr = kmldom::AsLinearRing(geometry);        
    const CoordinatesPtr coords = lr->get_coordinates();
    int array_size = coords->get_coordinates_array_size();

    LinePointerType line = LineType::New();
    for(int i=0;i<array_size;i++)
    {
      typename LineType::VertexType vertex;
      vertex[0] = coords->get_coordinates_array_at(i).get_longitude();
      vertex[1] = coords->get_coordinates_array_at(i).get_latitude();

      if(DataNodeType::Dimension > 2)
      {
        vertex[2]= coords->get_coordinates_array_at(i).get_altitude();
      }
      line->AddVertex(vertex);
    }

    DataNodePointerType node = DataNodeType::New();
    node->SetLine(line);

    return node;
  }  
  
  template<class TData>
      typename KMLVectorDataIO<TData>
  ::DataNodePointerType
      KMLVectorDataIO<TData>
  ::ConvertGeometryToPolygonNode(const GeometryPtr& geometry)
  {

    if(geometry == NULL)
    {
      itkGenericExceptionMacro(<<"Failed to convert GeometryPtr to LineNode");
    }

    // Get the polygon
    const PolygonPtr polygonKml = kmldom::AsPolygon(geometry);
    PolygonPointerType extRing = PolygonType::New();
    PolygonListPointerType intRings = PolygonListType::New();

    // Read the outerboundaryis of the polygon
    if ( polygonKml->has_outerboundaryis())
    {
      const OuterBoundaryIsPtr outerboundaryis = polygonKml->get_outerboundaryis();
      if(outerboundaryis->has_linearring())
      {
        const LinearRingPtr lr = outerboundaryis->get_linearring();        
        const CoordinatesPtr coords = lr->get_coordinates();
        int array_size = coords->get_coordinates_array_size();
                                

        for(int i=0;i<array_size;i++)
        {
          typename PolygonType::VertexType vertex;
          vertex[0] = coords->get_coordinates_array_at(i).get_longitude();
          vertex[1] = coords->get_coordinates_array_at(i).get_latitude();

          if(DataNodeType::Dimension > 2)
          {
            vertex[2]= coords->get_coordinates_array_at(i).get_altitude();
          }
          extRing->AddVertex(vertex);
        }
      }
    }

    // Read the innerboundaryis of the polygon
    for (unsigned int intRingIndex=0;intRingIndex<polygonKml->get_innerboundaryis_array_size();intRingIndex++)
    {
      const InnerBoundaryIsPtr innerboundaryis = polygonKml->get_innerboundaryis_array_at(intRingIndex);
      if(innerboundaryis->has_linearring())
      {
        const LinearRingPtr lr = innerboundaryis->get_linearring();        
        const CoordinatesPtr coords = lr->get_coordinates();
        int array_size = coords->get_coordinates_array_size();
        PolygonPointerType ring = PolygonType::New();                        
        
        for(int pIndex=0;pIndex<array_size;pIndex++)
        {
          typename PolygonType::VertexType vertex;
          vertex[0] = coords->get_coordinates_array_at(pIndex).get_longitude();
          vertex[1] = coords->get_coordinates_array_at(pIndex).get_latitude();

          if(DataNodeType::Dimension > 2)
          {
            vertex[2]= coords->get_coordinates_array_at(pIndex).get_altitude();
          }
          ring->AddVertex(vertex);
        }
        intRings->PushBack(ring);
      }
    }

    DataNodePointerType node = DataNodeType::New();        
    node->SetPolygonExteriorRing(extRing);
    node->SetPolygonInteriorRings(intRings);

    return node;
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
    bool status = kmlbase::File::ReadFileToString(this->m_FileName, &kml);
    if( status == false )
    {
      itkExceptionMacro(<<"Failed to open KML data file "<<this->m_FileName);
    }

    std::cout<<this->m_FileName<<std::endl;

    otbMsgDebugMacro( <<"Driver to read: KML");
    otbMsgDebugMacro( <<"Reading  file: "<< this->m_FileName);


    // Parse the file.
    std::string errors;
    ElementPtr root = kmldom::Parse(kml, &errors);

    if (!root) 
    {
      itkExceptionMacro(<<"Failed to open KML data file "<<errors);
    }

    const FeaturePtr feature = GetRootFeature(root);
    if (feature) {

      // Retrieving root node
      m_Tree = data->GetDataTree();
      DataNodePointerType rootNode = m_Tree->GetRoot()->Get();

      DataNodePointerType document = DataNodeType::New();
      document->SetNodeType(DOCUMENT);
      m_Tree->Add(document,rootNode);            

      // Walk Feature (get the Placemarks... and walk into them) //ENGLISH ??
      WalkFeature(feature,document);

    } else {
      itkExceptionMacro(<<"No root feature");
    }

  // std::cout<< m_Tree <<std::endl;
  }


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

    //Create the factory
    KmlFactory* factory = KmlFactory::GetFactory();
    if(factory == NULL)
    {
      itkExceptionMacro(<<"Impossible to create the KML Factory to write file "<<this->m_FileName);
    }

    KmlPtr kml = factory->CreateKml();

    // Retrieving root node
    DataTreeConstPointerType tree = data->GetDataTree();
    DataNodePointerType root = tree->GetRoot()->Get();

    typedef itk::PreOrderTreeIterator<DataTreeType> TreeIteratorType;

    TreeIteratorType it(tree);
    it.GoToBegin();

    DocumentPtr currentDocument = NULL;
    FolderPtr currentFolder = NULL;
    MultiGeometryPtr currentMultiGeometry = NULL;    
    PolygonPtr currentPolygon = NULL;
    OuterBoundaryIsPtr outerboundaryis = NULL;
    InnerBoundaryIsPtr innerboundaryis = NULL;    


    // Note that we force a precise structure when we write a kml file.
    // We will necessary have a <document> that will contain a <folder>.
    // And in a folder there automatically will be one or several <placemark>.
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
          DocumentPtr document = factory->CreateDocument();
          if(it.Get()->HasField("name"))
          {
            std::string fieldname = it.Get()->GetField("name");
            document->set_name(fieldname);                
          }
          kml->set_feature(document);
          currentDocument = document;
          break;
        }
        case FOLDER:
        {
          FolderPtr folder = factory->CreateFolder();
          std::string fieldname = it.Get()->GetField("name");
          folder->set_name(fieldname);             
          currentDocument->add_feature(folder);
          currentFolder = folder;
          break;
        }	    
        case FEATURE_POINT:
        {
        // Create <coordinates>
          CoordinatesPtr coordinates = factory->CreateCoordinates();
          if(DataNodeType::Dimension>2)
          {
            coordinates->add_latlngalt(it.Get()->GetPoint()[1],it.Get()->GetPoint()[0],it.Get()->GetPoint()[2]);
          }
          else
          {
            coordinates->add_latlng(it.Get()->GetPoint()[1],it.Get()->GetPoint()[0]);                  
          }

          // Create <Point> and give it <coordinates>.
          PointPtr point = factory->CreatePoint();
          point->set_coordinates(coordinates);

          if (currentMultiGeometry != NULL)
          {
            currentMultiGeometry->add_geometry(point);
          }
          else
          {
    
            PlacemarkPtr placemark = factory->CreatePlacemark();			
            placemark->set_geometry(point);
            if (currentFolder!= NULL)
            {
              currentFolder->add_feature(placemark);
            }
            else
            {
              currentDocument->add_feature(placemark);
            }
    
          }
          break;
        }
        case FEATURE_LINE:
        {

          VertexListConstPointerType vertexList = it.Get()->GetLine()->GetVertexList();

          // Create <coordinates>
          CoordinatesPtr coordinates = factory->CreateCoordinates();                
          LineStringPtr line = factory->CreateLineString();                
        
          typename VertexListType::ConstIterator vIt = vertexList->Begin();

          while(vIt != vertexList->End())
          {

            if(DataNodeType::Dimension>2)
            {
              coordinates->add_latlngalt(vIt.Value()[1],vIt.Value()[0],vIt.Value()[2]);
            }
            else
            {
              coordinates->add_latlng(vIt.Value()[1],vIt.Value()[0]);                  
            }
            line->set_coordinates(coordinates);
            ++vIt;
          }

          if (currentMultiGeometry != NULL)
          {
            currentMultiGeometry->add_geometry(line);
          }                        
          else
          {
            PlacemarkPtr placemark = factory->CreatePlacemark();			
            placemark->set_geometry(line);
            if (currentFolder!= NULL)
            {
              currentFolder->add_feature(placemark);
            }
            else
            {
              currentDocument->add_feature(placemark);
            }
          }
          break;
        }
        case FEATURE_POLYGON:
        {//TODO refine the solution of drawing the polygon 1m above the ground
        // In a polygon we just can find any LinearRings
          LinearRingPtr line = factory->CreateLinearRing();
          PolygonPtr polygon = factory->CreatePolygon();

          polygon->set_extrude(true);
          polygon->set_altitudemode(1);//ALTITUDEMODE_RELATIVETOGROUND

          CoordinatesPtr coordinates = factory->CreateCoordinates();                
          OuterBoundaryIsPtr outerboundaryis = factory->CreateOuterBoundaryIs();
          InnerBoundaryIsPtr innerboundaryis = factory->CreateInnerBoundaryIs();

          VertexListConstPointerType vertexList = it.Get()->GetPolygonExteriorRing()->GetVertexList();

          typename VertexListType::ConstIterator vIt = vertexList->Begin();

          if (vIt == vertexList->End())
          {
            itkExceptionMacro(<<"Polygon is empty");
          }
          
          while(vIt != vertexList->End())
          {
            if(DataNodeType::Dimension>2)
            {
              coordinates->add_latlngalt(vIt.Value()[1],vIt.Value()[0],vIt.Value()[2]+1);//Drawing polygon 1m above ground to avoid z-buffer issues
            }
            else
            {
              coordinates->add_latlngalt(vIt.Value()[1],vIt.Value()[0],1);//Drawing polygon 1m above ground to avoid z-buffer issues                  
            }
        
            line->set_coordinates(coordinates);
            ++vIt;
          }

          //Adding the first point again to close the polygon
          vIt = vertexList->Begin();
          
          if(DataNodeType::Dimension>2)
          {
            coordinates->add_latlngalt(vIt.Value()[1],vIt.Value()[0],vIt.Value()[2]+1);//Drawing polygon 1m above ground to avoid z-buffer issues
          }
          else
          {
            coordinates->add_latlngalt(vIt.Value()[1],vIt.Value()[0],1);//Drawing polygon 1m above ground to avoid z-buffer issues                  
          }
          line->set_coordinates(coordinates);

          outerboundaryis->set_linearring(line);
          polygon->set_outerboundaryis(outerboundaryis);

          // Retrieving internal rings as well
          for(typename PolygonListType::Iterator pIt = it.Get()->GetPolygonInteriorRings()->Begin();
              pIt!=it.Get()->GetPolygonInteriorRings()->End();++pIt)
          {

            vertexList = pIt.Get()->GetVertexList();	    
            vIt = vertexList->Begin();

            while(vIt != vertexList->End())
            {
              if(DataNodeType::Dimension>2)
              {
                coordinates->add_latlngalt(it.Get()->GetPoint()[1],it.Get()->GetPoint()[0],it.Get()->GetPoint()[2]+1);//Drawing polygon 1m above ground to avoid z-buffer issues
              } 
              else
              {
                coordinates->add_latlngalt(it.Get()->GetPoint()[1],it.Get()->GetPoint()[0],1);//Drawing polygon 1m above ground to avoid z-buffer issues                  
              }
        
              line->set_coordinates(coordinates);
              ++vIt;
            }

            //Adding the first point again to close the polygon
            vIt = vertexList->Begin();
            if(DataNodeType::Dimension>2)
            {
              coordinates->add_latlngalt(it.Get()->GetPoint()[1],it.Get()->GetPoint()[0],it.Get()->GetPoint()[2]+1);//Drawing polygon 1m above ground to avoid z-buffer issues
            } 
            else
            {
              coordinates->add_latlngalt(it.Get()->GetPoint()[1],it.Get()->GetPoint()[0],1);//Drawing polygon 1m above ground to avoid z-buffer issues                  
            }
        
            line->set_coordinates(coordinates);
            ++vIt;


            innerboundaryis->set_linearring(line);
            polygon->add_innerboundaryis(innerboundaryis);
            innerboundaryis->clear_linearring();
          }


          if (currentMultiGeometry != NULL)
          {
            currentMultiGeometry->add_geometry(polygon);
          }
          else
          {
            PlacemarkPtr placemark = factory->CreatePlacemark();			
            placemark->set_geometry(polygon);
            if (currentFolder!= NULL)
            {
              currentFolder->add_feature(placemark);
            }
            else
            {
              currentDocument->add_feature(placemark);
            }
          }
          break;
        }
        // MultiGeometry
        case FEATURE_COLLECTION:
        {
          MultiGeometryPtr multi = factory->CreateMultiGeometry();
          currentMultiGeometry = multi;
          PlacemarkPtr placemark = factory->CreatePlacemark();			
          placemark->set_geometry(multi);
          if (currentFolder!= NULL)
          {
            currentFolder->add_feature(placemark);
          }
          else
          {
            currentDocument->add_feature(placemark);
          }
          break;
        }
        default :
          break;

      } // end switch
      ++it;
    } // end while

    // Serialize to XML
    std::string xml = kmldom::SerializePretty(kml);

    // Write it
    kmlbase::File::WriteStringToFile(xml,this->m_FileName);

    // Eventually print it
    //std::cout << xml;

    otbMsgDevMacro( <<" KMLVectorDataIO::Write()  ");

  }
 
} // end namespace otb

#endif


