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
		const char * inputFilename  = argv[1];
    const char * outputFilename = argv[2];
		
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
//===========================================================================
		OGRRegisterAll();

    OGRDataSource       *poDS;

		otbGenericMsgTestingMacro(<< "READ FIRST TIME");
		
    poDS = OGRSFDriverRegistrar::Open( inputFilename, FALSE );
    if( poDS == NULL )
    {
        otbGenericMsgTestingMacro(<< "Open failed.\n");
        return EXIT_FAILURE;
    }

    unsigned int nbOfLayers = poDS->GetLayerCount();
		
		otbGenericMsgTestingMacro(<< "File Name " << poDS->GetName());

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
					//OGRFeature::DestroyFeature( poFeature );
				}
				
				
		}
		
		

//===========================================================================
	
	
	// Write 
		otbGenericMsgTestingMacro(<< "WRITE");

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
    if( poDSW == NULL )
    {
        printf( "Creation of output file failed.\n" );
        exit( 1 );
    }

		std::string name(poDSW->GetName());
		
		for (unsigned int i=0; i<nbOfLayers; i++)
		{
				otbGenericMsgTestingMacro(<< "Layer number " << i+1);
				OGRLayer  *poLayer2 = poDS->GetLayer(i);		
				poLayer2->ResetReading();
	
	
				OGRLayer* poWLayer = poDSW->CreateLayer("foo2");
				
  
    		if( poLayer2 == NULL )
    		{
        	printf( "Layer creation failed.\n" );
        	exit( 1 );
    		}
				
/*	    	OGRFieldDefn oField( "Name", OFTString );

	    	if( poWLayer->CreateField( &oField ) != OGRERR_NONE )
    		{
        	printf( "Creating Name field failed.\n" );
        	exit( 1 );
    		}


    		oField.SetWidth(32);*/
			
				OGRFeature* poFeature2;			
				while( (poFeature2 = poLayer2->GetNextFeature()) != NULL )
    		{
					otbGenericMsgTestingMacro(<< "Feature! ");
					OGRFeature* poWFeature;
					
					unsigned int nbFields = poFeature2->GetFieldCount();
					for (unsigned int i=0; i<nbFields; i++)
					{
						OGRFieldDefn* oField = poFeature2->GetFieldDefnRef(i);
						
						std::cout << "Field : " << poFeature2->GetRawFieldRef(i)->String << std::endl;
	    			if( poWLayer->CreateField( oField ) != OGRERR_NONE )
    				{
        			printf( "Creating Name field failed.\n" );
        			exit( 1 );
    				}
	
		      	poWFeature = OGRFeature::CreateFeature( poLayer2->GetLayerDefn() );

/*						if( oField->GetType() == OFTInteger )
						{
                poWFeature->SetField(oField->GetNameRef(), poFeature->GetFieldAsInteger(i));
						}
            else if( oField->GetType() == OFTReal )
						{
                poWFeature->SetField(oField->GetNameRef(),poFeature->GetFieldAsDouble(i));
						}
            else if( oField->GetType() == OFTString )
						{
                poWFeature->SetField(oField->GetNameRef(),poFeature->GetFieldAsString(i));
						}
            else
						{
                poWFeature->SetField(oField->GetNameRef(),poFeature->GetFieldAsString(i));
						}*/
						
						poWFeature->SetField(i,poFeature2->GetRawFieldRef(i));
						std::cout << "FieldW : " << poWFeature->GetRawFieldRef(i)->String << std::endl;
						

					}

					poWFeature->SetGeometry(poFeature2->GetGeometryRef());
	
	    		if( poWLayer->CreateFeature( poWFeature ) != OGRERR_NONE )
        	{
           	printf( "Failed to create feature in shapefile.\n" );
           	exit( 1 );
        	}
					
					OGRFeature::DestroyFeature(poWFeature);

				}		
		}
		
		OGRDataSource::DestroyDataSource( poDSW );


// Libération mémoire structure lecture
		for (unsigned int i=0; i<nbOfLayers; i++)
		{
				OGRLayer  *poLayer = poDS->GetLayer(i);				
				
				OGRFeature* poFeature;
				while( (poFeature = poLayer->GetNextFeature()) != NULL )
    		{
					OGRFeature::DestroyFeature(poFeature);
				}
		}

		OGRDataSource::DestroyDataSource( poDS );



//===========================================================================


    OGRDataSource *poDS2;

		otbGenericMsgTestingMacro(<< "READ SECOND TIME");
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
				OGRLayer  *poLayer3 = poDS2->GetLayer(i);				
				
				poLayer3->ResetReading();
				
				OGRFeature *poFeature3;
				
				int nb=0;
				while( (poFeature3 = poLayer3->GetNextFeature()) != NULL )
    		{
					otbGenericMsgTestingMacro(<< "Feature number " << ++nb);
					
					OGRFeatureDefn *poFDefn = poLayer3->GetLayerDefn();
					otbGenericMsgTestingMacro(<< "Name : " << poFDefn->GetName());
					otbGenericMsgTestingMacro(<< "NbOfFields : " << poFDefn->GetFieldCount());
					
				
//					std::cout << "Field : " << poFeature3->GetRawFieldRef(i)->String << std::endl;

		      for(int iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
					{
					  OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
						
						if( poFieldDefn->GetType() == OFTInteger )
						{
                otbGenericMsgTestingMacro(<<  "  OFTInteger " << poFeature3->GetFieldAsInteger(iField ));
						}
            else if( poFieldDefn->GetType() == OFTReal )
						{
                otbGenericMsgTestingMacro(<<  "  OFTReal " << poFeature3->GetFieldAsDouble(iField));
						}
            else if( poFieldDefn->GetType() == OFTString )
						{
                otbGenericMsgTestingMacro(<<  "  OFTString " << poFeature3->GetFieldAsString(iField));
						}
            else
						{
                otbGenericMsgDebugMacro(<<  "  OTHER " << poFeature3->GetFieldAsString(iField));
						}
 
					}
					
					OGRGeometry *poGeometry;

        	poGeometry = poFeature3->GetGeometryRef();
					
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
					OGRFeature::DestroyFeature( poFeature3 );
				}
				
				
		}

		OGRDataSource::DestroyDataSource( poDS2 );
//===========================================================================
	
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
