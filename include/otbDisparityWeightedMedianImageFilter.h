#ifndef otbDisparityWeightedMedianImageFilter_h
#define otbDisparityWeightedMedianImageFilter_h

/*===================================================================================================

 Implementation of the Weighted Median of the disparity Map
 The entries: the disparity map, the Left or Right original image
=====================================================================================================*/
#include <utility> 
#include <iterator>

#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"
#include <itkNumericTraits.h>
#include <itkConstNeighborhoodIterator.h>



namespace otb
{


namespace Functor
{


/** \class DisparityWeightMedianImageOperator

 */
template < class TInput, class TOutput >
class DisparityWeightMedianImageOperator
{
public:
typedef typename TInput::OffsetType      OffsetType;
typedef std::pair<int,double> pairCord;
typedef typename TInput::PixelType               PixelType;
public:
  DisparityWeightMedianImageOperator() { }
  virtual ~DisparityWeightMedianImageOperator() { }
  static bool comparePair(pairCord i,pairCord j) { 
	if( i.first < j.first ) return true;
    if( j.first < i.first ) return false;
    return (i.first < j.first ); }
    
//    bool mypred(std::pair<int, double> p1, double s2) { return p1.first == s2; } ;

    
  
 TOutput operator() (  const TInput & input)
  {	 

   // unsigned int Nband = input.GetPixel(0).Size(); // nbr of bands 4
    TOutput output ( 1);
    output.Fill(0); 
     
     
    unsigned int Wsize = input.Size();  //Size of the window
   
   
   	// std::cout << " Nband = "<< Nband; 
	int m_gamma(10);
     OffsetType Offset_j;
	
	
double d,Wtemp = 0., W = 0.,Wt = 0.;
std::vector< std::pair<int,double> > duo(Wsize); 	 

	
PixelType I(3);
						
	for (unsigned int j  = 0; j< Wsize; j++){
		Offset_j = input.GetOffset(j); 			
				I[0] = input.GetCenterPixel()[1]- input.GetPixel(j)[1];
				I[1] = input.GetCenterPixel()[2]- input.GetPixel(j)[2];
				I[2] = input.GetCenterPixel()[3]- input.GetPixel(j)[3];
				
								
				W =  std::exp( -(I.GetSquaredNorm())*m_gamma);
		
		duo[j].first = input.GetPixel(j)[0];               
		duo[j].second =  W; 
		Wt += W	;	
	}
	 Wt = Wt/2;
    // std::cout << "W totale =  " << Wt ;
		
 std::sort (duo.begin(), duo.end(), comparePair);
	
	
    
     for (std::vector<pairCord>::iterator it=duo.begin(); it !=duo.end(); ++it){
       		 Wtemp += it->second ;
       		 if (Wtemp >= Wt){
				 d = it->first; 
				 //std::cout << "Element found in myvector: " << d << '\n';
				 output[0] = static_cast<typename TOutput::ValueType>(d) ;
				 break;
		}	 
     }
     
	 
  return output;
  } // end operator ()

}; // end of functor class  DisparityWeightMedianImageOperator

}  // end of fonctor 


/** \class DisparityWeightedMedianImageFilter  ToutputImage correspond à la carte de disparité filtré 
 *									donc à une seule composante
 *
 * \ingroup OTBImageManipulation
 */
template < class TInputImage, class TOutputImage >
class ITK_EXPORT DisparityWeightedMedianImageFilter
 : public UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::DisparityWeightMedianImageOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >
           
{
public:
  /** Standard class typedefs */
  typedef DisparityWeightedMedianImageFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::DisparityWeightMedianImageOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
 

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
 // itkTypeMacro(DisparityWeightedMedianImageFilter, BinaryFunctorImageFilter);
    /** Creation through object factory macro */
  itkTypeMacro(DisparityWeightedMedianImageFilter, ImageToImageFilter);


protected:

DisparityWeightedMedianImageFilter()
  {

  }
  ~DisparityWeightedMedianImageFilter() ITK_OVERRIDE { }
void GenerateOutputInformation(void) ITK_OVERRIDE;

  DisparityWeightedMedianImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class DisparityWeightedMedianImageFilter


//========
template < class TInputImage, class TOutputImage >
void DisparityWeightedMedianImageFilter < TInputImage, TOutputImage >
::GenerateOutputInformation(void){
 Superclass::GenerateOutputInformation();


 
 this->GetOutput()->SetNumberOfComponentsPerPixel(1);

}

//=======




} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif



