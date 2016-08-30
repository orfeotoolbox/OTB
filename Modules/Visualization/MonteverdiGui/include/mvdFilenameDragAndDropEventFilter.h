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
#ifndef __mvdFilenameDragAndDropEventFilter_h
#define __mvdFilenameDragAndDropEventFilter_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

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
#include "mvdAbstractDragAndDropEventFilter.h"

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
 * \class FilenameDragAndDropEventFilter
 *
 * \ingroup OTBMonteverdiGui
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class ITK_EXPORT FilenameDragAndDropEventFilter :
    public AbstractDragAndDropEventFilter
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  FilenameDragAndDropEventFilter( QObject* p =NULL );

  /** \brief Destructor. */
  virtual ~FilenameDragAndDropEventFilter();

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /**
   */
  void FilenameDropped( const QString& filename );

  /**
   */
  void FilenamesDropped( const QStringList & );


  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /**
   * \see http://qt-project.org/doc/qt-4.8/qwidget.html#dragEnterEvent
   */
  virtual bool DragEnterEvent( QObject* object, QDragEnterEvent* event );

  /**
   * \see http://qt-project.org/doc/qt-4.8/qwidget.html#dragLeaveEvent
   */
  virtual bool DragLeaveEvent( QObject* object, QDragLeaveEvent* event );

  /**
   * \see http://qt-project.org/doc/qt-4.8/qwidget.html#dragMoveEvent
   */
  virtual bool DragMoveEvent( QObject* object, QDragMoveEvent* event );

  /**
   * \see http://qt-project.org/doc/qt-4.8/qwidget.html#dropEvent
   */
  virtual bool DropEvent( QObject* object, QDropEvent* event );

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

//
// Private attributes.
private:

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdFilenameDragAndDropEventFilter_h
