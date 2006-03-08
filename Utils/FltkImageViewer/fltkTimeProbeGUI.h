/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkTimeProbeGUI.h,v $
  Language:  C++
  Date:      $Date: 2002/03/19 20:12:09 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __fltkTimeProbeGUI_h
#define __fltkTimeProbeGUI_h

#include <FL/Fl_Tile.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Value_Output.H>
#include <string>


namespace fltk
{


class TimeProbeGUI : public Fl_Tile
{

  public:
    TimeProbeGUI();
    ~TimeProbeGUI();
    void SetName( const std::string & );
    void SetNumberOfStarts( unsigned int );
    void SetNumberOfStops( unsigned int );
    void SetMeanTime( double );

  private:
    Fl_Output        * m_Name;
    Fl_Value_Output  * m_MeanTime;
    Fl_Value_Output  * m_NumberOfStarts;
    Fl_Value_Output  * m_NumberOfStops;


};

}  //end namespace fltk

#endif
