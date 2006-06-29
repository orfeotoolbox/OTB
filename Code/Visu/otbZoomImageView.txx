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
#ifndef otbZoomImageView_txx
#define otbZoomImageView_txx

#include "otbZoomImageView.h"


namespace otb
{

template <class TPixel>
ZoomImageView<TPixel>::
ZoomImageView() : GLVectorImageViewBase<TPixel>()
  {
        this->m_ViewRectangle = false;
  }

template <class TPixel>
ZoomImageView<TPixel>::
~ZoomImageView()
{

}

template <class TPixel>
int
ZoomImageView<TPixel>::handle(int event)
  {
  int key;
 
  switch(event)
    {
    case FL_HIDE :
                this->GetViewer()->Hide();
                return 0;
                break;
    case FL_FOCUS :
                this->update();
                this->GetViewer()->ZoomAction();
                return 1;
                break;
    case FL_KEYBOARD:
    case FL_SHORTCUT:
    key = Fl::event_text()[0];
      switch(key)
        {
        case 'u':
        case 'U':
                this->update();
                this->GetViewer()->ZoomAction();
                return 1;
                break;
        case '+':
        case '=':
        {
                this->show();
                this->winZoom(this->cWinZoom*2);
                this->MajViewRegion();
                ::itk::OStringStream lStream;
                lStream << this->GetLabel() << " - ("<<this->cWinZoom<<"x)";
                this->m_flDoubleWindow->label( lStream.str().data() );
                this->update();
                this->GetViewer()->ZoomAction();
                return 1;
        }
                break;
        case '-':
        case '_':
        {
                this->winZoom(this->cWinZoom*0.5);
                this->MajViewRegion();
                ::itk::OStringStream lStream;
                lStream << this->GetLabel() << " - ("<<this->cWinZoom<<"x)";
                this->m_flDoubleWindow->label( lStream.str().data() );
                this->update();
                this->GetViewer()->ZoomAction();
                return 1;
        }
                break;
        default:
                break;
      }
    default:
      break;
  }
  
  return 0;
  
}
 
}; //namespace
#endif

