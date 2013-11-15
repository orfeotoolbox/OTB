/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbListSampleToVariableDimensionHistogramGenerator_h
#define __otbListSampleToVariableDimensionHistogramGenerator_h

#include "itkProcessObject.h"
#include "itkHistogram.h"
#include "itkStatisticsAlgorithm.h"
#include "itkDenseFrequencyContainer2.h"
#include "itkNumericTraits.h"

namespace otb {

/** \class ListSampleToVariableDimensionHistogramGenerator
 *  \brief Generates a VariableDimensionHistogram from a ListSample
 *
 *  This class is a fork from itk::ListSampleToHistogramGenerator
 *  adapted to produce VariableDimensionHistogram, whose dimension can
 *  be chosen at runtime.
 *
 *  Since the MeasurementVectorSize template parameter is no longer
 *  needed, it has been removed.
 *
 * For more information, please refer to the
 * ListSampleToHistogramGenerator class documentation.
 *
 * \sa VariableDimensionHistogram, ListSample, ListSampleToHistogramGenerator
 */
template<class TListSample,
    class THistogramMeasurement,
         class TFrequencyContainer = itk::Statistics::DenseFrequencyContainer2>
class ITK_EXPORT ListSampleToVariableDimensionHistogramGenerator :
  public itk::ProcessObject
{
public:
  /** Standard typedefs */
  typedef ListSampleToVariableDimensionHistogramGenerator Self;
  typedef itk::ProcessObject                              Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ListSampleToVariableDimensionHistogramGenerator, itk::ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** typedef the ListSampleType */
  typedef TListSample                                  ListSampleType;
  
  /** Type needed for defining the limits of the histogram bins */
  typedef typename itk::NumericTraits<THistogramMeasurement>::RealType
  HistogramMeasurementRealType;

  typedef itk::Statistics::Histogram<HistogramMeasurementRealType,
      TFrequencyContainer>                             HistogramType;

  typedef typename HistogramType::SizeType              HistogramSizeType;
  typedef typename HistogramType::MeasurementVectorType MeasurementVectorType;

  /** DataObject typedef*/
  typedef typename Superclass::DataObjectPointer        DataObjectPointer;

  // Set/Get the input list sample
  void SetListSample(const ListSampleType* inputlist);
  const ListSampleType* GetListSample() const;
  
  // Get the output histogram
  const HistogramType* GetOutput();
  
  void SetMarginalScale(float scale)
  { m_MarginalScale = scale; }

  void SetNumberOfBins(HistogramSizeType sizes)
  { m_Sizes = sizes; }

  itkSetMacro(AutoMinMax, bool);
  itkGetConstReferenceMacro(AutoMinMax, bool);

  void SetHistogramMin(const MeasurementVectorType& histogramMin)
  {
    m_HistogramMin = histogramMin;
    m_AutoMinMax = false;
  }

  void SetHistogramMax(const MeasurementVectorType& histogramMax)
  {
    m_HistogramMax = histogramMax;
    m_AutoMinMax = false;
  }

protected:
  ListSampleToVariableDimensionHistogramGenerator();
  virtual ~ListSampleToVariableDimensionHistogramGenerator() {}
  void GenerateData();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  DataObjectPointer MakeOutput(unsigned int idx);

private:
  HistogramSizeType     m_Sizes;
  float                 m_MarginalScale;
  MeasurementVectorType m_HistogramMin;
  MeasurementVectorType m_HistogramMax;
  bool                  m_AutoMinMax;

}; // end of class

} // end of namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbListSampleToVariableDimensionHistogramGenerator.txx"
#endif

#endif
