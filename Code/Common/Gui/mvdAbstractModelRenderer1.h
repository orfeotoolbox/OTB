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

#ifndef __mvdAbstractModelRenderer1_h
#define __mvdAbstractModelRenderer1_h

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
#include "Core/mvdTypes.h"


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
 * \class AbstractModelRenderer1
 *
 * \brief TODO.
 */
class AbstractModelRenderer1 :
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
     inline
     RenderingContext( const AbstractImageModel* model =NULL,
                       const ImageRegionType& region =ImageRegionType(),
                       double zoom =1.0,
                       unsigned int width =0,
                       unsigned int height =0,
                       int dx =0,
                       int dy =0,
                       bool refresh =true ) :
       m_ImageModel( model ),
       m_ImageRegion( region ),
       m_IsotropicZoom( zoom ),
       m_WidgetWidth( width ),
       m_WidgetHeight( height ),
       m_ForceRefresh( refresh )
    {
    }

    const AbstractImageModel* m_ImageModel;

    ImageRegionType m_ImageRegion;

    double m_IsotropicZoom;

    // TODO: remove unsigned before int (because Qt uses signed int).
    unsigned int m_WidgetWidth;

    // TODO: remove unsigned before int (becayse Qt uses signed int).
    unsigned int m_WidgetHeight;

    bool m_ForceRefresh;
  };

//
// Public methods.
public:
  /** Destructor */
  virtual ~AbstractModelRenderer1();

  /** */
  virtual void paintGL( const RenderingContext& context ) =0;

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

  /** Constructor */
  AbstractModelRenderer1( QObject* parent = NULL );


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
// SLOTS.
private slots:
};

} // end namespace 'mvd'

#endif // __mvdAbstractModelRenderer1_h
