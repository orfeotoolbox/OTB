#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROI_QB_MUL_1.png}, {ROI_mask_multi.png}
//    OUTPUTS: {ROI_QB_MUL_1_SVN_CLASS_MULTI.png}, {svm_image_model_multi.svn}
//  Software Guide : EndCommandLineArgs


//  Software Guide : BeginLatex
// This example illustrates .. figure~\ref{fig:SVMROISMULTI}. 
// \begin{figure}
// \center
// \includegraphics[width=0.35\textwidth]{ROI_QB_MUL_1.eps}
// \includegraphics[width=0.35\textwidth]{ROI_mask_multi.eps}
// \itkcaption[SVM Image Model Estimation]{Images used for the
// estimation of the SVM model. Left: RGB image. Right: image of labels.} 
// \label{fig:SVMROISMULTI}
// \end{figure}
// The first thing to do is include the header file for the class.
//
//  Software Guide : EndLatex 


#include "itkExceptionObject.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include <iostream>

//  Software Guide : BeginCodeSnippet
#include "otbSVMImageModelEstimator.h"
#include "itkImageToListAdaptor.h"
#include "itkListSample.h"
#include "otbSVMClassifier.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"


//  Software Guide : EndCodeSnippet

#include "otbImageFileReader.h"



int main( int argc, char* argv[] )
{
  

    const char* inputImageFileName = argv[1];
    const char* trainingImageFileName = argv[2];
    const char* outputImageFileName = argv[3];
    const char* outputModelFileName = argv[4];

//  Software Guide : BeginLatex
//
//  We define the types for the input and training images. Even if the
//  input image will be an RGB image, we can read it as a 3 component
//  vector image. This simlifies the interfacing with OTB's SVM
//  framework.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet    
    typedef unsigned short      InputPixelType;
    const   unsigned int       Dimension = 2;

    typedef otb::VectorImage< InputPixelType, Dimension >   InputImageType;

    
    typedef otb::Image< InputPixelType,  Dimension >     TrainingImageType;
//  Software Guide : EndCodeSnippet        
    
//  Software Guide : BeginLatex
//
//  The \doxygen{otb::SVMImageModelEstimator} class is templated over
//  the input (features) and the training (labels) images.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet    
    typedef otb::SVMImageModelEstimator< InputImageType,
                                  TrainingImageType >   EstimatorType;

//  Software Guide : EndCodeSnippet        
    
//  Software Guide : BeginLatex
//
//  As usual, we define the readers for the images.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet        
    typedef otb::ImageFileReader< InputImageType > InputReaderType;
    typedef otb::ImageFileReader< TrainingImageType > TrainingReaderType;

    InputReaderType::Pointer inputReader = InputReaderType::New();
    TrainingReaderType::Pointer trainingReader = TrainingReaderType::New();

//  Software Guide : EndCodeSnippet        
    
//  Software Guide : BeginLatex
//
//  We read the images. It is worth to note that, in order to ensure
//  the pipeline coherence, the output of the objects which preceed the
//  model estimator in the pipeline, must be up to date, so we call
//  the corresponding \code{Update} methods. 
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet            
    inputReader->SetFileName( inputImageFileName );
    trainingReader->SetFileName( trainingImageFileName );

    inputReader->Update();
    trainingReader->Update();

//  Software Guide : EndCodeSnippet        
    
//  Software Guide : BeginLatex
//
//  We can now instantiate the model estimator and set its parameters.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet                
    EstimatorType::Pointer svmEstimator = EstimatorType::New();

    svmEstimator->SetInputImage( inputReader->GetOutput() );
    svmEstimator->SetTrainingImage( trainingReader->GetOutput() );
    svmEstimator->SetNumberOfClasses( 4 );

//  Software Guide : EndCodeSnippet        
    
//  Software Guide : BeginLatex
//
//  The model estimation procedure is triggered by calling the
//  estimator's \code{Update} method.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet                    

    svmEstimator->Update();

//  Software Guide : EndCodeSnippet        
    
//  Software Guide : BeginLatex
//
//  Finally, the estimated model can be saved to a file for later use.
//
//  Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet                    

    svmEstimator->SaveModel(outputModelFileName);
    

//  Software Guide : EndCodeSnippet            


    typedef otb::Image< itk::FixedArray<InputPixelType,3>,
                             Dimension >	        ClassifyImageType;


    typedef otb::ImageFileReader< ClassifyImageType  >         ClassifyReaderType;

// Software Guide : EndCodeSnippet    


// Software Guide : BeginLatex
//
// We can now read the image by calling he \code{Update} method of the reader.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  
    

    ClassifyReaderType::Pointer cReader = ClassifyReaderType::New();

    cReader->SetFileName( inputImageFileName  );

    cReader->Update();
    
    // Software Guide : BeginLatex
//
// The image has now to be transformed to a sample which
// is compatible with the classification framework. We will use a
// \doxygen{itk::Statistics::ImageToListAdaptor} for this
// task. This class is templated over the image type used for
// storing the measures.
//
// Software Guide : EndLatex
    
// Software Guide : BeginCodeSnippet      
    

    typedef itk::Statistics::ImageToListAdaptor< ClassifyImageType > SampleType;
    SampleType::Pointer sample = SampleType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// After instatiantion, we can set the image as an imput of our
// sample adaptor.
//
// Software Guide : EndLatex
    
// Software Guide : BeginCodeSnippet      
    
    sample->SetImage(cReader->GetOutput());

// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Now, we need to declare the SVM model which is to be used by the
// classifier. The SVM model is templated over the type of value used
// for the measures and the type of pixel used for the labels.
//
// Software Guide : EndLatex
    
// Software Guide : BeginCodeSnippet      

    
    typedef InputPixelType LabelPixelType ;
    
    typedef otb::SVMModel< InputPixelType, LabelPixelType > ModelType;

    ModelType::Pointer model = svmEstimator->GetModel();
    //model->LoadModel(outputModelFileName);
// Software Guide : EndCodeSnippet


// Software Guide : BeginLatex
//
// We have now all the elements to create a classifier. The classifier
// is templated over the sample type (the type of the data to be
// classified) and the label type (the type of the ouput of the classifier).
//
// Software Guide : EndLatex
    
// Software Guide : BeginCodeSnippet      
    
    
    typedef otb::SVMClassifier< SampleType, LabelPixelType > ClassifierType ;

    ClassifierType::Pointer classifier = ClassifierType::New() ;

// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We set the classifier parameters : number of classes, SVM model,
// the sample data. And we trigger the classification process by
// calling the \code{Update} method.
//
// Software Guide : EndLatex
    
// Software Guide : BeginCodeSnippet          
    std::cout << "GNC" << std::endl;
    int numberOfClasses = model->GetNumberOfClasses();
    std::cout << "SNC = "<< numberOfClasses << std::endl;
    classifier->SetNumberOfClasses(numberOfClasses) ;
    std::cout << "SM" << std::endl;
    classifier->SetModel( model );
    std::cout << "SS" << std::endl;
    classifier->SetSample(sample.GetPointer()) ;
    std::cout << "Up" << std::endl;
    classifier->Update() ;
    std::cout << "---" << std::endl;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// After the classification step, we usually want to get the
// results. The classifier gives an output under the form of a sample
// list. This list supports the classical STL iterators. Therefore, we
// will create an output image and fill it up with the results of the
// classification. The pixel type of the output image is the same as
// the one used for the labels.
//
// Software Guide : EndLatex
    
// Software Guide : BeginCodeSnippet      
    
    

    typedef ClassifierType::ClassLabelType	            OutputPixelType;
    typedef otb::Image< OutputPixelType, Dimension >        OutputImageType;

    OutputImageType::Pointer outputImage = OutputImageType::New();
    std::cout << "---" << std::endl;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We allocate the memory for the outpu image using the information
// from the input image.
//
// Software Guide : EndLatex
    
// Software Guide : BeginCodeSnippet      
    
    

    typedef itk::Index<Dimension>         myIndexType;
    typedef itk::Size<Dimension>          mySizeType;
    typedef itk::ImageRegion<Dimension>        myRegionType;

    mySizeType size;
    size[0] = cReader->GetOutput()->GetRequestedRegion().GetSize()[0];
    size[1] = cReader->GetOutput()->GetRequestedRegion().GetSize()[1];

    myIndexType start;
    start[0] = 0;
    start[1] = 0;

    myRegionType region;
    region.SetIndex( start );
    region.SetSize( size );

    outputImage->SetRegions( region );
    outputImage->Allocate();
    std::cout << "---" << std::endl;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now declare the interators on the list that we get at the
// output of the classifier as well as the iterator to fill the output image.
//
// Software Guide : EndLatex
    
// Software Guide : BeginCodeSnippet      
    
    
    
    ClassifierType::OutputType* membershipSample =
      classifier->GetOutput() ;
    ClassifierType::OutputType::ConstIterator m_iter =
      membershipSample->Begin() ;
    ClassifierType::OutputType::ConstIterator m_last =
      membershipSample->End() ;

    typedef itk::ImageRegionIterator< OutputImageType>  OutputIteratorType;
    OutputIteratorType  outIt( outputImage,
			   outputImage->GetBufferedRegion() );

    outIt.GoToBegin();


// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We will iterate through the list, get the labels and assign pixel
// values to the output image.
//
// Software Guide : EndLatex
    
// Software Guide : BeginCodeSnippet      
    

    std::cout << "---" << std::endl;
    while (m_iter != m_last && !outIt.IsAtEnd())
    {
    outIt.Set(m_iter.GetClassLabel());
    ++m_iter ;
    ++outIt;
    }
    std::cout << "---" << std::endl;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Only for visualization purposes, we choose to rescale the image of
// classes before sving it to a file. We will use the
// \doxygen{itk::RescaleIntensityImageFilter} for this purpose.
//
// Software Guide : EndLatex
    
// Software Guide : BeginCodeSnippet      
    

    typedef otb::Image< unsigned char, Dimension >        FileImageType;

    
    typedef itk::RescaleIntensityImageFilter< OutputImageType,
      FileImageType > RescalerType;

    RescalerType::Pointer rescaler = RescalerType::New();
    std::cout << "---" << std::endl;
    rescaler->SetOutputMinimum( itk::NumericTraits< unsigned char >::min());
    rescaler->SetOutputMaximum( itk::NumericTraits< unsigned char >::max());

    rescaler->SetInput( outputImage );
    std::cout << "---" << std::endl;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now create an image file writer and save the image.
//
// Software Guide : EndLatex
    
// Software Guide : BeginCodeSnippet      

    typedef otb::ImageFileWriter< FileImageType >         WriterType;
	
    WriterType::Pointer writer = WriterType::New();

    writer->SetFileName( outputImageFileName  );
    writer->SetInput( rescaler->GetOutput() );
    
    writer->Update();
// Software Guide : EndCodeSnippet          



//  Software Guide : BeginLatex
// Figure \ref{fig:SVMCLASSMULTI} shows the result of the SVM classification.
// \begin{figure}
// \center
// \includegraphics[width=0.35\textwidth]{ROI_QB_MUL_1.eps}
// \includegraphics[width=0.35\textwidth]{ROI_QB_MUL_1_SVN_CLASSMULTI.eps}
// \itkcaption[SVM Image Classification]{Result of the SVM
// classification . Left: RGB image. Right: image of classes.} 
// \label{fig:SVMCLASSMULTI}
// \end{figure}
//  Software Guide : EndLatex
    
 


  return EXIT_SUCCESS;
}


