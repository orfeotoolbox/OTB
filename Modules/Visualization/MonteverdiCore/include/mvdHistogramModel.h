/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
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
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "ConfigureMonteverdi.h"
#endif //tag=QT4-boost-compatibility


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "itkHistogram.h"
#endif //tag=QT4-boost-compatibility

//
// OTB includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbObjectList.h"
#endif //tag=QT4-boost-compatibility

//
// Monteverdi includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "mvdAbstractModel.h"
#include "mvdAlgorithm.h"
#include "mvdCore.h"
#include "mvdSerializableInterface.h"
#endif //tag=QT4-boost-compatibility

#define HISTOGRAM_CURVE_TYPE 2
#define USE_FULL_IMAGE_FOR_PASS_1 0

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
 *
 * \ingroup OTBMonteverdiCore
 */
class ITK_EXPORT HistogramModel :
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
    itk::NumericTraits< DefaultImageType::InternalPixelType >::RealType RealType;

  /**
   */
  typedef RealType MeasurementType;

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
		  const QString& filename =QString() ) :
      m_Filename( filename ),
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

  private:
    bool m_IsBeingStored;
  };

//
// Public methods.
public:

  /** \brief Constructor. */
  HistogramModel( QObject* p =NULL );

  /** \brief Destructor. */
  virtual ~HistogramModel();

  /**
   */
  bool IsValid() const;

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

  /**
   */
  inline size_t GetDataCount( CountType band ) const;

  /**
   */
  void GetData( CountType band,
		double * const x,
		double * const y,
		double& xMin,
		double& xMax,
		double& yMin,
		double& yMax ) const;

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


//
// Private types.
private:

  /** */
  typedef itk::Statistics::Histogram< MeasurementType > Histogram;

  /** */
  typedef otb::ObjectList< Histogram > HistogramList;

//
// Private methods.
private:
  /**
   */
  /*
  template< typename TImage >
    void template_BuildModel_I();
  */

  /**
   */
  template< typename TImageModel >
    void template_BuildModel_M( BuildContext * =NULL );

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
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbStreamingHistogramVectorImageFilter.h"
#include "otbStreamingMinMaxVectorImageFilter.h"
#include "otbStreamingStatisticsVectorImageFilter.h"
#endif //tag=QT4-boost-compatibility

//
// Monteverdi includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "mvdAbstractImageModel.h"
#endif //tag=QT4-boost-compatibility

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
inline
size_t
HistogramModel
::GetDataCount( CountType band ) const
{
  // Get histogram of band.
  Histogram::Pointer histogram( m_Histograms->GetNthElement( band ) );
  assert( !histogram.IsNull() );

  // Get number of bins for each dimension.
  Histogram::SizeType size( histogram->GetSize() );

  // Ensure dimension is 1.
  assert( histogram->GetMeasurementVectorSize()==1 );

  // There are twince number of points than number of bins.
#if HISTOGRAM_CURVE_TYPE==0
  return 2 * size[ 0 ];

#elif HISTOGRAM_CURVE_TYPE==1
  return size[ 0 ];

#elif HISTOGRAM_CURVE_TYPE==2
  return 4 * size[ 0 ];

#else
  assert( false && "Unknown HISTOGRAM_CURVE_TYPE value" );

#endif
}

/*******************************************************************************/
/*
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
*/

/*******************************************************************************/
template< typename TImageModel >
void
HistogramModel
::template_BuildModel_M( BuildContext * )
{
  QTime lMain;
  QTime lPass1;
  QTime lPass2;

  lMain.start();

  qDebug() << tr( "%1: Generating histogram (M)..." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );


#if USE_FULL_IMAGE_FOR_PASS_1
  TImageModel * parentImageModel =
    qobject_cast< TImageModel * >( parent() );

#else // USE_FULL_IMAGE_FOR_PASS_1
  AbstractImageModel * parentImageModel =
    qobject_cast< AbstractImageModel* >( parent() );

#endif // USE_FULL_IMAGE_FOR_PASS_1

  assert( parentImageModel!=NULL );

  ImageProperties * imageProperties = parentImageModel->GetProperties();
  assert( imageProperties!=NULL );

  TImageModel * imageModel = parentImageModel->GetQuicklookModel();

  if( imageModel==NULL )
    imageModel = qobject_cast< TImageModel * >( parentImageModel );

  assert( imageModel!=NULL );

  CountType components = imageModel->ToImage()->GetNumberOfComponentsPerPixel();
  assert( components>0 );

  //
  // Always initialize min and Max pixels.

  m_MinPixel = DefaultImageType::PixelType( components );
  m_MaxPixel = DefaultImageType::PixelType( components );

  m_MinPixel.Fill( 0 );
  m_MaxPixel.Fill( 1 );

  try
    {
    //
    // 1st pass: process min/MAX for each band.

    qDebug() << QString( "%1: Pass #1 - finding pixel min/maxes..." )
                .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

    lPass1.start();

    // Define histogram-filter type.
    typedef
      otb::StreamingHistogramVectorImageFilter<
	typename TImageModel::SourceImageType >
      HistogramFilter;


    // Connect statistics pipe-section.
    typedef
      otb::StreamingStatisticsVectorImageFilter<
	typename TImageModel::SourceImageType >
      StatisticsFilter;

    typename StatisticsFilter::Pointer filterStats( StatisticsFilter::New() );

#if USE_FULL_IMAGE_FOR_PASS_1
    filterStats->SetInput( parentImageModel->ToImage() );

#else // USE_FULL_IMAGE_FOR_PASS_1
    filterStats->SetInput( imageModel->ToImage() );

#endif // USE_FULL_IMAGE_FOR_PASS_1

    filterStats->SetEnableMinMax( true );
    filterStats->SetIgnoreUserDefinedValue( imageProperties->IsNoDataEnabled() );
    filterStats->SetUserIgnoredValue( imageProperties->GetNoData() );

    // Connect statistics pipe-section.
    filterStats->Update();

    // qDebug() << "min:" << m_MinPixel << "max:" << m_MaxPixel;
    // std::cout << "min:" << m_MinPixel << "\tmax:" << m_MaxPixel << std::endl;

    // Extract-convert-remember min/MAX intensities for each band.
    m_MinPixel = filterStats->GetMinimum();
    m_MaxPixel = filterStats->GetMaximum();

    // Extract sigmas for each band from covariance matrix.
    typename StatisticsFilter::MatrixType covariance(
      filterStats->GetFilter()->GetCovariance()
    );

    typename HistogramFilter::FilterType::CountVectorType bins( components );
    typename StatisticsFilter::RealPixelType sums( filterStats->GetSum() );
    typename StatisticsFilter::RealPixelType means( filterStats->GetMean() );

    for( CountType i=0; i<components; ++i )
      {
      RealType n = sums[ i ] / means[ i ];

      // qDebug() << "#" << i << ": " << n;

      if( n <= 1.0 )
	{
	bins[ i ] = 1;
	}
      else
	{
	RealType sigma = sqrt( covariance( i, i ) );

	// qDebug() << "#" << i << ":" << sigma;

	assert( sigma >= 0.0 );
     
	if( sigma<=0.0 )
	  {
	  bins[ i ] = 1;
	  }
	else
	  {
	  // Scott's formula
	  // See http://en.wikipedia.org/wiki/Histogram#Number_of_bins_and_width
	  RealType h = 3.5 * sigma / pow( n, 1.0 / 3.0 );

	  /*
	    qDebug()
	    << "#" << i
	    << ": h = pow(" << n << "," << 1.0 / 3.0 << ") ="
	    << h;

	    qDebug()
	    << "#" << i
	    << ": bins[" << i << "] = ceil( ("
	    << m_MaxPixel[ i ] << "-" << m_MinPixel[ i ] << ") / " << h << ") ="
	    << ceil( ( m_MaxPixel[ i ] - m_MinPixel[ i ] ) / h );
	  */

	  bins[ i ] = ceil( ( m_MaxPixel[ i ] - m_MinPixel[ i ] ) / h );
	  }
	}
      }

    // std::cout << bins;

    qDebug() << QString( "%1: Pass #1 - done (%2 ms)." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
      .arg( lPass1.elapsed() );

    //
    // 2nd pass: compute histogram.

    qDebug() << QString( "%1: Pass #2 - computing histogram..." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

    lPass2.start();

    // Connect histogram-generator pipe-section.

    typename HistogramFilter::Pointer histogramFilter( HistogramFilter::New()  );

    histogramFilter->SetInput( imageModel->ToImage() );

    // Setup histogram filter.
    histogramFilter->GetFilter()->SetHistogramMin( m_MinPixel );
    histogramFilter->GetFilter()->SetHistogramMax( m_MaxPixel );
    histogramFilter->GetFilter()->SetNumberOfBins( bins );
    histogramFilter->GetFilter()->SetSubSamplingRate( 1 );
    histogramFilter->GetFilter()->SetNoDataFlag(
      imageProperties->IsNoDataEnabled() );
    histogramFilter->GetFilter()->SetNoDataValue( imageProperties->GetNoData() );

    // Go.
    histogramFilter->Update();

    qDebug() << QString( "%1: Pass #2 - done (%2 ms)." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
      .arg( lPass2.elapsed()  );

    //
    // Reference result.
    m_Histograms = histogramFilter->GetHistogramList();

    qDebug() << QString( "%1: Histogram (M) generated (%2 ms)." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
      .arg( lMain.elapsed() );
    }
  catch( const std::exception & exception )
    {
    qWarning()
      << tr( "Zero relevant pixels found when computing histogram (probably because of no-data settings)" );

    if( imageProperties->IsNoDataEnabled() )
      {
      DefaultImageType::PixelType::ValueType value( imageProperties->GetNoData() );

      m_MinPixel.Fill( value );
      m_MaxPixel.Fill( value );
      }

    qDebug() << QString( "%1: Pass #1 - aborted (%2 ms)." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
      .arg( lPass1.elapsed() );

    qDebug() << QString( "%1: Pass #2 - aborted (%2 ms)." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
      .arg( lPass2.elapsed()  );

    qDebug() << QString( "%1: No Histogram (M) has been generated (%2 ms)." )
      .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
      .arg( lMain.elapsed() );
    }
}

} // end namespace 'mvd'

#endif // __mvdHistogramModel_h
