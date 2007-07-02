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

#include <fstream>

/*#include "itkPoint.h"
#include "itkPointSet.h"


#include "itkPointSetToListAdaptor.h"
#include "itkSubsample.h"*/


#include "itkListSample.h"


#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkImageToListAdaptor.h"
#include "otbSVMClassifier.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"

namespace otb
{

class Linear : public GenericKernelFunctorBase
{
public:
  Linear(): GenericKernelFunctorBase() {};
  virtual ~Linear() {};
  
  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      return this->dot(x,y);
    }
};

}


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

    typedef double                               InputPixelType;
    typedef std::vector<InputPixelType>          InputVectorType;
    typedef int                                LabelPixelType;


    const   unsigned int        	         Dimension = 2;

    

    typedef otb::Image< itk::FixedArray<InputPixelType,3>,  Dimension >	InputImageType;


    typedef otb::ImageFileReader< InputImageType  >         ReaderType;


    ReaderType::Pointer reader = ReaderType::New();


    reader->SetFileName( imageFilename  );

    reader->Update();

    std::cout << "Image read" << std::endl;  


    typedef itk::Statistics::ImageToListAdaptor< InputImageType > SampleType;
    SampleType::Pointer sample = SampleType::New();

    sample->SetImage(reader->GetOutput());


    /** preparing classifier and decision rule object */
    typedef otb::SVMModel< InputPixelType, LabelPixelType > ModelType;

    ModelType::Pointer model = ModelType::New();

    
    otb::Linear lFunctor;
    model->SetKernelFunctor(&lFunctor);

    model->LoadModel( modelFilename );

    int numberOfClasses = model->GetNumberOfClasses();
    
    typedef otb::SVMClassifier< SampleType, LabelPixelType > ClassifierType ;

    ClassifierType::Pointer classifier = ClassifierType::New() ;
  
    classifier->SetNumberOfClasses(numberOfClasses) ;
    classifier->SetModel( model );
    classifier->SetSample(sample.GetPointer()) ;
    classifier->Update() ;

    /* Build the class map */
    std::cout << "Output image creation" << std::endl;

    typedef ClassifierType::ClassLabelType	                 OutputPixelType;
    typedef otb::Image< OutputPixelType, Dimension >        OutputImageType;

    
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
    outIt.Set(m_iter.GetClassLabel());
    ++m_iter ;
    ++outIt;
    }


    typedef otb::Image< unsigned char, Dimension >        FileImageType;

    
    typedef itk::RescaleIntensityImageFilter< OutputImageType,
      FileImageType > RescalerType;

    RescalerType::Pointer rescaler = RescalerType::New();
    
    rescaler->SetOutputMinimum( itk::NumericTraits< unsigned char >::min());
    rescaler->SetOutputMaximum( itk::NumericTraits< unsigned char >::max());

    rescaler->SetInput( outputImage );

    typedef otb::ImageFileWriter< FileImageType >         WriterType;
	
    WriterType::Pointer writer = WriterType::New();

    writer->SetFileName( outputFilename  );
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







