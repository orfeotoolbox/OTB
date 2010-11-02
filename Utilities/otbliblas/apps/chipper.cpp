/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Point Partitioning/blocking for OPC
 * Author:   Andrew Bell andrew.bell.ia at gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2010, Andrew Bell
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following 
 * conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in 
 *       the documentation and/or other materials provided 
 *       with the distribution.
 *     * Neither the name of the Andrew Bell or libLAS nor the names of 
 *       its contributors may be used to endorse or promote products derived 
 *       from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 ****************************************************************************/

#include "chipper.hpp"
// boost
#include <boost/cstdint.hpp>
// std
#include <cmath>

using namespace boost;
using namespace std;

/**
The objective is to split the region into non-overlapping blocks, each
containing approximately the same number of points, as specified by the
user.

First, the points are read into arrays - one for the x direction, and one for
the y direction.  The arrays are sorted and are initialized with indices into
the other array of the location of the other coordinate of the same point.

Partitions are created that place the maximum number of points in a
block, subject to the user-defined threshold, using a cumulate and round
procedure.

The distance of the point-space is checked in each direction and the
wider dimension is chosen for splitting at an appropriate partition point.
The points in the narrower direction are copied to locations in the spare
array at one side or the other of the chosen partition, and that portion
of the spare array then becomes the active array for the narrow direction.
This avoids resorting of the arrays, which are already sorted.

This procedure is then recursively applied to the created blocks until
they contains only one or two partitions.  In the case of one partition,
we are done, and we simply store away the contents of the block.  If there are
two partitions in a block, we avoid the recopying the narrow array to the
spare since the wide array already contains the desired points partitioned
into two blocks.  We simply need to locate the maximum and minimum values
from the narrow array so that the approriate extrema of the block can
be stored.
**/

namespace liblas { namespace chipper {

vector<uint32_t> Block::GetIDs() const
{
    vector<uint32_t> ids;

    for (uint32_t i = m_left; i <= m_right; ++i)
        ids.push_back((*m_list_p)[i].m_ptindex);
    return ids;
}

void Chipper::Chip()
{
    Load(m_xvec, m_yvec, m_spare);
    Partition(m_xvec.size());
    DecideSplit(m_xvec, m_yvec, m_spare, 0, m_partitions.size() - 1);
}

void Chipper::Load(RefList& xvec, RefList& yvec, RefList& spare )
{
    PtRef ref;
    uint32_t idx;
    uint32_t count;
    vector<PtRef>::iterator it;
   
    count = m_reader->GetHeader().GetPointRecordsCount();
    xvec.reserve(count);
    yvec.reserve(count);
    spare.resize(count);

    count = 0;
    while (m_reader->ReadNextPoint()) {
        const liblas::Point& pt = m_reader->GetPoint();

        ref.m_pos = pt.GetX();
        ref.m_ptindex = count;
        xvec.push_back(ref);

        ref.m_pos = pt.GetY();
        yvec.push_back(ref);
        count++;
    }
    // Sort xvec and assign other index in yvec to sorted indices in xvec.
    sort(xvec.begin(), xvec.end());
    for (uint32_t i = 0; i < xvec.size(); ++i) {
        idx = xvec[i].m_ptindex;
        yvec[idx].m_oindex = i;
    }

    // Sort yvec.
    sort(yvec.begin(), yvec.end());

    //Iterate through the yvector, setting the xvector appropriately.
    for (uint32_t i = 0; i < yvec.size(); ++i)
        xvec[yvec[i].m_oindex].m_oindex = i;
}

void Chipper::Partition(uint32_t size)
{
    uint32_t num_partitions;

    num_partitions = size / m_threshold;
    if ( size % m_threshold )
        num_partitions++;
    double total = 0;
    double partition_size = (double)size / num_partitions;
    m_partitions.push_back(0);
    for (uint32_t i = 0; i < num_partitions; ++i) {
        total += partition_size;
        m_partitions.push_back((uint32_t)detail::sround(total));
    }
}

void Chipper::DecideSplit(RefList& v1, RefList& v2, RefList& spare,
    uint32_t pleft, uint32_t pright)
{
    double v1range;
    double v2range;
    uint32_t left = m_partitions[pleft];
    uint32_t right = m_partitions[pright] - 1;

    // Decide the wider direction of the block, and split in that direction
    // to maintain squareness.
    v1range = v1[right].m_pos - v1[left].m_pos;
    v2range = v2[right].m_pos - v2[left].m_pos;
    if (v1range > v2range)
        Split(v1, v2, spare, pleft, pright);
    else
        Split(v2, v1, spare, pleft, pright);
}

void Chipper::Split(RefList& wide, RefList& narrow, RefList& spare,
    uint32_t pleft, uint32_t pright)
{
    uint32_t lstart;
    uint32_t rstart;
    uint32_t pcenter;
    uint32_t left;
    uint32_t right;
    uint32_t center;

    left = m_partitions[pleft];
    right = m_partitions[pright] - 1;

    // There are two cases in which we are done.
    // 1) We have a distance of two between left and right.
    // 2) We have a distance of three between left and right.

    if (pright - pleft == 1)
        Emit(wide, left, right, narrow, left, right);
    else if (pright - pleft == 2)
        FinalSplit(wide, narrow, pleft, pright);
    else
    {
        pcenter = (pleft + pright) / 2;
        center = m_partitions[pcenter];

        // We are splitting in the wide direction - split elements in the
        // narrow array by copying them to the spare array in the correct
        // partition.  The spare array then becomes the active narrow array
        // for the [left,right] partition.
        lstart = left;
        rstart = center;
        for (int64_t i = left; i <= right; ++i)
        {
            if (narrow[i].m_oindex < center)
            {
                spare[lstart] = narrow[i];
                wide[narrow[i].m_oindex].m_oindex = lstart;
                lstart++;
            }
            else
            {
                spare[rstart] = narrow[i];
                wide[narrow[i].m_oindex].m_oindex = rstart;
                rstart++;
            }
        }

        // Save away the direction so we know which array is X and which is Y
        // so that when we emit, we can properly label the max/min points.
        Direction dir = narrow.m_dir;
        spare.m_dir = dir;
        DecideSplit(wide, spare, narrow, pleft, pcenter);
        DecideSplit(wide, spare, narrow, pcenter, pright);
        narrow.m_dir = dir;
    }
}

// In this case the wide array is like we want it.  The narrow array is
// ordered, but not for our split, so we have to find the max/min entries
// for each partition in the final split.
void Chipper::FinalSplit(RefList& wide, RefList& narrow,
    uint32_t pleft, uint32_t pright)  
{
    long left1 = -1;
    long left2 = -1;
    long right1 = -1;
    long right2 = -1;

    uint32_t left = m_partitions[pleft];
    uint32_t right = m_partitions[pright] - 1;
    uint32_t center = m_partitions[pright - 1];

    // Find left values for the partitions.
    for (int64_t i = left; i <= right; ++i)
    {
        if (left1 < 0 && (narrow[i].m_oindex < center))
        {
            left1 = i;
            if (left2 >= 0)
                break;
        }
        else if (left2 < 0 && (narrow[i].m_oindex >= center))
        {
            left2 = i;
            if (left1 >= 0)
                break;
        }
    }
    // Find right values for the partitions.
    for (int64_t i = right; i >= left; --i)
    {
        if (right1 < 0 && (narrow[i].m_oindex < center))
        {
            right1 = i;
            if (right2 >= 0)
                break;
        }
        else if (right2 < 0 && (narrow[i].m_oindex >= center))
        {
            right2 = i;
            if (right1 >= 0)
                break;
        }
    }

    // Emit results.
    Emit(wide, left, center - 1, narrow, left1, right1 );
    Emit(wide, center, right, narrow, left2, right2 );
}

void Chipper::Emit(RefList& wide, uint32_t widemin, uint32_t widemax,
    RefList& narrow, uint32_t narrowmin, uint32_t narrowmax )
{
    Block b;

    b.m_list_p = &wide;
    if (wide.m_dir == DIR_X) { 
        
        // minx, miny, maxx, maxy
        liblas::Bounds<double> bnd(wide[widemin].m_pos, narrow[narrowmin].m_pos, wide[widemax].m_pos,  narrow[narrowmax].m_pos);
        b.SetBounds(bnd);

        // b.m_xmin = wide[widemin].m_pos;
        // b.m_xmax = wide[widemax].m_pos;
        // b.m_ymin = narrow[narrowmin].m_pos;
        // b.m_ymax = narrow[narrowmax].m_pos;
    }
    else {
        liblas::Bounds<double> bnd(narrow[narrowmin].m_pos, wide[widemin].m_pos, narrow[narrowmax].m_pos, wide[widemax].m_pos);
        b.SetBounds(bnd);

        // b.m_xmin = narrow[narrowmin].m_pos;
        // b.m_xmax = narrow[narrowmax].m_pos;
        // b.m_ymin = wide[widemin].m_pos;
        // b.m_ymax = wide[widemax].m_pos;
    }
    b.m_left = widemin;
    b.m_right = widemax;
    m_blocks.push_back(b);
}

}} // namespace liblas::chipper
