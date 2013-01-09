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
#ifndef __mvdColorSetupWidget_h
#define __mvdColorSetupWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.
namespace Ui
{
class ColorSetupWidget;
}

/** \class ColorSetupWidget
 *
 */
class Monteverdi2_EXPORT ColorSetupWidget :
    public QWidget
{
  Q_OBJECT;

  /** */
  Q_PROPERTY( QStringList m_Components
	      READ GetComponents
	      WRITE SetComponents );

//
// Public types.
public:
  enum Channel
  {
    CHANNEL_NONE = -1,

    CHANNEL_RED = 0,
    CHANNEL_GREEN = 1,
    CHANNEL_BLUE = 2,

    CHANNEL_COUNT
  };

//
// Public methods.
public:

  /** Constructor */
  ColorSetupWidget( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** Destructor */
  virtual ~ColorSetupWidget();

  /** */
  void SetComponents( const QStringList& );

  /** */
  inline
    QStringList
    GetComponents() const
  {
    return m_Components;
  }

//
// SIGNALS.
signals:
  /** */
  void currentChannelIndexChanged( Channel channel, int index );

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

//
// Private methods.
private:

//
// Private attributes.
private:
  /** */
  Ui::ColorSetupWidget* m_UI;

  /** */
  QStringList m_Components;

//
// SLOTS.
private slots:
  /** */
  inline
    void
    onRedIndexChanged( int index )
  {
    emit currentChannelIndexChanged( CHANNEL_RED, index );
  }

  /** */
  inline
    void
    onGreenIndexChanged( int index )
  {
    emit currentChannelIndexChanged( CHANNEL_GREEN, index );
  }

  /** */
  inline
    void
    onBlueIndexChanged( int index )
  {
    emit currentChannelIndexChanged( CHANNEL_BLUE, index );
  }
};

} // end namespace 'mvd'

#endif // __mvdColorSetupWidget_h
