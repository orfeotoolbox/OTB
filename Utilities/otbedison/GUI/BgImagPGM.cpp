/////////////////////////////////////////////////////////////////////////////
// Name:        imagpnm.cpp
// Purpose:     wxImage PNM handler
// Author:      Sylvain Bougnoux
// RCS-ID:      $Id: imagpnm.cpp,v 1.17 1999/12/15 22:38:02 VS Exp $
// Copyright:   (c) Sylvain Bougnoux
// Licence:     wxWindows licence
// Modified by: Bogdan Georgescu
//              read gray pgm
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "BgImagPGM.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/setup.h"
#endif

#include "BgImagPGM.h"
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/txtstrm.h>

//-----------------------------------------------------------------------------
// bgPGMHandler
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(bgPGMHandler,wxImageHandler)

void bgPGMHandler::Skip_Comment(wxInputStream &stream)
{
  wxTextInputStream text_stream(stream);
  if (stream.Peek()==wxT('#'))
    {
      text_stream.ReadLine();
      Skip_Comment(stream);
    }
}

bool bgPGMHandler::LoadFile( wxImage *image, wxInputStream& stream, bool verbose, int WXUNUSED(index) )
{
    wxUint32  width, height;
    wxUint16  maxval;
    char      c(0);
    image->Destroy();

    /*
     * Read the PGM header
     */

    wxBufferedInputStream buf_stream(stream);
    wxTextInputStream text_stream(buf_stream);
    Skip_Comment(buf_stream);
    if (buf_stream.GetC()==wxT('P')) c=buf_stream.GetC();
    switch (c)
    {
        case wxT('2'):
        case wxT('5'): break;
//      case wxT('3'): 
//	     case wxT('6'): break;
        default:
            if (verbose) wxLogError(_("PGM: File format is not recognized."));
            return FALSE;
    }
    text_stream.ReadLine(); // for the \n
    Skip_Comment(buf_stream);
    text_stream >> width >> height ;
    Skip_Comment(buf_stream);
    text_stream >> maxval;
    //cout << line << " " << width << " " << height << " " << maxval << endl;
    image->Create( width, height );
    unsigned char *ptr = image->GetData();
    if (!ptr)
    {
        if (verbose)
           wxLogError( _("PGM: Couldn't allocate memory.") );
        return FALSE;
    }

   if (c=='2') // Ascii Gray
      {
        wxUint32 value, size=width*height;
        for (wxUint32 i=0; i<size; ++i)
          {
            //this is very slow !!!
            //I wonder how we can make any better ?
            value=text_stream.Read32();
            *ptr++=(unsigned char)value;
            *ptr++=(unsigned char)value;
            *ptr++=(unsigned char)value;
#if wxCHECK_VERSION(2, 3, 0)
            if (!buf_stream)
#else
            if (buf_stream.LastError()!=wxSTREAM_NOERROR)
#endif
              {
                if (verbose) wxLogError(_("PNM: File seems truncated."));
                return FALSE;
              }
          }
      }
    if (c=='5') // Raw gray
    {
      buf_stream.Read( ptr, width*height );
      // rearange
      int i,idx;
      int size = width*height;
      idx = size*3-1;
      for (i=(size-1); i>=0; i--)
      {
         ptr[idx--] = ptr[i];
         ptr[idx--] = ptr[i];
         ptr[idx--] = ptr[i];
      }
    }
    image->SetMask( FALSE );
#if wxCHECK_VERSION(2, 3, 0)
    const wxStreamError err = buf_stream.GetLastError();
    return err == wxSTREAM_NO_ERROR || err == wxSTREAM_EOF;
#else
    return (buf_stream.LastError()==wxStream_NOERROR || buf_stream.LastError()==wxStream_EOF);
#endif
}

bool bgPGMHandler::SaveFile( wxImage *image, wxOutputStream& stream, bool WXUNUSED(verbose) )
{
    //wxTextOutputStream text_stream(stream);
    //text_stream << "P5" << endl
    //<< image->GetWidth() << " " << image->GetHeight() << endl
    //<< "255" << endl;
	char header[70];
    int width, height;
    width = image->GetWidth();
    height = image->GetHeight();
	sprintf(header, "P5\n%d %d\n255\n", width, height);
	stream.Write(header, strlen(header));
     unsigned char *ptr = image->GetData();
    // convert it to bw;
    unsigned char *tptr;
    tptr = new unsigned char[width*height];
    int i,idx;
    idx = 0;
    for (i=0; i<width*height; i++)
    {
       tptr[i] = (unsigned char) (RED_WEIGHT*ptr[idx] + GREEN_WEIGHT*ptr[idx+1] + BLUE_WEIGHT*ptr[idx+2]);
       idx += 3;
    }
    stream.Write(tptr,width*height);
    delete [] tptr;
#if wxCHECK_VERSION(2, 3, 0)
    return stream.IsOk();
#else
    return (stream.LastError()==wxStream_NOERROR);
#endif
}

bool bgPGMHandler::DoCanRead( wxInputStream& stream )
{
    off_t pos = stream.TellI();
    Skip_Comment(stream);
    if ( stream.GetC() == 'P' )
    {
        switch (stream.GetC())
        {
            case '2':
            case '5':
                stream.SeekI(pos);
                return TRUE;
        }
    }
    stream.SeekI(pos);
    return FALSE;
}
