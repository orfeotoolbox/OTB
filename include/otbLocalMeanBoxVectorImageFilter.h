
#ifndef otbLocalMeanBoxVectorImageFilter_h
#define otbLocalMeanBoxVectorImageFilter_h


/*===================================================================================================

implémentation of local mean using Summed areas 

=====================================================================================================*/


#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"
#include "otbBinaryFunctorNeighborhoodVectorImageFilter.h"
#include <itkNumericTraits.h>
#include <itkConstNeighborhoodIterator.h>

namespace otb
{

namespace Functor
{
	
	
/** \class BoxOperator
 * \ingroup 
 */
 
template < class TInput, class TOutput >
class LocalMeanBoxOperator

{
public:
  LocalMeanBoxOperator() { }
  virtual ~LocalMeanBoxOperator() { }

  
 
  TOutput operator() ( const TInput & input )
  {
	
	typename TInput::RadiusType r_Box = input.GetRadius();  
	unsigned int r_Mean = r_Box[0]-1;
	unsigned int size_Mean = 2*r_Mean+1;
	
	//std::cout<< "r_Box" << r_Box[0] << std::endl;

    unsigned int Nband =  input.GetPixel(0).Size(); // nbr de bande
    TOutput output ( Nband );
    output.Fill(0);
     
 

typename TInput::OffsetType  Offset1 = {{r_Mean,r_Mean}};
typename TInput::OffsetType  Offset2 = {{-(r_Mean+1),-(r_Mean+1)}};
typename TInput::OffsetType  Offset3 = {{r_Mean,-(r_Mean+1)}};
typename TInput::OffsetType  Offset4 = {{-(r_Mean+1),r_Mean}};

typename TInput::IndexType index;
index =input.GetIndex();
 
    
  /*  ============ Calcul de la moyenne ==================================*/
    for ( unsigned int b = 0; b < Nband; ++b ){ 
	double somme(0.);
	
						
		somme = input.GetPixel(Offset1)[b] + input.GetPixel(Offset2)[b] 
				-input.GetPixel(Offset3)[b] - input.GetPixel(Offset4)[b] ;               						
			
			somme /= size_Mean*size_Mean;			
			output[b] = static_cast<typename TOutput::ValueType>(somme) ;
			
				//~ if((index[0] == 100 && index[1] == 97)){
					
					//~ ///refaire le calcul de la moyenne  pour cette ligne
				//~ std::cout << "input Image =  " << input.GetPixel(Offset1)[b]<< std::endl;
				//~ std::cout << "Output Image =  " << input.GetPixel(Offset2)[b]  << std::endl;				
				//~ std::cout << "Output Image =  " <<input.GetPixel(Offset3)[b]<< std::endl;
				//~ std::cout << "Output Image =  " <<input.GetPixel(Offset4)[b]<< std::endl;
				//~ std::cout << "Moyenne =  " << somme << std::endl;			
				
			//~ }
			

	}
    return output;
    
}  // end of operator ()  
  
}; // end of class  LocalMeanBoxOperator
 
}  // end of fonctor 


/** \class LocalMeanBoxVectorImageFilter
 *
 *
 * \ingroup 
 */
 
 // ImageRegionConstIterator
template < class TInputImage, class TOutputImage >
class ITK_EXPORT LocalMeanBoxVectorImageFilter
  : public UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::LocalMeanBoxOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >
{
public:
  /* Standard class typedefs */
  typedef LocalMeanBoxVectorImageFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::LocalMeanBoxOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >  Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  
  typedef typename TInputImage::RegionType RegionType;
  
 typedef itk::ConstNeighborhoodIterator<TInputImage> NeighborhoodIteratorType;
 typedef typename NeighborhoodIteratorType::RadiusType   RadiusType;

   
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(LocalMeanBoxVectorImageFilter, ImageToImageFilter);

protected:
  LocalMeanBoxVectorImageFilter()  
  {
	//  this->SetNumberOfThreads(1);
  }
  ~LocalMeanBoxVectorImageFilter() ITK_OVERRIDE { }
  
//=================== Overload of "ThreadedGenerateData" To enlarge the Neighborhood with the radius ===================== 
void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE{


RadiusType R=this->GetRadius() ;
RadiusType Radius;
Radius[0] = R[0]+1;
Radius[1] = R[1]+1;

  RegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );

  NeighborhoodIteratorType neighInputIt ( Radius, this->GetInput(),inputRegionForThread );
  
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;
  neighInputIt.OverrideBoundaryCondition(&nbc);
  neighInputIt.GoToBegin();

  itk::ImageRegionIterator<TOutputImage> outputIt (  this->GetOutput(), outputRegionForThread );
  outputIt.GoToBegin();

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  while ( !outputIt.IsAtEnd() && !neighInputIt.IsAtEnd() ){
	 
    outputIt.Set( this->GetFunctor()(( neighInputIt ) ));

    ++neighInputIt;
    ++outputIt;

    progress.CompletedPixel();
  }
}
//=================== Overload of "GenerateInputRequestedRegion" To enlarge the RequestedRegion with the radius =====================
void GenerateInputRequestedRegion() ITK_OVERRIDE{
Superclass::GenerateInputRequestedRegion();

 // get pointers to the input and output
  typename TInputImage::Pointer inputPtr =  const_cast<TInputImage *>(this->GetInput());
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

RadiusType R=this->GetRadius() ;
RadiusType Radius;
Radius[0] = R[0]+1;
Radius[1] = R[1]+1;
 if (!inputPtr || !outputPtr)
    {
    return;
    }
  // get a copy of the input requested region (should equal the output
  // requested region)
  
  RegionType inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius(Radius);

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    return;
    }
  else
    {

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << this->GetNameOfClass()
        << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }

}

private:
  LocalMeanBoxVectorImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class LocalMeanBoxVectorImageFilter


} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif




