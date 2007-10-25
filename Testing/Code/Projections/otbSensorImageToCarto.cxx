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

/*!
 *
 * PURPOSE:
 *
 * Application pour projeter une région d'une image en coordonnées géographiques 
 * en utilisant un Interpolator+regionextractor et un Iterator. 
 * Pas de prise en compte du MNT.
 */

// iostream is used for general output
#include <iostream>
#include <iterator>
#include <stdlib.h>

#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkExtractImageFilter.h"

#include "otbImageGeometryHandler.h"
#include "otbInverseSensorModel.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImage.h"
#include "otbMacro.h"
#include "otbMapProjections.h"
#include "otbMapProjection.h"
// this is the most important class and is called as the first line of all applications.
// without this all the important factories are not created.
//#include "init/ossimInit.h"

int otbSensorImageToCarto( int argc, char* argv[] )
{
  try 
    {        

   ossimInit::instance()->initialize(argc, argv);

   if(argc!=10)
   {
      std::cout << argv[0] <<" <input filename> <output filename> <X origine> <Y origine> <taille_x> <taille_y> <NumberOfstreamDivisions>" 
                << "<xSPacing> <ySpacing>" << std::endl;

      return EXIT_FAILURE;
   }
typedef itk::Point <double, 2> 		 PointType;
PointType				 outputpoint; 

/*************************************************/
/*  Création de l'image de sortie outputimage    */
/*************************************************/
typedef otb::Image<unsigned char, 2>    CharImageType;
typedef otb::Image<unsigned int, 2>     ImageType;
typedef otb::Image<unsigned int, 2>     InputImageType;
ImageType::Pointer 	  	         outputimage = ImageType::New();

ImageType::PixelType			 pixelvalue;

ImageType::IndexType  			 start;
start[0]=0;     
start[1]=0;     

ImageType::SizeType  			 size;
size[0]=atoi(argv[5]);      //Taille en X.
size[1]=atoi(argv[6]);	    //Taille en Y.

ImageType::SpacingType  		 spacing;
spacing[0]=atof(argv[8]);
spacing[1]=atof(argv[9]);

ImageType::PointType			 origin;
origin[0]=strtod(argv[3], NULL);         //latitude de l'origine.
origin[1]=strtod(argv[4], NULL);         //longitude de l'origine.

ImageType::RegionType			 region;

region.SetSize(size);
region.SetIndex(start);

outputimage-> SetOrigin(origin);
outputimage-> SetRegions(region);
outputimage->SetSpacing(spacing);
outputimage-> Allocate();     //Notre image de sortie est créée.
otbGenericMsgDebugMacro(<< "Output image created!! " ); 

/******************************/
/*  Création de mon handler   */
/******************************/
	
ossimKeywordlist geom_kwl;
      typedef otb::ImageGeometryHandler  HandlerType;
      HandlerType::Pointer   handler= HandlerType::New();
      otbGenericMsgDebugMacro(<< "Handler created " ); 
      handler->SetFileName(argv[1]);
      geom_kwl=handler->GetGeometryKeywordlist();
std::cout << geom_kwl << std::endl; 

/********************************************************/
/*   Création de notre modèle en fonction de l'image    */
/********************************************************/
typedef otb::ImageFileReader<ImageType>  ReaderType;
ReaderType::Pointer	                 reader=ReaderType::New();
//ReaderType::Pointer	                 reader1=ReaderType::New();
reader->SetFileName(argv[1]);
reader->GenerateOutputInformation();




  	otb::ImageKeywordlist otb_tmp_image;
/*	itk::ExposeMetaData< otb::ImageKeywordlist >(image_medianFilter->GetOutput()->GetMetaDataDictionary(),
											 otb::MetaDataKey::m_OSSIMKeywordlistKey,
											 otb_tmp_image);*/

	otb_tmp_image = reader->GetOutput()->GetImageKeywordlist();

	ossimKeywordlist ossim_kwl_image;
	otb_tmp_image.convertToOSSIMKeywordlist(ossim_kwl_image);
std::cout << "	ossim_kwl_image: "<<ossim_kwl_image<<std::endl;

ossimRefPtr<ossimProjection> toto = ossimProjectionFactoryRegistry::instance()->createProjection(ossim_kwl_image);
if( toto.valid() == false)
{
//        itkGenericExceptionMacro(<<"Invalid Model pointer !");
        otbGenericMsgDebugMacro(<<"Invalid Model pointer !");
}




      typedef otb::InverseSensorModel<double>  ModelType;
      ModelType::Pointer   model= ModelType::New();
      otbGenericMsgDebugMacro(<< "Model set geometry " ); 
      model->SetImageGeometry(ossim_kwl_image); //Notre modèle est créé à ce niveau.
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

//reader1->SetFileName(argv[2]);
//ImageType::Pointer 	  	         image = reader1->GetOutput();
//reader->Update();
ImageType::Pointer  			 inputimage= reader->GetOutput();
ImageType::IndexType 			 currentindex;
ImageType::IndexType 			 currentindexbis;
ImageType::IndexType 			 pixelindex;
ImageType::IndexType 			 pixelindexbis;
otbGenericMsgDebugMacro(<< "Reader created " ); 

//Stocker les caractéristiques de notre image capteur:
// ImageType::SizeType			 inputimagesize;
// inputimagesize=inputimage->GetLargestPossibleRegion().GetSize();
// //inputimagesize=inputimage->GetSize();
// std::cout << "Dimension de notre image capteur:"<<  endl
//                 << "("<<  inputimagesize[0]<< ","<<  inputimagesize[1]<< ")"<<std::endl; 


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
/*            Création de notre writer                  */
/********************************************************/

typedef otb::ImageFileWriter<ImageType>  WriterType;
typedef otb::ImageFileWriter<CharImageType>  CharWriterType;
WriterType::Pointer	                 extractorwriter=WriterType::New();
CharWriterType::Pointer	                 writer=CharWriterType::New();
extractorwriter->SetFileName("image_temp.jpeg");
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
typedef otb::UtmProjection                      utmProjection;
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
//IteratorType outputIt(outputimage, region); //Définition de notre itérateur.
//std::cout << "Iterator created " << std::endl; 

//Donner une valeur par défaut numberofstreamdivision ou le faire fixer par l'utilisateur.
unsigned int NumberOfStreamDivisions;
if (atoi(argv[7])==0)
{NumberOfStreamDivisions=10;}//ou pourrait etre calculer en fonction de la taille de outputimage
else{NumberOfStreamDivisions=atoi(argv[7]);}
//std::cout << "NumberOfStreamDivisions =" << NumberOfStreamDivisions << std::endl;

//Boucle pour parcourir chaque région
unsigned int count=0;
unsigned int It, j, k;
int max_x, max_y, min_x, min_y;
ImageType::IndexType  			 iterationRegionStart;
ImageType::SizeType  			 iteratorRegionSize;
ImageType::RegionType			 iteratorRegion;

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
// std::cout <<iteratorRegionSize[1]<< std::endl;  
// std::cout <<iterationRegionStart[0]<< std::endl; 
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
//std::cout << "La valeur stockée" << endl
//          << pixelIndexArray[It] <<  "," << pixelIndexArray[It+1] <<std::endl;

/**On stocke les pixel index dans un tableau currentindexarray**/
 currentIndexArray[It]=currentindex[0];
 currentIndexArray[It+1]=currentindex[1];
otbMsgDevMacro(<< "La valeur stockée" << std::endl
          << pixelIndexArray[It] <<  "," << pixelIndexArray[It+1]);
 It=It+2;
}//Fin boucle: on a stocké tous les index qui nous interesse
//std::cout  << "It= " << It << std::endl;

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
	
 otbGenericMsgDebugMacro(<< "max_x=" << max_x<< std::endl
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
//extractregion=inputimage->GetRequestedRegion();
extract->SetExtractionRegion(extractregion);
extract->SetInput(reader->GetOutput());
extractorwriter->Update();

/**Interpolation:**/
interpolator->SetInputImage(reader->GetOutput());
//interpolator->SetDefaultPixelValue(0);
for ( k=0; k<It/2; k++)
{
pixelindexbis[0]= pixelIndexArray[2*k];
pixelindexbis[1]= pixelIndexArray[2*k+1];
currentindexbis[0]= currentIndexArray[2*k];
currentindexbis[1]= currentIndexArray[2*k+1];
//Test si notre index est dans la région extraite:
if (interpolator->IsInsideBuffer(pixelindexbis))
{pixelvalue=int(interpolator->EvaluateAtIndex(pixelindexbis));}
else {pixelvalue=0;}
otbMsgDevMacro(<< "La valeur du pixel est:" << std::endl
           << float(pixelvalue) );
outputimage->SetPixel(currentindexbis,pixelvalue);
 }
 delete pixelIndexArray;
otbGenericMsgDebugMacro(<< "pixelIndexArray deleted" );
delete currentIndexArray; 
otbGenericMsgDebugMacro(<< "currentIndexArray deleted" );
}//Fin boucle principale
writer->SetFileName(argv[2]);
otbGenericMsgDebugMacro(<< "FilenameSet" );
rescaler->SetInput(outputimage);
CharImageType::Pointer      charoutputimage=CharImageType::New();
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

