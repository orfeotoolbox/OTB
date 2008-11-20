/////////////////////////////////////////////////////////////////////////////
// Name:        imagpnm.h
// Purpose:     wxImage PNM handler
// Author:      Sylvain Bougnoux
// RCS-ID:      $Id: imagpnm.h,v 1.1 1999/12/15 22:37:51 VS Exp $
// Copyright:   (c) Sylvain Bougnoux
// Licence:     wxWindows licence
// Modified by: Chris M. Christoudias
//              read/write pnm image
/////////////////////////////////////////////////////////////////////////////

#ifndef _BG_IMAGPNM_H_
#define _BG_IMAGPNM_H_

#ifdef __GNUG__
#pragma interface "BgImagPNM.h"
#endif

#include <wx/image.h>

//-----------------------------------------------------------------------------
// bgPGMHandler
//-----------------------------------------------------------------------------

class bgPNMHandler : public wxImageHandler
{
  DECLARE_DYNAMIC_CLASS(bgPNMHandler)
public:
  inline bgPNMHandler()
  {
      m_name = "PNM file";
      m_extension = "pnm";
      m_type = wxBITMAP_TYPE_PNM;
      m_mime = "image/pnm";
  };

  virtual bool LoadFile( wxImage *image, wxInputStream& stream, bool verbose=TRUE, int index=0 );
  virtual bool SaveFile( wxImage *image, wxOutputStream& stream, bool verbose=TRUE );
  virtual bool DoCanRead( wxInputStream& stream );

  void Skip_Comment(wxInputStream &stream);
};

#endif
