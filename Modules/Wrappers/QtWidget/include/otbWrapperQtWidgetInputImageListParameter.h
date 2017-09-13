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

#ifndef otbWrapperQtWidgetInputImageListParameter_h
#define otbWrapperQtWidgetInputImageListParameter_h


#include "OTBQtWidgetExport.h"
#include "otbWrapperQtWidgetParameterBase.h"


#include <QtGui>


namespace otb
{


namespace Wrapper
{


class InputImageListParameter;


/** \class QtWidgetInputImageListParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetInputImageListParameter :
    public QtWidgetParameterBase
{
  Q_OBJECT

public:
  QtWidgetInputImageListParameter( InputImageListParameter *, QtWidgetModel * );
  ~QtWidgetInputImageListParameter() ITK_OVERRIDE;


signals:
  void Change();
  void FileSelectionWidgetAdded( QWidget * );

protected slots:
  //void SetFileName( const QString& value );
  //virtual void SelectFile();
  virtual void UpFile();
  virtual void DownFile();
  virtual void AddFile();
  virtual void SuppressFile();
  virtual void EraseFile();
  virtual void UpdateImageList();

private:
  QtWidgetInputImageListParameter(const QtWidgetInputImageListParameter&); //purposely not implemented
  void operator=(const QtWidgetInputImageListParameter&); //purposely not implemented

  void DoCreateWidget() ITK_OVERRIDE;

  void DoUpdateGUI() ITK_OVERRIDE;

  void RecreateImageList();
  void UpdateFileList( std::map<unsigned int, unsigned int> idMap );

  InputImageListParameter::Pointer m_InputImageListParam;


private:
};


}
}

#endif
