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

#include "otbWrapperQtWidgetInputImageListParameter.h"

#include "otbWrapperQtWidgetListEditItemModel.h"
#include "otbWrapperQtWidgetListEditWidget.h"


namespace otb
{
namespace Wrapper
{

QtWidgetInputImageListParameter
::QtWidgetInputImageListParameter( InputImageListParameter * param,
				   QtWidgetModel * m ) :
  QtWidgetParameterBase( param, m )
{
  connect(
    this, SIGNAL( Change() ),
    GetModel(), SLOT( NotifyUpdate() )
  );
}

QtWidgetInputImageListParameter::~QtWidgetInputImageListParameter()
{
}

void QtWidgetInputImageListParameter::DoUpdateGUI()
{
#if 0
  if(!m_InputImageListParam)
    return;

  //update fileSelectionList only if HasUserValue flag is set(from xml)
  if(m_InputImageListParam->HasUserValue())
    {
    std::vector<std::string> fileList = m_InputImageListParam->GetFileNameList();

    for( unsigned int i = m_FileSelectionList.size(); i < fileList.size(); i++ )
      {
      this->AddFile();
      }
    unsigned int i = 0;
    std::vector<std::string>::iterator it;
    for (it = fileList.begin(); it != fileList.end(); ++it)
      {
      m_FileSelectionList[i++]->GetInput()->setText(
	QFile::decodeName( it->c_str() )
      );
      }
    }
#endif
}

void
QtWidgetInputImageListParameter
::DoCreateWidget()
{
  ListEditWidget * widget = new ListEditWidget();

  assert( widget->GetItemModel() );
  assert( dynamic_cast< StringListInterface * >( GetParam() )!=nullptr );

  widget->GetItemModel()->SetStringList(
    dynamic_cast< StringListInterface * >( GetParam() )
  );

  //
  // Global Layout
  QGridLayout * layout = new QGridLayout();

  layout->setSpacing( 1 );
  layout->setContentsMargins( 2, 2, 2, 2 );

  layout->addWidget( widget );

  setLayout( layout );
}

void
QtWidgetInputImageListParameter::UpdateImageList()
{
#if 0
  // Adding a NullElement so to make the m_FileSelectionList and
  // m_InputImageList's ImageList are of same size.
  for(unsigned int i = m_InputImageListParam->Size(); i < m_FileSelectionList.size(); i++)
    {
    m_InputImageListParam->AddNullElement();
    }

  for(unsigned int j = 0; j < m_InputImageListParam->Size(); j++)
    {
    if(m_InputImageListParam->SetNthFileName(j, m_FileSelectionList[j]->GetFilename()) == false)
      {
      std::ostringstream oss;
      oss << "The given file " << m_FileSelectionList[j]->GetFilename() << " is not valid.";
      this->GetModel()->SendLogWARNING(oss.str());
      }
    }

  emit Change();

  // notify of value change
  QString key(m_InputImageListParam->GetKey());
  emit ParameterChanged(key);
#endif
}

void
QtWidgetInputImageListParameter::UpFile()
{
#if 0
 if(m_FileSelectionList.size() < 2)
    return;

  m_FileLayout = new QVBoxLayout();
  m_FileLayout->setSpacing(2);

  // Map link between old and new index in the list
  std::map<unsigned int, unsigned int> idMap;

  // Init map
  for(unsigned int i = 0; i < m_FileSelectionList.size(); i++)
    {
    idMap[i] = i;
    }

  // If the first item is checked, uncheck it...
  // It won't be moved
  if(m_FileSelectionList[0]->IsChecked())
    {
    m_FileSelectionList[0]->SetChecked(false);
    }


  // If other item are checked, up the index
  // Starts at 1 because the first item mustn't move
  for(unsigned int i = 1; i < m_FileSelectionList.size(); i++)
    {
    if(m_FileSelectionList[i]->IsChecked())
      {
      unsigned int tmp = idMap[i];
      idMap[i] = i-1;
      idMap[idMap[i-1]] = tmp;
      }
    }

  this->UpdateFileList(idMap);

  this->RecreateImageList();
#endif
}

void
QtWidgetInputImageListParameter::DownFile()
{
#if 0
  if(m_FileSelectionList.size() < 2) return;

  m_FileLayout = new QVBoxLayout();
  m_FileLayout->setSpacing(0);

  // Map link between old and new index in the list
  std::map<unsigned int, unsigned int> idMap;

  // Init map
  for(unsigned int i = 0; i < m_FileSelectionList.size(); i++)
    {
    idMap[i] = i;
    }

  // If the last item is checked, uncheck it...
  // It won't be moved
  if(m_FileSelectionList[m_FileSelectionList.size() - 1]->IsChecked())
    {
    m_FileSelectionList[m_FileSelectionList.size() - 1]->SetChecked(false);
    }

  // If other item are checked, up the index
  // Stops at size-1 because the last item mustn't move
  for(int i = m_FileSelectionList.size() - 2; i >= 0; i--)
    {
    if(m_FileSelectionList[i]->IsChecked())
      {
      unsigned int tmp = idMap[i];
      idMap[i] = i + 1;
      idMap[idMap[i + 1]] = tmp;
      }
    }

  this->UpdateFileList(idMap);

  this->RecreateImageList();
#endif
}


void
QtWidgetInputImageListParameter
::UpdateFileList( std::map< unsigned int, unsigned int > /* idMap */ )
{
#if 0
  std::vector<QtFileSelectionWidget *> tmpList;
  // Keys become values and inverse
  std::map<unsigned int, unsigned int> idMapBis;
  for(unsigned int i = 0; i < idMap.size(); i++)
    {
    idMapBis[idMap[i]] = i;
    }

  // Create the new item list
  for(unsigned int i = 0; i < m_FileSelectionList.size(); i++)
    {
    m_FileLayout->addWidget(m_FileSelectionList[idMapBis[i]]);
    tmpList.push_back(m_FileSelectionList[idMapBis[i]]);
    }

  m_FileSelectionList = tmpList;
  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_FileLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();

  // notify of value change
  QString key(m_InputImageListParam->GetKey());
  emit ParameterChanged(key);
#endif
}


void
QtWidgetInputImageListParameter::AddFile()
{
#if 0
  m_FileLayout = new QVBoxLayout();
  m_FileLayout->setSpacing(0);

  for(unsigned int i = 0; i < m_FileSelectionList.size(); i++)
    {
    m_FileLayout->addWidget(m_FileSelectionList[i]);
    }

  QtFileSelectionWidget * fileSelection = new QtFileSelectionWidget();
  fileSelection->SetIOMode( QtFileSelectionWidget::IO_MODE_INPUT );
  fileSelection->setFixedHeight(30);
  m_FileLayout->addWidget(fileSelection);
  m_FileSelectionList.push_back(fileSelection);
  /* No need of AddNullElement() here. Moved adding NullElement when updating the list  */
  //m_InputImageListParam->AddNullElement();
  connect(
    fileSelection,
    SIGNAL( FilenameChanged() ),
    this,
    SLOT( UpdateImageList() )
  );

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_FileLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();

  emit FileSelectionWidgetAdded( fileSelection );
#endif
}

void
QtWidgetInputImageListParameter::SuppressFile()
{
#if 0
  m_FileLayout = new QVBoxLayout();
  m_FileLayout->setSpacing(0);
  std::vector<QtFileSelectionWidget *> tmpList;
  for(unsigned int i = 0; i < m_FileSelectionList.size(); i++)
    {
    if(!m_FileSelectionList[i]->IsChecked())
      {
      m_FileLayout->addWidget(m_FileSelectionList[i]);
      tmpList.push_back(m_FileSelectionList[i]);
      }
    }

  m_FileSelectionList = tmpList;

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_FileLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();
  this->RecreateImageList();
#endif
}

void
QtWidgetInputImageListParameter::EraseFile()
{
#if 0
  m_FileSelectionList.clear();

  m_FileLayout = new QVBoxLayout();

  QtFileSelectionWidget * fileSelection = new QtFileSelectionWidget();
  fileSelection->SetIOMode( QtFileSelectionWidget::IO_MODE_INPUT );
  fileSelection->setFixedHeight(30);
  m_FileLayout->addWidget(fileSelection);
  m_FileSelectionList.push_back(fileSelection);
  m_InputImageListParam->AddNullElement();
  connect(fileSelection, SIGNAL(FilenameChanged()), this, SLOT(UpdateImageList()));

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_FileLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();
  this->RecreateImageList();
#endif
}

void QtWidgetInputImageListParameter::RecreateImageList()
{
#if 0
  // save value
  m_InputImageListParam->ClearValue();

  if(m_FileSelectionList.size() == 0)
    {
    this->AddFile();
    }
  else
    {
    for(unsigned int j = 0; j < m_FileSelectionList.size(); j++)
      {
      m_InputImageListParam->AddFromFileName(m_FileSelectionList[j]->GetFilename());
      connect(m_FileSelectionList[j], SIGNAL(FilenameChanged()), this, SLOT(UpdateImageList()));
      }

    emit Change();
    // notify of value change
    QString key(m_InputImageListParam->GetKey());

    emit ParameterChanged(key);
    }
#endif
}


}
}
