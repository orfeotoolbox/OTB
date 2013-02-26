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

#ifndef __mvdAbstractViewManipulator_h
#define __mvdAbstractViewManipulator_h

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
#include "mvdTypes.h"


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

/** \class AbstractViewManipulator
 *
 *  \brief Base class for QWidget manipulation
 */
class Monteverdi2_EXPORT AbstractViewManipulator :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  AbstractViewManipulator( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~AbstractViewManipulator();

  /** */
  virtual void mouseMoveEvent ( QMouseEvent * event ) = 0;
  virtual void mousePressEvent ( QMouseEvent * event ) = 0;
  virtual void mouseReleaseEvent ( QMouseEvent * event ) = 0;
  virtual void wheelEvent ( QWheelEvent* event) = 0;
  virtual void resizeEvent ( QResizeEvent * event ) = 0;
  virtual void keyPressEvent( QKeyEvent * event )  = 0;

  // TODO: See if HasZoomChanged() pure virtual method can be transformed into Qt signal.
  virtual bool HasZoomChanged() const = 0;

  /** */
  inline const ImageRegionType& GetViewportImageRegion() const;

  /** */
  inline double GetIsotropicZoom() const;

  /** */
  inline IndexType GetViewportOrigin() const;

  /** */
  void SetSpacing(const SpacingType& spacing);

  /** */
  inline SpacingType GetSpacing() const;

  /** */
  inline PointType ScreenIndexToPhysicalPoint(const IndexType & index);

//
// Public SLOTS.
public slots:
  /** */
  virtual
    void OnModelImageRegionChanged( const ImageRegionType& largestRegion, 
      const SpacingType & spacing) =0;
  /** */
  void OnViewportOriginChanged(const IndexType& origin);

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
    NavigationContext() :
      m_ViewportImageRegion(),
      m_ModelImageRegion(),
      m_SizeXBeforeConstrain(),
      m_SizeYBeforeConstrain()
    {
      // TODO: Initialize default viewport region to (0, 0) i.e. no
      // viewed image.

      ImageRegionType::SizeType region;

      region[ 0 ] = 1;
      region[ 1 ] = 1;

      m_ViewportImageRegion.SetSize( region );
    }

    ImageRegionType m_ViewportImageRegion;
    ImageRegionType m_ModelImageRegion;
    // Stored as double to keep precision when dividing 
    // by scale 
    double m_SizeXBeforeConstrain;
    double m_SizeYBeforeConstrain;
  };

  /** Mouse context */
  struct MouseContext
  {
    /** \brief Default constructor. */
    MouseContext() :
      x( 0 ),
      y( 0 ),
      xMove( 0 ),
      yMove( 0 ),
      dx( 0 ),
      dy( 0 )
    {
    }

    int x;  // mousePress x
    int y;  // mousePress y
    int xMove;  // for mouseMove x
    int yMove;  // for mouseMove y
    int dx; // mouseMove in x (Drag)
    int dy; // mouseMove in y (Drag)
  };

//
// Protected methods.
protected:
  
//
// Protected attributes.
protected:

  /** */
  NavigationContext m_NavigationContext;

  /** */
  MouseContext m_MouseContext;

  /** */
  double       m_IsotropicZoom;

  /** */
  IndexType    m_ViewportOrigin;

  /** */
  SpacingType  m_Spacing;

  /** */
  SpacingType  m_NativeSpacing;

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
/*****************************************************************************/
inline
const ImageRegionType&
AbstractViewManipulator
::GetViewportImageRegion() const
{
  return m_NavigationContext.m_ViewportImageRegion;
}

/*****************************************************************************/
inline 
double
AbstractViewManipulator
::GetIsotropicZoom() const
{
  return m_IsotropicZoom;
}

/*****************************************************************************/
inline 
IndexType
AbstractViewManipulator
::GetViewportOrigin() const
{
  return m_ViewportOrigin;
}

/*****************************************************************************/
inline 
SpacingType
AbstractViewManipulator
::GetSpacing() const
{
  return m_Spacing;
}

/*****************************************************************************/
inline 
PointType
AbstractViewManipulator
::ScreenIndexToPhysicalPoint(const IndexType & index)
{
  PointType pt;
  
  //
  // compute the physcial coordinates
  // computation takes into account the origin of the viewport in the widget,
  // the spacing of the rendered image and the isotropicZoom
  pt[0] = ( index[0] - GetViewportOrigin()[0] ) * vcl_abs(GetSpacing()[0]) / GetIsotropicZoom();
  pt[1] = ( index[1] - GetViewportOrigin()[1] ) * vcl_abs(GetSpacing()[1]) / GetIsotropicZoom();

  return pt;
}

} // end namespace 'mvd'

#endif // __mvdAbstractViewManipulator_h
