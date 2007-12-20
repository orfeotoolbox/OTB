/*
 * Copyright 1994, 1995 Vectaport Inc., Cartoactive Systems
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 */

#include <ossim/vec/ossimVpfExtent.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/vec/vpf.h>

/*****************************************************************************/

ostream& operator <<(ostream& out,
                     const ossimVpfExtent& data)
{
   out << "x1: " << data.theExtentType.x1 << endl
       << "y1: " << data.theExtentType.y1 << endl
       << "x2: " << data.theExtentType.x2 << endl
       << "y2: " << data.theExtentType.y2;

   return out;
}


ossimVpfExtent::ossimVpfExtent (double x1, double y1, double x2, double y2)
{
    this->theExtentType.x1 = x1;
    this->theExtentType.y1 = y1;
    this->theExtentType.x2 = x2;
    this->theExtentType.y2 = y2;
}

ossimVpfExtent::ossimVpfExtent ()
{
   theExtentType.x1 = 0.0;
   theExtentType.y1 = 0.0;
   theExtentType.x2 = 0.0;
   theExtentType.y2 = 0.0;
}

ossimVpfExtent::ossimVpfExtent(const extent_type& etype)
{
   theExtentType.x1 = etype.x1;
   theExtentType.y1 = etype.y1;
   theExtentType.x2 = etype.x2;
   theExtentType.y2 = etype.y2;
}

int ossimVpfExtent::contained (ossimVpfExtent& extent2) {
    return ::contained( theExtentType, extent2.theExtentType );
}

int ossimVpfExtent::completely_within (ossimVpfExtent& extent2) {
    return ::completely_within( theExtentType, extent2.theExtentType );
}

int ossimVpfExtent::within (double x, double y) {
    return ::fwithin( x, y, theExtentType );
}

ossimVpfExtent ossimVpfExtent::operator+ (const ossimVpfExtent& b)const
{
    ossimVpfExtent m;
    
    m.theExtentType.x1 = ossim::min(theExtentType.x1, b.theExtentType.x1);
    m.theExtentType.y1 = ossim::min(theExtentType.y1, b.theExtentType.y1);
    m.theExtentType.x2 = ossim::max(theExtentType.x2, b.theExtentType.x2);
    m.theExtentType.y2 = ossim::max(theExtentType.y2, b.theExtentType.y2);
    
    return m;
}
