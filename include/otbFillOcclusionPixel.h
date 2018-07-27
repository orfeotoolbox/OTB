#ifndef otbFillOcclusionPixel_h
#define otbFillOcclusionPixel_h

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


/** \class FillPixelFilterOperator

 */
template < class TInput, class TOutput >
class FillPixelFilterOperator
{
public:
typedef typename TInput::OffsetType      OffsetType;
typedef std::pair<int,double> pairCord;
typedef typename TInput::PixelType               PixelType;
public:
  FillPixelFilterOperator() { }
  virtual ~FillPixelFilterOperator() { }
  static bool comparePair(pairCord i,pairCord j) { 
  if( i.first < j.first ) return true;
    if( j.first < i.first ) return false;
    return (i.first < j.first ); }
    
//    bool mypred(std::pair<int, double> p1, double s2) { return p1.first == s2; } ;

    
  
 TOutput operator() (  const TInput & input)
  {  

   // unsigned int Nband = input.GetPixel(0).Size(); // nbr de bande 4
    TOutput output (1);
   
    unsigned int Wsize = input.Size();



       // std::cout << " Nband = "<< Nband; 
  double sSpace = 9;
  double sColor = 255*0.1;
  
   sSpace = 1.0f/(sSpace*sSpace);
   sColor = 1.0f/(sColor*sColor);
     OffsetType Offset_j;
  
  
double d,Wtemp = 0., W = 0.,Wt = 0.;
std::vector< std::pair<int,double> > duo(Wsize);   

  
PixelType I(3);

   if(input.GetCenterPixel()[4] == 0.0)
       {

          for (unsigned int j  = 0; j< Wsize; j++){
    Offset_j = input.GetOffset(j);      
        I[0] = input.GetCenterPixel()[1]- input.GetPixel(j)[1];
        I[1] = input.GetCenterPixel()[2]- input.GetPixel(j)[2];
        I[2] = input.GetCenterPixel()[3]- input.GetPixel(j)[3];

      //  std::cout << input[0][j] << std::endl ;

       // std::cout << input.GetPixel(j)[0] <<  " -- " << Offset_j[0] << std::endl ;
        
                
        W =  std::exp( - (std::pow(Offset_j[0],2.0)+std::pow(Offset_j[1],2.0)) *sSpace 
        -(I.GetSquaredNorm())*sColor);

     //   W = std::exp( - (std::pow(Offset_j[0],2.0))*sSpace) * std::exp(-(I.GetSquaredNorm())*sColor);
        
      //  - std::pow((double)(input.GetCenterPixel()[1]- input.GetPixel(j)[1]),2.0) *sColor);
        
        
              
    duo[j].first = input.GetPixel(j)[0];               
    duo[j].second =  W; 
    Wt += W ; 
  }
   Wt = Wt/2;
    // std::cout << "W totale =  " << Wt ;
    
 std::sort (duo.begin(), duo.end(), comparePair);
  
  
    
     for (std::vector<pairCord>::iterator it=duo.begin(); it !=duo.end(); ++it){
           Wtemp += it->second ;
           if (Wtemp >= Wt){

        d = it->first; 
         //std::cout << "Element found in myvector: " << d << '\n';
         // output[0] = static_cast<typename TOutput::ValueType>(d) ;
        output = d ;
         break;
    }   
  
   
     }  


       }
       else
       {
         output = input.GetCenterPixel()[0] ;

       }






   
  return output;
  } // end operator ()

}; // end of functor class  FillPixelFilterOperator

}  // end of fonctor 


/** \class FillPixelFilter  ToutputImage correspond Ã  la carte de disparitÃ© filtrÃ© 
 *                  donc Ã  une seule composante
 *
 * \ingroup OTBImageManipulation
 */
template < class TInputImage, class TOutputImage >
class ITK_EXPORT FillPixelFilter
 : public UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::FillPixelFilterOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >
           
{
public:
  /** Standard class typedefs */
  typedef FillPixelFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::FillPixelFilterOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
 

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
 // itkTypeMacro(FillPixelFilter, BinaryFunctorImageFilter);
    /** Creation through object factory macro */
  itkTypeMacro(FillPixelFilter, ImageToImageFilter);


protected:

FillPixelFilter()
  {

  }
  ~FillPixelFilter() ITK_OVERRIDE { }
void GenerateOutputInformation(void) ITK_OVERRIDE;

  FillPixelFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class FillPixelFilter


//========
template < class TInputImage, class TOutputImage >
void FillPixelFilter < TInputImage, TOutputImage >
::GenerateOutputInformation(void){

 Superclass::GenerateOutputInformation();

 
 this->GetOutput()->SetNumberOfComponentsPerPixel(1);

}

//=======




} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION

#endif

#endif




