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

#ifndef __mvdAbstractImageViewManipulator_h
#define __mvdAbstractImageViewManipulator_h

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
#include "mvdTypes.h"
#include "mvdAbstractImageViewRenderer.h"
#include "mvdGui.h"


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
 * \ingroup OTBMonteverdiGui
 *
 *  \brief Base class for QWidget manipulation
 */
class ITK_EXPORT AbstractImageViewManipulator :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Destructor. */
  virtual ~AbstractImageViewManipulator() {};

  //
  // Accessors

  /**
   */
  virtual void SetViewportSize( int width, int height ) =0;
  /**
   */
  virtual SizeType GetViewportSize() const =0;

  /**
   */
  virtual void SetOrigin( const PointType& origin ) =0;
  /**
   */
  virtual PointType GetOrigin() const =0;

  /**
   */
  virtual void SetSpacing( const SpacingType& spacing ) =0;
  /**
   */
  virtual SpacingType GetSpacing() const =0;

  /**
   */
  virtual void SetNativeSpacing( const SpacingType& spacing ) =0;

  /**
   */
  virtual void SetWkt( const std::string& wkt ) =0;

  /**
   */
  virtual
    void SetKeywordList( const DefaultImageType::ImageKeywordlistType& kwl ) =0;

  /**
   */
  virtual PointType GetCenter() const =0;

  /**
   */
  virtual ZoomType GetFixedZoomType() const =0;

  //
  // Controls

  /**
   */
  virtual
    void
    SetupRenderingContext(
      AbstractImageViewRenderer::RenderingContext * const ) const =0;

  /**
   */
  virtual void ZoomIn() =0;

  /**
   */
  virtual void ZoomOut() =0;

  /**
   */
  virtual const PointType& Transform( PointType&, const QPoint& ) const =0;

  //
  // Events

  /**
   */
  virtual void MouseMoveEvent( QMouseEvent* event ) =0;
  /**
   */
  virtual void MousePressEvent( QMouseEvent* event ) =0;
  /**
   */
  virtual void MouseReleaseEvent( QMouseEvent* event ) =0;
  /**
   */
  virtual void MouseDoubleClickEvent( QMouseEvent * ) {};

  /**
   */
  virtual void WheelEvent( QWheelEvent* event) =0;

  /**
   */
  virtual void KeyPressEvent( QKeyEvent* event ) =0;

  /**
   */
  virtual void KeyReleaseEvent( QKeyEvent* event ) =0;

  /**
   */
  virtual void ResizeEvent( QResizeEvent* event ) =0;

//
// Public SLOTS.
public slots:
  /**
   */
  virtual void CenterOn( const PointType& point ) =0;
  /**
   */
  virtual void ZoomTo( double scale ) =0;

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:
  /**
   */
  void RefreshViewRequested();
  /**
   */
  void ZoomToExtentRequested();
  /**
   */
  void ZoomToFullResolutionRequested();
  /**
   */
  void CenterRoiRequested( const PointType& center );
  /**
   */
  void RoiChanged( const PointType& origin,
                   const SizeType& size,
                   const SpacingType& spacing,
                   const PointType& center );

  // Signal for DatasetModel
  // void RenderingContextChanged(const PointType& center, double zoom);

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

  /** \brief Constructor. */
  AbstractImageViewManipulator( QObject* p =NULL ):QObject( p ){};

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
