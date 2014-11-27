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
#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{
namespace Wrapper
{

class Parameter;
class QtWidgetModel;
class QtWidgetParameterBase;

/** \class QtWidgetParameterFactory
 * \brief 
 *
 * \ingroup OTBQtWidget
 */
class ITK_ABI_EXPORT QtWidgetParameterFactory : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef QtWidgetParameterFactory     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** New() method */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(QtWidgetParameterFactory, Object);

  /** Create the appropriate ImageIO depending on the particulars of the file. */
  static QtWidgetParameterBase* CreateQtWidget( Parameter* param, QtWidgetModel* model );

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
