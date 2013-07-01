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
#ifndef __mvdWrapperQtWidgetFactory_h
#define __mvdWrapperQtWidgetFactory_h

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
#include "itkObject.h"
#include "itkObjectFactory.h"

//
// OTB includes (sorted by alphabetic order)
#include "otbWrapperQtWidgetParameterBase.h"

//
// Monteverdi includes (sorted by alphabetic order)

/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace otb
{
namespace Wrapper
{
class Parameter;
class QtWidgetModel;
}
}

namespace mvd
{

namespace Wrapper
{

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ApplicationLauncher
 *
 * \brief WIP.
 */

/** \class ImageIOFactory
 * \brief Create instances of ImageIO objects using an object factory.
 */
class ITK_EXPORT QtWidgetParameterFactory : public itk::Object
{

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

public:
  /** Standard class typedefs. */
  typedef QtWidgetParameterFactory     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** New() method */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(QtWidgetParameterFactory, itk::Object);

  /** Create the appropriate ImageIO depending on the particulars of the file. */
  static
    otb::Wrapper::QtWidgetParameterBase*
    CreateQtWidget( otb::Wrapper::Parameter* param, 
		    otb::Wrapper::QtWidgetModel* model );

  /*-[ PROTECTED SECTION ]------------------------------------------------------*/

protected:
  QtWidgetParameterFactory();
  virtual ~QtWidgetParameterFactory();

  /*-[ PRIVATE SECTION ]------------------------------------------------------*/

private:
  QtWidgetParameterFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

}
}

#endif
