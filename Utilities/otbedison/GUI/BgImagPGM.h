/////////////////////////////////////////////////////////////////////////////
// Name:        imagpnm.h
// Purpose:     wxImage PNM handler
// Author:      Sylvain Bougnoux
// RCS-ID:      $Id: imagpnm.h,v 1.1 1999/12/15 22:37:51 VS Exp $
// Copyright:   (c) Sylvain Bougnoux
// Licence:     wxWindows licence
// Modified by: Bogdan Georgescu
//              read gray pgm
/////////////////////////////////////////////////////////////////////////////

#ifndef _BG_IMAGPGM_H_
#define _BG_IMAGPGM_H_

#ifdef __GNUG__
#pragma interface "BgImagPGM.h"
#endif

#include <wx/image.h>

//-----------------------------------------------------------------------------
// bgPGMHandler
//-----------------------------------------------------------------------------

#define RED_WEIGHT 0.299
#define GREEN_WEIGHT 0.587
#define BLUE_WEIGHT 0.114

class bgPGMHandler : public wxImageHandler
{

  DECLARE_DYNAMIC_CLASS(bgPGMHandler)

public:
  inline bgPGMHandler()
  {
      m_name = "PGM file";
      m_extension = "pgm";
      m_type = wxBITMAP_TYPE_ANY;
      m_mime = "image/pgm";
  };

  virtual bool LoadFile( wxImage *image, wxInputStream& stream, bool verbose=TRUE, int index=0 );
  virtual bool SaveFile( wxImage *image, wxOutputStream& stream, bool verbose=TRUE );
  virtual bool DoCanRead( wxInputStream& stream );

  void Skip_Comment(wxInputStream &stream);
};

#endif

  // _BG_IMAGPGM_H_