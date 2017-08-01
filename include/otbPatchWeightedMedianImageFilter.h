#ifndef otbPatchWeightedMedianImageFilter_h
#define otbPatchWeightedMedianImageFilter_h

/*===================================================================================================

 Implementation of the Weighted Median of palanes
 The entries: Coefs of planes, the Left or Right original image
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


/** \class WeightPatchMedianImageOperator

 */
template < class TInput, class TOutput >
class WeightPatchMedianImageOperator
{
//~ private:
//~ int m_gamma;


public:
typedef typename TInput::OffsetType      OffsetType;
typedef std::pair<double,double> pairCord;
typedef typename TInput::PixelType               PixelType;

public:
  WeightPatchMedianImageOperator() { }
   virtual ~WeightPatchMedianImageOperator() { }
  static bool comparePair(pairCord i,pairCord j) { 
	if( i.first < j.first ) return true;
    if( j.first < i.first ) return false;
    return (i.first < j.first ); }
    


    
  
 TOutput operator() (TInput & input)
  {	 

   // unsigned int Nband = input.GetPixel(0).Size(); // nbr de bande 4
    TOutput output ( 3);
    output.Fill(0); 
     
     
    unsigned int Wsize = input.Size();  //taille de la fenetre ipol 19*19 ;
   
     OffsetType Offset_j;
	int m_gamma(10);
	
double p, W = 0.,Wt = 0.;
std::vector< std::pair<double,double> > duoAf(Wsize); 
std::vector< std::pair<double,double> > duoBf(Wsize);
std::vector< std::pair<double,double> > duoCf(Wsize);	 

std::vector<double> Wtemp;//(3);
Wtemp.resize(3,0);	
PixelType I(3);
						
	for (unsigned int j  = 0; j< Wsize; j++){
		Offset_j = input.GetOffset(j); 			
				I[0] = input.GetCenterPixel()[3]- input.GetPixel(j)[3];
				I[1] = input.GetCenterPixel()[4]- input.GetPixel(j)[4];
				I[2] = input.GetCenterPixel()[5]- input.GetPixel(j)[5];
				
								
				W =  std::exp(-(I.GetSquaredNorm())*m_gamma);
				
			//	- std::pow((double)(input.GetCenterPixel()[1]- input.GetPixel(j)[1]),2.0) *sColor);
				
				
							
		duoAf[j].first = input.GetPixel(j)[0];               
		duoAf[j].second =  W; 
		
		duoBf[j].first = input.GetPixel(j)[1];               
		duoBf[j].second =  W;
		
		duoCf[j].first = input.GetPixel(j)[2];               
		duoCf[j].second =  W;
		
		Wt += W	;	
	}
	 Wt = Wt/2;
    // std::cout << "W totale =  " << Wt ;
		
 std::sort (duoAf.begin(), duoAf.end(), comparePair);
 std::sort (duoBf.begin(), duoBf.end(), comparePair);
 std::sort (duoCf.begin(), duoCf.end(), comparePair);
	
	std::vector<pairCord>::iterator itAf, itBf,itCf;
    
     for ( itAf=duoAf.begin(),itBf=duoBf.begin(),itCf=duoCf.begin(); 
			itAf !=duoAf.end(),itBf !=duoBf.end(),itCf !=duoCf.end(); ++itAf,++itBf,++itCf){
				
       		 Wtemp[0] += itAf->second ;
       		 Wtemp[1] += itBf->second ;
       		 Wtemp[2] += itCf->second ;
       		 if (Wtemp[0] >= Wt){
				 p = itAf->first; 
				 //std::cout << "Element found in myvector: " << d << '\n';
				 output[0] = static_cast<typename TOutput::ValueType>(p) ;
				 break;
			 }
				 
			 if (Wtemp[1] >= Wt){
				 p = itBf->first; 
				 //std::cout << "Element found in myvector: " << d << '\n';
				 output[1] = static_cast<typename TOutput::ValueType>(p) ;
				 break;
				} 
			 if (Wtemp[2] >= Wt){
				 p = itCf->first; 
				 //std::cout << "Element found in myvector: " << d << '\n';
				 output[2] = static_cast<typename TOutput::ValueType>(p) ;
				 break;
				} 
			
		}		
	
	 
     
     
	 
  return output;
  } // end operator ()

}; // end of functor class  WeightPatchMedianImageOperator

}  // end of fonctor 


/** \class PatchWeightedMedianImageFilter  ToutputImage correspond à la carte de disparité filtré 
 *									donc à une seule composante
 *
 * \ingroup OTBImageManipulation
 */
template < class TInputImage, class TOutputImage >
class ITK_EXPORT PatchWeightedMedianImageFilter
 : public UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::WeightPatchMedianImageOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >
           
{
public:
  /** Standard class typedefs */
  typedef PatchWeightedMedianImageFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::WeightPatchMedianImageOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
 

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
 // itkTypeMacro(WeightMedianImageFilter, BinaryFunctorImageFilter);
    /** Creation through object factory macro */
  itkTypeMacro(PatchWeightedMedianImageFilter, ImageToImageFilter);


protected:

PatchWeightedMedianImageFilter()
  {

  }
  ~PatchWeightedMedianImageFilter() ITK_OVERRIDE { }
void GenerateOutputInformation(void) ITK_OVERRIDE;

  PatchWeightedMedianImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class PatchWeightedMedianImageFilter


//========
template < class TInputImage, class TOutputImage >
void PatchWeightedMedianImageFilter < TInputImage, TOutputImage >
::GenerateOutputInformation(void){
 Superclass::GenerateOutputInformation();


 
 this->GetOutput()->SetNumberOfComponentsPerPixel(3);

}

//=======




} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif



