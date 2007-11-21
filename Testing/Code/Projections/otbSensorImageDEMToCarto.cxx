/*!
 *
 * PURPOSE:
 *
 * Application pour projeter une région d'une image en coordonnées géographiques 
 * en utilisant un Interpolator+regionextractor+Iterator+DEMHandler. 
 * Prise en compte du MNT
 */

#include <iostream>
#include <iterator>
#include <stdlib.h>

#include "otbImageGeometryHandler.h"
#include "otbInverseSensorModel.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImage.h"
#include "otbMacro.h"

#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkExtractImageFilter.h"

#include "otbMapProjections.h"
#include "otbMapProjection.h"


int otbSensorImageDEMToCarto( int argc, char* argv[] )
{
  try 
    {        


   if(argc!=11)
   {
      std::cout << argv[0] <<" <input filename> <output filename> <X origine> <Y origine> <taille_x> <taille_y> <NumberOfstreamDivisions> <srtm directory> " 
                << "<xSpacing> <ySpacing>" << std::endl;

      return EXIT_FAILURE;
   }
typedef itk::Point <double, 2> 		 PointType;
PointType				 outputpoint; 

/*************************************************/
/*            Création des diverses images       */
/*     (outputimage,DEMimage,charoutputimage)    */
/*************************************************/
typedef otb::Image<unsigned char, 2>     CharImageType;
typedef otb::Image<unsigned int, 2>      ImageType;
typedef otb::Image<unsigned int, 2>      InputImageType;
ImageType::Pointer 	  	         outputimage = ImageType::New();
CharImageType::Pointer      		 charoutputimage=CharImageType::New();

ImageType::PixelType			 pixelvalue;
ImageType::IndexType  			 start;
start[0]=0;     
start[1]=0;     

ImageType::SizeType  			 size;
size[0]=atoi(argv[5]);      //Taille en X.
size[1]=atoi(argv[6]);	    //Taille en Y.

ImageType::SpacingType  		 spacing;
spacing[0]=atof(argv[9]);
spacing[1]=atof(argv[10]);

ImageType::PointType			 origin;
origin[0]=strtod(argv[3], NULL);         //longitude de l'origine.
origin[1]=strtod(argv[4], NULL);         //latitude de l'origine.

ImageType::RegionType			 region;

region.SetSize(size);
region.SetIndex(start);

outputimage-> SetOrigin(origin);
outputimage-> SetRegions(region);
outputimage->SetSpacing(spacing);
outputimage-> Allocate();     //Notre image de sortie est allouée.
otbGenericMsgDebugMacro(<< "Output image created!! ");

/******************************/
/*  Création de mon handler   */
/******************************/
	
ossimKeywordlist geom_kwl;
      typedef otb::ImageGeometryHandler  HandlerType;
      HandlerType::Pointer   handler= HandlerType::New();
      otbGenericMsgDebugMacro(<< "Handler created " );
      handler->SetFileName(argv[1]);
      geom_kwl=handler->GetGeometryKeywordlist();

/********************************************************/
/*   Création de notre modèle en fonction de l'image    */
/********************************************************/

      typedef otb::InverseSensorModel<double>  ModelType;
      ModelType::Pointer   model= ModelType::New();
      otbGenericMsgDebugMacro(<< "Model set geometry " );
      model->SetImageGeometry(geom_kwl); //Notre modèle est créé à ce niveau.
         if(!model)
      {
       otbGenericMsgDebugMacro(<< "Unable to create a model");
         return 1;
      }
      otbGenericMsgDebugMacro(<< "InverseSensorModel created " );
      
      ModelType::OutputPointType inputpoint;


/********************************************************/
/*                  Création d'un reader                */
/********************************************************/

typedef otb::ImageFileReader<ImageType>  ReaderType;
ReaderType::Pointer	                 reader=ReaderType::New();
reader->SetFileName(argv[1]);
ImageType::Pointer  			 inputimage= reader->GetOutput();
ImageType::IndexType 			 currentindex;
ImageType::IndexType 			 currentindexbis;
ImageType::IndexType 			 pixelindex;
ImageType::IndexType 			 pixelindexbis;
otbGenericMsgDebugMacro(<< "Reader created " );

/********************************************************/
/*            Création de notre extractor               */
/********************************************************/

typedef itk::ExtractImageFilter<InputImageType,ImageType>   ExtractType;
ExtractType::Pointer			             extract=ExtractType::New();
otbGenericMsgDebugMacro(<< "Region Extractor created " );

/********************************************************/
/*            Création de notre interpolator            */
/********************************************************/

typedef itk::LinearInterpolateImageFunction<ImageType, double>  InterpolatorType;
InterpolatorType::Pointer	interpolator=InterpolatorType::New();
otbGenericMsgDebugMacro(<< "Interpolator created " );

/********************************************************/
/*            Création de nos writer                    */
/********************************************************/

typedef otb::ImageFileWriter<ImageType>      WriterType;
typedef otb::ImageFileWriter<CharImageType>  CharWriterType;

WriterType::Pointer	                     	 extractorwriter=WriterType::New();
CharWriterType::Pointer	                     writer=CharWriterType::New();

extractorwriter->SetFileName("image_temp1.jpeg");
extractorwriter->SetInput(extract->GetOutput());
otbGenericMsgDebugMacro(<< "extractorwriter created" );

/********************************************************/
/*            Création de notre rescaler                */
/********************************************************/

typedef itk::RescaleIntensityImageFilter<ImageType,CharImageType>  RescalerType;
RescalerType::Pointer	                 rescaler=RescalerType::New();
rescaler->SetOutputMinimum(50);
rescaler->SetOutputMaximum(400);
otbGenericMsgDebugMacro(<< "rescaler created" );

/********************************************************/
/*            Création de notre projection              */
/********************************************************/
typedef otb::UtmInverseProjection                      utmProjection;
typedef utmProjection::OutputPointType	        OutputPoint;
typedef utmProjection::InputPointType	        InputPoint;
InputPoint                                      geoPoint;
utmProjection::Pointer   utmprojection= utmProjection::New();
utmprojection->SetZone(31);
utmprojection->SetHemisphere('N');

/*************************************************/   
/*     Création de RegionIteratorwithIndex       */
/*************************************************/

typedef itk::ImageRegionIteratorWithIndex<ImageType>	IteratorType;
otbGenericMsgDebugMacro(<< "Iterator created " );

//Donner une valeur par défaut numberofstreamdivision ou le faire fixer par l'utilisateur.
unsigned int NumberOfStreamDivisions;
if (atoi(argv[7])==0)
{NumberOfStreamDivisions=10;}//ou pourrait le calculer en fonction de la taille de outputimage
else{NumberOfStreamDivisions=atoi(argv[7]);}
otbGenericMsgDebugMacro(<< "NumberOfStreamDivisions =" << NumberOfStreamDivisions );

//Boucle pour parcourir chaque région
unsigned int count=0;
unsigned int It, j, k, It1;
int max_x, max_y, min_x, min_y;
ImageType::IndexType  			 iterationRegionStart;
ImageType::SizeType  			 iteratorRegionSize;
ImageType::RegionType			 iteratorRegion;

model->SetDEMDirectory(argv[8]);

for(count=0;count<NumberOfStreamDivisions;count++)
{//début boucle principale
/**Création de la région pour chaque portion**/
iteratorRegionSize[0]=atoi(argv[5]);      //Taille en X.
if (count==NumberOfStreamDivisions-1)
{iteratorRegionSize[1]=(atoi(argv[6]))-((int)(((atoi(argv[6]))/NumberOfStreamDivisions)+0.5))*(count);
iterationRegionStart[1]=(atoi(argv[5]))-(iteratorRegionSize[1]); 
}
else
{iteratorRegionSize[1]=(int)(((atoi(argv[6]))/NumberOfStreamDivisions)+0.5);	  //Taille en Y.
iterationRegionStart[1]=count*iteratorRegionSize[1]; 
}    
iterationRegionStart[0]=0;//Début de chaque ligne==>0     
iteratorRegion.SetSize(iteratorRegionSize);
iteratorRegion.SetIndex(iterationRegionStart); 

/**Création d'un tableau de pixelindex**/
unsigned int pixelIndexArrayDimension= iteratorRegionSize[0]*iteratorRegionSize[1]*2;
int *pixelIndexArray=new int[pixelIndexArrayDimension];
int *currentIndexArray=new int[pixelIndexArrayDimension];

/**Création de l'itérateur pour chaque portion:**/
IteratorType outputIt(outputimage, iteratorRegion);

/**On applique l'itération sur chaque portion**/
It=0;
It1=0;
for (outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt)
{//Début boucle
//On get l'index courant
currentindex=outputIt.GetIndex();
//On le transforme en Point physique
outputimage->TransformIndexToPhysicalPoint(currentindex, outputpoint);
      otbMsgDevMacro(<< "Pour l'Index N°:(" << currentindex[0]<< ","<< currentindex[1] << ")"<<  std::endl
                << "Le point physique correspondant est: ("<<  outputpoint[0]<< ","<<  outputpoint[1]<< ")"); 

//On applique la projection:
geoPoint= utmprojection->TransformPoint(outputpoint);	
otbMsgDevMacro(<< "Le point géographique correspondant est: ("<<  geoPoint[0]<< ","<<  geoPoint[1]<< ")"); 

//On calcule les coordonnées pixeliques sur l'image capteur
inputpoint = model->TransformPoint(geoPoint);
otbMsgDevMacro(<< "Les coordonnées en pixel sur l'image capteur correspondant à ce point sont:" << std::endl
          << inputpoint[0] << ","<< inputpoint[1] );
inputimage->TransformPhysicalPointToIndex(inputpoint,pixelindex);
    otbMsgDevMacro(<< "L'index correspondant à ce point est:" << std::endl
              << pixelindex[0] << ","<< pixelindex[1] );

/**On stocke les pixel index dans un tableau pixelindexarray**/
 pixelIndexArray[It]=pixelindex[0];
 pixelIndexArray[It+1]=pixelindex[1];
 otbMsgDevMacro(<< "La valeur stockée" << std::endl
          << pixelIndexArray[It] <<  "," << pixelIndexArray[It+1] <<std::endl);

/**On stocke les pixel index dans un tableau currentindexarray**/
 currentIndexArray[It]=currentindex[0];
 currentIndexArray[It+1]=currentindex[1];
 otbMsgDevMacro(<< "La valeur stockée" << std::endl
           << pixelIndexArray[It] <<  "," << pixelIndexArray[It+1] <<std::endl);
 
 
 It=It+2;
 It1=It1+1;
}//Fin boucle: on a stocké tous les index qui nous interesse

/**Calcul des max et min pour pouvoir extraire la bonne région:**/
max_x=pixelIndexArray[0];
min_x=pixelIndexArray[0];
max_y=pixelIndexArray[1];
min_y=pixelIndexArray[1];
 
 	for (j=0;j<It;j++)
 	{
 		if(j%2==0 && pixelIndexArray[j]>max_x){max_x=pixelIndexArray[j];}
 		if(j%2==0 && pixelIndexArray[j]<min_x){min_x=pixelIndexArray[j];}
 		if(j%2!=0 && pixelIndexArray[j]>max_y){max_y=pixelIndexArray[j];}
 		if(j%2!=0 && pixelIndexArray[j]<min_y){min_y=pixelIndexArray[j];}
 	}//Fin while
	
 otbMsgDevMacro(<< "max_x=" << max_x<< std::endl
           << "max_y=" << max_y<< std::endl
	   << "min_x=" << min_x<< std::endl
	   << "min_y=" << min_y);

/**Créer un extractor pour chaque portion:**/
InputImageType::RegionType	            extractregion;

InputImageType::IndexType  		    extractstart;

if (min_x<10 && min_y<10)
	{
	extractstart[0]=0;     
	extractstart[1]=0;
	}

else
	{
	extractstart[0]=min_x-10;     
	extractstart[1]=min_y-10; 
	} 

InputImageType::SizeType  		    extractsize;

extractsize[0]=(max_x-min_x)+20;      //Taille en X.
extractsize[1]=(max_y-min_y)+20;	//Taille en Y.
extractregion.SetSize(extractsize);
extractregion.SetIndex(extractstart);
extract->SetExtractionRegion(extractregion);
extract->SetInput(reader->GetOutput());
extractorwriter->Update();

/**Interpolation:**/
interpolator->SetInputImage(reader->GetOutput());
for ( k=0; k<It/2; k++)
	{
	pixelindexbis[0]= pixelIndexArray[2*k];
	pixelindexbis[1]= pixelIndexArray[2*k+1];
	currentindexbis[0]= currentIndexArray[2*k];
	currentindexbis[1]= currentIndexArray[2*k+1];

//Test si notre index est dans la région extraite:
		if (interpolator->IsInsideBuffer(pixelindexbis) )
			{
			pixelvalue=int (interpolator->EvaluateAtIndex(pixelindexbis));
			}
		else {pixelvalue=0;}
		
otbMsgDevMacro(<< "La valeur du pixel est:"<< float(pixelvalue) );
	   
outputimage->SetPixel(currentindexbis,pixelvalue);

 	}
delete pixelIndexArray;
otbMsgDevMacro(<< "pixelIndexArray deleted" );
delete currentIndexArray; 
otbMsgDevMacro(<< "currentIndexArray deleted" );
}//Fin boucle principale

//Création de l'image de sortie
writer->SetFileName(argv[2]);
otbGenericMsgDebugMacro(<< "FilenameSet" );
rescaler->SetInput(outputimage);
charoutputimage=rescaler->GetOutput();
writer->SetInput(charoutputimage);
writer->Update();
otbGenericMsgDebugMacro(<< "Outputimage created" );






    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( std::bad_alloc & err ) 
    { 
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;


}//Fin main()

