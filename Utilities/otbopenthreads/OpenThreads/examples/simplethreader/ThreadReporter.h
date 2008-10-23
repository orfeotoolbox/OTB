//
// OpenThread library, Copyright (C) 2002 - 2003  The Open Thread Group
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

//
// ThreadReporter - Basic thread reporter class.
// ~~~~~~~~~~~~~~~
//

#ifndef _THREADREPORTER_H_
#define _THREADREPORTER_H_

/*
#ifdef _WIN32
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#endif
*/

#include <list>
#include "ThreadObserver.h"

using std::list;

class ThreadReporter {

public:

    ThreadReporter();

    virtual ~ThreadReporter();

    void addObserver(ThreadObserver *observer);

    void removeObserver(ThreadObserver *observer);

protected:

    void notifyObserversStarted(const int threadId);

    void notifyObserversFinished(const int threadId);

    void notifyObserversMessage(const int threadId, const char *message);

    void notifyObserversError(const int threadId, const char *errorMessage);

private:

    list<ThreadObserver *> _observers;

};

#endif
