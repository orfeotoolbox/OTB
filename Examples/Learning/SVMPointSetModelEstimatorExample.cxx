/*=========================================================================

  Program   :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J.Inglada
  Language  :   C++
  Date      :   5 May 2006
  Version   :   
  Role      :   
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define MAIN


#include "itkExceptionObject.h"
#include "itkPointSet.h"
#include <iostream>
#include <cstdlib> 

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb::otbSVMPointSetModelEstimator} in order to perform the
// SVM learning from an \doxygen{itk::PointSet} data structure.
//
// The first step required to use this filter is to include its header file. 
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "otbSVMPointSetModelEstimator.h"
// Software Guide : EndCodeSnippet


int main( int argc, char* argv[] )
{

// Software Guide : BeginLatex
// 
// In the framework of supervised learning and classification, we will
// always use feature vectors for the characterization of the
// classes. On the other hand, the class labels are scalar
// values. Here, we start by defining the type of the features as the
// \code{PixelType}, which will be used to define the feature
// \code{VectorType}. We also declare the type for the labels.
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  typedef float                      PixelType;
  typedef std::vector<PixelType>     VectorType;
  typedef int                        LabelPixelType;
// Software Guide : EndCodeSnippet
  const   unsigned int        	                    Dimension = 2;

// Software Guide : BeginLatex
//
// We can now proceed to define the point sets used for storing the
// features and the labels.
//
// Software Guide : EndLatex
  
// Software Guide : BeginCodeSnippet  
  typedef itk::PointSet< VectorType,  Dimension >      FeaturePointSetType;

  typedef itk::PointSet< LabelPixelType,  Dimension >  LabelPointSetType;


  FeaturePointSetType::Pointer fPSet = FeaturePointSetType::New();
  LabelPointSetType::Pointer   lPSet = LabelPointSetType::New();

// Software Guide : EndCodeSnippet    


// Software Guide : BeginLatex
//
// We will need to get access to the data stored in the point sets, so
// we define the appropriate 
//
// Software Guide : EndLatex

  typedef FeaturePointSetType::PointType    FeaturePointType;
  typedef LabelPointSetType::PointType      LabelPointType;

  typedef FeaturePointSetType::PointsContainer    FeaturePointsContainer;
  typedef LabelPointSetType::PointsContainer      LabelPointsContainer;

  FeaturePointsContainer::Pointer fCont = FeaturePointsContainer::New();
  LabelPointsContainer::Pointer lCont = LabelPointsContainer::New();

  /* We learn the y>x | y<x boundary*/
  srand((unsigned)time(0));
  int lowest = 0;
  int range = 1000;
  
  for(unsigned int pointId = 0; pointId<500; pointId++)
    {
    
    FeaturePointType fP;
    LabelPointType lP;

    int x_coord = lowest+static_cast<int>(range*(rand()/(RAND_MAX + 1.0)));
    int y_coord = lowest+static_cast<int>(range*(rand()/(RAND_MAX + 1.0))); 

    std::cout << "coords : " << x_coord << " " << y_coord << std::endl;
    mP[0] = x_coord;
    mP[1] = y_coord;

    lP[0] = x_coord;
    lP[1] = y_coord;


    VectorType feature; 
    feature.push_back(static_cast<PixelType>((x_coord*1.0-lowest)/range));
    feature.push_back(static_cast<PixelType>((y_coord*1.0-lowest)/range));

    LabelPixelType label;

    if(x_coord < y_coord)
      label= -1;
    else
      label = 1;

    std::cout << "Label : " << label << std::endl;
    std::cout << "Features : " << feature[0] << " " << feature[1] << std::endl;

    fCont->InsertElement( pointId , mP );
    fPSet->SetPointData( pointId, feature );   


    lCont->InsertElement( pointId , lP );
    lPSet->SetPointData( pointId, label );   


    }

  fPSet->SetPoints( mCont );
  lPSet->SetPoints( lCont );
    
  typedef otb::SVMPointSetModelEstimator< FeaturePointSetType,
    LabelPointSetType >   EstimatorType;
	
	
  EstimatorType::Pointer estimator = EstimatorType::New();

  estimator->SetInputPointSet( fPSet );
  estimator->SetTrainingPointSet( lPSet );
  estimator->SetNumberOfClasses( 2 );


  estimator->Update();

  std::cout << "Saving model" << std::endl;
  estimator->SaveModel("model.svm");


  return EXIT_SUCCESS;
}


