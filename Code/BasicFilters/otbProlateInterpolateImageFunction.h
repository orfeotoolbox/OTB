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
#ifndef _otbProlateInterpolateImageFunction_h
#define _otbProlateInterpolateImageFunction_h

#include "otbGenericInterpolateImageFunction.h"
#include "itkSize.h"

namespace otb
{
  
namespace Function 
{
/**
 * Prolate function give the value of the prolate profil following 
 * one of the 2 image dimension. 
 */
template<class TInput, class TOutput>
class ProlateFunction
{
  public:
  typedef typename std::vector<double> VectorType;


  // Accessors definitions
  void SetRadius(unsigned int rad){ m_Radius = rad; };
  unsigned int GetRadius() const { return m_Radius; };
  unsigned int GetOriginalProfilSize() const { return m_OriginalProfilSize; };
  VectorType GetOriginalProfil() const { return m_OriginalProfil;};

  inline TOutput operator()( const TInput & A ) const
    { 
      TOutput val = 0.;
      if (m_Radius != 0)
	{
	  unsigned int ival = static_cast<unsigned int>(m_OriginalProfilSize*static_cast<double>(vcl_abs(A))/static_cast<double>(m_Radius));
	  val = m_OriginalProfil[ival];
	}
      else
	{
	  val = m_OriginalProfil[0];
	}
      return val;
    }

  private:
  /** Useless, only to be compatible with the GenericInterpolateImage. */
  unsigned int m_Radius;
  /** Length of the original profil. */
  static const unsigned int m_OriginalProfilSize;
  /** Original prolate profil */
  static const double m_OriginalProfil[721]; 

}; 
 
}//namespace Function

/** \class ProlateInterpolateImageFunction
 * \brief Prolate interpolation of an otb::image.
 *
 * ProlateInterpolateImageFunction interpolates image intensity according to a 
 * resampling profil.
 * \ingroup ImageFunctions ImageInterpolators
 */
template<class TInputImage, class TBoundaryCondition = itk::ConstantBoundaryCondition<TInputImage>, class TCoordRep=double, class TInputInterpolator=double, class TOutputInterpolator=double>
class ITK_EXPORT ProlateInterpolateImageFunction : 
public GenericInterpolateImageFunction< TInputImage, 
                                        ITK_TYPENAME Function::ProlateFunction< TInputInterpolator, TOutputInterpolator>, 
                                        TBoundaryCondition, 
                                        TCoordRep > 
  {
    public:
    /** Standard class typedefs. */
    typedef ProlateInterpolateImageFunction                                                               Self;
    typedef GenericInterpolateImageFunction<TInputImage, 
                                            Function::ProlateFunction< TInputInterpolator, TOutputInterpolator>, 
                                            TBoundaryCondition, 
                                            TCoordRep>                                                    Superclass;
    typedef itk::SmartPointer<Self>                                                                            Pointer;
    typedef itk::SmartPointer<const Self>                                                                      ConstPointer;
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(ProlateInterpolateImageFunction, GenericInterpolateImageFunction);
    
    /** Method for creation through the object factory. */
    itkNewMacro(Self);  
    
    /** Input and output images typedef definition. */
    typedef typename Superclass::InputImageType InputImageType;
    typedef typename Superclass::OutputType     OutputType;
    typedef TInputInterpolator                  InputInterpolatorType;
    typedef TOutputInterpolator                 OutputInterpolatorType;

    /** Dimension underlying input image. */
    itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);
    
    /** Superclass typedef inheritance. */
    typedef typename Superclass::IndexType                                             IndexType;
    typedef typename Superclass::SizeType                                              SizeType;
    typedef typename Superclass::RealType                                              RealType;
    typedef Function::ProlateFunction<InputInterpolatorType, OutputInterpolatorType>   FunctionType;
    typedef typename Superclass::IteratorType                                          IteratorType;
    typedef typename Superclass::ContinuousIndexType                                   ContinuousIndexType;
    typedef typename std::vector<double>                                               VectorType;
   
    unsigned int GetOriginalProfilSize() const { return this->GetFunction().GetOriginalProfilSize; };
    VectorType GetOriginalProfil() const { return this->GetFunction().GetOriginalProfil();};
    
   
    protected:
    ProlateInterpolateImageFunction();
    ~ProlateInterpolateImageFunction();
    void PrintSelf(std::ostream& os, itk::Indent indent) const;

    private:
    ProlateInterpolateImageFunction(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented  
  };

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbProlateInterpolateImageFunction.txx"
#endif

#endif
