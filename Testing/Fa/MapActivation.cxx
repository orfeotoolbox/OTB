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
#include <iostream>
#include <fstream>
#include "otbVectorImage.h"
#include "otbSOMMap.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"

#include "otbSOMMap.h"
#include "otbSOM.h"
#include "otbSOMActivationBuilder.h"
#include "itkVectorExpandImageFilter.h"
#include "itkVectorNearestNeighborInterpolateImageFunction.h"

#include "itkExpandImageFilter.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

#include "itkListSample.h"

#include "otbSOMClassifier.h"
#include <itkVariableLengthVector.h>
#include <itkMembershipSample.h>







int main(int argc, char* argv[] )
{

  if (argc < 14)
    {
    std::cout << "Usage : " << argv[0] << " inputTabImage size radius NumberOfIterations BetaInit BetaEnd MaxWeight  MinWeight som som-zoom actMap actMap-zoom som" 
	      << std::endl ;
    return EXIT_FAILURE;
    }


  	const int nbparam= 9; //9;
  
 	typedef otb::Image<double, 2 > ListImageType;
  
  	typedef itk::ImageFileReader<ListImageType> TabReaderType;
	TabReaderType::Pointer Tabreader = TabReaderType::New();
    Tabreader->SetFileName(argv[1]);
	Tabreader->Update();
	ListImageType::Pointer tabreadImage = ListImageType::New();
	tabreadImage = Tabreader->GetOutput();
	
	typedef itk::ImageRegionIterator< ListImageType > IteratorType;
	IteratorType It1( tabreadImage, tabreadImage->GetLargestPossibleRegion() );
	const int nblines=591; // (const int)(tabreadImage->GetLargestPossibleRegion().GetSize()[1]);

	const int nbcolonnes=9;//tabreadImage->GetLargestPossibleRegion().GetSize()[0]; 
	
	double vectTab[nblines][nbcolonnes];
	std::cout<<"lignes = "<<nblines<<" colonnes = "<<nbcolonnes<<std::endl; 
  	for ( It1.GoToBegin(); !It1.IsAtEnd() ;++It1)
  	{
		vectTab[It1.GetIndex()[1]][It1.GetIndex()[0]]=It1.Get();
			
  	}

	
	
	
	typedef itk::Vector< double, nbparam > MeasurementVectorType ;
	//typedef itk::VariableLengthVector< double > MeasurementVectorType ;
  	typedef itk::Statistics::ListSample< MeasurementVectorType > SampleType ;
	SampleType::Pointer liste = SampleType::New() ;
	
	MeasurementVectorType tab;

	for(int j=0;j<nblines;j++)
	{
		for(int i=0;i<nbcolonnes;i++)
		{
			tab[i] = vectTab[j][i];
		}
		liste->PushBack(tab);
	}
// 	std::cout<<"liste: "<<liste->GetMeasurementVectorSize()<< " " << liste->GetMeasurementVector(1)
// 	<<" " <<liste->GetMeasurementVector(2)<< " " <<liste->GetMeasurementVector(3)<<std::endl;
	
	
	typedef itk::Statistics::EuclideanDistance<MeasurementVectorType> DistanceType;
	typedef otb::SOMMap<MeasurementVectorType,DistanceType,2> MapType;
	
	typedef otb::SOM<SampleType,MapType> SOMType;
	SOMType::Pointer som = SOMType::New();
    som->SetListSample(liste);
	SOMType::SizeType som_size;
    som_size[0]=atoi(argv[2]);
    som_size[1]=atoi(argv[2]);
    som->SetMapSize(som_size);
	SOMType::SizeType radius;
    radius[0] = atoi(argv[3]);
    radius[1] = atoi(argv[3]);
    som->SetNeighborhoodSizeInit(radius);
	
	som->SetNumberOfIterations(atoi(argv[4]));
    som->SetBetaInit(atoi(argv[5]));
    som->SetBetaEnd(atoi(argv[6]));
    som->SetMaxWeight(atoi(argv[7]));
	som->SetMinWeight(atoi(argv[8]));
	som->Update();

	
	typedef otb::ImageFileWriter<MapType> SomWriterType;
	SomWriterType::Pointer somwriter = SomWriterType::New();
    somwriter->SetFileName(argv[9]/*"som.hd"*/);
    somwriter->SetInput(som->GetOutput());
    somwriter->Update();
	

	//Just for visualization purposes, we zoom the image.
    typedef itk::VectorExpandImageFilter< MapType, MapType > ExpandType;
    typedef itk::VectorNearestNeighborInterpolateImageFunction< MapType, double > InterpolatorType;
	
    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    ExpandType::Pointer expand = ExpandType::New();
    expand->SetInput(som->GetOutput());
    expand->SetExpandFactors( 40 );
    expand->SetInterpolator( interpolator );
    MeasurementVectorType pix;
    for(int j=0;j<nbcolonnes;j++)
	{
		pix[j]= 255;
   	}
	
	
    expand->SetEdgePaddingValue(pix);
    somwriter->SetInput(expand->GetOutput());
	somwriter->SetFileName(argv[10]/*"som-zoom.mhd"*/);
    somwriter->Update();
	
	
	
	
	typedef unsigned char OutputPixelType;
	
    typedef otb::Image<OutputPixelType,2> OutputImageType;
    typedef otb::ImageFileWriter<OutputImageType> ActivationWriterType;
	typedef otb::SOMActivationBuilder<SampleType,MapType,OutputImageType> SOMActivationBuilderType;
	SOMActivationBuilderType::Pointer somAct = SOMActivationBuilderType::New();
    somAct->SetInput(som->GetOutput());
    somAct->SetListSample(liste);
	
	ActivationWriterType::Pointer actWriter = ActivationWriterType::New();
    actWriter->SetFileName(argv[11]/*"actMap.png"*/);
    actWriter->SetInput(somAct->GetOutput());
    actWriter->Update();
	//Just for visualization purposes, we zoom the image.
    typedef itk::ExpandImageFilter< OutputImageType, OutputImageType > ExpandType2;
    typedef itk::NearestNeighborInterpolateImageFunction< OutputImageType,double > InterpolatorType2;

    InterpolatorType2::Pointer interpolator2 = InterpolatorType2::New();
    ExpandType2::Pointer expand2 = ExpandType2::New();
    expand2->SetInput(somAct->GetOutput());
    expand2->SetExpandFactors( 40 );
    expand2->SetInterpolator( interpolator2 );
    expand2->SetEdgePaddingValue(255);
    actWriter->SetInput(expand2->GetOutput());
	actWriter->SetFileName(argv[12]/*"actMap-zoom.png"*/);
    actWriter->Update();
	
	
	//Classifier :
	
	typedef otb::ImageFileReader<MapType> SOMReaderType;
	typedef otb::SOMClassifier<SampleType,MapType,unsigned char>  ClassifierType;
	
	SOMReaderType::Pointer somreader = SOMReaderType::New();
  	somreader->SetFileName(argv[13]/*"som.mhd"*/);
  	somreader->Update();
	
	ClassifierType::Pointer classifier = ClassifierType::New() ;
	
 	classifier->SetSample(liste);
  	classifier->SetMap(somreader->GetOutput());
  	classifier->Update() ;
	
	ClassifierType::OutputType* membershipSample = classifier->GetOutput();
// 	std::cout<<"liste: "<<membershipSample->GetMeasurementVectorSize()<< " " << membershipSample->GetMeasurementVector(1)<<std::endl;
	
	ClassifierType::OutputType::ConstIterator m_iter =  membershipSample->Begin();
  	ClassifierType::OutputType::ConstIterator m_last =  membershipSample->End();
	
	/*int count[16];
	for(int i=0;i<16;i++)
	{
		count[i]=0;
	}
	*/
	while (m_iter != m_last)
    {
    std::cout<<" classlabel : "<<m_iter.GetClassLabel()<<
				" ClassSize= "<< membershipSample->GetClassSampleSize(m_iter.GetClassLabel()) <<
				 " instance id = " << m_iter.GetInstanceIdentifier() <<
				  " measurement vector = " << m_iter.GetMeasurementVector() <<

				//" class sample: "<< membershipSample->GetClassSample(m_iter.GetClassLabel())<<
				std::endl;
	//count[m_iter.GetClassLabel()]++;
    ++m_iter ;
	
    }
	/*for(int i=0;i<16;i++)
	{
		std::cout<<" classe: "<< i <<" nb elements= "<<count[i]<<std::endl;
	}*/
	
	int nbclass = membershipSample->GetNumberOfClasses() ;
	std::cout<< " nb of classes= "<< nbclass << std::endl;
	
	for (int i=0 ; i< nbclass ; i++)
	{
		std::cout<<"classlabel= "<< i << "  classSize= "<< membershipSample->GetClassSampleSize(i) <<std::endl;
		
	}
		
	
	return EXIT_SUCCESS;
	
}
