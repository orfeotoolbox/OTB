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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "ogrsf_frmts.h"
#include "otbMacro.h"
#include "itkObject.h"
//#include "shapeDir/ogrshape.h"
#include <iostream>

//#include "otbShapeFileReader.h"
//#include "otbShapeFileWriter.h"

int otbOGRDriverTest(int argc, char* argv[])
{
  try
  {
    // Verify the number of parameters in the command line
//    const char * outputFilename = argv[2];
		const char * inputFilename  = argv[1];
    const char * outputFilename  = "point_out";
//    const char * outputFilename = "point_out2";
		
/*	// Read SHP
		OGRRegisterAll();
		
		SHPHandle handle = SHPOpen(inputFilename,"rb");
		
		
		
		OGRGeometry* poGeometry;

		poGeometry = SHPReadOGRObject(handle,0);
		
		OGRwkbGeometryType geom =	poGeometry->getGeometryType();
					
		otbGenericMsgTestingMacro(<< "Geometry : " << poGeometry->getGeometryName());
				
		switch(geom)
		{
			case wkbPolygon :
			{
					otbGenericMsgTestingMacro(<< "Polygon");
					OGRPolygon* polygon = dynamic_cast<OGRPolygon*>(poGeometry);
							
					otbGenericMsgTestingMacro(<< "Holes number : " <<	polygon->getNumInteriorRings());
					OGRLinearRing* ring = polygon->getExteriorRing();
								
					unsigned int nbPoints = ring->getNumPoints();
					otbGenericMsgTestingMacro(<< "Points number : " <<	nbPoints);
		
					OGRRawPoint* pointsList = new OGRRawPoint[ring->getNumPoints()];
					ring->getPoints(pointsList);
								
					for (unsigned int i=0; i<nbPoints; i++)
					{
							otbGenericMsgTestingMacro(<< "(" << pointsList[i].x << "," << pointsList[i].y << ") ");
					}
								
					otbGenericMsgTestingMacro(<< std::endl);								
		
					delete[] pointsList;

					break;
			}
						default : otbGenericMsgTestingMacro(<< "Other geometry");
		}
		
		SHPClose(handle);
		
		
		SHPHandle handleW = SHPCreate(outputFilename, 0);
					
		SHPWriteOGRObject(handleW, 0, poGeometry);
		
		SHPClose(handleW);		*/

		OGRRegisterAll();

    OGRDataSource       *poDS, *poDS2;

    poDS = OGRSFDriverRegistrar::Open( inputFilename, FALSE );
    if( poDS == NULL )
    {
        otbGenericMsgTestingMacro(<< "Open failed.\n");
        return EXIT_FAILURE;
    }

    unsigned int nbOfLayers = poDS->GetLayerCount();

		otbGenericMsgTestingMacro(<< "Number Of Layers : " << poDS->GetLayerCount());

		for (unsigned int i=0; i<nbOfLayers; i++)
		{
				otbGenericMsgTestingMacro(<< "Layer number " << i+1);
				OGRLayer  *poLayer = poDS->GetLayer(i);				
				
				poLayer->ResetReading();
				
				OGRFeature *poFeature;
				
				int nb=0;
				while( (poFeature = poLayer->GetNextFeature()) != NULL )
    		{
					otbGenericMsgTestingMacro(<< "Feature number " << ++nb);
					
					OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
					otbGenericMsgTestingMacro(<< "Name : " << poFDefn->GetName());
					otbGenericMsgTestingMacro(<< "NbOfFields : " << poFDefn->GetFieldCount());
					
					int iField;

	        for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
					{
					  OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
						
						if( poFieldDefn->GetType() == OFTInteger )
						{
                otbGenericMsgTestingMacro(<<  "  OFTInteger " << poFeature->GetFieldAsInteger(iField ));
						}
            else if( poFieldDefn->GetType() == OFTReal )
						{
                otbGenericMsgTestingMacro(<<  "  OFTReal " << poFeature->GetFieldAsDouble(iField));
						}
            else if( poFieldDefn->GetType() == OFTString )
						{
                otbGenericMsgTestingMacro(<<  "  OFTString " << poFeature->GetFieldAsString(iField));
						}
            else
						{
                otbGenericMsgDebugMacro(<<  "  OTHER " << poFeature->GetFieldAsString(iField));
						}
 
					}
					
					OGRGeometry *poGeometry;

        	poGeometry = poFeature->GetGeometryRef();
					
					OGRwkbGeometryType geom =	poGeometry->getGeometryType();
					
					otbGenericMsgTestingMacro(<< "Geometry : " << poGeometry->getGeometryName());
					
					switch(geom)
					{
						case wkbPolygon :
							{
								otbGenericMsgTestingMacro(<< "Polygon");
								OGRPolygon* polygon = dynamic_cast<OGRPolygon*>(poGeometry);
								
								otbGenericMsgTestingMacro(<< "Holes number : " <<	polygon->getNumInteriorRings());
								OGRLinearRing* ring = polygon->getExteriorRing();
								
								unsigned int nbPoints = ring->getNumPoints();
								otbGenericMsgTestingMacro(<< "Points number : " <<	nbPoints);
		
								OGRRawPoint* pointsList = new OGRRawPoint[ring->getNumPoints()];
								ring->getPoints(pointsList);
								
								for (unsigned int i=0; i<nbPoints; i++)
								{
									otbGenericMsgTestingMacro(<< "(" << pointsList[i].x << "," << pointsList[i].y << ") ");
								}
								
								otbGenericMsgTestingMacro(<< std::endl);								
								

								delete[] pointsList;

								break;
							}
						default : otbGenericMsgTestingMacro(<< "Other geometry");
					}
					
					otbGenericMsgTestingMacro(<< "==========================");
					OGRFeature::DestroyFeature( poFeature );
				}
				
				
		}

	// Write 

    const char *pszDriverName = "ESRI Shapefile";
    OGRSFDriver *poDriver;
   	
		poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
                pszDriverName );
    if( poDriver == NULL )
    {
        printf( "%s driver not available.\n", pszDriverName );
        exit( 1 );
    }

    OGRDataSource *poDSW;

	  poDSW = poDriver->CreateDataSource( outputFilename, NULL );
		std::string name(poDSW->GetName());

		for (unsigned int i=0; i<nbOfLayers; i++)
		{
				otbGenericMsgTestingMacro(<< "Layer number " << i+1);
				OGRLayer  *poLayer = poDS->GetLayer(i);				
							
				OGRLayer* poWLayer = poDSW->CreateLayer("Test");
				
				OGRFeature* poFeature;
				int nb=0;
				while( (poFeature = poLayer->GetNextFeature()) != NULL )
    		{
					poWLayer->CreateFeature(poFeature);
					nb++;
				}			
		}
		
		OGRDataSource::DestroyDataSource( poDS );
		OGRDataSource::DestroyDataSource( poDSW );

		// Reread
		poDS2 = OGRSFDriverRegistrar::Open( outputFilename, FALSE );
    if( poDS2 == NULL )
    {
        otbGenericMsgTestingMacro(<< "Open failed.\n");
        return EXIT_FAILURE;
    }

    nbOfLayers = poDS2->GetLayerCount();

		otbGenericMsgTestingMacro(<< "Number Of Layers : " << poDS2->GetLayerCount());

		for (unsigned int i=0; i<nbOfLayers; i++)
		{
				otbGenericMsgTestingMacro(<< "Layer number " << i+1);
				OGRLayer  *poLayer = poDS2->GetLayer(i);				
				
				poLayer->ResetReading();
				
				OGRFeature *poFeature;
				
				int nb=0;
				while( (poFeature = poLayer->GetNextFeature()) != NULL )
    		{
					otbGenericMsgTestingMacro(<< "Feature number " << ++nb);
					
					OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
					otbGenericMsgTestingMacro(<< "Name : " << poFDefn->GetName());
					otbGenericMsgTestingMacro(<< "NbOfFields : " << poFDefn->GetFieldCount());
					
					int iField;

	        for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
					{
					  OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
						
						if( poFieldDefn->GetType() == OFTInteger )
						{
                otbGenericMsgTestingMacro(<<  "  OFTInteger " << poFeature->GetFieldAsInteger(iField ));
						}
            else if( poFieldDefn->GetType() == OFTReal )
						{
                otbGenericMsgTestingMacro(<<  "  OFTReal " << poFeature->GetFieldAsDouble(iField));
						}
            else if( poFieldDefn->GetType() == OFTString )
						{
                otbGenericMsgTestingMacro(<<  "  OFTString " << poFeature->GetFieldAsString(iField));
						}
            else
						{
                otbGenericMsgDebugMacro(<<  "  OTHER " << poFeature->GetFieldAsString(iField));
						}
 
					}
					
					OGRGeometry *poGeometry;

        	poGeometry = poFeature->GetGeometryRef();
					
					OGRwkbGeometryType geom =	poGeometry->getGeometryType();
					
					otbGenericMsgTestingMacro(<< "Geometry : " << poGeometry->getGeometryName());
					
					switch(geom)
					{
						case wkbPolygon :
							{
								otbGenericMsgTestingMacro(<< "Polygon");
								OGRPolygon* polygon = dynamic_cast<OGRPolygon*>(poGeometry);
								
								otbGenericMsgTestingMacro(<< "Holes number : " <<	polygon->getNumInteriorRings());
								OGRLinearRing* ring = polygon->getExteriorRing();
								
								unsigned int nbPoints = ring->getNumPoints();
								otbGenericMsgTestingMacro(<< "Points number : " <<	nbPoints);
		
								OGRRawPoint* pointsList = new OGRRawPoint[ring->getNumPoints()];
								ring->getPoints(pointsList);
								
								for (unsigned int i=0; i<nbPoints; i++)
								{
									otbGenericMsgTestingMacro(<< "(" << pointsList[i].x << "," << pointsList[i].y << ") ");
								}
								
								otbGenericMsgTestingMacro(<< std::endl);								
								

								delete[] pointsList;

								break;
							}
						default : otbGenericMsgTestingMacro(<< "Other geometry");
					}
					
					otbGenericMsgTestingMacro(<< "==========================");
					OGRFeature::DestroyFeature( poFeature );
				}
				
				
		}


	
  } 
  catch( itk::ExceptionObject & err ) 
  { 
    std::cerr << "Exception OTB attrappee dans exception ITK !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
  } 
  catch( ... )
  {
    std::cerr << "Exception OTB non attrappee !" << std::endl; 
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
