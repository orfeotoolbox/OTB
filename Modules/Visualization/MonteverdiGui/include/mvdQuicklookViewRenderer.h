/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#ifndef mvdQuicklookViewRenderer_h
#define mvdQuicklookViewRenderer_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

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
#include "OTBMonteverdiGUIExport.h"
//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdImageViewRenderer.h"


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
 *
 * \ingroup OTBMonteverdiGUI
 */
class OTBMonteverdiGUI_EXPORT QuicklookViewRenderer :
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

    ~RenderingContext() ITK_OVERRIDE {}

    PointType m_RoiOrigin;
    PointType m_RoiExtent;
  };

//
// Public methods.
public:
  /** Constructor */
  QuicklookViewRenderer( QObject* p = NULL );

  /** Destructor */
  ~QuicklookViewRenderer() ITK_OVERRIDE;

  //
  // ImageViewRenderer overloads.

  
  AbstractImageViewRenderer::RenderingContext* NewRenderingContext() const ITK_OVERRIDE;

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

  
  void UpdateActors( const AbstractImageViewRenderer::RenderingContext* c ) ITK_OVERRIDE;

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

  void SetWktAndKwl();

  //
  // ImageViewRenderer methods.
  void virtual_SetProjection() ITK_OVERRIDE;
  void virtual_UpdateProjection() ITK_OVERRIDE;

  //
  // AbstractImageViewRenderer overloads.
  // TODO: Move virtual_*Scene() methods to protected section.
  void virtual_FinishScene() ITK_OVERRIDE;

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

#endif // mvdQuicklookViewRenderer_h
