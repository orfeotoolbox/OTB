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
#ifndef __otbWrapperQtWidgetOutputImageParameter_h
#define __otbWrapperQtWidgetOutputImageParameter_h

#include <QtGui>
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetOutputImageParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetOutputImageParameter(OutputImageParameter*, QtWidgetModel*);
  virtual ~QtWidgetOutputImageParameter();

protected slots:
  void SetFileName( const QString& value );
  void SelectFile();

protected:
  void CreateWidget();

private:
  QtWidgetOutputImageParameter(const QtWidgetOutputImageParameter&); //purposely not implemented
  void operator=(const QtWidgetOutputImageParameter&); //purposely not implemented

  std::string m_FileName;
  OutputImageParameter::Pointer m_OutputImageParam;

};


}
}

#endif
