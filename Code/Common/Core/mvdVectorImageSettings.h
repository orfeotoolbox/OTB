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
#ifndef __mvdVectorImageSettings_h
#define __mvdVectorImageSettings_h

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

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdCore.h"


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
 * \class VectorImageSettings
 *
 * \brief WIP.
 */
class Monteverdi2_EXPORT VectorImageSettings
{

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/
//
// Public types.
public:
  /**
   */
  typedef UIntVector ChannelVector;

//
// Public methods.
public:
  /**
   * \brief Constructor.
   */
  VectorImageSettings();

  /**
   * \brief Copy constructor.
   *
   * \param other
   *
   * Copy settings POD content without the modified flag.
   */
  VectorImageSettings( const VectorImageSettings & other );

  /**
   * \brief Destructor.
   */
  ~VectorImageSettings();

  /**
   * \brief Assignment operator.
   *
   * \param other
   */
  VectorImageSettings & operator=( const VectorImageSettings & other );

  /**
   */
  inline bool IsApplied() const;

  /**
   */
  inline bool IsModified() const;

  /**
   */
  inline void ClearModified();

  /**
   */
  inline void SetApplied();

  //
  // COLOR COMPOSITION.
  //

  /**
   */
  inline void SetRgbChannels( const ChannelVector& rgb );

  /**
   */
  inline const ChannelVector& GetRgbChannels() const;

  /**
   */
  inline
    void SetRgbChannel( ChannelVector::size_type i,
			const ChannelVector::value_type& channel );

  /**
   */
  inline
    const ChannelVector::value_type&
    GetRgbChannel( ChannelVector::size_type i ) const;

  /**
   * \return the band-index for the given component taking the
   * grayscale-mode activation state flag into account.
   */
  inline
    ChannelVector::value_type
    GetSmartChannel( RgbwChannel channel ) const;

  /**
   * \return the band-index for the given component not taking the
   * grayscale-mode activation state flag into account.
   */
  ChannelVector::value_type GetRgbwChannel( RgbwChannel channel ) const;

  /**
   * \return the channels band-index vector taking the
   * grayscale-mode activation state flag into account.
   */
  inline void GetSmartChannels( ChannelVector& channels ) const;

  //
  // COLOR DYNAMICS.
  //

  /**
   */
  inline void SetRgbDynamicsParams( const ParametersType& params );

  /**
   */
  inline const ParametersType& GetRgbDynamicsParams() const;

  /**
   */
  inline ParametersType GetSmartDynamicsParams() const;

  /**
   * Set low-intensity dynamics parameter for given channel. If
   * channel is RGBW_CHANNEL_WHITE, this method sets all RGB
   * channels.
   *
   * \param channel The RGB/W channel to set low-intensity dynamics for.
   * \param intensity low-intensity value.
   */
  void SetLowIntensity( RgbwChannel channel,
                        ParametersType::ValueType intensity );

  /**
   */
  ParametersType::ValueType GetLowIntensity( RgbwChannel channel ) const;

  /**
   * Set high-intensity dynamics parameter for given channel. If
   * channel is RGBW_CHANNEL_WHITE, this method sets all RGB
   * channels.
   *
   * \param channel The RGB/W channel to set high-intensity dynamics for.
   * \param intensity high-intensity value.
   */
  void SetHighIntensity( RgbwChannel channel,
                         ParametersType::ValueType intensity );

  /**
   */
  ParametersType::ValueType GetHighIntensity( RgbwChannel channel ) const;

  //
  // GRAYSCALE MODE.
  //

  /**
   * \brief Set/clear grayscale-mode activation-state flag for image.
   */
  inline void SetGrayscaleActivated( bool activated );

  /**
   * \return Grayscale-mode activation state flag.
   */
  inline bool IsGrayscaleActivated() const;

  /**
   * \brief Set white (gray) channel index.
   *
   * \param index The new index of white (gray) channel.
   */
  inline void SetGrayChannel( unsigned int );

  /**
   * \return White (gray) channel index.
   */
  inline unsigned int GetGrayChannel() const;

  /**
   */
  inline void SetGrayDynamicsParams( const ParametersType& params );

  /**
   */
  inline const ParametersType& GetGrayDynamicsParams() const;

  /**
   */
  inline void SetGamma( double gamma );

  /**
   */
  inline double GetGamma() const;


  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  /**
   */
  inline
    const ParametersType::ValueType& GetRgbDynamicsParam( CountType i ) const;

  /**
   */
  inline
    void SetRgbDynamicsParam( CountType i,
			      const ParametersType::ValueType& param );

  /**
   */
  inline
    const ParametersType::ValueType& GetGrayDynamicsParam( bool high ) const;

  /**
   */
  inline
    void SetGrayDynamicsParam( bool high,
			       const ParametersType::ValueType& param );

  /**
   */
  inline void SetModified();


//
// Private attributes.
private:
  /**
   * \brief Color-composition setup (file-component to video
   * RGB-components).
   */
  ChannelVector m_RgbChannels;

  /**
   * \brief Color-dynamics parameters (\sa
   * HistogramModel::Quantile()).
   */
  ParametersType m_RgbDynamicsParams;

  /**
   * \brief Grayscale-mode activation state.
   */
  bool m_IsGrayscaleActivated;

  /**
   * \brief Grayscale-mode band-index.
   */
  ChannelVector::value_type m_GrayChannel;

  /**
   * \brief Color-dynamics parameters (\sa
   * HistogramModel::Quantile()).
   */
  ParametersType m_GrayDynamicsParams;

  /**
   */
  double m_Gamma;

  /**
   * \brief Flag which notices that rendering settings have been
   * edited.
   */
  // TODO: Optimize using C++ bitset bool foo:1;
  bool m_IsModified: 1;

  /**
   * \brief Flag which notices that rendering settings have been
   * applied to display.
   */
  // TODO: Optimize using C++ bitset bool foo:1;
  bool m_IsApplied: 1;
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

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

namespace mvd
{

/*****************************************************************************/
inline
VectorImageSettings &
VectorImageSettings
::operator=( const VectorImageSettings & other )
{
  if( &other==this )
    return *this;

  m_RgbChannels = other.m_RgbChannels;
  m_RgbDynamicsParams = other.m_RgbDynamicsParams;
  m_IsGrayscaleActivated = other.m_IsGrayscaleActivated;
  m_GrayChannel = other.m_GrayChannel;
  m_GrayDynamicsParams = other.m_GrayDynamicsParams;
  m_IsApplied = other.m_IsApplied;
  m_Gamma = other.m_Gamma;

  return *this;
}

/*****************************************************************************/
inline
bool
VectorImageSettings
::IsApplied() const
{
  return m_IsApplied;
}

/*****************************************************************************/
inline
bool
VectorImageSettings
::IsModified() const
{
  return m_IsModified;
}

/*****************************************************************************/
inline
void
VectorImageSettings
::SetModified()
{
  // qDebug() << this << "::SetModified()";

  m_IsModified = true;
  m_IsApplied = false;
}

/*****************************************************************************/
inline
void
VectorImageSettings
::ClearModified()
{
  // qDebug() << this << "::ClearModified()";

  m_IsModified = false;
}

/*****************************************************************************/
inline
void
VectorImageSettings
::SetApplied()
{
  // qDebug() << this << "::SetApplied()";

  m_IsApplied = true;
}

/*****************************************************************************/
inline
void
VectorImageSettings
::SetRgbChannels( const ChannelVector& rgb )
{
  // qDebug() << "setRgbChannels()";

  assert( m_RgbChannels.empty() || m_RgbChannels.size()==rgb.size() );

  if( !m_RgbChannels.empty() &&
      std::equal( m_RgbChannels.begin(), m_RgbChannels.end(), rgb.begin() ) )
    return;

  m_RgbChannels = rgb;

  SetModified();
}

/*****************************************************************************/
inline
const VectorImageSettings::ChannelVector&
VectorImageSettings
::GetRgbChannels() const
{
  return m_RgbChannels;
}

/*****************************************************************************/
inline
void
VectorImageSettings
::GetSmartChannels( VectorImageSettings::ChannelVector& channels ) const
{
  if( IsGrayscaleActivated() )
    {
    channels.resize( m_RgbChannels.size() );
    std::fill( channels.begin(), channels.end(), m_GrayChannel );
    }
  else
    {
    channels = m_RgbChannels;
    }
}

/*****************************************************************************/
inline
void
VectorImageSettings
::SetRgbChannel( ChannelVector::size_type i,
		 const ChannelVector::value_type& channel )
{
  // qDebug() << "SetRgbChannel()";

  if( m_RgbChannels[ i ]==channel )
    return;

  SetModified();

  m_RgbChannels[ i ] = channel;
}

/*****************************************************************************/
inline
const VectorImageSettings::ChannelVector::value_type&
VectorImageSettings
::GetRgbChannel( ChannelVector::size_type i ) const
{
  return m_RgbChannels[ i ];
}

/*****************************************************************************/
inline
VectorImageSettings::ChannelVector::value_type
VectorImageSettings
::GetSmartChannel( RgbwChannel channel ) const
{
  return GetRgbwChannel(
    m_IsGrayscaleActivated
    ? RGBW_CHANNEL_WHITE
    : channel
  );

  /*
  if( m_IsGrayscaleActivated )
    {
    return m_GrayChannel;
    }
  else
    {
    return GetRgbwChannel( channel );
    }
  */
}

/*****************************************************************************/
inline
void
VectorImageSettings
::SetRgbDynamicsParams( const ParametersType& params )
{
  // qDebug() << "SetDynamicsParams()";

  assert( m_RgbDynamicsParams.size()==params.size() );

  if( std::equal( m_RgbDynamicsParams.begin(),
		  m_RgbDynamicsParams.end(),
		  params.begin() ) )
    return;

  m_RgbDynamicsParams = params;

  SetModified();
}

/*****************************************************************************/
inline
const ParametersType&
VectorImageSettings
::GetRgbDynamicsParams() const
{
  return m_RgbDynamicsParams;
}

/*****************************************************************************/
inline
ParametersType
VectorImageSettings
::GetSmartDynamicsParams() const
{
  return m_IsGrayscaleActivated ? m_GrayDynamicsParams : m_RgbDynamicsParams;
}

/*****************************************************************************/
inline
const ParametersType::ValueType&
VectorImageSettings
::GetRgbDynamicsParam( CountType i ) const
{
  return m_RgbDynamicsParams[ i ];
}

/*****************************************************************************/
inline
void
VectorImageSettings
::SetRgbDynamicsParam( CountType i,
		       const ParametersType::ValueType& param )
{
  // qDebug() << "SetDynamicsParam()";

  if( m_RgbDynamicsParams[ i ]==param )
    return;

  SetModified();

  m_RgbDynamicsParams[ i ] = param;
}

/*****************************************************************************/
inline
bool
VectorImageSettings
::IsGrayscaleActivated() const
{
  return m_IsGrayscaleActivated;
}

/*****************************************************************************/
inline
void
VectorImageSettings
::SetGrayscaleActivated( bool activated )
{
  m_IsGrayscaleActivated = activated;

  SetModified();
}

/*****************************************************************************/
inline
unsigned int
VectorImageSettings
::GetGrayChannel() const
{
  return m_GrayChannel;
}

/*****************************************************************************/
inline
void
VectorImageSettings
::SetGrayChannel( unsigned int index )
{
  m_GrayChannel = index;

  SetModified();
}

/*****************************************************************************/
inline
void
VectorImageSettings
::SetGrayDynamicsParams( const ParametersType& params )
{
  // qDebug() << "SetDynamicsParams()";

  assert( m_GrayDynamicsParams.size()==params.size() );

  if( std::equal( m_GrayDynamicsParams.begin(),
		  m_GrayDynamicsParams.end(),
		  params.begin() ) )
    return;

  m_GrayDynamicsParams = params;

  SetModified();
}

/*****************************************************************************/
inline
const ParametersType&
VectorImageSettings
::GetGrayDynamicsParams() const
{
  return m_GrayDynamicsParams;
}

/*****************************************************************************/
inline
const ParametersType::ValueType&
VectorImageSettings
::GetGrayDynamicsParam( bool high ) const
{
  return m_GrayDynamicsParams[ high ? 1 : 0 ];
}

/*****************************************************************************/
inline
void
VectorImageSettings
::SetGrayDynamicsParam( bool high,
			const ParametersType::ValueType& param )
{
  // qDebug() << "SetDynamicsParam()";

  CountType ofs = high ? 1 : 0;

  if( m_GrayDynamicsParams[ 0 * 2 + ofs ]==param )
    return;

  SetModified();

  // Indices are detailed for better readability since the compiler
  // will optimize constants.
  m_GrayDynamicsParams[ 0 * 2 + ofs ] = param;
  m_GrayDynamicsParams[ 1 * 2 + ofs ] = param;
  m_GrayDynamicsParams[ 2 * 2 + ofs ] = param;
}

/*****************************************************************************/
inline
void
VectorImageSettings
::SetGamma( double value )
{
  m_Gamma = value;

  SetModified();
}

/*****************************************************************************/
inline
double
VectorImageSettings
::GetGamma() const
{
  return m_Gamma;
}

} // end namespace 'mvd'

#endif // __mvdVectorImageSettings_h
