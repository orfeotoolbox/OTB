/*! \file kbool/include/kbool/scanbeam.h
    \author Probably Klaas Holwerda

    Copyright: 2001-2004 (C) Probably Klaas Holwerda

    Licence: wxWidgets Licence

    RCS-ID: $Id: scanbeam.h 9094 2006-06-13 19:12:40Z dburken $
*/

#ifndef SCANBEAM_H
#define SCANBEAM_H

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

#include <ossim/kbool/booleng.h>
#include <ossim/kbool/_lnk_itr.h>

#include <ossim/kbool/record.h>
#include <ossim/kbool/link.h>

enum SCANTYPE{NODELINK,LINKLINK,GENLR,LINKHOLES,INOUT};

#if defined(WXUSINGDLL)
    template class A2DKBOOLDLLEXP DL_Iter<Record*>;
#endif

class A2DKBOOLDLLEXP ScanBeam : public DL_List<Record*>
{
   protected:
      Bool_Engine* _GC;

	public:
			ScanBeam(Bool_Engine* GC);
			~ScanBeam();
         void SetType(Node* low,Node* high);

			bool  FindNew(SCANTYPE scantype,TDLI<KBoolLink>* _I, bool& holes );
			bool  RemoveOld(SCANTYPE scantype,TDLI<KBoolLink>* _I, bool& holes );

   private:

			bool  ProcessHoles(bool atinsert,TDLI<KBoolLink>* _LI);
         int	Process_LinkToLink_Crossings();			// find crossings
			int 	Process_PointToLink_Crossings();
			int 	Process_LinkToLink_Flat(KBoolLine* flatline);
         void	SortTheBeam( bool backangle );
			bool 	checksort();
			bool 	writebeam();
			void	Calc_Ysp();
			//int 	FindCloseLinksAndCross(TDLI<KBoolLink>* _I,Node* _lowf);
         void 	Generate_INOUT(int graphnumber);

         Node*				  _low;
         DL_Iter<Record*> _BI;
         int 				  lastinserted;
         BEAM_TYPE		  _type;
};

#endif 
