
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

#include "itkPoint.h"
#include "itkPointSetToListAdaptor.h"
#include "itkSubsample.h"
#include "otbSVMClassifier.h"

int otbSVMClassifierNew(int argc, char* argv[] )
{

  try 
    {         
    typedef itk::PointSet< double, 2 > PointSetType ;
    
    typedef itk::Statistics::PointSetToListAdaptor< PointSetType >
      DataSampleType;
    
    
    typedef otb::SVMClassifier< DataSampleType > ClassifierType ;
    
    ClassifierType::Pointer classifier = ClassifierType::New();
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







