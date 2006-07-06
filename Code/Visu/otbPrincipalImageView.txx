
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

#ifndef otbPrincipalImageView_txx
#define otbPrincipalImageView_txx

#include "otbPrincipalImageView.h"


namespace otb
{

template <class TPixel>
PrincipalImageView<TPixel>::
PrincipalImageView() : GLVectorImageViewBase<TPixel>()
  {
  }

template <class TPixel>
PrincipalImageView<TPixel>::
~PrincipalImageView()
{

}

template <class TPixel>
int
PrincipalImageView<TPixel>::handle(int event)
  {

  int key;
  switch(event)
    {
    case FL_PUSH:
    case FL_DRAG:
      {
        int x = Fl::event_x();
        int y = Fl::event_y();
        IndexType lCenter;
        IndexType lCenterPointImage;
        lCenter[0] = x;
        lCenter[1] = y;
        //Position du centre de l'image en coordonnees image
        lCenterPointImage = this->WindowCoord2ImageCoord(lCenter);
        // Mise a jour de la fenetre Zoom
        this->GetViewer()->PrincipalAction(lCenterPointImage);
      }
      break;
    case FL_FOCUS :
                this->update();
//THOMAS
//                this->GetViewer()->PrincipalResize();
                return 1;
                break;
    case FL_HIDE :
                this->GetViewer()->Hide();
                return 0;
                break;
    case FL_RELEASE:
      return 0;
      break;

    case FL_KEYBOARD:
    case FL_SHORTCUT:
    key = Fl::event_text()[0];
      switch(key)
        {
        case 'u':
        case 'U':
              this->update();
              return 1;
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

