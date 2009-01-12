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
#ifndef __otbProlateInterpolateImageFunction_h
#define __otbProlateInterpolateImageFunction_h

#include "otbGenericInterpolateImageFunction.h"
#include "itkSize.h"
#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"
#include "vnl/algo/vnl_fft_1d.h"
namespace otb
{

namespace Function
{
  /** \class ProlateFunction
   *
   * \brief Prolate function give the value of the prolate profile following
   * one of the 2 image dimension.
   *
   */
template<class TInput, class TOutput>
class ProlateFunction
{
 public:
  ProlateFunction(){ m_Radius = 1; };
  ~ProlateFunction(){};

  typedef std::vector<double> VectorType;
  // Accessors definitions
  void SetRadius(unsigned int rad){ m_Radius = rad; }
  unsigned int GetRadius() const { return m_Radius; }
  unsigned int GetOriginalProfileSize() const { return m_OriginalProfileSize; }
  VectorType GetOriginalProfile() const { return m_OriginalProfile;}
  double ComputeEnergy(double resampleRatio);

  inline TOutput operator()( const TInput & A ) const
    {
      TOutput val = itk::NumericTraits< TOutput >::Zero;
      if ( A != itk::NumericTraits< TInput >::Zero && vcl_abs(A) != static_cast<TInput>(m_Radius) && m_Radius!=0 )
	{
	   double ival = static_cast<double>(m_OriginalProfileSize)*static_cast<double>(vcl_abs(A))/static_cast<double>(m_Radius);
	   double ivalFloor = vcl_floor(ival);
	   double left = ival - ivalFloor;

	   if ( ivalFloor < m_OriginalProfileSize-1 )
	     {
	       val = left*m_OriginalProfile[static_cast<unsigned int>(ivalFloor)] + (1-left)*m_OriginalProfile[static_cast<unsigned int>(ivalFloor)+1];
	     }
	   else
	     {
	       itkGenericExceptionMacro(<<"Out of Profile limits ("<<ivalFloor<<" -1 > 721)");

	     }
	}
      else
	{
	  if ( A == itk::NumericTraits< TInput >::Zero || m_Radius==0)
	    {
	      val = m_OriginalProfile[0];
	    }
	  else
	    {
	      if ( vcl_abs(A) == static_cast<TInput>(m_Radius) )
		{
		  val = m_OriginalProfile[m_OriginalProfileSize-1];
		}
	    }
	}
      return val;
    }

  private:
  /** Useless, only to be compatible with the GenericInterpolateImage. */
  unsigned int m_Radius;
  /** Length of the original profil. */
  static const unsigned int m_OriginalProfileSize;
  /** Original prolate profil */
  static const double m_OriginalProfile[721];

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

    unsigned int GetOriginalProfileSize() const { return this->GetFunction().GetOriginalProfileSize; };
    VectorType GetOriginalProfile() const { return this->GetFunction().GetOriginalProfile();};

    protected:
    ProlateInterpolateImageFunction();
    ~ProlateInterpolateImageFunction();
    void PrintSelf(std::ostream& os, itk::Indent indent) const;

    private:
    ProlateInterpolateImageFunction(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented
  };

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbProlateInterpolateImageFunction.txx"
#endif

#endif
