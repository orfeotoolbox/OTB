/******************************************************************************
 * $Id: sharedptr.hpp 813 2008-07-25 21:53:52Z mloskot $
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Shared pointer implementation for C++ libLAS 
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * The SharedPtr class is based on implementation of CountedPtr
 * from the book "The C++ Standard Library - A Tutorial and Reference"
 *
 * (C) Copyright Nicolai M. Josuttis 1999
 * (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net (modifications)
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
 *     * Neither the name of the Martin Isenburg or Iowa Department 
 *       of Natural Resources nor the names of its contributors may be 
 *       used to endorse or promote products derived from this software 
 *       without specific prior written permission.
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

#ifndef LIBLAS_DETAIL_SHAREDPTR_HPP_INCLUDED
#define LIBLAS_DETAIL_SHAREDPTR_HPP_INCLUDED
#include <cassert>

namespace liblas { namespace detail {

template <class T>
class SharedPtr
{
public:

    // Initialize pointer with existing pointer, where pointer p
    // is required to be a return value of new operator
    // TODO: Relax the "0 != p requirement"
    //  m_count(new long(p ? 1 : 0))
    explicit SharedPtr(T* p = 0)
        : m_ptr(p), m_count(new long(1))
    {}

    // Copy pointer (one more owner)
    SharedPtr(SharedPtr<T> const& other) throw()
        : m_ptr(other.m_ptr), m_count(other.m_count)
    {
            ++*m_count;
    }

    // Destructor (delete value if this was the last owner)
    ~SharedPtr() throw()
    {
        dispose();
    }

    // Assignment operator (unshare old and share new value)
    SharedPtr<T>& operator=(SharedPtr<T> const& rhs) throw()
    {
        if (&rhs != this)
        {
            dispose();
            m_ptr = rhs.m_ptr;
            m_count = rhs.m_count;
            ++*m_count;
        }
        return *this;
    }

    T& operator*() const throw()
    {
        assert(0 != m_ptr);
        return *m_ptr;
    }

    T* operator->() const throw()
    {
        assert(0 != m_ptr);
        return m_ptr;
    }

    T* get() const
    {
        return m_ptr;
    }

    long use_count() const
    {
        return (*m_count);
    }

private:

    T* m_ptr;
    long* m_count; // shared number of owners

    void dispose()
    {
        
        if (--*m_count == 0)
        {
            delete m_count;
            delete m_ptr;
        }
    }

};

}} // namespace liblas::detail

#endif // LIBLAS_DETAIL_SHAREDPTR_HPP_INCLUDED
