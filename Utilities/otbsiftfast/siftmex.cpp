// Copyright (C) zerofrog(@gmail.com), 2008-2009
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//Lesser GNU General Public License for more details.
//
//You should have received a copy of the GNU Lesser General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "mex.h"
#include <stdio.h>
#include <string.h>

#include <list>
#include <string>
using namespace std;

#ifdef DVPROFILE
#include "profiler.h"
#endif

#include "siftfast.h"

typedef unsigned long long u64;

// [frames,descr]=sift_mex(I,...)
// takes an image and outputs frames and a descriptor
void mexFunction(int nout, mxArray *out[], int nin, const mxArray *in[])
{
    if(nin == 0 || !mxIsDouble(in[0]) )
        mexErrMsgTxt("sift_fast takes 1 argument: a grayscale image in real format ");

    const int* dimensions = mxGetDimensions(in[0]);
    Image image = CreateImageFromMatlabData(mxGetPr(in[0]), dimensions[0], dimensions[1]);
    Keypoint keypts = GetKeypoints(image);

    // write the keys to the output
    int numkeys = 0;
    Keypoint key = keypts;
    while(key) {
        numkeys++;
        key = key->next;
    }

    double* frames = NULL, *descr = NULL;
    if( nout > 0 ) {
        out[0] = mxCreateDoubleMatrix(4,numkeys,mxREAL);
        frames = mxGetPr(out[0]);
    }
    if( nout > 1 ) {
        out[1] = mxCreateDoubleMatrix(128,numkeys,mxREAL);
        descr = mxGetPr(out[1]);
    }

    key = keypts;
    while(key) {

        if( frames != NULL ) {
            frames[0] = key->col; frames[1] = key->row; frames[2] = key->scale; frames[3] = key->ori;
            frames += 4;
        }

        if( descr != NULL ) {
            for(int i = 0; i < 128; ++i)
                descr[i] = (double)key->descrip[i];
            descr += 128;
        }

        key = key->next;
    }
    
    FreeKeypoints(keypts);
    
#ifdef DVPROFILE
    DVProfWrite("prof.txt");
    DVProfClear();
#endif

    DestroyAllImages();
}
