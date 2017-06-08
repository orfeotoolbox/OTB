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

#ifndef otbWrapperQtWidgetInputVectorDataListParameter_h
#define otbWrapperQtWidgetInputVectorDataListParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbQtFileSelectionWidget.h"
#endif //tag=QT4-boost-compatibility

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetInputVectorDataListParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetInputVectorDataListParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetInputVectorDataListParameter(InputVectorDataListParameter*, QtWidgetModel*);
  ~QtWidgetInputVectorDataListParameter() override;


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
  virtual void UpdateVectorDataList();

private:
  QtWidgetInputVectorDataListParameter(const QtWidgetInputVectorDataListParameter&); //purposely not implemented
  void operator=(const QtWidgetInputVectorDataListParameter&); //purposely not implemented

  void DoCreateWidget() override;

  void DoUpdateGUI() override;

  void RecreateVectorDataList();
  void UpdateFileList( std::map<unsigned int, unsigned int> idMap );

  InputVectorDataListParameter::Pointer m_InputVectorDataListParam;

  QHBoxLayout * m_HLayout;
  QVBoxLayout * m_FileLayout;
  QScrollArea * m_Scroll;
  std::vector<QtFileSelectionWidget *> m_FileSelectionList;
};


}
}

#endif
