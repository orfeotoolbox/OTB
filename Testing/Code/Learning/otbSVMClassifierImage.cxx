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

#include "itkVector.h"
#include "itkPoint.h"
#include "itkImage.h"
#include "itkVectorImage.h"

#include "itkPointSetToListAdaptor.h"
#include "itkSubsample.h"
#include "itkListSample.h"
#include "itkMinimumDecisionRule.h"
#include "otbSVMClassifier.h"
#include "otbSVMMembershipFunction.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkEuclideanDistance.h"

int otbSVMClassifierImage(int argc, char* argv[] )
{

  try
    {
    namespace stat = itk::Statistics ;
 
    if (argc != 4)
      {
      std::cout << "Usage : " << argv[0] << " inputImage modelFile outputImage" 
                << std::endl ;
      return EXIT_FAILURE;
      }

    const char * imageFilename  = argv[1];
    const char * modelFilename  = argv[2];
    const char * outputFilename = argv[3];
       


    /** Read the input image and build the sample */

    typedef float                                InputPixelType;
    typedef unsigned char	                 OutputPixelType;

    const   unsigned int        	         Dimension = 2;

    typedef itk::VectorImage< InputPixelType,  Dimension >	InputImageType;
    typedef itk::Image< OutputPixelType, Dimension >        OutputImageType;

    typedef otb::ImageFileReader< InputImageType  >         ReaderType;
    typedef otb::ImageFileWriter< OutputImageType >         WriterType;

    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();

    reader->SetFileName( imageFilename  );
    writer->SetFileName( outputFilename  );

    reader->Update();

    typedef itk::Vector< float, 3 > MeasurementVectorType ;
    typedef itk::Statistics::ListSample< MeasurementVectorType > SampleType ;
    SampleType::Pointer sample = SampleType::New() ;


    typedef itk::ImageRegionIterator< InputImageType>  InputIteratorType;
    InputIteratorType  inIt( reader->GetOutput(),
			     reader->GetOutput()->GetBufferedRegion() );
    
    inIt.GoToBegin();
    

    while(!inIt.IsAtEnd() )
      {
      MeasurementVectorType mv ;
      mv[0] = inIt.Get()[0];
      mv[1] = inIt.Get()[1];
      mv[2] = inIt.Get()[2];

  
      sample->PushBack(mv) ;

      ++inIt;
      }

  

    


    /** preparing classifier and decision rule object */
    typedef otb::SVMModel< MeasurementVectorType > ModelType;

    ModelType::Pointer model = ModelType::New();

    model->LoadModel( modelFilename );

    int numberOfClasses = model->GetNumberOfClasses();
    
    typedef otb::SVMClassifier< SampleType > ClassifierType ;
    typedef itk::MinimumDecisionRule DecisionRuleType ;


    ClassifierType::Pointer classifier = ClassifierType::New() ;
  
    classifier->SetNumberOfClasses(numberOfClasses) ;
    classifier->SetModel( model );
    classifier->SetSample(sample.GetPointer()) ;
    classifier->Update() ;

    /* Build the class map */
    std::cout << "Output image creation" << std::endl;  
    OutputImageType::Pointer outputImage = OutputImageType::New();

    typedef itk::Index<Dimension>         myIndexType;
    typedef itk::Size<Dimension>          mySizeType;
    typedef itk::ImageRegion<Dimension>        myRegionType;

    mySizeType size;
    size[0] = reader->GetOutput()->GetRequestedRegion().GetSize()[0];
    size[1] = reader->GetOutput()->GetRequestedRegion().GetSize()[1];

    myIndexType start;
    start[0] = 0;
    start[1] = 0;

    myRegionType region;
    region.SetIndex( start );
    region.SetSize( size );

    outputImage->SetRegions( region );
    outputImage->Allocate();

    
    std::cout << "classifier get output" << std::endl;  
    ClassifierType::OutputType* membershipSample =
      classifier->GetOutput() ;
    std::cout << "Sample iterators" << std::endl;  
    ClassifierType::OutputType::ConstIterator m_iter =
      membershipSample->Begin() ;
    ClassifierType::OutputType::ConstIterator m_last =
      membershipSample->End() ;

    std::cout << "Image iterator" << std::endl;  
    typedef itk::ImageRegionIterator< OutputImageType>  OutputIteratorType;
    OutputIteratorType  outIt( outputImage,
			   outputImage->GetBufferedRegion() );

    outIt.GoToBegin();


    std::cout << "Iteration for output image = " << (membershipSample->Size()) << std::endl;  

    while (m_iter != m_last && !outIt.IsAtEnd())
    {
      outIt.Set(static_cast<OutputPixelType>(m_iter.GetClassLabel()));
      ++m_iter ;
      ++outIt;
      }


    typedef itk::RescaleIntensityImageFilter< OutputImageType,
                                            OutputImageType > RescalerType;

    RescalerType::Pointer rescaler = RescalerType::New();
    
    rescaler->SetOutputMinimum( itk::NumericTraits< OutputPixelType >::min());
    rescaler->SetOutputMaximum( itk::NumericTraits< OutputPixelType >::max());

    rescaler->SetInput( outputImage );
    writer->SetInput( rescaler->GetOutput() );
    
    writer->Update();
    

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







