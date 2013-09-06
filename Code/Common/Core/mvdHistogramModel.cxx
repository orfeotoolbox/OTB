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
#include "mvdHistogramModel.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTextStream.h"
#include "mvdQuicklookModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::HistogramModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

const int HistogramModel::BINS_OVERSAMPLING_RATE = 3;


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
HistogramModel
::HistogramModel( QObject* parent ) :
  AbstractModel( parent ),
  m_Histograms(),
  m_MinPixel(),
  m_MaxPixel()
{
}

/*******************************************************************************/
HistogramModel
::~HistogramModel()
{
}

/*******************************************************************************/
double
HistogramModel
::Percentile( CountType band, MeasurementType intensity, Bound bound ) const
{
  // Get histogram of band.
  Histogram::Pointer histogram( m_Histograms->GetNthElement( band ) );

  // Contruct 1D measurement vector.
  assert( Histogram::MeasurementVectorSize==1 );
  Histogram::MeasurementVectorType measurement( 1 );
  measurement[ 0 ] = intensity;

  // Due to float/double conversion, it can happen
  // that the minimum or maximum value go slightly outside the histogram
  // Clamping the value solves the issue and avoid RangeError
 // itk::NumericsTraits<>::Clamp(...) was removed 
  // TODO : when otb::Clamp will be developped, use this function
  measurement[0]  = 
    measurement[0] < histogram->GetBinMin(0, 0)
    ? histogram->GetBinMin(0, 0)
    : ( measurement[0] > histogram->GetBinMax(0, histogram->GetSize(0) - 1)
	? histogram->GetBinMax(0, histogram->GetSize(0) - 1)
	: measurement[0] );

  // Get the index of measurement in 1D-histogram.
  Histogram::IndexType index;
  if( !histogram->GetIndex( measurement, index ) )
    throw itk::RangeError( __FILE__, __LINE__ );

  assert( Histogram::IndexType::GetIndexDimension()==1 );

  // Min/max intensities of bin.
  MeasurementType minI = histogram->GetBinMin( 0, index[ 0 ] );
  MeasurementType maxI = histogram->GetBinMax( 0, index[ 0 ] );

  // Frequency of current bin
  Histogram::FrequencyType frequency( histogram->GetFrequency( index ) );

  // Initialize result (contribution of current bin)
  const MeasurementType epsilon = 1.0E-6;
  double percent = 0.;
  
  if ( vcl_abs(maxI - minI) > epsilon )
    {
    percent = frequency 
      * (bound == BOUND_LOWER ? (intensity - minI) : (maxI - intensity) ) 
      / ( maxI - minI );
    }
  
  // Number of bins of histogram.
  Histogram::SizeType::SizeValueType binCount = histogram->Size();

  // Initialize bound indices.
  assert( index[ 0 ]>=0 );
  Histogram::SizeType::SizeValueType index0 = index[ 0 ];
  Histogram::SizeType::SizeValueType i0 = 0;
  Histogram::SizeType::SizeValueType iN = binCount;

  switch( bound )
    {
    case BOUND_LOWER:
      i0 = 0;
      iN = index[ 0 ];
      break;

    case BOUND_UPPER:
      i0 = index0 < binCount ? index0 + 1 : binCount;
      iN = binCount;
      break;

    default:
      assert( false && "Implement case statement of switch instruction." );
      break;
    };

  // Traverse lower/upper bound (contribution of other bins)
  Histogram::SizeType::SizeValueType i;

  for( i=i0; i<iN; ++i )
    percent += histogram->GetFrequency( i, 0 );

  // Calculate frequency rate.
  percent /= histogram->GetTotalFrequency();

  // Return frequency rate.
  return percent;
}

/*******************************************************************************/
void
HistogramModel
::GetData( CountType band,
	   double * const x,
	   double * const y,
	   double& xMin,
	   double& xMax,
	   double& yMin,
	   double& yMax ) const
{
  assert( x!=NULL );
  assert( y!=NULL );

  // Get histogram of band.
  Histogram::Pointer histogram( m_Histograms->GetNthElement( band ) );
  assert( !histogram.IsNull() );

  // Get number of bins for each dimension.
  Histogram::SizeType size( histogram->GetSize() );

  // Ensure dimension is 1.
  assert( Histogram::MeasurementVectorSize==1 );

  // Initialize bounds.
  assert( std::numeric_limits< double >::has_infinity );
  yMax = -std::numeric_limits< double >::infinity();
  yMin = 0;
  xMin = histogram->GetBinMin( 0, 0 );
  xMax = histogram->GetBinMax( 0, size[ 0 ] - 1 );

  // Traverse samples.
  for( unsigned long bin=0; bin<size[ 0 ]; ++bin )
    {
    Histogram::FrequencyType f( histogram->GetFrequency( bin, 0 ) );

    if( f>yMax )
      yMax = f;

    if( f<yMin )
      yMin = f;

#if HISTOGRAM_CURVE_TYPE==0
    CountType i = 2 * bin;

    x[ i ] = histogram->GetBinMin( 0, bin );
    y[ i ] = f;

    ++ i;

    x[ i ] = histogram->GetBinMax( 0, bin );
    y[ i ] = f;

    // Warning:
    // coordinates for (x, y) curve will cause graphical artefacts if
    // max[ bin ]!=min[ bin + 1].

#elif HISTOGRAM_CURVE_TYPE==1
    x[ bin ] = histogram->GetBinMin( 0, bin );
    y[ bin ] = f;

#elif HISTOGRAM_CURVE_TYPE==2
    CountType i = 4 * bin;

    x[ i ] = histogram->GetBinMin( 0, bin );
    y[ i ] = 0;

    x[ i + 1 ] = x[ i ];
    y[ i + 1 ] = f;

    x[ i + 2 ] = histogram->GetBinMax( 0, bin );
    y[ i + 2 ] = f;

    x[ i + 3 ] = x[ i + 2 ];
    y[ i + 3 ] = 0;

#else
  assert( false && "Unknown HISTOGRAM_CURVE_TYPE value" );

#endif
    }
}

/*******************************************************************************/
void
HistogramModel
::virtual_BuildModel( void* context )
{
  if( context==NULL )
    {
    template_BuildModel_M< VectorImageModel >();

    // template_BuildModel_I< VectorImageModel::SourceImageType >();

    // template_BuildModel< otb::Image< FixedArray< double, 4 >, 2 > >();
    // template_BuildModel< otb::Image< itk::FixedArray< float, 4 >, 2 > >();

    /*
      template_BuildModel< otb::VectorImage< float, 2 > >();
      template_BuildModel< otb::Image< float, 2 > >();
    */
    }
  else
    {
    // Retrieve proper build-context.
    BuildContext* buildContext = static_cast< BuildContext* >( context );

    if( buildContext->IsBeingStored() )
      {
      template_BuildModel_M< VectorImageModel >( buildContext );

      Write( buildContext->m_Filename, SerializableInterface::MODE_TEXT );
      }
    else
      {
      if( buildContext->m_Filename.isEmpty() )
	{
	template_BuildModel_M< VectorImageModel >( buildContext );
	}
      else
	{
	Read( buildContext->m_Filename, SerializableInterface::MODE_TEXT );
	}
      }
    }
}

/*******************************************************************************/
void
HistogramModel
::virtual_Read( QIODevice* device )
{
  // qDebug() << this << "::virtual_Read(" << device << ")";

  qDebug() <<
    tr( "%1: Reading histogram..." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

  QTime time;
  time.start();


  QTextStream stream( device );


  QString string;
  QString version;

  ReadStreamTag( stream, string, PROJECT_NAME, false );
  ReadStreamTag( stream, version, QString(), false );
  ReadStreamTag( stream, string, "HISTOGRAM-MODEL", false );


  ReadStreamTag( stream, string, "MIN-PIXEL" );
  stream >> m_MinPixel;

  ReadStreamTag( stream, string, "MAX-PIXEL" );
  stream >> m_MaxPixel;

  ReadStreamTag( stream, string, "BANDS" );
  CountType size = 0;
  stream >> size;
  CheckStreamStatus( stream );

  HistogramList::Pointer histograms( HistogramList::New() );

  histograms->Initialize();
  histograms->Resize( size );

  for( CountType i=0; i<size; ++i )
    {
    Histogram::Pointer histogram( Histogram::New() );

    stream >> *histogram;

    histograms->SetNthElement( i, histogram );
    }

  m_Histograms = histograms;

  qDebug() <<
    tr( "%1: Histogram read (%2 ms)..." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
    .arg( time.elapsed()  );
}

/*******************************************************************************/
void
HistogramModel
::virtual_Write( QIODevice& device ) const
{
  // qDebug() << this << "::virtual_Write(" << &device << ")";

  qDebug() <<
    tr( "%1: Writing histogram..." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) );

  QTime time;
  time.start();

#if 0
  QByteArray byteArray;
  QTextStream stream( &byteArray, device.openMode() );
#else
  QTextStream stream( &device );
#endif

  stream << PROJECT_NAME << endl;
  stream << Monteverdi2_VERSION_STRING << endl;
  stream << "HISTOGRAM-MODEL" << endl;

  WriteStreamTag( stream, "MIN-PIXEL" );
  stream << m_MinPixel << endl;

  WriteStreamTag( stream, "MAX-PIXEL" );
  stream << m_MaxPixel << endl;

  WriteStreamTag( stream, "BANDS" );
  stream << m_Histograms->Size() << endl;

  for( HistogramList::ConstIterator it( m_Histograms->Begin() );
       it!=m_Histograms->End();
       ++it )
    {
    stream << *it.Get();
    }

  stream << flush;
  CheckStreamStatus( stream );

  qDebug() <<
    tr( "%1: Histogram written (%2 ms)..." )
    .arg( QDateTime::currentDateTime().toString( Qt::ISODate ) )
    .arg( time.elapsed()  );

#if 0
  qDebug() << byteArray;

  if( device.write( byteArray )!=byteArray.size() )
    throw SystemError();
#else
#endif
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
