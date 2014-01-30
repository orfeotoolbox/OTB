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

#ifndef __mvdAbstractImageViewRenderer_h
#define __mvdAbstractImageViewRenderer_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

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
class Monteverdi2_EXPORT AbstractImageModel;


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class AbstractImageViewRenderer
 *
 * \brief TODO.
 */
class AbstractImageViewRenderer :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  /**
   */
  struct RenderingContext
  {
    /**
     */
    inline
    RenderingContext() :
      m_RenderMode( RENDER_MODE_FULL )
    {
    }

    /**
     */
    enum RenderMode
    {
      RENDER_MODE_LIGHT = 0,
      RENDER_MODE_FULL
    };

    RenderMode m_RenderMode;
  };

//
// Public methods.
public:
  /**
   * Destructor.
   */
  virtual ~AbstractImageViewRenderer();

  /**
   */
  inline void InitializeGL();
  /**
   */
  inline void ResizeGL( int width, int height );
  /**
   */
  inline void PaintGL( const RenderingContext& context );

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

// public slots
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /**
   * Constructor.
   */
  AbstractImageViewRenderer( QObject* parent = NULL );


//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  /**
   */
  virtual void virtual_InitializeGL() =0;
  /**
   */
  virtual void virtual_ResizeGL( int width, int height ) =0;
  /**
   */
  virtual void virtual_PaintGL( const RenderingContext& context ) =0;

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

/*****************************************************************************/
inline
void
AbstractImageViewRenderer
::InitializeGL()
{
  virtual_InitializeGL();
}

/*****************************************************************************/
inline
void
AbstractImageViewRenderer
::ResizeGL( int width, int height )
{
  virtual_ResizeGL( width, height );
}

/*****************************************************************************/
inline
void
AbstractImageViewRenderer
::PaintGL( const RenderingContext& context )
{
  virtual_PaintGL( context );
}

} // end namespace 'mvd'

#endif // __mvdAbstractImageViewRenderer_h
