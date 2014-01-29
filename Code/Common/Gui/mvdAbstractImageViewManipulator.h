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

#ifndef __mvdAbstractImageViewManipulator_h
#define __mvdAbstractImageViewManipulator_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


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

/** \class AbstractImageViewManipulator
 *
 *  \brief Base class for QWidget manipulation
 */
class Monteverdi2_EXPORT AbstractImageViewManipulator :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  AbstractImageViewManipulator( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~AbstractImageViewManipulator();

  /**
   */
  virtual void mouseMoveEvent( QMouseEvent* event ) = 0;
  virtual void mousePressEvent( QMouseEvent* event ) = 0;
  virtual void mouseReleaseEvent( QMouseEvent* event ) = 0;

  virtual void wheelEvent( QWheelEvent* event) = 0;

  virtual void keyPressEvent( QKeyEvent * event )  = 0;

  virtual void resizeEvent( QResizeEvent* event ) = 0;

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected types.
protected:
  /** Navigation context  */
  struct NavigationContext
  {
    /**
     * \brief Constructor.
     *
     * Default constructor (safely) initializes POD (Plain Old Data)
     * structure.
     */
    NavigationContext()
    {
    }
  };

  /** Mouse context */
  struct MouseContext
  {
    /**
     * \brief Default constructor.
     */
    MouseContext()
    {
    }
  };

//
// Protected methods.
protected:
  
//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types.
private:

//
// Private methods.
private:

//
// Private attributes.
private:

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// SLOTS.
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdAbstractImageViewManipulator_h
