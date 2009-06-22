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
// #include "otbRenderingImageFilter.h"
#include "otbVisualizationPixelTraits.h"
#include "itkListSample.h"
#include "otbListSampleToHistogramListGenerator.h"
#include "itkConceptChecking.h"

namespace otb
{

// enum ImageInformationType {SCALAR, TWOBANDS, THREEBANDS, SENSORINVERTED, SENSORWAVELENTHORDER};

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


  typedef itk::VariableLengthVector<ScalarType>                       SampleType;
  typedef itk::Statistics::ListSample<SampleType>                     ListSampleType;
  typedef typename ListSampleType::Pointer                       ListSamplePointerType;

  typedef itk::Statistics::DenseFrequencyContainer                   DFContainerType;

  typedef otb::ListSampleToHistogramListGenerator
      <ListSampleType,ScalarType,DFContainerType>                     HistogramFilterType;
  typedef itk::Statistics::Histogram<
                  typename itk::NumericTraits<ScalarType>::RealType,1,
                  typename itk::Statistics::DenseFrequencyContainer> HistogramType;
  typedef typename HistogramType::Pointer HistogramPointerType;
  typedef ObjectList<HistogramType>                         HistogramListType;
  typedef typename HistogramListType::Pointer               HistogramListPointerType;

  itkConceptMacro(SameTypeCheck,
    (itk::Concept::SameType<typename HistogramFilterType::HistogramType, HistogramType>));


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

  virtual void SetListSample(ListSamplePointerType listSample)
  {
    m_ListSample = listSample;
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
  RenderingFunction()
  {
    m_HistogramList = NULL;
    m_ListSample = NULL;
  }
  /** Destructor */
  virtual ~RenderingFunction() {}

  virtual void RenderHistogram()
  {
    int m_NumberOfHistogramBins=255;//FIXME is it to be accessed from outside?
    if(m_ListSample.IsNull())
    {
      itkExceptionMacro(<<"No sample list provided to render histogram");
    }
      // Create the histogram generation filter
//     ListSampleType pixelRepresentationListSample(this->GetPixelRepresentationSize());
    ListSamplePointerType pixelRepresentationListSample = ListSampleType::New();
    for (typename ListSampleType::ConstIterator it = m_ListSample->Begin(); it != m_ListSample->End(); ++it)
    {
      //Here we have an issue:
      //- the ListSample contains VariableLengthVector
      //- the EvaluatePixelRepresentation is defined to apply to PixelType
      //Either we convert the ListSample to PixelType or we define a EvaluatePixelRepresentation
      //for vector.
//       PixelType sample = itk::NumericTraits<PixelType>::Zero;
//       sample = sample + (it.GetMeasurementVector());//FIXME better in a VisualizationPixelTraits
      PixelType sample;
      VisualizationPixelTraits::Convert(it.GetMeasurementVector(), sample);
      SampleType sampleVector;
      VisualizationPixelTraits::Convert(this->EvaluatePixelRepresentation(sample), sampleVector);
      pixelRepresentationListSample->PushBack(sampleVector);

    }

    typename HistogramFilterType::Pointer histogramFilter = HistogramFilterType::New();
    histogramFilter->SetListSample(pixelRepresentationListSample);

    histogramFilter->SetNumberOfBins(m_NumberOfHistogramBins);

    // Generate
    histogramFilter->Update();
    otbMsgDevMacro(<<"ImageRenderingFunction::RenderHistogram(): Histogram has been updated");

    // Retrieve the histogram
    m_HistogramList = histogramFilter->GetOutput();
  }


private:
  RenderingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  HistogramListPointerType m_HistogramList;
  ListSamplePointerType m_ListSample;
};
} // end namespace Function
} // end namepsace otb

#endif


