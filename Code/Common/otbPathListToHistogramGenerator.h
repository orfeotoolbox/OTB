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


#include "itkListSampleToHistogramGenerator.h"
#include "itkObject.h"
#include "itkListSample.h"
#include "itkVector.h"
#include "itkDenseFrequencyContainer.h"

namespace otb {



/** \class PathListToHistogramGenerator
 *  \brief This class generates an histogram from a list of path.
 *
 *  The concept of Histogram in ITK is quite generic. It has been designed to
 *  manage multiple components data. This class facilitates the computation of
 *  an histogram from a list of path. Internally it creates a List that is feed into
 *  the ListSampleToHistogramGenerator.
 *
 */


template< class TPath,class TFunction >
class PathListToHistogramGenerator : public itk::Object
{
public:
  /** Standard typedefs */
  typedef PathListToHistogramGenerator   Self ;
  typedef itk::Object                    Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PathListToHistogramGenerator, itk::Object) ;

  /** standard New() method support */
  itkNewMacro(Self) ;

  typedef TPath                                   	  PathType;
  typedef typename PathType::Pointer                      PathPointer;
  typedef std::vector< PathPointer >   		          PathListType;

  typedef PathListType *                                  PathListPointer;
  typedef const PathListType *                            PathListConstPointer;

  typedef TFunction                                       FunctionType;

  typedef typename TFunction::OutputType                  RealType;

  typedef RealType                                                 MeasurementType;
  typedef itk::Vector< MeasurementType ,1 >                        ListSampleVectorType ;
  typedef itk::Statistics::ListSample< ListSampleVectorType >      ListSampleType ;
  typedef typename ListSampleType::Pointer                         ListSamplePointer;
  typedef typename ListSampleType::ConstPointer                    ListSampleConstPointer;

  typedef itk::Statistics::DenseFrequencyContainer        FrequencyContainerType;


  typedef itk::Statistics::ListSampleToHistogramGenerator<
                         ListSampleType,
			 MeasurementType,
                         FrequencyContainerType,1>        GeneratorType;


  typedef typename GeneratorType::Pointer                 GeneratorPointer;

  typedef typename GeneratorType::HistogramType           HistogramType;
  typedef typename HistogramType::Pointer                 HistogramPointer;
  typedef typename HistogramType::ConstPointer            HistogramConstPointer;
  typedef typename HistogramType::SizeType                SizeType;
  typedef typename HistogramType::MeasurementVectorType   MeasurementVectorType;

public:

  /** Triggers the Computation of the histogram */
  void Compute( void );

  /** Connects the input PathList for which the histogram is going to be computed */
  void SetInput( PathListPointer path);

  /** Return the histogram. o00
   \warning This output is only valid after the Compute() method has been invoked
   \sa Compute */
  const HistogramType * GetOutput() const;

  /** Set number of histogram bins */
  void SetNumberOfBins( const SizeType & size );

  /** Set marginal scale value to be passed to the histogram generator */
  void SetMarginalScale( double marginalScale );
  void SetHistogramMin(const MeasurementVectorType & histogramMin);
  void SetHistogramMax(const MeasurementVectorType & histogramMax);
  void SetAutoMinMax(bool autoMinMax);

protected:
  PathListToHistogramGenerator();
  virtual ~PathListToHistogramGenerator() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;


private:

  PathListPointer     m_PathList;
  GeneratorPointer    m_HistogramGenerator;



};


} // end of namespace OTB

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPathListToHistogramGenerator.txx"
#endif

#endif
