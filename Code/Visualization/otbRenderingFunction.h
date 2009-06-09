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
#ifndef __otbRenderingFunction_h
#define __otbRenderingFunction_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkVariableLengthVector.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "otbPixelRepresentationFunction.h"
#include "otbChannelSelectorFunctor.h"

namespace otb
{
namespace Function
{
/** \class Identiy
* \brief Default math functor parameter for rendering function.
*  \ingroup Visualization
 */
template <class TInputPixel, class TOutputPixel>
class Identity
{
public:
  Identity(){};
  ~Identity(){};
  bool operator !=(const Identity &) const
  {
    return false;
  }
  bool operator ==(const Identity & other) const
  {
    return !(*this != other);
  }

  inline TOutputPixel operator()(const TInputPixel & A) const
  {
    return static_cast<TOutputPixel>(A);
  }
};

/** \class RenderingFunction
 * \brief Base class for rendering functions.
 *  Please note that this class is pure virtual, and should be
 *  subclassed.
 *  \ingroup Visualization
 */
template <class TPixelPrecision, class TRGBPixel,
  class TPixelRepresentationFunction = ChannelSelectorFunctor<
        TPixelPrecision>,
  class TTransferFunction = Identity<
        typename itk::NumericTraits<TPixelPrecision>::ValueType,
        typename itk::NumericTraits<TPixelPrecision>::ValueType
        > >
class RenderingFunction
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef RenderingFunction             Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** type macro */
  itkTypeMacro(RenderingFunction,Object);

  /** PixelType macros */
  typedef TRGBPixel                                  OutputPixelType;
  typedef TPixelPrecision                            PixelType;
  typedef typename itk::NumericTraits<PixelType>::ValueType ScalarType;
  typedef itk::VariableLengthVector<ScalarType>       VectorPixelType;
  typedef itk::RGBPixel<ScalarType> RGBPixelType;
  typedef itk::RGBAPixel<ScalarType> RGBAPixelType;

  /** Pixel representation */
  typedef TPixelRepresentationFunction PixelRepresentationFunctionType;

  typedef TTransferFunction                          TransferFunctionType;

  /** Extrema vector */
  typedef std::vector<ScalarType>               ExtremaVectorType;

  /** Evaluate method (scalar version) */
  virtual const OutputPixelType Evaluate(ScalarType spixel) const = 0;

  /** Evaluate method (vector version) */
  virtual const OutputPixelType Evaluate(const VectorPixelType & vpixel) const = 0;

  /** Evaluate method (RGB pixel version) */
  virtual const OutputPixelType Evaluate(const RGBPixelType & vpixel) const = 0;

  /** Evaluate method (RGBA pixel version) */
  virtual const OutputPixelType Evaluate(const RGBAPixelType & vpixel) const = 0;

  /** Get a string description of a pixel  (scalar version) */
  virtual const std::string Describe(ScalarType spixel) const = 0;

  /** Get a string description of a pixel (vector version) */
  virtual const std::string Describe(const VectorPixelType & vpixel) const = 0;

  /** Get a string description of a pixel  (RGB pixel version) */
  virtual const std::string Describe(const RGBPixelType & spixel) const = 0;//FIXME should it provide a default implementation?

  /** Get a string description of a pixel (RGBA pixel version) */
  virtual const std::string Describe(const RGBAPixelType & vpixel) const = 0;//FIXME should it provide a default implementation?

  /** This method is available to allow implementation of
   * preprocessing.
   */
  virtual void Initialize(){};

  /** Set the minimum (scalar version) */
  virtual void SetMinimum(ScalarType spixel)
  {
    m_Minimum.clear();
    m_Minimum.push_back(spixel);
  }

  /** Set the maximum (scalar version) */
  virtual void SetMaximum(ScalarType spixel)
  {
    m_Maximum.clear();
    m_Maximum.push_back(spixel);
  }

 /** Set minimum (vector version) */
  virtual void SetMinimum(const VectorPixelType & vpixel)
  {
    m_Minimum.clear();
    for(unsigned int i = 0; i < vpixel.Size();++i)
      {
      m_Minimum.push_back(vpixel[i]);
      }
  }

  /** Set maximum (vector version) */
  virtual void SetMaximum(const VectorPixelType & vpixel)
  {
    m_Maximum.clear();
    for(unsigned int i = 0; i < vpixel.Size();++i)
      {
      m_Maximum.push_back(vpixel[i]);
      }
  }

  /** Set minimum (std::vector version) */
  virtual void SetMinimum(const ExtremaVectorType & vpixel)
  {
    m_Minimum = vpixel;
  }

  /** Set maximum (std::vector version) */
  virtual void SetMaximum(const ExtremaVectorType & vpixel)
  {
    m_Maximum = vpixel;
  }


  /** Get the transfer function for tuning */
//   TransferFunctionType & GetTransferFunction()
//   {
//     return m_TransferFunction;
//   }
//   itkGetConstReferenceMacro(TransferFunction, TransferFunctionType);
  virtual const TransferFunctionType & GetTransferFunction() const
  {
    return this->m_TransferFunction;
  }
  virtual TransferFunctionType  & GetTransferFunction()
  {
    return this->m_TransferFunction;
  }
//   itkSetMacro(TransferFunction, TransferFunctionType);
  virtual void SetTransferFunction (const TransferFunctionType function)
  {
      this->m_TransferFunction = function;
      this->Modified();
  }

//   itkGetConstReferenceMacro(PixelRepresentationFunction, PixelRepresentationFunctionType);
  virtual const PixelRepresentationFunctionType & GetPixelRepresentationFunction() const
  {
    return this->m_PixelRepresentationFunction;
  }
  virtual PixelRepresentationFunctionType & GetPixelRepresentationFunction()
  {
    return this->m_PixelRepresentationFunction;
  }
//   itkSetMacro(PixelRepresentationFunction, PixelRepresentationFunctionType);
  virtual void SetPixelRepresentationFunction (const PixelRepresentationFunctionType function)
  {
      this->m_PixelRepresentationFunction = function;
      this->Modified();
  }
protected:
  /** Constructor */
  RenderingFunction() : m_Minimum(), m_Maximum()  {}
  /** Destructor */
  virtual ~RenderingFunction() {}

  /** Extrema values */
  ExtremaVectorType m_Minimum;
  ExtremaVectorType m_Maximum;

  /** PixelRepresentation function*/
  PixelRepresentationFunctionType m_PixelRepresentationFunction;//FIXME private

  /** Transfer function*/
  TransferFunctionType m_TransferFunction;//FIXME private

private:
  RenderingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented




};
} // end namespace Function
} // end namepsace otb

#endif


