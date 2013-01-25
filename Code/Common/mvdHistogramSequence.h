/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __mvdHistogramSequence_h
#define __mvdHistogramSequence_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include "itkHistogram.h"

//
// OTB includes (sorted by alphabetic order)
#include "otbObjectList.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractModel.h"
#include "mvdTypes.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class HistogramSequence
 */
class Monteverdi2_EXPORT HistogramSequence :
    public AbstractModel
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** Constructor */
  HistogramSequence( QObject* parent =NULL );

  /** Destructor */
  virtual ~HistogramSequence();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  //
  // AbstractModel overrides.

  /** */
  virtual void virtual_BuildModel();

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types.
private:
  /** */
  typedef
    // itk::NumericTraits< T >::FloatType and
    // itk::NumericTraits< T >::RealType do not depend on template
    // parameter T. They are always typedef, respectively, as float
    // and double.
    itk::NumericTraits< DefaultImageType::InternalPixelType >::RealType
    MeasurementType;

  /** */
  typedef itk::Statistics::Histogram< MeasurementType, 1 > Histogram;

  /** */
  typedef otb::ObjectList< Histogram > HistogramList;

//
// Private methods.
private:
  template< typename TImage >
    void template_BuildModel_I();

  template< typename TImageModel >
    void template_BuildModel_M();
//
// Private attributes.
private:
  /** */
  HistogramList::Pointer m_Histograms;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'


/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbStreamingHistogramVectorImageFilter.h"
#include "otbStreamingMinMaxVectorImageFilter.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractImageModel.h"

namespace mvd
{

/*******************************************************************************/
template< typename TImage >
inline
void
HistogramSequence
::template_BuildModel_I()
{
  qDebug() << "Generating histogram (I)...";

  AbstractImageModel* imageModel =
    qobject_cast< AbstractImageModel* >( parent() );

  assert( imageModel!=NULL );

  //
  // 1st pass: process min/MAX for each band.

  // Connect min/MAX pipe-section.
  typedef
    otb::StreamingMinMaxVectorImageFilter< TImage >
    MinMaxFilter;

  typename MinMaxFilter::Pointer filterMinMax( MinMaxFilter::New() );

  filterMinMax->SetInput(
    otb::DynamicCast< TImage >( imageModel->ToImageBase() )
  );

  filterMinMax->Update();

  // Extract min/MAX intensities for each bands.
  typename MinMaxFilter::PixelType lSrcMin( filterMinMax->GetMinimum() );
  typename MinMaxFilter::PixelType lSrcMax( filterMinMax->GetMaximum() );

  //
  // 2nd pass: compute histogram.
  typedef
    otb::StreamingHistogramVectorImageFilter< TImage >
    HistogramFilter;

  typename HistogramFilter::Pointer histogramFilter( HistogramFilter::New() );

  histogramFilter->SetInput(
    otb::DynamicCast< TImage >( imageModel->ToImageBase() )
  );

  histogramFilter->GetFilter()->SetHistogramMin( lSrcMin );
  histogramFilter->GetFilter()->SetHistogramMax( lSrcMax );
  histogramFilter->GetFilter()->SetSubSamplingRate( 1 );

  histogramFilter->Update();

  //
  // Reference result.
  m_Histograms = histogramFilter->GetHistogramList();

  qDebug() << "Histogram (I) generated.";
}

/*******************************************************************************/
template< typename TImageModel >
inline
void
HistogramSequence
::template_BuildModel_M()
{
  qDebug() << "Generate histogram (M)...";

  TImageModel* imageModel =
    qobject_cast< TImageModel* >( parent() );

  assert( imageModel!=NULL );

  //
  // 1st pass: process min/MAX for each band.

  // Connect min/MAX pipe-section.
  typedef
    otb::StreamingMinMaxVectorImageFilter<
      typename TImageModel::SourceImageType >
    MinMaxFilter;

  typename MinMaxFilter::Pointer filterMinMax( MinMaxFilter::New() );

  filterMinMax->SetInput( imageModel->ToImage() );

  filterMinMax->Update();

  // Extract min/MAX intensities for each bands.
  typename MinMaxFilter::PixelType lSrcMin( filterMinMax->GetMinimum() );
  typename MinMaxFilter::PixelType lSrcMax( filterMinMax->GetMaximum() );


  //
  // 2nd pass: compute histogram.
  typedef
    otb::StreamingHistogramVectorImageFilter<
      typename TImageModel::SourceImageType >
    HistogramFilter;

  typename HistogramFilter::Pointer histogramFilter( HistogramFilter::New()  );

  histogramFilter->SetInput( imageModel->ToImage() );

  histogramFilter->GetFilter()->SetHistogramMin( lSrcMin );
  histogramFilter->GetFilter()->SetHistogramMax( lSrcMax );
  histogramFilter->GetFilter()->SetSubSamplingRate( 1 );

  histogramFilter->Update();

  //
  // Reference result.
  m_Histograms = histogramFilter->GetHistogramList();

  qDebug() << "Histogram (M) generated.";
}

} // end namespace 'mvd'

#endif // __mvdHistogramSequence_h
