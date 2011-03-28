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
#ifndef __otbWrapperQtWidgetChoiceParameterFactory_h
#define __otbWrapperQtWidgetChoiceParameterFactory_h

#include "otbWrapperChoiceParameter.h"
#include "otbWrapperQtWidgetParameterFactory.h"

namespace otb
{
namespace Wrapper
{

class QtWidgetChoiceParameterFactory
: public QtWidgetParameterFactory
{
public:
  /** Standard class typedef */
  typedef QtWidgetChoiceParameterFactory Self;
  typedef QtWidgetParameterFactory          Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);
  
  /** RTTI support */
  itkTypeMacro(QtWidgetChoiceParameterFactory,otb::Wrapper::QtWidgetParameterFactory);

  /** Create the appropriate ImageIO depending on the particulars of the file. */
  static QWidget * CreateQtWidget(Parameter * param);

protected:
  QtWidgetChoiceParameterFactory();
  virtual ~QtWidgetChoiceParameterFactory();

private:
  QtWidgetChoiceParameterFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
}
}

#endif
