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

#ifndef __mvdImageViewManipulator_h
#define __mvdImageViewManipulator_h

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
#include "otbViewSettings.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "Gui/mvdAbstractImageViewManipulator.h"
#include "Gui/mvdImageViewRenderer.h"


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
 * \class ImageViewManipulator
 *
 *  \brief TODO.
 */
class Monteverdi2_EXPORT ImageViewManipulator :
    public AbstractImageViewManipulator
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
#if USE_VIEW_SETTINGS_SIDE_EFFECT
  ImageViewManipulator( const otb::ViewSettings::Pointer& viewSettings,
                        QObject* parent =NULL );
#else // USE_VIEW_SETTINGS_SIDE_EFFECT
  ImageViewManipulator( QObject* parent =NULL );
#endif // USE_VIEW_SETTINGS_SIDE_EFFECT

  /** \brief Destructor. */
  virtual ~ImageViewManipulator();

  /**
   */
  virtual void SetViewportSize( int width, int height );
  /**
   */
  virtual void SetOrigin( const PointType& origin );
  /**
   */
  virtual void SetSpacing( const SpacingType& spacing );
  /**
   */
  virtual void SetWkt( const std::string& wkt );
  /**
   */
  virtual void SetKeywordList(
    const DefaultImageType::ImageKeywordlistType& kwl
  );

  /**
   */
  virtual
    void
    SetupRenderingContext(
      AbstractImageViewRenderer::RenderingContext * const ) const;

  /**
   */
  virtual void MouseMoveEvent( QMouseEvent * event );
  /**
   */
  virtual void MousePressEvent( QMouseEvent * event );
  /**
   */
  virtual void MouseReleaseEvent( QMouseEvent * event );
  /**
   */
  virtual void WheelEvent( QWheelEvent* event);
  /**
   */
  virtual void ResizeEvent( QResizeEvent * event );
  /**
   */
  virtual void KeyPressEvent( QKeyEvent * event );

  /*-[ PUBLIC SLOTS SECTION ]-----------------------------------------------**/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Private types.
private:

//
// Private methods.
private:
  /**
   */
  void Translate( const QPoint& vector );

//
// Private attributes.
private:

  /**
   */
  otb::ViewSettings::Pointer m_ViewSettings;
  /**
   */
  QPoint m_MousePressPosition;
  /**
   */
  otb::ViewSettings::PointType m_MousePressOrigin;

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

#endif // __mvdImageViewManipulator_h
