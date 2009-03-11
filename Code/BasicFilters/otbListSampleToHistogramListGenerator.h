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
#ifndef __itkListSampleToHistogramListGenerator_h
#define __itkListSampleToHistogramListGenerator_h

#include "itkObject.h"
#include "itkListSampleBase.h"
#include "itkHistogram.h"
#include "otbObjectList.h"
#include "itkStatisticsAlgorithm.h"
#include "itkDenseFrequencyContainer.h"
#include "itkNumericTraits.h"

namespace otb{

/** \class ListSampleToHistogramListGenerator
 *  \brief Generates a list of 1D  Histograms from a ListSample
 *  This class is similar to
 *  ListSampleToVariableDimensionHistogramGenerator, except from the
 *  fact that it computes one histogram per channel instead of a
 *  single joint-histogram. The output is a Histogram list (\sa
 *  ObjectList).
 * \sa ListSampleToVariableDimensionHistogram
 */
template< class TListSample, 
          class THistogramMeasurement,  
          class TFrequencyContainer = itk::Statistics::DenseFrequencyContainer>
class ITK_EXPORT ListSampleToHistogramListGenerator :
    public itk::Object
{
public:
  /** Standard typedefs */
  typedef ListSampleToHistogramListGenerator Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ListSampleToHistogramListGenerator, Object);
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

   /** Type needed for defining the limits of the histogram bins */
  typedef typename itk::NumericTraits
  < THistogramMeasurement >::RealType                    HistogramMeasurementRealType;
  typedef itk::Statistics::Histogram< HistogramMeasurementRealType,1,
                     TFrequencyContainer >               HistogramType;
  typedef typename HistogramType::SizeType               HistogramSizeType;
  typedef typename TListSample::MeasurementVectorType    MeasurementVectorType;
  typedef ObjectList<HistogramType>                      HistogramListType;
  typedef typename HistogramListType::Pointer            HistogramListPointerType;

  /** plug in the ListSample object */
  void SetListSample(const TListSample* list)
    { 
     m_List = list; 
    }

  void SetMarginalScale(float scale)
  { m_MarginalScale = scale; }

  void SetNumberOfBins(unsigned int size)
  {
    m_Size[0]=size;
  }

  HistogramListType* GetOutput() const
  { return m_HistogramList; }

  void Update() 
  { this->GenerateData(); }

  itkSetMacro(AutoMinMax,bool);
  itkGetConstReferenceMacro(AutoMinMax,bool);

  void SetHistogramMin(const MeasurementVectorType & histogramMin)
    {
    m_HistogramMin = histogramMin;
    m_AutoMinMax = false;
    }

  void SetHistogramMax(const MeasurementVectorType & histogramMax)
    {
    m_HistogramMax = histogramMax;
    m_AutoMinMax = false;
    }


protected:
  ListSampleToHistogramListGenerator();
  virtual ~ListSampleToHistogramListGenerator() {}
  void GenerateData();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  const TListSample* m_List;
  HistogramSizeType m_Size;
  float m_MarginalScale;
  MeasurementVectorType m_HistogramMin;
  MeasurementVectorType m_HistogramMax;
  bool m_AutoMinMax;
  HistogramListPointerType m_HistogramList;

}; // end of class

} // end of namespace itk 

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbListSampleToHistogramListGenerator.txx"
#endif

#endif
