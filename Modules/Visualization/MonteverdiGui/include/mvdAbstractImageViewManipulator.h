/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef mvdAbstractImageViewManipulator_h
#define mvdAbstractImageViewManipulator_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "OTBMonteverdiGUIExport.h"
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
 * \ingroup OTBMonteverdiGUI
 *
 *  \brief Base class for QWidget manipulation
 */
class OTBMonteverdiGUI_EXPORT AbstractImageViewManipulator : public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** \brief Destructor. */
  ~AbstractImageViewManipulator() override{};

  //
  // Accessors

  /**
   */
  virtual void SetViewportSize(int width, int height) = 0;
  /**
   */
  virtual SizeType GetViewportSize() const = 0;

  /**
   */
  virtual void SetOrigin(const PointType& origin) = 0;
  /**
   */
  virtual PointType GetOrigin() const = 0;

  /**
   */
  virtual void SetSpacing(const SpacingType& spacing) = 0;
  /**
   */
  virtual SpacingType GetSpacing() const = 0;

  /**
   */
  virtual void SetNativeSpacing(const SpacingType& spacing) = 0;

  /**
   */
  virtual void SetWkt(const std::string& wkt) = 0;

  /**
   */
  virtual void SetKeywordList(const DefaultImageType::ImageKeywordlistType& kwl) = 0;

  /**
   */
  virtual PointType GetCenter() const = 0;

  /**
   */
  virtual ZoomType GetFixedZoomType() const = 0;

  //
  // Controls

  /**
   */
  virtual void SetupRenderingContext(AbstractImageViewRenderer::RenderingContext* const) const = 0;

  /**
   */
  virtual void ZoomIn() = 0;

  /**
   */
  virtual void ZoomOut() = 0;

  /**
   */
  virtual const PointType& Transform(PointType&, const QPoint&) const = 0;

  //
  // Events

  /**
   */
  virtual void MouseMoveEvent(QMouseEvent* event) = 0;
  /**
   */
  virtual void MousePressEvent(QMouseEvent* event) = 0;
  /**
   */
  virtual void MouseReleaseEvent(QMouseEvent* event) = 0;
  /**
   */
  virtual void MouseDoubleClickEvent(QMouseEvent*){};

  /**
   */
  virtual void WheelEvent(QWheelEvent* event) = 0;

  /**
   */
  virtual void KeyPressEvent(QKeyEvent* event) = 0;

  /**
   */
  virtual void KeyReleaseEvent(QKeyEvent* event) = 0;

  /**
   */
  virtual void ResizeEvent(QResizeEvent* event) = 0;

  /**
   */
  virtual void ResetViewport() = 0;

  //
  // Public SLOTS.
public slots:
  /**
   */
  virtual void CenterOn(const PointType& point) = 0;
  /**
   */
  virtual void ZoomTo(double scale) = 0;

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
  void CenterRoiRequested(const PointType& center);
  /**
   */
  void RoiChanged(const PointType& origin, const SizeType& size, const SpacingType& spacing, const PointType& center);

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
  AbstractImageViewManipulator(QObject* p = NULL) : QObject(p){};

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

#endif // mvdAbstractImageViewManipulator_h
