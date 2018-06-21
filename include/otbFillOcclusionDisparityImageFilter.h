#ifndef otbFillOcclusionDisparityImageFilter_h
#define otbFillOcclusionDisparityImageFilter_h

/*===================================================================================================

Implementation of the Occlusion Zone Filling, Using 
Left or Right Disparity Maps and the Occlusion Mask

=====================================================================================================*/


#include <otbBinaryFunctorNeighborhoodVectorImageFilter.h>
//#include <itkNumericTraits.h>
#include <itkConstNeighborhoodIterator.h>
#include <algorithm> //std::max_element



namespace otb
{

namespace Functor
{

 
/** \class FillOcclusionImageOperator

 */
template < class TInput1, class TInput2, class TOutput >
class FillOcclusion
{
public:
  FillOcclusion() {}
  virtual ~FillOcclusion() {} 

    int  GetNumberOfComponentsPerPixel() const
    {
    return m_numberOfComponents;   
    }


  void SetNumberOfComponent( unsigned int nb)
  {
    m_numberOfComponents = nb;
  }


   unsigned char GetRadiusMin(void)
    {
     return m_RadiusMax;     
    }

  unsigned char GetRadiusMax(void)
    {
    return m_RadiusMin;
    }

  void SetRadius(const unsigned char& min, const unsigned char& max)
    {
      m_RadiusMin = min < max ? min : max;
      m_RadiusMax = max > min ? max : min;
    }

  void SetDispMax( int disp)
    {
       m_dispMax = disp ;
    }

  int GetDispMax()
    {
      return m_dispMax ;
    }

  void SetDispMin( int disp)
    {
       m_dispMin = disp ;
    }

   int GetDispMin()
    {
      return m_dispMin ;
    }




  
 TOutput operator() (const TInput1 & input1, const TInput2 & input2)
  {
  //input1 : occlusion map
  //input 2 : left disparityMap
  TOutput output(1); 
  output=0; 

  double tmp(0.);
  std::vector<double> tmp_vector;
  tmp_vector.resize(input1.Size(),0);
  int grayMin = 255;
  int grayMax = 0 ; 

  for(unsigned int i=0; i<input2.Size(); i++)
    {
    tmp_vector[i] = input2.GetPixel(i);
    }

  for(unsigned int i=0;i<input1.Size(); i++ )
    {
    if(input1.GetPixel(i)==255)
      {
      tmp = input2.GetPixel(i);
      }
    else
      {
      tmp = *std::max_element(tmp_vector.begin(),tmp_vector.end())  ; 
      }
    }
  output = tmp ;

  return output ;




	
  
  } // end operator ()

protected:
  unsigned char                   m_RadiusMin;
  unsigned char                   m_RadiusMax;
  int                             m_dispMin;
  int                             m_dispMax;
  unsigned int                    m_numberOfComponents ;

}; // end of functor class  FillOcclusion operator

}  // end of fonctor 


 
 
template < class TInputImage1, class TInputImage2, class TOutputImage  >
class ITK_EXPORT FillOcclusionDisparityImageFilter
 : public BinaryFunctorNeighborhoodVectorImageFilter<TInputImage1, TInputImage2, TOutputImage,
            Functor::FillOcclusion<
              typename itk::ConstNeighborhoodIterator<TInputImage1>,
              typename itk::ConstNeighborhoodIterator<TInputImage2>,
              typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs */
  typedef FillOcclusionDisparityImageFilter Self;
  typedef BinaryFunctorNeighborhoodVectorImageFilter< TInputImage1, TInputImage2, TOutputImage,
            Functor::FillOcclusion<
               typename itk::ConstNeighborhoodIterator<TInputImage1>,
               typename itk::ConstNeighborhoodIterator<TInputImage2>,
               typename TOutputImage::PixelType > >  Superclass;
              
              
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

    /** Creation through object factory macro */
  itkTypeMacro(FillOcclusionDisparityImageFilter, ImageToImageFilter);

   int GetDispMax()
    {
     return this->GetFunctor().GetDispMax();     
    }

  int GetDispMin()
    {
     return this->GetFunctor().GetDispMin();     
    }

  void SetDispMax(int disp)
    {
      this->GetFunctor().SetDispMax(disp);
    }

  void SetDispMin(int disp)
    {
      this->GetFunctor().SetDispMin(disp);
    }


protected:

FillOcclusionDisparityImageFilter()
  {
  }
  ~FillOcclusionDisparityImageFilter() ITK_OVERRIDE { }

    void GenerateOutputInformation(void) override
    {
    Superclass::GenerateOutputInformation();
    unsigned int nb_comp (this->GetInput(1)->GetNumberOfComponentsPerPixel());
    this->GetOutput()->SetNumberOfComponentsPerPixel(nb_comp);
    this->GetFunctor().SetNumberOfComponent(nb_comp);
    } 

  private:

  FillOcclusionDisparityImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class FillOcclusionDisparityImageFilter


} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif




