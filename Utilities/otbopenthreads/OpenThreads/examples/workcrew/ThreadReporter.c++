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
// ThreadReporter - Basic Thread Reporter class.
// ~~~~~~~~~~~~~~~
//

#include "ThreadReporter.h"

//-----------------------------------------------------------------------------
//
// Description: Constructor
//
// Use: public
//
ThreadReporter::ThreadReporter() {
    _observers.clear();
}

//-----------------------------------------------------------------------------
//
// Description: Destructor
//
// Use: public
//
ThreadReporter::~ThreadReporter() {
    _observers.clear();
}

//-----------------------------------------------------------------------------
//
// Description: Add an observer to the list
//
// Use: public
//
void ThreadReporter::addObserver(ThreadObserver *observer) {

    _observers.push_back(observer);

}

//-----------------------------------------------------------------------------
//
// Description: Remove an observer from the list
//
// Use: public
//
void ThreadReporter::removeObserver(ThreadObserver *observer) {

    std::list<ThreadObserver *>::iterator iter;
    for(iter=_observers.begin(); iter != _observers.end(); ++iter) {
        if(*iter == observer) {
            _observers.erase(iter);
            return;
        }
    }
}

//-----------------------------------------------------------------------------
//
// Description: Notify the observers the thread has started
//
// Use: protected
//
void ThreadReporter::notifyObserversStarted(const int threadId) {

    std::list<ThreadObserver *>::iterator iter;
    for(iter=_observers.begin(); iter != _observers.end(); ++iter) {
        ThreadObserver *observer = static_cast<ThreadObserver *>(*iter);
        observer->threadStarted(threadId);
    }
}

//-----------------------------------------------------------------------------
//
// Description: Notify the observers the thread has finished
//
// Use: protected
//
void ThreadReporter::notifyObserversFinished(const int threadId) {

    std::list<ThreadObserver *>::iterator iter;
    for(iter=_observers.begin(); iter != _observers.end(); ++iter) {
        ThreadObserver *observer = static_cast<ThreadObserver *>(*iter);
        observer->threadFinished(threadId);
    }
}

//-----------------------------------------------------------------------------
//
// Description: Notify the observers of a thread message
//
// Use: protected
//
void ThreadReporter::notifyObserversMessage(const int threadId,
					    const char *message) {

    std::list<ThreadObserver *>::iterator iter;
    for(iter=_observers.begin(); iter != _observers.end(); ++iter) {
        ThreadObserver *observer = static_cast<ThreadObserver *>(*iter);
        observer->threadMessage(threadId, message);
    }
}

//-----------------------------------------------------------------------------
//
// Description: Notify the observers of a thread error
//
// Use: protected
//
void ThreadReporter::notifyObserversError(const int threadId,
					  const char *errorMessage) {

    std::list<ThreadObserver *>::iterator iter;
    for(iter=_observers.begin(); iter != _observers.end(); ++iter) {
        ThreadObserver *observer = static_cast<ThreadObserver *>(*iter);
        observer->threadError(threadId, errorMessage);
    }
}
