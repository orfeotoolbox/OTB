/*=========================================================================

  Program   :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J.Inglada
  Language  :   C++
  Date      :   28 April 2006
  Version   :   
  Role      :   Test for the SVMClassifier class (instanciation) 
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <fstream>

#include "itkPoint.h"
#include "itkPointSet.h"

#include "itkPointSetToListAdaptor.h"
#include "itkSubsample.h"
#include "itkListSample.h"
#include "otbSVMClassifier.h"

int otbSVMClassifierPointSet(int argc, char* argv[] )
{

  try
    {
    namespace stat = itk::Statistics ;
 
    if (argc != 2)
      {
      std::cout << "Usage : " << argv[0] << " modelFile" 
                << std::endl ;
      return EXIT_FAILURE;
      }

    const char * modelFilename  = argv[1];

    std::cout << "Building the pointset" << std::endl;  


    typedef double InputPixelType;
    typedef int LabelPixelType;
    typedef std::vector<InputPixelType>                             InputVectorType;
    const   unsigned int        	                    Dimension = 2;

    typedef itk::PointSet< InputVectorType,  Dimension >
                                                       MeasurePointSetType;


    MeasurePointSetType::Pointer mPSet = MeasurePointSetType::New();

    typedef MeasurePointSetType::PointType    MeasurePointType;

    typedef MeasurePointSetType::PointsContainer      MeasurePointsContainer;

    MeasurePointsContainer::Pointer mCont = MeasurePointsContainer::New();
    
    for(unsigned int pointId = 0; pointId<20; pointId++)
      {

      MeasurePointType mP;

      mP[0] = pointId;
      mP[1] = pointId;


      InputVectorType measure; 
      //measure.push_back(pow(pointId,2.0));
      measure.push_back(double(2.0*pointId));
      measure.push_back(double(-10));


      mCont->InsertElement( pointId , mP );
      mPSet->SetPointData( pointId, measure );   


      }

    mPSet->SetPoints( mCont );

    std::cout << "PointSet built" << std::endl;  

    typedef itk::Statistics::PointSetToListAdaptor< MeasurePointSetType >
      SampleType;
    SampleType::Pointer sample = SampleType::New();
    sample->SetPointSet( mPSet );

    std::cout << "Sample set to Adaptor" << std::endl;  


    /** preparing classifier and decision rule object */
    typedef otb::SVMModel< SampleType::MeasurementVectorType::ValueType, LabelPixelType > ModelType;

    ModelType::Pointer model = ModelType::New();

    model->LoadModel( modelFilename );

    std::cout << "Model loaded" << std::endl;
    
    int numberOfClasses = model->GetNumberOfClasses();
    
    typedef otb::SVMClassifier< SampleType, LabelPixelType > ClassifierType ;

    ClassifierType::Pointer classifier = ClassifierType::New() ;
  
    classifier->SetNumberOfClasses(numberOfClasses) ;
    classifier->SetModel( model );
    classifier->SetSample(sample.GetPointer()) ;
    classifier->Update() ;

    /* Build the class map */
    std::cout << "Output image creation" << std::endl;  

    
    std::cout << "classifier get output" << std::endl;  
    ClassifierType::OutputType* membershipSample =
      classifier->GetOutput() ;
    std::cout << "Sample iterators" << std::endl;  
    ClassifierType::OutputType::ConstIterator m_iter =
      membershipSample->Begin() ;
    ClassifierType::OutputType::ConstIterator m_last =
      membershipSample->End() ;


    double error = 0.0;
    unsigned int pointId = 0;
    while (m_iter != m_last)
      {
      ClassifierType::ClassLabelType label = m_iter.GetClassLabel();
      
      InputVectorType measure; 
      
      mPSet->GetPointData(pointId, &measure);
      
      if(label != ((measure[0]+measure[1])>0) )
	error++;

      std::cout << label << "/" <<
	((measure[0]+measure[1])>0) << std::endl;
      
      
      ++pointId;
      ++m_iter ;
      }
    
    std::cout << "Error = " << error/pointId << std::endl;
    


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
 
  return EXIT_SUCCESS;
}







