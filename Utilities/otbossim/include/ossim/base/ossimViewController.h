//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimViewController.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimViewController_HEADER
#define ossimViewController_HEADER
#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimViewInterface.h>

/*!
 * All view sources will derive from this class.  For example
 * we can have a mapped view or Perspective view of the scene
 * being rendered.  All sources within the containers are expected
 * to derive from the view interface.
 */
class OSSIMDLLEXPORT ossimViewController : public ossimSource
{
public:

   ossimViewController();
   
   ossimViewController(ossimObject* owner,
                       ossim_uint32 inputListSize,
                       ossim_uint32 outputListSize,
                       bool   inputListFixedFlag=true,
                       bool   outputListFixedFlag=false);

   virtual ~ossimViewController();
 
   virtual ossimString getShortName()const;
   
   virtual ossimString getLongName()const;

   virtual bool propagateView();
   virtual bool setView(ossimObject* object);
   virtual ossimObject* getView();
   virtual const ossimObject* getView()const;
   virtual bool canConnectMyInputTo(ossim_int32 index,
                                    const ossimConnectableObject* obj)const;
   virtual const ossimObject* findFirstViewOfType(RTTItypeid typeId)const;
protected:
   ossimRefPtr<ossimObject> theView;
   
TYPE_DATA
};

#endif
