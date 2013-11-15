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

#ifndef __otbPathListToHistogramGenerator_h
#define __otbPathListToHistogramGenerator_h

#include "itkSampleToHistogramFilter.h"
#include "itkHistogram.h"
#include "itkProcessObject.h"
#include "itkListSample.h"
#include "itkVector.h"
#include "itkDenseFrequencyContainer2.h"
#include "otbObjectList.h"

//TODO : write a HistogramSource when outputing Histogram

namespace otb
{

/** \class PathListToHistogramGenerator
 *  \brief This class generates an histogram from a list of path.
 *
 *  The concept of Histogram in ITK is quite generic. It has been designed to
 *  manage multiple components data. This class facilitates the computation of
 *  an histogram from a list of path. Internally it creates a List that is feed into
 *  the ListSampleToHistogramGenerator.
 *
 */

template<class TPath, class TFunction>
class PathListToHistogramGenerator : public itk::ProcessObject
{
public:
  /** Standard typedefs */
  typedef PathListToHistogramGenerator  Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PathListToHistogramGenerator, itk::ProcessObject);

  /** standard New() method support */
  itkNewMacro(Self);

  typedef TPath                         PathType;
  typedef typename PathType::Pointer    PathPointer;
  typedef ObjectList<PathType>          PathListType;

  typedef TFunction                     FunctionType;

  typedef typename TFunction::OutputType RealType;

  typedef RealType                                          MeasurementType;
  typedef itk::Vector<MeasurementType, 1>                   ListSampleVectorType;
  typedef itk::Statistics::ListSample<ListSampleVectorType> ListSampleType;
  typedef typename ListSampleType::Pointer                  ListSamplePointer;
  typedef typename ListSampleType::ConstPointer             ListSampleConstPointer;

  typedef itk::Statistics::DenseFrequencyContainer2         FrequencyContainerType;
  typedef itk::Statistics::Histogram< MeasurementType,
            FrequencyContainerType >                        HistogramType;

  typedef itk::Statistics::SampleToHistogramFilter<
      ListSampleType,
      HistogramType>                                    GeneratorType;
  typedef typename GeneratorType::Pointer               GeneratorPointer;

  typedef typename HistogramType::Pointer               HistogramPointer;
  typedef typename HistogramType::ConstPointer          HistogramConstPointer;
  typedef typename HistogramType::SizeType              SizeType;
  typedef typename HistogramType::MeasurementVectorType MeasurementVectorType;

  /** DataObject typedef*/
  typedef typename Superclass::DataObjectPointer        DataObjectPointer;

  void GraftNthOutput(unsigned int idx, itk::DataObject *graft);
  void GraftOutput(itk::DataObject *graft);

public:

  /** Connects the input PathList for which the histogram is going to be computed */
  void SetInput(const PathListType* path);
  const PathListType* GetInput() const;

  // Return the output histogram. 
   const HistogramType * GetOutput() const;

  /** Set number of histogram bins */
  void SetNumberOfBins(const SizeType& size);

  /** Set marginal scale value to be passed to the histogram generator */
  void SetMarginalScale(double marginalScale);
  void SetHistogramMin(const MeasurementVectorType& histogramMin);
  void SetHistogramMax(const MeasurementVectorType& histogramMax);
  void SetAutoMinMax(bool autoMinMax);

protected:
  PathListToHistogramGenerator();
  virtual ~PathListToHistogramGenerator() {}
  virtual void GenerateData();
  DataObjectPointer MakeOutput(unsigned int);
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  
  GeneratorPointer m_HistogramGenerator;

};

} // end of namespace OTB

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPathListToHistogramGenerator.txx"
#endif

#endif
