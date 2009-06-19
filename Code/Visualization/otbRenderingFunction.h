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
#include "itkHistogram.h"
#include "otbObjectList.h"
namespace otb
{

enum ImageInformationType {SCALAR, TWOBANDS, THREEBANDS, SENSORINVERTED, SENSORWAVELENTHORDER};

namespace Function
{


/** \class RenderingFunction
 * \brief Base class for rendering functions.
 *  Please note that this class is pure virtual, and should be
 *  subclassed.
 *  \ingroup Visualization
 */
template <class TPixel, class TRGBPixel >
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
  typedef TRGBPixel                                         OutputPixelType;
  typedef TPixel                                            PixelType;
  typedef typename itk::NumericTraits<PixelType>::ValueType ScalarType;
  typedef itk::VariableLengthVector<ScalarType>             VectorPixelType;
  typedef itk::RGBPixel<ScalarType>                         RGBPixelType;
  typedef itk::RGBAPixel<ScalarType>                        RGBAPixelType;
  typedef typename itk::NumericTraits<ScalarType>::RealType RealScalarType;
  typedef itk::VariableLengthVector<RealScalarType>         InternalPixelType;

  typedef itk::Statistics::Histogram<
                  typename itk::NumericTraits<ScalarType>::RealType,1,
                  typename itk::Statistics::DenseFrequencyContainer> HistogramType;
  typedef typename HistogramType::Pointer HistogramPointerType;
  typedef ObjectList<HistogramType>                         HistogramListType;
  typedef typename HistogramListType::Pointer               HistogramListPointerType;
  typedef itk::VariableLengthVector<ScalarType>                       SampleType;
  typedef itk::Statistics::ListSample<SampleType>                     ListSampleType;

  typedef  itk::Array< double >           ParametersType;


  /** Evaluate method (scalar version) */
  virtual OutputPixelType Evaluate(const PixelType &  spixel) const
  {
    return EvaluateTransferFunction(EvaluatePixelRepresentation(spixel));
  }

  /** Get a string description of a pixel  (scalar version) */
  virtual const std::string Describe(const PixelType & spixel) const = 0;

  /** Evaluate pixel representation */
  virtual InternalPixelType EvaluatePixelRepresentation(const PixelType &  spixel) const = 0;

  /** Return the Pixel representation size*/
  virtual unsigned int GetPixelRepresentationSize() const = 0;

  /** Evaluate transfer function */
  virtual OutputPixelType EvaluateTransferFunction(const InternalPixelType &  spixel) const = 0;

  /** Set/Get the sample list */
//   virtual void SetHistogramList(HistogramListPointerType histogramList)
//   {
//     m_HistogramList = histogramList;
//     this->Modified();
//   }
  virtual HistogramListPointerType GetHistogramList()
  {
    return m_HistogramList;
  }

  virtual void SetSampleList(SampleListPointerType sampleList)
  {
    m_SampleList = sampleList;
    this->Modified();
  }

  /** Set the Rendering function parameters */
  virtual void SetParameters( const ParametersType & ){};

  /** This method is available to allow implementation of
   * preprocessing.
   */
//   virtual void Initialize(ImageInformationType){};
  virtual void Initialize(){};//FIXME should disappear and be automatic (IsModified())
protected:
  /** Constructor */
  RenderingFunction() {}
  /** Destructor */
  virtual ~RenderingFunction() {}

  void RenderHistogram()
  {

  }


private:
  RenderingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  HistogramListPointerType m_HistogramList;
  SampleListPointerType m_SampleList;
};
} // end namespace Function
} // end namepsace otb

#endif


