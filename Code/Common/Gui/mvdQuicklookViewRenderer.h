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

#ifndef __mvdQuicklookViewRenderer_h
#define __mvdQuicklookViewRenderer_h

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
#include "otbGlROIActor.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "Gui/mvdImageViewRenderer.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace otb
{
}

namespace mvd
{
//
// Internal classes pre-declaration.


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class QuicklookViewRenderer
 */
class Monteverdi2_EXPORT QuicklookViewRenderer :
    public ImageViewRenderer
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:

  /**
   */
  struct RenderingContext :
    public ImageViewRenderer::RenderingContext
  {
    /**
     */
    inline
    RenderingContext() :
      ImageViewRenderer::RenderingContext(),
      m_RoiOrigin(),
      m_RoiExtent()
    {
      m_RoiOrigin.Fill( 0 );
      m_RoiOrigin.Fill( 0 );
    }

    virtual ~RenderingContext() {}

    PointType m_RoiOrigin;
    PointType m_RoiExtent;
  };

//
// Public methods.
public:
  /** Constructor */
  QuicklookViewRenderer( QObject* parent = NULL );

  /** Destructor */
  virtual ~QuicklookViewRenderer();

  //
  // ImageViewRenderer overloads.

  virtual
  AbstractImageViewRenderer::RenderingContext* NewRenderingContext() const;

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

  //
  // ImageViewRenderer overloads.

  virtual
  void UpdateActors( const AbstractImageViewRenderer::RenderingContext* c );

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types
private:

//
// Private methods.
private:

  //
  // AbstractImageViewRenderer overloads.

  virtual void virtual_FinishScene();

//
// Private attributes.
private:
  /**
   */
  otb::GlROIActor::Pointer m_GlRoiActor;

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

#endif // __mvdQuicklookViewRenderer_h
