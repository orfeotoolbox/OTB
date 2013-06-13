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
#ifndef __mvdHistogramModel_h
#define __mvdHistogramModel_h

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
#include "mvdAlgorithm.h"
#include "mvdSerializableInterface.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{

namespace
{
}

//
// Internal classes pre-declaration.
class AbstractImageModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class HistogramModel
 */
class Monteverdi2_EXPORT HistogramModel :
    public AbstractModel,
    private SerializableInterface
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  /** */
  typedef
    // itk::NumericTraits< T >::FloatType and
    // itk::NumericTraits< T >::RealType do not depend on template
    // parameter T. They are always typedef, respectively, as float
    // and double.
    //
    // So, itk::NumericTraits< DefaultImageType::InternalPixelType
    // >::RealType is equivalent to itk::NumericTraits< float
    // >::RealType which is always an alias of double.
    //
    // This typedef is used for compatibility with
    // itk::Histogram<>::MeasurementType.
    itk::NumericTraits< DefaultImageType::InternalPixelType >::RealType
    MeasurementType;

  /**
   * \class BuildContext
   * \brief WIP.
   */
  class BuildContext
  {
    //
    // Public methods.
  public:
    /** \brief Constructor. */
    BuildContext( bool isBeingStored,
		  const QString& filename =QString(),
		  bool noDataFlag =false,
		  ComponentType noDataValue = ComponentType( 0 ) ) :
      m_Filename( filename ),
      m_NoDataFlag( noDataFlag ),
      m_NoDataValue( noDataValue ),
      m_IsBeingStored( isBeingStored )
    {
    }

    /**
     */
    inline bool IsBeingStored() const
    {
      return m_IsBeingStored;
    }

    //
    // Public attributes
  public:
    QString m_Filename;
    bool m_NoDataFlag;
    ComponentType m_NoDataValue;

  private:
    bool m_IsBeingStored;
  };

//
// Public methods.
public:

  /** \brief Constructor. */
  HistogramModel( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~HistogramModel();

  /** */
  inline MeasurementType Quantile( CountType band, double p ) const;

  /** */
  inline MeasurementType Quantile( CountType band, double p, Bound bound ) const;

  /** */
  double
    Percentile( CountType band,
		MeasurementType intensity,
		Bound bound ) const;

  /** */
  inline VectorPixelType GetMinPixel() const;

  /** */
  inline VectorPixelType GetMaxPixel() const;

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
  virtual void virtual_BuildModel( void* context =NULL );

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private constants.
private:
  /**
   */
  static const int BINS_OVERSAMPLING_RATE;

//
// Private types.
private:

  /** */
  typedef itk::Statistics::Histogram< MeasurementType, 1 > Histogram;

  /** */
  typedef otb::ObjectList< Histogram > HistogramList;

//
// Private methods.
private:
  /**
   */
#if 0
  template< typename TImage >
    void template_BuildModel_I();
#endif

  /**
   */
  template< typename TImageModel >
    void template_BuildModel_M( BuildContext* context =NULL );

  //
  // SerializableInterface overrides.
  //

  virtual void virtual_Read( QIODevice* device );

  virtual void virtual_Write( QIODevice& device ) const;

//
// Private attributes.
private:
  /** */
  HistogramList::Pointer m_Histograms;
  /** */
  DefaultImageType::PixelType m_MinPixel;
  /** */
  DefaultImageType::PixelType m_MaxPixel;

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
#include "Core/mvdAbstractImageModel.h"

namespace mvd
{

/*******************************************************************************/
inline
DefaultImageType::PixelType
HistogramModel
::GetMinPixel() const
{
  return m_MinPixel;
}

/*******************************************************************************/
inline
DefaultImageType::PixelType
HistogramModel
::GetMaxPixel() const
{
  return m_MaxPixel;
}

/*******************************************************************************/
inline
HistogramModel::MeasurementType
HistogramModel
::Quantile( unsigned int band,
	    double p ) const
{
  assert( band<m_Histograms->Size() );

  return m_Histograms->GetNthElement( band )->Quantile( 0, p );
}

/*******************************************************************************/
inline
HistogramModel::MeasurementType
HistogramModel
::Quantile( unsigned int band,
	    double p,
	    Bound bound ) const
{
  assert( band<m_Histograms->Size() );

  return m_Histograms->GetNthElement( band )->Quantile(
    0,
    bound==BOUND_UPPER ? 1.0 - p : p
  );
}

/*******************************************************************************/
#if 0
template< typename TImage >
void
HistogramModel
::template_BuildModel_I()
{
  QTime lMain;
  QTime lPass1;
  QTime lPass2;

  lMain.start();

  qDebug() << tr( "%1: Generating histogram (I)..." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );


  AbstractImageModel* imageModel =
    qobject_cast< AbstractImageModel* >( parent() );

  assert( imageModel!=NULL );

  //
  // 1st pass: process min/MAX for each band.

  qDebug() << tr( "%1: Pass #1 - finding pixel min/maxes..." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

  lPass1.start();

  // Connect min/MAX pipe-section.
  typedef
    otb::StreamingMinMaxVectorImageFilter< TImage >
    MinMaxFilter;

  typename MinMaxFilter::Pointer filterMinMax( MinMaxFilter::New() );

  filterMinMax->SetInput(
    // otb::DynamicCast< TImage >( imageModel->ToImageBase() )
    imageModel->ToImageBase()
  );

  filterMinMax->Update();

  /*
  // Extract min/MAX intensities for each band.
  // itk::VariableLengthVector< FLOAT_TYPE >
  typename MinMaxFilter::PixelType lSrcMin( f );
  typename MinMaxFilter::PixelType lSrcMax( filterMinMax->GetMaximum() );
  */

  // Extract-convert-remember min/MAX intensities for each band.
  m_MinPixel = filterMinMax->GetMinimum();
  m_MaxPixel = filterMinMax->GetMaximum();

  qDebug() << tr( "%1: Pass #1 - done (%2 ms)." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
    .arg( lPass1.elapsed() );

  //
  // 2nd pass: compute histogram.

  qDebug() << tr( "%1: Pass #2 - computing histogram..." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

  lPass2.start();

  // Connect histogram-generator pipe-section.
  typedef
    otb::StreamingHistogramVectorImageFilter< TImage >
    HistogramFilter;

  typename HistogramFilter::Pointer histogramFilter( HistogramFilter::New() );

  histogramFilter->SetInput(
    // otb::DynamicCast< TImage >( imageModel->ToImageBase() )
    imageModel->ToImageBase()
  );

  // Setup histogram filter.
  histogramFilter->GetFilter()->SetHistogramMin( m_MinPixel );
  histogramFilter->GetFilter()->SetHistogramMax( m_MaxPixel );
  histogramFilter->SetNumberOfBins(
    HistogramModel::BINS_OVERSAMPLING_RATE * 256
  );
  histogramFilter->GetFilter()->SetSubSamplingRate( 1 );

  // Go.
  histogramFilter->Update();

  qDebug() << tr( "%1: Pass #2 - done (%2 ms)." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
    .arg( lPass2.elapsed()  );

  //
  // Reference result.
  m_Histograms = histogramFilter->GetHistogramList();

  qDebug() << tr( "%1: Histogram (I) generated (%2 ms)." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
    .arg( lMain.elapsed() );
}
#endif

/*******************************************************************************/
template< typename TImageModel >
void
HistogramModel
::template_BuildModel_M( BuildContext* context )
{
  QTime lMain;
  QTime lPass1;
  QTime lPass2;

  lMain.start();

  qDebug() << tr( "%1: Generating histogram (M)..." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );


  AbstractImageModel* parentImageModel =
    qobject_cast< AbstractImageModel* >( parent() );
  assert( parentImageModel!=NULL );


  TImageModel* imageModel = parentImageModel->GetQuicklookModel();
  assert( imageModel!=NULL );

  //
  // 1st pass: process min/MAX for each band.

  qDebug() << tr( "%1: Pass #1 - finding pixel min/maxes..." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

  lPass1.start();

  // Connect min/MAX pipe-section.
  typedef
    otb::StreamingMinMaxVectorImageFilter<
      typename TImageModel::SourceImageType >
    MinMaxFilter;

  typename MinMaxFilter::Pointer filterMinMax( MinMaxFilter::New() );

  filterMinMax->SetInput( imageModel->ToImage() );

  if( context==NULL )
    filterMinMax->GetFilter()->SetNoDataFlag( false );
  else
    {
    filterMinMax->GetFilter()->SetNoDataFlag( context->m_NoDataFlag );
    filterMinMax->GetFilter()->SetNoDataValue( context->m_NoDataValue );
    }

  filterMinMax->Update();

  /*
  // Extract min/MAX intensities for each bands.
  typename MinMaxFilter::PixelType lSrcMin( filterMinMax->GetMinimum() );
  typename MinMaxFilter::PixelType lSrcMax( filterMinMax->GetMaximum() );
  */

  // Extract-convert-remember min/MAX intensities for each band.
  m_MinPixel = filterMinMax->GetMinimum();
  m_MaxPixel = filterMinMax->GetMaximum();

  qDebug() << tr( "%1: Pass #1 - done (%2 ms)." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
    .arg( lPass1.elapsed() );

  //
  // 2nd pass: compute histogram.

  qDebug() << tr( "%1: Pass #2 - computing histogram..." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

  lPass2.start();

  // Connect histogram-generator pipe-section.
  typedef
    otb::StreamingHistogramVectorImageFilter<
      typename TImageModel::SourceImageType >
    HistogramFilter;

  typename HistogramFilter::Pointer histogramFilter( HistogramFilter::New()  );

  histogramFilter->SetInput( imageModel->ToImage() );

  // Setup histogram filter.
  histogramFilter->GetFilter()->SetHistogramMin( m_MinPixel );
  histogramFilter->GetFilter()->SetHistogramMax( m_MaxPixel );
  histogramFilter->GetFilter()->SetNumberOfBins(
    HistogramModel::BINS_OVERSAMPLING_RATE * 256
  );
  histogramFilter->GetFilter()->SetSubSamplingRate( 1 );

  if( context==NULL )
    histogramFilter->GetFilter()->SetNoDataFlag( false );
  else
    {
    histogramFilter->GetFilter()->SetNoDataFlag( context->m_NoDataFlag );
    histogramFilter->GetFilter()->SetNoDataValue( context->m_NoDataValue );
    }

  // Go.
  histogramFilter->Update();

  qDebug() << tr( "%1: Pass #2 - done (%2 ms)." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
    .arg( lPass2.elapsed()  );

  //
  // Reference result.
  m_Histograms = histogramFilter->GetHistogramList();

  qDebug() << tr( "%1: Histogram (M) generated (%2 ms)." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
    .arg( lMain.elapsed() );
}

} // end namespace 'mvd'

#endif // __mvdHistogramModel_h
