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
#ifndef __otbWrapperQtWidgetInputImageListParameter_h
#define __otbWrapperQtWidgetInputImageListParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbQtFileSelectionWidget.h"
#endif //tag=QT4-boost-compatibility

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetInputImageListParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class ITK_ABI_EXPORT QtWidgetInputImageListParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetInputImageListParameter(InputImageListParameter*, QtWidgetModel*);
  virtual ~QtWidgetInputImageListParameter();


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

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();

  void RecreateImageList();
  void UpdateFileList( std::map<unsigned int, unsigned int> idMap );

  InputImageListParameter::Pointer m_InputImageListParam;

  QHBoxLayout * m_HLayout;
  QVBoxLayout * m_FileLayout;
  QScrollArea * m_Scroll;
  std::vector<QtFileSelectionWidget *> m_FileSelectionList;
};


}
}

#endif
