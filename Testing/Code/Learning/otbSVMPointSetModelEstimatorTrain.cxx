/*=========================================================================

  Program   :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J.Inglada
  Language  :   C++
  Date      :   4 May 2006
  Version   :   
  Role      :   Test for the SVMPointSetModelEstimator class (training) 
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkExceptionObject.h"
#include "itkPointSet.h"
#include <iostream>

#include "otbSVMPointSetModelEstimator.h"



int otbSVMPointSetModelEstimatorTrain( int argc, char* argv[] )
{
  try 
    {

    const char* outputModelFileName = argv[1];
    
    typedef std::vector<double>                             InputPixelType;
    typedef double                             LabelPixelType;
    const   unsigned int        	                    Dimension = 2;

    typedef itk::PointSet< InputPixelType,  Dimension >
                                                       MeasurePointSetType;

    typedef itk::PointSet< LabelPixelType,  Dimension >     LabelPointSetType;


    MeasurePointSetType::Pointer mPSet = MeasurePointSetType::New();
    LabelPointSetType::Pointer lPSet = LabelPointSetType::New();


    typedef MeasurePointSetType::PointType    MeasurePointType;
    typedef LabelPointSetType::PointType    LabelPointType;

    typedef MeasurePointSetType::PointsContainer      MeasurePointsContainer;
    typedef LabelPointSetType::PointsContainer      LabelPointsContainer;

    MeasurePointsContainer::Pointer mCont = MeasurePointsContainer::New();
    LabelPointsContainer::Pointer lCont = LabelPointsContainer::New();
    
    for(unsigned int pointId = 0; pointId<20; pointId++)
      {

      MeasurePointType mP;
      LabelPointType lP;

      mP[0] = pointId;
      mP[1] = pointId;

      lP[0] = pointId;
      lP[1] = pointId;


      InputPixelType measure; 
//      measure.push_back(pow(pointId,2.0));
      measure.push_back(double(2.0*pointId));
      measure.push_back(double(-10));

      LabelPixelType label = static_cast<LabelPixelType>(
	(measure[0]+measure[1])>0); //2x-10>0

      std::cout << "Label : " << label << std::endl;

      mCont->InsertElement( pointId , mP );
      mPSet->SetPointData( pointId, measure );   


      lCont->InsertElement( pointId , lP );
      lPSet->SetPointData( pointId, label );   


      }

    mPSet->SetPoints( mCont );
    lPSet->SetPoints( lCont );
    
    typedef otb::SVMPointSetModelEstimator< MeasurePointSetType,
                                           LabelPointSetType >   EstimatorType;
	
	
    EstimatorType::Pointer estimator = EstimatorType::New();

    estimator->SetInputPointSet( mPSet );
    estimator->SetTrainingPointSet( lPSet );
    estimator->SetNumberOfClasses( 2 );

    estimator->Update();

    std::cout << "Saving model" << std::endl;
    estimator->SaveModel(outputModelFileName);


        
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Unknown exception !" << std::endl; 
    return EXIT_FAILURE;
    } 
  // Software Guide : EndCodeSnippet

//#endif
  return EXIT_SUCCESS;
}


