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
#include "otbChannelSelectorFunctor.h"
#include "itkHistogram.h"
#include "otbObjectList.h"
#include "otbVisualizationPixelTraits.h"
#include "itkListSample.h"
#include "otbListSampleToHistogramListGenerator.h"
#include "itkConceptChecking.h"
#include "itkDenseFrequencyContainer2.h"

#include "itkMetaDataDictionary.h"

namespace otb
{

namespace Function
{

/** \class RenderingFunction
 * \brief Base class for rendering functions.
 *  Please note that this class is pure virtual, and should be
 *  subclassed.
 *  \ingroup Visualization
 */
template <class TPixel, class TRGBPixel>
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
  itkTypeMacro(RenderingFunction, Object);

  /** PixelType macros */
  typedef TRGBPixel                                         OutputPixelType;
  typedef TPixel                                            PixelType;
  typedef typename itk::NumericTraits<PixelType>::ValueType ScalarType;
  typedef itk::VariableLengthVector<ScalarType>             VectorPixelType;
  typedef itk::RGBPixel<ScalarType>                         RGBPixelType;
  typedef itk::RGBAPixel<ScalarType>                        RGBAPixelType;
  typedef typename itk::NumericTraits<ScalarType>::RealType RealScalarType;
  typedef itk::VariableLengthVector<RealScalarType>         InternalPixelType;

  typedef itk::VariableLengthVector<ScalarType>   SampleType;
  typedef itk::Statistics::ListSample<SampleType> ListSampleType;
  typedef typename ListSampleType::Pointer        ListSamplePointerType;

  typedef itk::Statistics::DenseFrequencyContainer2 DFContainerType;
  typedef itk::Statistics::Histogram< RealScalarType,  DFContainerType> HistogramType;
  typedef otb::ListSampleToHistogramListGenerator
  <ListSampleType, ScalarType, DFContainerType>           HistogramFilterType;


  typedef typename HistogramType::Pointer     HistogramPointerType;
  typedef ObjectList<HistogramType>           HistogramListType;
  typedef typename HistogramListType::Pointer HistogramListPointerType;

  itkConceptMacro(SameTypeCheck,
                  (itk::Concept::SameType<typename HistogramFilterType::HistogramType, HistogramType>));

  typedef  itk::Array<double> ParametersType;

  typedef itk::MetaDataDictionary MetaDataDictionaryType;

  /** Evaluate method: required interface */
  virtual OutputPixelType Evaluate(const PixelType&  spixel) const
  {
    return EvaluateTransferFunction(EvaluatePixelRepresentation(spixel));
  }

  /** Get a string description of a pixel: required interface */
  virtual const std::string Describe(const PixelType& spixel) const = 0;

  /** Evaluate pixel representation: required interface */
  virtual InternalPixelType EvaluatePixelRepresentation(const PixelType&  spixel) const = 0; //FIXME pure virtual or exception?

  /** Return the Pixel representation size: required interface*/
  virtual unsigned int GetPixelRepresentationSize() const = 0; //FIXME pure virtual or exception?

  /** Evaluate transfer function: this is part of the required interface */
  virtual OutputPixelType EvaluateTransferFunction(const InternalPixelType&  spixel) const = 0; //FIXME pure virtual or exception?

  /** Set the Rendering function parameters: optional interface */
  virtual void SetParameters(const ParametersType&)
  {
    itkExceptionMacro(<< "Subclasses should override this method");
  }
  virtual ParametersType GetParameters() const
  {
    itkExceptionMacro(<< "Subclasses should override this method");
  }

  virtual void SetChannelList(std::vector<unsigned int>& /*channels*/)
  {
    itkExceptionMacro(<< "Subclasses should override this method");
  }

  virtual std::vector<unsigned int> GetChannelList()
  {
    itkExceptionMacro(<< "Subclasses should override this method");
  }

  virtual void SetAutoMinMax(bool /*autoMinMax*/)
  {
    itkExceptionMacro(<< "Subclasses should override this method");
  }

  /** Get the histogram of the pixel representation generated from the sample list */
  virtual HistogramListPointerType GetHistogramList()
  {
    //FIXME Update condition?
    return m_HistogramList;
  }

  /** Set the sample list */
  virtual void SetListSample(ListSamplePointerType listSample)
  {
    if (this->m_ListSample != listSample)
      {
      m_ListSample = listSample;
      this->Modified();
      }
  }


  /** NoData Vaue setter */
  itkSetMacro( NoDataValue, ScalarType );

  /** NoData Vaue gtter */
  itkGetMacro( NoDataValue, ScalarType );
  itkGetConstMacro( NoDataValue, ScalarType );

  /** This method is available to allow implementation of
   * preprocessing.
   */
  // REVIEW: I agree, we should not be calling intialize ourselve, it
  // would better be seamless
  virtual void Initialize(const MetaDataDictionaryType& itkNotUsed(metadatadictionary)){} //FIXME should disappear and be
  //automatic (IsModified())

protected:
  /** Constructor */
  RenderingFunction()
  {
    m_HistogramList = NULL;
    m_ListSample = NULL;
    m_NoDataValue = itk::NumericTraits<ScalarType>::Zero;
  }
  /** Destructor */
  virtual ~RenderingFunction() {}

  virtual void RenderHistogram()
  {
    int m_NumberOfHistogramBins = 255; //FIXME is it to be accessed from outside?
    if (m_ListSample.IsNull())
      {
      itkExceptionMacro(<< "No listSample provided to render histogram");
      }
    // Create the histogram generation filter
    ListSamplePointerType pixelRepresentationListSample = ListSampleType::New();
    
    for (typename ListSampleType::ConstIterator it = m_ListSample->Begin(); it != m_ListSample->End(); ++it)
      {
      PixelType sample;
      VisualizationPixelTraits::Convert(it.GetMeasurementVector(), sample);
      SampleType sampleVector;
      VisualizationPixelTraits::Convert(this->EvaluatePixelRepresentation(sample), sampleVector);
      // Be sure to set the right MeasurementVectorSize : Get it after
      // the Convert : Set it once
      if(it == m_ListSample->Begin())
        pixelRepresentationListSample->SetMeasurementVectorSize(sampleVector.GetSize());
      pixelRepresentationListSample->PushBack(sampleVector);
      }

    typename HistogramFilterType::Pointer histogramFilter = HistogramFilterType::New();
    histogramFilter->SetListSample(pixelRepresentationListSample);

    histogramFilter->SetNumberOfBins(m_NumberOfHistogramBins);
    histogramFilter->NoDataFlagOn();
    histogramFilter->SetNoDataValue(m_NoDataValue);

    // Generate
    histogramFilter->Update();
    otbMsgDevMacro(<< "ImageRenderingFunction::RenderHistogram(): Histogram has been updated");

    // Retrieve the histogram
    m_HistogramList = const_cast<HistogramListType*>(histogramFilter->GetOutput());
  }

  ListSamplePointerType GetListSample()
  {
    return m_ListSample;
  }

 
private:
  RenderingFunction(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  HistogramListPointerType m_HistogramList;
  ListSamplePointerType    m_ListSample;
  ScalarType m_NoDataValue;
};
} // end namespace Function
} // end namepsace otb

#endif
