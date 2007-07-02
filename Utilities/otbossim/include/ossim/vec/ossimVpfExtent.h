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
#ifndef ossimVpfExtent_HEADER
#define ossimVpfExtent_HEADER
#include <iostream>
using namespace std;
#include <ossim/vec/vpf.h>


class ossimVpfExtent
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimVpfExtent& data);
   
   ossimVpfExtent (double x1, double y1, double x2, double y2);
   ossimVpfExtent();
   ossimVpfExtent(const extent_type& etype);
   
   int contained (ossimVpfExtent& extent2);
   int completely_within (ossimVpfExtent& extent2);
   int within (double x, double y);
   ossimVpfExtent operator+ (const ossimVpfExtent& b)const;
protected:
   extent_type theExtentType;
};

#endif
