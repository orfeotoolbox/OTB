/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "mvdProjectionBarWidget.h"
#include "ui_mvdProjectionBarWidget.h"

namespace mvd
{

ProjectionBarWidget::ProjectionBarWidget( QWidget* p, Qt::WindowFlags flags  ) :
        QWidget( p, flags )
        , m_UI( new mvd::Ui::ProjectionBarWidget() )
{
  m_UI->setupUi( this );
}

ProjectionBarWidget::~ProjectionBarWidget()
{
  delete m_UI;
  m_UI = NULL;
}

void ProjectionBarWidget::SetProjectionScale(double scale_x, double )
{
  QString text = "1:1";

  if( scale_x>1.0 )
    text = QString( "%1:1" ).arg( scale_x );

  else if( scale_x<1.0 )
    text = QString( "1:%1" ).arg( 1.0 / scale_x );

  m_UI->projectionScaleLineEdit->setText(text);
}

void ProjectionBarWidget::on_projectionScaleLineEdit_returnPressed()
{
  ChangeScale();
}

void ProjectionBarWidget::on_projectionScaleLineEdit_editingFinished()
{
  if(m_UI->projectionScaleLineEdit->isModified())
    {
    ChangeScale();
    }
}

void ProjectionBarWidget::ChangeScale()
{
  // Cancel if scale text is empty.
  if( m_UI->projectionScaleLineEdit->text().isEmpty() )
    return;

  // Split scale text.
  QStringList scale( m_UI->projectionScaleLineEdit->text().split( ':' ) );

  if( scale.size()!=1 && scale.size()!=2 )
    return;

  // Convert scale numerator.
  bool isOk = true;
  double numerator = scale.front().toDouble( &isOk );

  if( !isOk || numerator==0.0 )
    return;

  // Convert scale denominator.
  double denominator = 1.0;

  if( scale.size()>1 )
    {
    denominator = scale.back().toDouble( &isOk );
    if( !isOk )
      return;
    }

  // Emit scale changed.
  emit ProjectionScaleChanged( numerator / denominator );
}


}
