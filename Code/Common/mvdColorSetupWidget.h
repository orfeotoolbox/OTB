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
#include "mvdGui.h"

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

/**
 * \class ColorSetupWidget
 */
class Monteverdi2_EXPORT ColorSetupWidget :
    public QWidget
{
  Q_OBJECT;

  /**
   * The (user) component-name list.
   */
  Q_PROPERTY( QStringList components
	      READ GetComponents
	      WRITE SetComponents );

//
// Public types.
public:

//
// Public methods.
public:

  /** Constructor */
  ColorSetupWidget( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** Destructor */
  virtual ~ColorSetupWidget();

  /**
   * Set the component-name list.
   *
   * This sets up the content of the red, green and blue combo
   * boxes. Each component name is prepended with its index
   * (e.g. '<i>: NAME'), if non-empty. Otherwise, 'BAND <i>' is
   * inserted into the combo-box.
   *
   * Calling SetComponents() resets the current-index of each
   * combo-box to the first entry of the list, which causes the
   * CurrentIndexChanged() signal to be emitted for each component.
   *
   * Example: The user { "Red", "Green", "Blue", "" } component-name
   * list will be displayed as the { "0: Red", "1: Green", "2: Blue",
   * "BAND 3" } combo-box list.
   *
   * \param component component-name list.
   */
  void SetComponents( const QStringList& component );

  /**
   * Get the component-name list.
   *
   * Example: The user { "Red", "Green", "Blue", "" } list as it were
   * set and not the displayed { "0: Red", "1: Green", "2: Blue",
   * "BAND 3" } combo-box list.
   *
   * \return the component-name list.
   */
  inline
    QStringList
    GetComponents() const
  {
    return m_Components;
  }

  /**
   * Set the current (selected) index of a video channel.
   *
   * Calling SetCurrentIndex() let the CurrentIndexChanged()
   * signal to be emitted.
   *
   * \param channel The video channel for which to set the current
   * index.
   * \param index The index in the component-name list.
   *
   * Example: If the { "Red", "Green", "Blue", "" } component-name
   * list has been set, index 1 set "1: Green" as the current select
   * combo-box item.
   */
  void SetCurrentIndex( RgbaChannel channel, int index );

  /**
   * \return the current (selected) index of a video-channel in the
   * component-name list.
   */
  int GetCurrentIndex( RgbaChannel channel );

//
// SIGNALS.
signals:
  /**
   * Signal emitted when the current-index of a video-channel has been
   * modified.
   *
   * \param channel The video-channel which has been modified.
   * \param index The curren-index in the component-list.
   */
  void CurrentIndexChanged( RgbaChannel channel, int index );

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
  /**
   * uic generated.
   */
  Ui::ColorSetupWidget* m_UI;

  /** 
   * The (user) component-name list.
   */
  QStringList m_Components;

//
// SLOTS.
private slots:
  /**
   * Slot called when the curernt-index of the red video-channel has
   * been modified.
   */
  inline
    void
    on_redComboBox_currentIndexChanged( int index )
  {
    emit CurrentIndexChanged( RGBA_CHANNEL_RED, index );
  }

  /**
   * Slot called when the curernt-index of the green video-channel has
   * been modified.
   */
  inline
    void
    on_greenComboBox_currentIndexChanged( int index )
  {
    emit CurrentIndexChanged( RGBA_CHANNEL_GREEN, index );
  }

  /**
   * Slot called when the curernt-index of the blue video-channel has
   * been modified.
   */
  inline
    void
    on_blueComboBox_currentIndexChanged( int index )
  {
    emit CurrentIndexChanged( RGBA_CHANNEL_BLUE, index );
  }
};

} // end namespace 'mvd'

#endif // __mvdColorSetupWidget_h
