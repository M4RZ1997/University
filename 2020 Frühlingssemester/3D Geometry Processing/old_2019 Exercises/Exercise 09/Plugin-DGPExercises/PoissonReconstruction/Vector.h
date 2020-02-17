/*
Copyright (c) 2006, Michael Kazhdan and Matthew Bolitho
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution. 

Neither the name of the Johns Hopkins University nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

#ifndef __VECTOR_HPP
#define __VECTOR_HPP


#define Assert assert
#include <assert.h>
#include "Array.h"

template< class T >
class PoissonVector
{
public:
  PoissonVector( void );
  PoissonVector( const PoissonVector<T>& V );
  PoissonVector( size_t N );
  PoissonVector( size_t N, ConstPointer( T ) pV );
	~PoissonVector( void );

	const T& operator () (size_t i) const;
	T& operator () (size_t i);
	const T& operator [] (size_t i) const;
	T& operator [] (size_t i);

	void SetZero();

	size_t Dimensions() const;
	void Resize( size_t N );

	PoissonVector operator * (const T& A) const;
	PoissonVector operator / (const T& A) const;
	PoissonVector operator - (const PoissonVector& V) const;
	PoissonVector operator + (const PoissonVector& V) const;

	PoissonVector& operator *= ( const T& A );
	PoissonVector& operator /= ( const T& A );
	PoissonVector& operator += ( const PoissonVector& V );
	PoissonVector& operator -= ( const PoissonVector& V );

	PoissonVector& Add( const PoissonVector* V , int count );
	PoissonVector& AddScaled( const PoissonVector& V , const T& scale );
	PoissonVector& SubtractScaled( const PoissonVector& V , const T& scale );
	static void Add( const PoissonVector& V1 , const T& scale1 , const PoissonVector& V2 , const T& scale2 , PoissonVector& Out );
	static void Add( const PoissonVector& V1 , const T& scale1 , const PoissonVector& V2 , PoissonVector& Out );

	PoissonVector operator - () const;

	PoissonVector& operator = (const PoissonVector& V);

	T Dot( const PoissonVector& V ) const;

	T Length() const;

	T Norm( size_t Ln ) const;
	void Normalize();

	bool write( FILE* fp ) const;
	bool write( const char* fileName ) const;
	bool read( FILE* fp );
	bool read( const char* fileName );

	Pointer( T ) m_pV;
protected:
	size_t m_N;

};


#include "Vector.inl"

#endif
