//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$
#ifndef ossim2dTo2dShiftTransform_HEADER
#define ossim2dTo2dShiftTransform_HEADER
#include <ossim/base/ossim2dTo2dTransform.h>

class OSSIM_DLL ossim2dTo2dShiftTransform : public ossim2dTo2dTransform
{
public:
   /**
    * Constructor to initialize the shift transform
    */
   ossim2dTo2dShiftTransform(const ossimDpt& shift = ossimDpt(0.0,0.0))
   :ossim2dTo2dTransform(),
   m_shift(shift)
   {}
   
   /**
    * Copy constructor
    */
   ossim2dTo2dShiftTransform(const ossim2dTo2dShiftTransform& src)
   :ossim2dTo2dTransform(src),
   m_shift(src.m_shift)
   {}
   
   /**
    * Duplication method that duplicates this object
    */
   virtual ossimObject* dup()const{return new ossim2dTo2dShiftTransform(*this);}
   
   /**
    * operator = allows one to copy the contents of the class through a common = 
    * operator.  The = is passed to the derived class.
    */
   const ossim2dTo2dShiftTransform& operator =(const ossim2dTo2dShiftTransform& src)
   {
      if(this == &src) return *this;
      
      ossim2dTo2dTransform::operator =(*this); // call base classes equal operator
      m_shift = src.m_shift;
      
      return *this;
   }
   
   /**
    * Apply the shift to the input.
    *
    * @param input The value to shift.
    * @param output the shifted value.
    */
   virtual void forward(const ossimDpt& input,
                        ossimDpt& output) const
   {
      output = input + m_shift;
   }
   /**
    * Apply the shift to the input and put the result in the same variable
    *
    * @param modify_this The value to shift.  The result is placed back into this value.
    */
   virtual void forward(ossimDpt&  modify_this) const
   {
      modify_this.x += m_shift.x;
      modify_this.y += m_shift.y;
   }

   /**
    * Negate the shift to the input and put the result in output.
    *
    * @param input The value to apply the negated shift.
    * @param output the shifted value.
    */
   virtual void inverse(const ossimDpt& input,
                        ossimDpt& output) const
   {
      output = input - m_shift;
   }
   
   /**
    * Negate the shift operation and put the result in the same variable
    *
    * @param modify_this The value to shift.  The result is placed back into this value.
    */
   virtual void inverse(ossimDpt&  modify_this) const
   {
      modify_this.x -= m_shift.x;
      modify_this.y -= m_shift.y;
   }
   /**
    * Saves the state of this object.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0)const;
   
   /**
    * loads the state of this object from a keywordlist.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);
   
   /**
    * Print the contents of the class and pass to the derived class.
    */
   virtual std::ostream& print(std::ostream& out) const
   {
      ossim2dTo2dTransform::print(out);
      
      out << "shift: " << m_shift;
      
      return out;
   }
   
protected:
   virtual ~ossim2dTo2dShiftTransform(){}
   
   ossimDpt m_shift;
   
TYPE_DATA;
};
#endif
