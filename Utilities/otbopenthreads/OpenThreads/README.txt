
This library is intended to provide a minimal & complete Object-Oriented (OO) 
thread interface for C++ programmers.  It is loosely modeled on the Java thread
API, and the POSIX Threads standards.  The architecture of the library is 
designed around "swappable" thread models which are defined at compile-time
in a shared object library. 

It is of importance to note that while a factory pattern design could have been
used to achieve the goal of genaric interface, it would have required the 
programmer to allocate each of the 4 fundemental types (Thread, Mutex, Barrier,
& Condition ) on the heap.  Due to the cost associated with heap allocation 
for some of these constructs, such allocation was deemed unacceptable, and thus
the factory pattern was not used.

Instead, a somewhat obtuse - but effective - technique was chosen to provide 
the necessary data/implementation hiding.  This technique uses private void
pointers to encapsulate object private data.  The void pointers actually point
at concrete data structures, but give a uniform interface to the dso.

It is the intent of the Open Thread Group that the interfaces (header files) 
will be used to construct optimized implementations using platform optimized
multi-processing constructs such as the sproc methods used on IRIX & windows 
threads.   A sample implementation has been provided using the POSIX standards.

A doxygen file has been provided to create code documentation & man pages.  
It can be found in the root directory under docs/ 

Although the Open Thread Group does not have an email alias at this point in 
time,  one may contact the group via Don Burns (don@andesengineernig.com) or 
Robert Osfield (robert@openscenegraph.com).


- The Open Thread Group

