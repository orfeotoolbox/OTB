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
#ifndef __otbImageToVectorImageCastFilter_h
#define __otbImageToVectorImageCastFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkVariableLengthVector.h"

namespace otb
{
namespace Functor
  {
    template <class TInput, class TOutput>
      class VectorCast
      {
      public:
	typedef itk::VariableLengthVector<TOutput> VectorOutputType;
	VectorCast() {};
	~VectorCast(){};

	bool operator!=(const VectorCast&) const
	  {
	    return false; 
	  } 
	bool operator==(const VectorCast& other) const
	  {
	    return(*this != other);
	  }
	inline VectorOutputType operator () (const TInput & A)
	  {
	    VectorOutputType output;
	    output.SetSize(1);
	    output[0] = static_cast<TOutput>(A);
	    return output;
	  }
      };
  } // End namespace Functor
  /**
   * \class ImageToVectorImageCastFilter
   * \brief This is a helper class that convert an otb::Image into a single-channel otb::VectorImage.
   * \ingroup Singlethreaded
   * 
   */
template <class TInputImage, class TOutputVectorImage>
class ITK_EXPORT ImageToVectorImageCastFilter
  : public itk::UnaryFunctorImageFilter<TInputImage, TOutputVectorImage,
  Functor::VectorCast<typename TInputImage::PixelType, typename TOutputVectorImage::ValueType> >
  {
  public: 
    /// Standard class typedefs
    typedef ImageToVectorImageCastFilter Self;
    typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputVectorImage, Functor::VectorCast<typename TInputImage::PixelType,
      typename TOutputVectorImage::ValueType> > Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;
    
    /// Method for creation through the object factory
    itkNewMacro(Self);
    
    /// Runtime information
    itkTypeMacro(ImageToVectorImageCastFilter,itk::UnaryFunctorImageFilter);

  protected:
    /// Constructor
    ImageToVectorImageCastFilter() {}
    /// Detructor
    virtual ~ImageToVectorImageCastFilter() {}
    /// Additionnal output allocation
    virtual void AllocateOutputs(void)
      {
	this->GetOutput()->SetNumberOfComponentsPerPixel(1);
	Superclass::AllocateOutputs();
      }

  private:
    ImageToVectorImageCastFilter(const Self&); //purposely not implemented
    void operator = (const Self&); // purposely not implemented
  };
}// end namespace otb
#endif
