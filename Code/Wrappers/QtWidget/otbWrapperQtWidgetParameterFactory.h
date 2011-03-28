/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbWrapperQtWidgetFactory_h
#define __otbWrapperQtWidgetFactory_h

#include <QtGui>
#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class ImageIOFactory
 * \brief Create instances of ImageIO objects using an object factory.
 */
class ITK_EXPORT QtWidgetParameterFactory : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef QtWidgetParameterFactory     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(QtWidgetParameterFactory, Object);

  /** Create the appropriate ImageIO depending on the particulars of the file. */
  static QWidget* CreateQtWidget( Parameter* param );

  /** Register Built-in factories */
  static void RegisterBuiltInFactories();

protected:
  QtWidgetParameterFactory();
  virtual ~QtWidgetParameterFactory();

private:
  QtWidgetParameterFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};


}
}

#endif
