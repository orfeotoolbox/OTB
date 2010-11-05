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
#ifndef __otbImageFunctionAdapter_h
#define __otbImageFunctionAdapter_h

#include "itkImageFunction.h"
#include "itkNumericTraits.h"

#include "otbFourierMellinDescriptorsImageFunction.h"
#include "otbRealMomentsImageFunction.h"
#include "otbComplexMomentsImageFunction.h"
#include "otbFlusserMomentsImageFunction.h"
#include "otbHuMomentsImageFunction.h"
#include "otbRadiometricMomentsImageFunction.h"
#include "otbLocalHistogramImageFunction.h"

#include <complex>

namespace otb
{


template< class TInputImage, class TInternalImageFunctionType, class TCoordRep = double >
class ITK_EXPORT ImageFunctionAdapterBase :
    public itk::ImageFunction< TInputImage,
                               itk::VariableLengthVector<
                               ITK_TYPENAME itk::NumericTraits<typename TInputImage::PixelType>
                               ::RealType >,
                               TCoordRep >
{
  public:
  // Standard class typedefs. //
  typedef ImageFunctionAdapterBase                                      Self;
  typedef itk::ImageFunction< TInputImage,
                              itk::VariableLengthVector<
                              ITK_TYPENAME itk::NumericTraits<typename TInputImage::PixelType>
                               ::RealType >,
                              TCoordRep >                               Superclass;
  typedef itk::SmartPointer<Self>                                       Pointer;
  typedef itk::SmartPointer<const Self>                                 ConstPointer;

  // Run-time type information (and related methods). //
  itkTypeMacro(ImageFunctionAdapterBase, ImageFunction);

  // Method for creation through the object factory. //
  itkNewMacro(Self);

  // InputImageType typedef support. //
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;
  typedef typename Superclass::OutputType          OutputType;
  typedef typename OutputType::ValueType           OutputValueType;
  
  // Template Partial Specialization Specific typedef //
  typedef TInternalImageFunctionType               InternalImageFunctionType;
  typedef typename InternalImageFunctionType::Pointer       InternalImageFunctionPointerType;
  
  // Dimension of the underlying image. //
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  // Evalulate the function at specified index //
  virtual OutputType EvaluateAtIndex(const IndexType& index) const
  {
    OutputType result;
    
    result.SetSize(1);
    
    result[0] = 0.0;
    
    return result;
  };

  // Evaluate the function at non-integer positions //
  virtual OutputType Evaluate(const PointType& point) const
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  virtual OutputType EvaluateAtContinuousIndex(
    const ContinuousIndexType& cindex) const
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  // Get/Set the internal image function //
  InternalImageFunctionPointerType GetImageFunction() const
  {
    return m_ImageFunction;
  }
  
protected:
  ImageFunctionAdapterBase()
  {
    m_ImageFunction = InternalImageFunctionType::New();
  }
  
  virtual ~ImageFunctionAdapterBase() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "Internal Image Function: " << m_ImageFunction << std::endl;
  }
  
private:
  ImageFunctionAdapterBase(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  // Internal Image Function //
  InternalImageFunctionPointerType    m_ImageFunction;
};


// ----- Template Partial Specialization ----- //

//Dummy
template< class TInputImage, class TInternalImageFunctionType, class TCoordRep = double >
class ITK_EXPORT ImageFunctionAdapter :
    public otb::ImageFunctionAdapterBase< TInputImage, 
                                          TInternalImageFunctionType,
                                          TCoordRep >
{
public:
  typedef ImageFunctionAdapter                                          Self;
  typedef ImageFunctionAdapterBase< TInputImage,
                                    TInternalImageFunctionType,
                                    TCoordRep >                         Superclass;
  typedef itk::SmartPointer<Self>                                       Pointer;
  typedef itk::SmartPointer<const Self>                                 ConstPointer;
  itkTypeMacro(ImageFunctionAdapter, ImageFunction);
  itkNewMacro(Self);
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::OutputType          OutputType;
  typedef typename Superclass::InternalImageFunctionType               InternalImageFunctionType;

  virtual OutputType EvaluateAtIndex(const IndexType& index) const;

protected:
  ImageFunctionAdapter() {}
  virtual ~ImageFunctionAdapter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageFunctionAdapter(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented  
};


//FourierMellinDescriptors
template< class TInputImage, class TCoordRep >
class ITK_EXPORT ImageFunctionAdapter< TInputImage, 
                                       otb::FourierMellinDescriptorsImageFunction<TInputImage, TCoordRep>,
                                       TCoordRep > :
    public otb::ImageFunctionAdapterBase< TInputImage, 
                                          FourierMellinDescriptorsImageFunction<
                                          TInputImage,
                                          TCoordRep >,
                                          TCoordRep >
{
public:
  // Standard class typedefs. //
  typedef ImageFunctionAdapter                                          Self;
  typedef ImageFunctionAdapterBase< TInputImage,
                                    FourierMellinDescriptorsImageFunction<
                                    TInputImage,
                                    TCoordRep >,
                                    TCoordRep >                         Superclass;
  typedef itk::SmartPointer<Self>                                       Pointer;
  typedef itk::SmartPointer<const Self>                                 ConstPointer;
  // Run-time type information (and related methods). //
  itkTypeMacro(ImageFunctionAdapter, ImageFunction);
  // Method for creation through the object factory. //
  itkNewMacro(Self);
  // Usefull typedefs //
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::OutputType          OutputType;
  // Template Partial Specialization Specific typedef //
  typedef typename Superclass::InternalImageFunctionType               InternalImageFunctionType;

  // Evalulate the function at specified index //
  virtual OutputType EvaluateAtIndex(const IndexType& index) const;

protected:
  ImageFunctionAdapter() {}
  virtual ~ImageFunctionAdapter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageFunctionAdapter(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented  
};

//RealMoments
template< class TInputImage, class TCoordRep >
class ITK_EXPORT ImageFunctionAdapter< TInputImage,
                                       otb::RealMomentsImageFunction<TInputImage, TCoordRep>,
                                       TCoordRep > :
    public otb::ImageFunctionAdapterBase< TInputImage,
                                          RealMomentsImageFunction<
                                          TInputImage,
                                          TCoordRep >,
                                          TCoordRep >
{
public:
  // Standard class typedefs. //
  typedef ImageFunctionAdapter                                          Self;
  typedef ImageFunctionAdapterBase< TInputImage,
                                    RealMomentsImageFunction<
                                    TInputImage,
                                    TCoordRep >,
                                    TCoordRep >                         Superclass;
  typedef itk::SmartPointer<Self>                                       Pointer;
  typedef itk::SmartPointer<const Self>                                 ConstPointer;
  // Run-time type information (and related methods). //
  itkTypeMacro(ImageFunctionAdapter, ImageFunction);
  // Method for creation through the object factory. //
  itkNewMacro(Self);
  // Usefull typedefs //
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::OutputType          OutputType;
  // Template Partial Specialization Specific typedef //
  typedef typename Superclass::InternalImageFunctionType               InternalImageFunctionType;

  // Evalulate the function at specified index //
  virtual OutputType EvaluateAtIndex(const IndexType& index) const;

protected:
  ImageFunctionAdapter() {}
  virtual ~ImageFunctionAdapter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageFunctionAdapter(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented
};

//ComplexMoments
template< class TInputImage, class TCoordRep >
class ITK_EXPORT ImageFunctionAdapter< TInputImage,
                                       otb::ComplexMomentsImageFunction< TInputImage, TCoordRep>,
                                       TCoordRep > :
    public otb::ImageFunctionAdapterBase< TInputImage,
                                          ComplexMomentsImageFunction<
                                          TInputImage,
                                          TCoordRep >,
                                          TCoordRep >
{
public:
  // Standard class typedefs. //
  typedef ImageFunctionAdapter                                          Self;
  typedef ImageFunctionAdapterBase< TInputImage,
                                    ComplexMomentsImageFunction<
                                    TInputImage,
                                    TCoordRep >,
                                    TCoordRep >                         Superclass;
  typedef itk::SmartPointer<Self>                                       Pointer;
  typedef itk::SmartPointer<const Self>                                 ConstPointer;
  // Run-time type information (and related methods). //
  itkTypeMacro(ImageFunctionAdapter, ImageFunction);
  // Method for creation through the object factory. //
  itkNewMacro(Self);
  // Usefull typedefs //
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::OutputType          OutputType;
  // Template Partial Specialization Specific typedef //
  typedef typename Superclass::InternalImageFunctionType               InternalImageFunctionType;

  // Evalulate the function at specified index //
  virtual OutputType EvaluateAtIndex(const IndexType& index) const;

protected:
  ImageFunctionAdapter() {}
  virtual ~ImageFunctionAdapter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageFunctionAdapter(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented
};

//FlusserMoments
template< class TInputImage, class TCoordRep >
class ITK_EXPORT ImageFunctionAdapter< TInputImage,
                                       otb::FlusserMomentsImageFunction< TInputImage, TCoordRep>,
                                       TCoordRep > :
    public otb::ImageFunctionAdapterBase< TInputImage, 
                                          FlusserMomentsImageFunction<
                                          TInputImage,
                                          TCoordRep >,
                                          TCoordRep >
{
public:
  // Standard class typedefs. //
  typedef ImageFunctionAdapter                                          Self;
  typedef ImageFunctionAdapterBase< TInputImage,
                                    FlusserMomentsImageFunction<
                                    TInputImage,
                                    TCoordRep >,
                                    TCoordRep >                         Superclass;
  typedef itk::SmartPointer<Self>                                       Pointer;
  typedef itk::SmartPointer<const Self>                                 ConstPointer;
  // Run-time type information (and related methods). //
  itkTypeMacro(ImageFunctionAdapter, ImageFunction);
  // Method for creation through the object factory. //
  itkNewMacro(Self);
  // Usefull typedefs //
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::OutputType          OutputType;
  // Template Partial Specialization Specific typedef //
  typedef typename Superclass::InternalImageFunctionType               InternalImageFunctionType;

  // Evalulate the function at specified index //
  virtual OutputType EvaluateAtIndex(const IndexType& index) const;

protected:
  ImageFunctionAdapter() {};
  virtual ~ImageFunctionAdapter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageFunctionAdapter(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented
};

//HuMoments
template< class TInputImage, class TCoordRep >
class ITK_EXPORT ImageFunctionAdapter< TInputImage,
                                       otb::HuMomentsImageFunction< TInputImage, TCoordRep>,
                                       TCoordRep > :
    public otb::ImageFunctionAdapterBase< TInputImage,
                                          HuMomentsImageFunction<
                                          TInputImage,
                                          TCoordRep >,
                                          TCoordRep >
{
public:
  // Standard class typedefs. //
  typedef ImageFunctionAdapter                                          Self;
  typedef ImageFunctionAdapterBase< TInputImage,
                                    HuMomentsImageFunction<
                                    TInputImage,
                                    TCoordRep >,
                                    TCoordRep >                         Superclass;
  typedef itk::SmartPointer<Self>                                       Pointer;
  typedef itk::SmartPointer<const Self>                                 ConstPointer;
  // Run-time type information (and related methods). //
  itkTypeMacro(ImageFunctionAdapter, ImageFunction);
  // Method for creation through the object factory. //
  itkNewMacro(Self);
  // Usefull typedefs //
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::OutputType          OutputType;
  // Template Partial Specialization Specific typedef //
  typedef typename Superclass::InternalImageFunctionType               InternalImageFunctionType;

  // Evalulate the function at specified index //
  virtual OutputType EvaluateAtIndex(const IndexType& index) const;

protected:
  ImageFunctionAdapter() {}
  virtual ~ImageFunctionAdapter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageFunctionAdapter(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented
};

//RadiometricMoments
template< class TInputImage, class TCoordRep >
class ITK_EXPORT ImageFunctionAdapter< TInputImage,
                                       otb::RadiometricMomentsImageFunction< TInputImage, TCoordRep>,
                                       TCoordRep > :
    public otb::ImageFunctionAdapterBase< TInputImage,
                                          RadiometricMomentsImageFunction<
                                          TInputImage,
                                          TCoordRep >,
                                          TCoordRep >
{
public:
  // Standard class typedefs. //
  typedef ImageFunctionAdapter                                          Self;
  typedef ImageFunctionAdapterBase< TInputImage,
                                    RadiometricMomentsImageFunction<
                                    TInputImage,
                                    TCoordRep >,
                                    TCoordRep >                         Superclass;
  typedef itk::SmartPointer<Self>                                       Pointer;
  typedef itk::SmartPointer<const Self>                                 ConstPointer;
  // Run-time type information (and related methods). //
  itkTypeMacro(ImageFunctionAdapter, ImageFunction);
  // Method for creation through the object factory. //
  itkNewMacro(Self);
  // Usefull typedefs //
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::OutputType          OutputType;
  // Template Partial Specialization Specific typedef //
  typedef typename Superclass::InternalImageFunctionType               InternalImageFunctionType;

  // Evalulate the function at specified index //
  virtual OutputType EvaluateAtIndex(const IndexType& index) const;

protected:
  ImageFunctionAdapter() {}
  virtual ~ImageFunctionAdapter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageFunctionAdapter(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented
};


//LocalHistogram
template< class TInputImage, class TCoordRep >
class ITK_EXPORT ImageFunctionAdapter< TInputImage,
                                       otb::LocalHistogramImageFunction< TInputImage, TCoordRep>,
                                       TCoordRep > :
    public otb::ImageFunctionAdapterBase< TInputImage,
                                          LocalHistogramImageFunction<
                                          TInputImage,
                                          TCoordRep >,
                                          TCoordRep >
{
public:
  // Standard class typedefs. //
  typedef ImageFunctionAdapter                                          Self;
  typedef ImageFunctionAdapterBase< TInputImage,
                                    LocalHistogramImageFunction<
                                    TInputImage,
                                    TCoordRep >,
                                    TCoordRep >                         Superclass;
  typedef itk::SmartPointer<Self>                                       Pointer;
  typedef itk::SmartPointer<const Self>                                 ConstPointer;
  // Run-time type information (and related methods). 
  itkTypeMacro(ImageFunctionAdapter, ImageFunction);
  // Method for creation through the object factory. 
  itkNewMacro(Self);
  // Usefull typedefs //
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::OutputType          OutputType;
  // Template Partial Specialization Specific typedef
  typedef typename Superclass::InternalImageFunctionType               InternalImageFunctionType;

  // Evalulate the function at specified index //
  virtual OutputType EvaluateAtIndex(const IndexType& index) const;

protected:
  ImageFunctionAdapter() {}
  virtual ~ImageFunctionAdapter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageFunctionAdapter(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFunctionAdapter.txx"
#endif

#endif
