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

#ifndef __mvdImageViewRenderer_h
#define __mvdImageViewRenderer_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
#define USE_VIEW_SETTINGS_SIDE_EFFECT 0

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
#include "otbGlView.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "Gui/mvdAbstractImageViewRenderer.h"


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
 * \class ImageViewRenderer
 */
class ImageViewRenderer :
    public AbstractImageViewRenderer
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
    public AbstractImageViewRenderer::RenderingContext
  {
    /**
     */
    inline
    RenderingContext() :
      AbstractImageViewRenderer::RenderingContext(),
      m_ViewSettings()
    {
    }

    virtual ~RenderingContext() {}

    otb::ViewSettings::Pointer m_ViewSettings;
  };

//
// Public methods.
public:
  /** Constructor */
  ImageViewRenderer( QObject* parent = NULL );

  /** Destructor */
  virtual ~ImageViewRenderer();

  /**
   */
  inline const otb::ViewSettings::Pointer GetViewSettings() const;
  /**
   */
  inline otb::ViewSettings::Pointer GetViewSettings();

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
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  /**
   */
  virtual void virtual_SetImageList( const VectorImageModelList& images );

  /**
   */
  virtual
  AbstractImageViewRenderer::RenderingContext*
  virtual_NewRenderingContext() const;

  /**
   */
  virtual void virtual_InitializeGL();
  /**
   */
  virtual void virtual_ResizeGL( int width, int height );
  /**
   */
  virtual
  void 
  virtual_PaintGL( const AbstractImageViewRenderer::RenderingContext* context );

//
// Private attributes.
private:
  /**
   */
  otb::GlView::Pointer m_GlView;

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
const otb::ViewSettings::Pointer
ImageViewRenderer
::GetViewSettings() const
{
  assert( !m_GlView.IsNull() );

  return m_GlView->GetSettings();
}

/*****************************************************************************/
inline
otb::ViewSettings::Pointer
ImageViewRenderer
::GetViewSettings()
{
  assert( !m_GlView.IsNull() );

  return m_GlView->GetSettings();
}

} // end namespace 'mvd'

#endif // __mvdImageViewRenderer_h
