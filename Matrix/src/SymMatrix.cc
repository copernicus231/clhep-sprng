// -*- C++ -*-
// ---------------------------------------------------------------------------
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
// 
// 
// Copyright Cornell University 1993, 1996, All Rights Reserved.
// 
// This software written by Nobu Katayama and Mike Smyth, Cornell University.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice and author attribution, this list of conditions and the
//    following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright
//    notice and author attribution, this list of conditions and the
//    following disclaimer in the documentation and/or other materials
//    provided with the distribution.
// 3. Neither the name of the University nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// Creation of derivative forms of this software for commercial
// utilization may be subject to restriction; written permission may be
// obtained from Cornell University.
// 
// CORNELL MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.  By way
// of example, but not limitation, CORNELL MAKES NO REPRESENTATIONS OR
// WARRANTIES OF MERCANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT
// THE USE OF THIS SOFTWARE OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS,
// COPYRIGHTS, TRADEMARKS, OR OTHER RIGHTS.  Cornell University shall not be
// held liable for any liability with respect to any claim by the user or any
// other party arising from use of the program.
//

#ifdef GNUPRAGMA
#pragma implementation
#endif

#include <string.h>
#include <float.h>        // for DBL_EPSILON

#include "CLHEP/Matrix/defs.h"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/DiagMatrix.h"
#include "CLHEP/Matrix/Vector.h"

#ifdef HEP_DEBUG_INLINE
#include "CLHEP/Matrix/SymMatrix.icc"
#endif

namespace CLHEP {

// Simple operation for all elements

#define SIMPLE_UOP(OPER)          \
   register HepMatrix::mIter a=m.begin();            \
   register HepMatrix::mIter e=m.begin()+num_size(); \
   for(;a<e; a++) (*a) OPER t;

#define SIMPLE_BOP(OPER)          \
   register HepMatrix::mIter a=m.begin();            \
   register HepMatrix::mcIter b=m2.m.begin();         \
   register HepMatrix::mcIter e=m.begin()+num_size(); \
   for(;a<e; a++, b++) (*a) OPER (*b);

#define SIMPLE_TOP(OPER)          \
   register HepMatrix::mcIter a=m1.m.begin();           \
   register HepMatrix::mcIter b=m2.m.begin();         \
   register HepMatrix::mIter t=mret.m.begin();         \
   register HepMatrix::mcIter e=m1.m.begin()+m1.num_size(); \
   for( ;a<e; a++, b++, t++) (*t) = (*a) OPER (*b);

#define CHK_DIM_2(r1,r2,c1,c2,fun) \
   if (r1!=r2 || c1!=c2)  { \
     HepGenMatrix::error("Range error in SymMatrix function " #fun "(1)."); \
   }

#define CHK_DIM_1(c1,r2,fun) \
   if (c1!=r2) { \
     HepGenMatrix::error("Range error in SymMatrix function " #fun "(2)."); \
   }

// Constructors. (Default constructors are inlined and in .icc file)

HepSymMatrix::HepSymMatrix(int p)
   : m(p*(p+1)/2), nrow(p)
{
   size = nrow * (nrow+1) / 2;
   m.assign(size,0);
}

HepSymMatrix::HepSymMatrix(int p, int init)
   : m(p*(p+1)/2), nrow(p)
{
   size = nrow * (nrow+1) / 2;

   m.assign(size,0);
   switch(init)
   {
   case 0:
      break;
      
   case 1:
      {
	 HepMatrix::mIter a; 
	 for(int i=0;i<nrow;++i) {
            a = m.begin() + (i+1)*i/2 + i;
	    *a = 1.0;
	 }
	 break;
      }
   default:
      error("SymMatrix: initialization must be either 0 or 1.");
   }
}

HepSymMatrix::HepSymMatrix(int p, HepRandom &r)
   : m(p*(p+1)/2), nrow(p)
{
   size = nrow * (nrow+1) / 2;
   HepMatrix::mIter a = m.begin();
   HepMatrix::mIter b = m.begin() + size;
   for(;a<b;a++) *a = r();
}

//
// Destructor
//
HepSymMatrix::~HepSymMatrix() {
}

HepSymMatrix::HepSymMatrix(const HepSymMatrix &m1)
   : m(m1.size), nrow(m1.nrow), size(m1.size)
{
   m = m1.m;
}

HepSymMatrix::HepSymMatrix(const HepDiagMatrix &m1)
   : m(m1.nrow*(m1.nrow+1)/2), nrow(m1.nrow)
{
   size = nrow * (nrow+1) / 2;

   int n = num_row();
   m.assign(size,0);

   HepMatrix::mIter mrr = m.begin();
   HepMatrix::mcIter mr = m1.m.begin();
   for(int r=1;r<=n;r++) {
      *mrr = *(mr++);
      if(r<n) mrr += (r+1);
   }
}

//
//
// Sub matrix
//
//

HepSymMatrix HepSymMatrix::sub(int min_row, int max_row) const
#ifdef HEP_GNU_OPTIMIZED_RETURN
return mret(max_row-min_row+1);
{
#else
{
  HepSymMatrix mret(max_row-min_row+1);
#endif
  if(max_row > num_row())
    error("HepSymMatrix::sub: Index out of range");
  HepMatrix::mIter a = mret.m.begin();
  HepMatrix::mcIter b1 = m.begin() + (min_row+2)*(min_row-1)/2;
  int rowsize=mret.num_row();
  for(int irow=1; irow<=rowsize; irow++) {
    HepMatrix::mcIter b = b1;
    for(int icol=0; icol<irow; ++icol) {
      *(a++) = *(b++);
    }
    if(irow<rowsize) b1 += irow+min_row-1;
  }
  return mret;
}

HepSymMatrix HepSymMatrix::sub(int min_row, int max_row) 
{
  HepSymMatrix mret(max_row-min_row+1);
  if(max_row > num_row())
    error("HepSymMatrix::sub: Index out of range");
  HepMatrix::mIter a = mret.m.begin();
  HepMatrix::mIter b1 = m.begin() + (min_row+2)*(min_row-1)/2;
  int rowsize=mret.num_row();
  for(int irow=1; irow<=rowsize; irow++) {
    HepMatrix::mIter b = b1;
    for(int icol=0; icol<irow; ++icol) {
      *(a++) = *(b++);
    }
    if(irow<rowsize) b1 += irow+min_row-1;
  }
  return mret;
}

void HepSymMatrix::sub(int row,const HepSymMatrix &m1)
{
  if(row <1 || row+m1.num_row()-1 > num_row() )
    error("HepSymMatrix::sub: Index out of range");
  HepMatrix::mcIter a = m1.m.begin();
  HepMatrix::mIter b1 = m.begin() + (row+2)*(row-1)/2;
  int rowsize=m1.num_row();
  for(int irow=1; irow<=rowsize; ++irow) {
    HepMatrix::mIter b = b1;
    for(int icol=0; icol<irow; ++icol) {
      *(b++) = *(a++);
    }
    if(irow<rowsize) b1 += irow+row-1;
  }
}

//
// Direct sum of two matricies
//

HepSymMatrix dsum(const HepSymMatrix &m1,
				     const HepSymMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
  return mret(m1.num_row() + m2.num_row(), 0);
{
#else
{
  HepSymMatrix mret(m1.num_row() + m2.num_row(),
				       0);
#endif
  mret.sub(1,m1);
  mret.sub(m1.num_row()+1,m2);
  return mret;
}

/* -----------------------------------------------------------------------
   This section contains support routines for matrix.h. This section contains
   The two argument functions +,-. They call the copy constructor and +=,-=.
   ----------------------------------------------------------------------- */
HepSymMatrix HepSymMatrix::operator- () const 
#ifdef HEP_GNU_OPTIMIZED_RETURN
      return m2(nrow);
{
#else
{
   HepSymMatrix m2(nrow);
#endif
   register HepMatrix::mcIter a=m.begin();
   register HepMatrix::mIter b=m2.m.begin();
   register HepMatrix::mcIter e=m.begin()+num_size();
   for(;a<e; a++, b++) (*b) = -(*a);
   return m2;
}

   

HepMatrix operator+(const HepMatrix &m1,const HepSymMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1);
{
#else
{
  HepMatrix mret(m1);
#endif
  CHK_DIM_2(m1.num_row(),m2.num_row(), m1.num_col(),m2.num_col(),+);
  mret += m2;
  return mret;
}
HepMatrix operator+(const HepSymMatrix &m1,const HepMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m2);
{
#else
{
  HepMatrix mret(m2);
#endif
  CHK_DIM_2(m1.num_row(),m2.num_row(),m1.num_col(),m2.num_col(),+);
  mret += m1;
  return mret;
}

HepSymMatrix operator+(const HepSymMatrix &m1,const HepSymMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.nrow);
{
#else
{
  HepSymMatrix mret(m1.nrow);
#endif
  CHK_DIM_1(m1.nrow, m2.nrow,+);
  SIMPLE_TOP(+)
  return mret;
}

//
// operator -
//

HepMatrix operator-(const HepMatrix &m1,const HepSymMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1);
{
#else
{
  HepMatrix mret(m1);
#endif
  CHK_DIM_2(m1.num_row(),m2.num_row(),
			 m1.num_col(),m2.num_col(),-);
  mret -= m2;
  return mret;
}
HepMatrix operator-(const HepSymMatrix &m1,const HepMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1);
{
#else
{
  HepMatrix mret(m1);
#endif
  CHK_DIM_2(m1.num_row(),m2.num_row(),
			 m1.num_col(),m2.num_col(),-);
  mret -= m2;
  return mret;
}

HepSymMatrix operator-(const HepSymMatrix &m1,const HepSymMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.num_row());
{
#else
{
  HepSymMatrix mret(m1.num_row());
#endif
  CHK_DIM_1(m1.num_row(),m2.num_row(),-);
  SIMPLE_TOP(-)
  return mret;
}

/* -----------------------------------------------------------------------
   This section contains support routines for matrix.h. This file contains
   The two argument functions *,/. They call copy constructor and then /=,*=.
   ----------------------------------------------------------------------- */

HepSymMatrix operator/(
const HepSymMatrix &m1,double t)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1);
{
#else
{
  HepSymMatrix mret(m1);
#endif
  mret /= t;
  return mret;
}

HepSymMatrix operator*(const HepSymMatrix &m1,double t)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1);
{
#else
{
  HepSymMatrix mret(m1);
#endif
  mret *= t;
  return mret;
}

HepSymMatrix operator*(double t,const HepSymMatrix &m1)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1);
{
#else
{
  HepSymMatrix mret(m1);
#endif
  mret *= t;
  return mret;
}


HepMatrix operator*(const HepMatrix &m1,const HepSymMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.num_row(),m2.num_col());
{
#else
  {
    HepMatrix mret(m1.num_row(),m2.num_col());
#endif
    CHK_DIM_1(m1.num_col(),m2.num_row(),*);
    HepMatrix::mcIter mit1, mit2, sp,snp; //mit2=0
    double temp;
    HepMatrix::mIter mir=mret.m.begin();
    for(mit1=m1.m.begin();
        mit1<m1.m.begin()+m1.num_row()*m1.num_col();
	mit1 = mit2)
    {
      snp=m2.m.begin();
      for(int step=1;step<=m2.num_row();++step)
	{
	  mit2=mit1;
	  sp=snp;
	  snp+=step;
	  temp=0;
	  while(sp<snp)
	    temp+=*(sp++)*(*(mit2++));
          if( step<m2.num_row() ) {	// only if we aren't on the last row
	    sp+=step-1;
	    for(int stept=step+1;stept<=m2.num_row();stept++)
	      {
		temp+=*sp*(*(mit2++));
		if(stept<m2.num_row()) sp+=stept;
	      }
	    }	// if(step
	  *(mir++)=temp;
	}	// for(step
      }	// for(mit1
    return mret;
  }

HepMatrix operator*(const HepSymMatrix &m1,const HepMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.num_row(),m2.num_col());
{
#else
{
  HepMatrix mret(m1.num_row(),m2.num_col());
#endif
  CHK_DIM_1(m1.num_col(),m2.num_row(),*);
  int step,stept;
  HepMatrix::mcIter mit1,mit2,sp,snp;
  double temp;
  HepMatrix::mIter mir=mret.m.begin();
  for(step=1,snp=m1.m.begin();step<=m1.num_row();snp+=step++)
    for(mit1=m2.m.begin();mit1<m2.m.begin()+m2.num_col();mit1++)
      {
	mit2=mit1;
	sp=snp;
	temp=0;
	while(sp<snp+step)
	  {
	    temp+=*mit2*(*(sp++));
	    if( m2.num_size()-(mit2-m2.m.begin())>m2.num_col() ){
	      mit2+=m2.num_col();
	    }
	  }
        if(step<m1.num_row()) {	// only if we aren't on the last row
	  sp+=step-1;
	  for(stept=step+1;stept<=m1.num_row();stept++)
	    {
	      temp+=*mit2*(*sp);
	      if(stept<m1.num_row()) {
		mit2+=m2.num_col();
		sp+=stept;
	      }
	    }
          }	// if(step
	*(mir++)=temp;
      }	// for(mit1
  return mret;
}

HepMatrix operator*(const HepSymMatrix &m1,const HepSymMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.num_row(),m1.num_row());
{
#else
{
  HepMatrix mret(m1.num_row(),m1.num_row());
#endif
  CHK_DIM_1(m1.num_col(),m2.num_row(),*);
  int step1,stept1,step2,stept2;
  HepMatrix::mcIter snp1,sp1,snp2,sp2;
  double temp;
  HepMatrix::mIter mr = mret.m.begin();
  snp1=m1.m.begin();
  for(step1=1;step1<=m1.num_row();++step1) {
    snp2=m2.m.begin();
    for(step2=1;step2<=m2.num_row();++step2)
      {
	sp1=snp1;
	sp2=snp2;
	snp2+=step2;
	temp=0;
	if(step1<step2)
	  {
	    while(sp1<snp1+step1) {
	      temp+=(*(sp1++))*(*(sp2++));
	      }
	    sp1+=step1-1;
	    for(stept1=step1+1;stept1!=step2+1;++stept1) {
	      temp+=(*sp1)*(*(sp2++));
	      if(stept1<m2.num_row()) sp1+=stept1;
	      }
            if(step2<m2.num_row()) {	// only if we aren't on the last row
	      sp2+=step2-1;
	      for(stept2=step2+1;stept2<=m2.num_row();stept1++,stept2++) {
		temp+=(*sp1)*(*sp2);
		if(stept2<m2.num_row()) {
	           sp1+=stept1;
		   sp2+=stept2;
		   }
		}	// for(stept2
	      }	// if(step2
	  }	// step1<step2
	else
	  {
	    while(sp2<snp2) {
	      temp+=(*(sp1++))*(*(sp2++));
	      }
	    if(step2<m2.num_row()) {	// only if we aren't on the last row
	      sp2+=step2-1;
	      for(stept2=step2+1;stept2!=step1+1;stept2++) {
		temp+=(*(sp1++))*(*sp2);
		if(stept2<m1.num_row()) sp2+=stept2;
		}
	      if(step1<m1.num_row()) {	// only if we aren't on the last row
		sp1+=step1-1;
		for(stept1=step1+1;stept1<=m1.num_row();stept1++,stept2++) {
		  temp+=(*sp1)*(*sp2);
		  if(stept1<m1.num_row()) {
	             sp1+=stept1;
		     sp2+=stept2;
		     }
		  }	// for(stept1
		}	// if(step1
	      }	// if(step2
	  }	// else
	*(mr++)=temp;
      }	// for(step2
    if(step1<m1.num_row()) snp1+=step1;
    }	// for(step1
  return mret;
}

HepVector operator*(const HepSymMatrix &m1,const HepVector &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.num_row());
{
#else
{
  HepVector mret(m1.num_row());
#endif
  CHK_DIM_1(m1.num_col(),m2.num_row(),*);
  HepMatrix::mcIter sp,snp,vpt;
  double temp;
  int step,stept;
  HepMatrix::mIter vrp=mret.m.begin();
  for(step=1,snp=m1.m.begin();step<=m1.num_row();++step)
    {
      sp=snp;
      vpt=m2.m.begin();
      snp+=step;
      temp=0;
      while(sp<snp)
	temp+=*(sp++)*(*(vpt++));
      if(step<m1.num_row()) sp+=step-1;
      for(stept=step+1;stept<=m1.num_row();stept++)
	{ 
	  temp+=*sp*(*(vpt++));
	  if(stept<m1.num_row()) sp+=stept;
	}
      *(vrp++)=temp;
    }	// for(step
  return mret;
}

HepSymMatrix vT_times_v(const HepVector &v)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(v.num_row());
{
#else
{
  HepSymMatrix mret(v.num_row());
#endif
  HepMatrix::mIter mr=mret.m.begin();
  HepMatrix::mcIter vt1,vt2;
  for(vt1=v.m.begin();vt1<v.m.begin()+v.num_row();vt1++)
    for(vt2=v.m.begin();vt2<=vt1;vt2++)
      *(mr++)=(*vt1)*(*vt2);
  return mret;
}

/* -----------------------------------------------------------------------
   This section contains the assignment and inplace operators =,+=,-=,*=,/=.
   ----------------------------------------------------------------------- */

HepMatrix & HepMatrix::operator+=(const HepSymMatrix &m2)
{
  CHK_DIM_2(num_row(),m2.num_row(),num_col(),m2.num_col(),+=);
  HepMatrix::mcIter sjk = m2.m.begin();
  // j >= k
  for(int j=0; j!=nrow; ++j) {
     for(int k=0; k<=j; ++k) {
	m[j*ncol+k] += *sjk;
	// make sure this is not a diagonal element
	if(k!=j) m[k*nrow+j] += *sjk;
        ++sjk;
     } 
  }   
  return (*this);
}

HepSymMatrix & HepSymMatrix::operator+=(const HepSymMatrix &m2)
{
  CHK_DIM_2(num_row(),m2.num_row(),num_col(),m2.num_col(),+=);
  SIMPLE_BOP(+=)
  return (*this);
}

HepMatrix & HepMatrix::operator-=(const HepSymMatrix &m2)
{
  CHK_DIM_2(num_row(),m2.num_row(),num_col(),m2.num_col(),-=);
  HepMatrix::mcIter sjk = m2.m.begin();
  // j >= k
  for(int j=0; j!=nrow; ++j) {
     for(int k=0; k<=j; ++k) {
	m[j*ncol+k] -= *sjk;
	// make sure this is not a diagonal element
	if(k!=j) m[k*nrow+j] -= *sjk;
        ++sjk;
     } 
  }   
  return (*this);
}

HepSymMatrix & HepSymMatrix::operator-=(const HepSymMatrix &m2)
{
  CHK_DIM_2(num_row(),m2.num_row(),num_col(),m2.num_col(),-=);
  SIMPLE_BOP(-=)
  return (*this);
}

HepSymMatrix & HepSymMatrix::operator/=(double t)
{
  SIMPLE_UOP(/=)
  return (*this);
}

HepSymMatrix & HepSymMatrix::operator*=(double t)
{
  SIMPLE_UOP(*=)
  return (*this);
}

HepMatrix & HepMatrix::operator=(const HepSymMatrix &m1)
{
   // define size, rows, and columns of *this
   nrow = ncol = m1.nrow;
   if(nrow*ncol != size)
   {
      size = nrow*ncol;
      m.resize(size);
   }
   // begin copy
   mcIter sjk = m1.m.begin();
   // j >= k
   for(int j=0; j!=nrow; ++j) {
      for(int k=0; k<=j; ++k) {
	 m[j*ncol+k] = *sjk;
	 // we could copy the diagonal element twice or check 
	 // doing the check may be a tiny bit faster,
	 // so we choose that option for now
	 if(k!=j) m[k*nrow+j] = *sjk;
         ++sjk;
      } 
   }   
   return (*this);
}

HepSymMatrix & HepSymMatrix::operator=(const HepSymMatrix &m1)
{
   if(m1.nrow != nrow)
   {
      nrow = m1.nrow;
      size = m1.size;
      m.resize(size);
   }
   m = m1.m;
   return (*this);
}

HepSymMatrix & HepSymMatrix::operator=(const HepDiagMatrix &m1)
{
   if(m1.nrow != nrow)
   {
      nrow = m1.nrow;
      size = nrow * (nrow+1) / 2;
      m.resize(size);
   }

   m.assign(size,0);
   HepMatrix::mIter mrr = m.begin();
   HepMatrix::mcIter mr = m1.m.begin();
   for(int r=1; r<=nrow; r++) {
      *mrr = *(mr++);
      if(r<nrow) mrr += (r+1);
   }
   return (*this);
}

// Print the Matrix.

std::ostream& operator<<(std::ostream &s, const HepSymMatrix &q)
{
  s << std::endl;
/* Fixed format needs 3 extra characters for field, while scientific needs 7 */
  int width;
  if(s.flags() & std::ios::fixed)
    width = s.precision()+3;
  else
    width = s.precision()+7;
  for(int irow = 1; irow<= q.num_row(); irow++)
    {
      for(int icol = 1; icol <= q.num_col(); icol++)
	{
	  s.width(width);
	  s << q(irow,icol) << " ";
	}
      s << std::endl;
    }
  return s;
}

HepSymMatrix HepSymMatrix::
apply(double (*f)(double, int, int)) const
#ifdef HEP_GNU_OPTIMIZED_RETURN
return mret(num_row());
{
#else
{
  HepSymMatrix mret(num_row());
#endif
  HepMatrix::mcIter a = m.begin();
  HepMatrix::mIter b = mret.m.begin();
  for(int ir=1;ir<=num_row();ir++) {
    for(int ic=1;ic<=ir;ic++) {
      *(b++) = (*f)(*(a++), ir, ic);
    }
  }
  return mret;
}

void HepSymMatrix::assign (const HepMatrix &m1)
{
   if(m1.nrow != nrow)
   {
      nrow = m1.nrow;
      size = nrow * (nrow+1) / 2;
      m.resize(size);
   }
   HepMatrix::mcIter a = m1.m.begin();
   HepMatrix::mIter b = m.begin();
   for(int r=1;r<=nrow;r++) {
      HepMatrix::mcIter d = a;
      for(int c=1;c<=r;c++) {
	 *(b++) = *(d++);
      }
      if(r<nrow) a += nrow;
   }
}

HepSymMatrix HepSymMatrix::similarity(const HepMatrix &m1) const
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.num_row());
{
#else
{
  HepSymMatrix mret(m1.num_row());
#endif
  HepMatrix temp = m1*(*this);
// If m1*(*this) has correct dimensions, then so will the m1.T multiplication.
// So there is no need to check dimensions again.
  int n = m1.num_col();
  HepMatrix::mIter mr = mret.m.begin();
  HepMatrix::mIter tempr1 = temp.m.begin();
  for(int r=1;r<=mret.num_row();r++) {
    HepMatrix::mcIter m1c1 = m1.m.begin();
    for(int c=1;c<=r;c++) {
      register double tmp = 0.0;
      HepMatrix::mIter tempri = tempr1;
      HepMatrix::mcIter m1ci = m1c1;
      for(int i=1;i<=m1.num_col();i++) {
	tmp+=(*(tempri++))*(*(m1ci++));
      }
      *(mr++) = tmp;
      m1c1 += n;
    }
    tempr1 += n;
  }
  return mret;
}

HepSymMatrix HepSymMatrix::similarity(const HepSymMatrix &m1) const
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.num_row());
{
#else
{
  HepSymMatrix mret(m1.num_row());
#endif
  HepMatrix temp = m1*(*this);
  int n = m1.num_col();
  HepMatrix::mIter mr = mret.m.begin();
  HepMatrix::mIter tempr1 = temp.m.begin();
  for(int r=1;r<=mret.num_row();r++) {
    HepMatrix::mcIter m1c1 = m1.m.begin();
    int c;
    for(c=1;c<=r;c++) {
      register double tmp = 0.0;
      HepMatrix::mIter tempri = tempr1;
      HepMatrix::mcIter m1ci = m1c1;
      int i;
      for(i=1;i<c;i++) {
	tmp+=(*(tempri++))*(*(m1ci++));
      }
      for(i=c;i<=m1.num_col();i++) {
	tmp+=(*(tempri++))*(*(m1ci));
	if(i<m1.num_col()) m1ci += i;
      }
      *(mr++) = tmp;
      m1c1 += c;
    }
    tempr1 += n;
  }
  return mret;
}

double HepSymMatrix::similarity(const HepVector &m1)
const {
  register double mret = 0.0;
  HepVector temp = (*this) *m1;
// If m1*(*this) has correct dimensions, then so will the m1.T multiplication.
// So there is no need to check dimensions again.
  HepMatrix::mIter a=temp.m.begin();
  HepMatrix::mcIter b=m1.m.begin();
  HepMatrix::mIter e=a+m1.num_row();
  for(;a<e;) mret += (*(a++)) * (*(b++));
  return mret;
}

HepSymMatrix HepSymMatrix::similarityT(const HepMatrix &m1) const
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.num_col());
{
#else
{
  HepSymMatrix mret(m1.num_col());
#endif
  HepMatrix temp = (*this)*m1;
  int n = m1.num_col();
  HepMatrix::mIter mrc = mret.m.begin();
  HepMatrix::mIter temp1r = temp.m.begin();
  for(int r=1;r<=mret.num_row();r++) {
    HepMatrix::mcIter m11c = m1.m.begin();
    for(int c=1;c<=r;c++) {
      register double tmp = 0.0;
      for(int i=1;i<=m1.num_row();i++) {
	HepMatrix::mIter tempir = temp1r + n*(i-1);
	HepMatrix::mcIter m1ic = m11c + n*(i-1);
	tmp+=(*(tempir))*(*(m1ic));
      }
      *(mrc++) = tmp;
      m11c++;
    }
    temp1r++;
  }
  return mret;
}

void HepSymMatrix::invert(int &ifail) {
  
  ifail = 0;

  switch(nrow) {
  case 3:
    {
      double det, temp;
      double t1, t2, t3;
      double c11,c12,c13,c22,c23,c33;
      c11 = (*(m.begin()+2)) * (*(m.begin()+5)) - (*(m.begin()+4)) * (*(m.begin()+4));
      c12 = (*(m.begin()+4)) * (*(m.begin()+3)) - (*(m.begin()+1)) * (*(m.begin()+5));
      c13 = (*(m.begin()+1)) * (*(m.begin()+4)) - (*(m.begin()+2)) * (*(m.begin()+3));
      c22 = (*(m.begin()+5)) * (*m.begin()) - (*(m.begin()+3)) * (*(m.begin()+3));
      c23 = (*(m.begin()+3)) * (*(m.begin()+1)) - (*(m.begin()+4)) * (*m.begin());
      c33 = (*m.begin()) * (*(m.begin()+2)) - (*(m.begin()+1)) * (*(m.begin()+1));
      t1 = fabs(*m.begin());
      t2 = fabs(*(m.begin()+1));
      t3 = fabs(*(m.begin()+3));
      if (t1 >= t2) {
	if (t3 >= t1) {
	  temp = *(m.begin()+3);
	  det = c23*c12-c22*c13;
	} else {
	  temp = *m.begin();
	  det = c22*c33-c23*c23;
	}
      } else if (t3 >= t2) {
	temp = *(m.begin()+3);
	det = c23*c12-c22*c13;
      } else {
	temp = *(m.begin()+1);
	det = c13*c23-c12*c33;
      }
      if (det==0) {
	ifail = 1;
	return;
      }
      {
	double s = temp/det;
	HepMatrix::mIter mm = m.begin();
	*(mm++) = s*c11;
	*(mm++) = s*c12;
	*(mm++) = s*c22;
	*(mm++) = s*c13;
	*(mm++) = s*c23;
	*(mm) = s*c33;
      }
    }
    break;
 case 2:
    {
      double det, temp, s;
      det = (*m.begin())*(*(m.begin()+2)) - (*(m.begin()+1))*(*(m.begin()+1));
      if (det==0) {
	ifail = 1;
	return;
      }
      s = 1.0/det;
      *(m.begin()+1) *= -s;
      temp = s*(*(m.begin()+2));
      *(m.begin()+2) = s*(*m.begin());
      *m.begin() = temp;
      break;
    }
 case 1:
    {
      if ((*m.begin())==0) {
	ifail = 1;
	return;
      }
      *m.begin() = 1.0/(*m.begin());
      break;
    }
 case 5:
    {
      invert5(ifail);
      return;
    }
 case 6:
    {
      invert6(ifail);
      return;
    }
 case 4:
    {
      invert4(ifail);
      return;
    }
 default:
    {
      invertBunchKaufman(ifail);
      return;
    }
  }
  return; // inversion successful
}

double HepSymMatrix::determinant() const {
  static const int max_array = 20;
  // ir must point to an array which is ***1 longer than*** nrow
  static std::vector<int> ir_vec (max_array+1); 
  if (ir_vec.size() <= static_cast<unsigned int>(nrow)) ir_vec.resize(nrow+1);
  int * ir = &ir_vec[0];   

  double det;
  HepMatrix mt(*this);
  int i = mt.dfact_matrix(det, ir);
  if(i==0) return det;
  return 0.0;
}

double HepSymMatrix::trace() const {
   double t = 0.0;
   for (int i=0; i<nrow; i++) 
     t += *(m.begin() + (i+3)*i/2);
   return t;
}

void HepSymMatrix::invertBunchKaufman(int &ifail) {
  // Bunch-Kaufman diagonal pivoting method
  // It is decribed in J.R. Bunch, L. Kaufman (1977). 
  // "Some Stable Methods for Calculating Inertia and Solving Symmetric 
  // Linear Systems", Math. Comp. 31, p. 162-179. or in Gene H. Golub, 
  // Charles F. van Loan, "Matrix Computations" (the second edition 
  // has a bug.) and implemented in "lapack"
  // Mario Stanke, 09/97

  int i, j, k, s;
  int pivrow;

  // Establish the two working-space arrays needed:  x and piv are
  // used as pointers to arrays of doubles and ints respectively, each
  // of length nrow.  We do not want to reallocate each time through
  // unless the size needs to grow.  We do not want to leak memory, even
  // by having a new without a delete that is only done once.
  
  static const int max_array = 25;
#ifdef DISABLE_ALLOC
  static std::vector<double> xvec (max_array);
  static std::vector<int>    pivv (max_array);
  typedef std::vector<int>::iterator pivIter; 
#else
  static std::vector<double,Alloc<double,25> > xvec (max_array);
  static std::vector<int,   Alloc<int,   25> > pivv (max_array);
  typedef std::vector<int,Alloc<int,25> >::iterator pivIter; 
#endif	
  if (xvec.size() < static_cast<unsigned int>(nrow)) xvec.resize(nrow);
  if (pivv.size() < static_cast<unsigned int>(nrow)) pivv.resize(nrow);
     // Note - resize shuld do  nothing if the size is already larger than nrow,
     //        but on VC++ there are indications that it does so we check.
     // Note - the data elements in a vector are guaranteed to be contiguous,
     //        so x[i] and piv[i] are optimally fast.
  mIter   x   = xvec.begin();
  // x[i] is used as helper storage, needs to have at least size nrow.
  pivIter piv = pivv.begin();
  // piv[i] is used to store details of exchanges
      
  double temp1, temp2;
  HepMatrix::mIter ip, mjj, iq;
  double lambda, sigma;
  const double alpha = .6404; // = (1+sqrt(17))/8
  const double epsilon = 32*DBL_EPSILON;
  // whenever a sum of two doubles is below or equal to epsilon
  // it is set to zero.
  // this constant could be set to zero but then the algorithm
  // doesn't neccessarily detect that a matrix is singular
  
  for (i = 0; i < nrow; ++i) piv[i] = i+1;
      
  ifail = 0;
      
  // compute the factorization P*A*P^T = L * D * L^T 
  // L is unit lower triangular, D is direct sum of 1x1 and 2x2 matrices
  // L and D^-1 are stored in A = *this, P is stored in piv[]
	
  for (j=1; j < nrow; j+=s)  // main loop over columns
  {
      mjj = m.begin() + j*(j-1)/2 + j-1;
      lambda = 0;           // compute lambda = max of A(j+1:n,j)
      pivrow = j+1;
      //ip = m.begin() + (j+1)*j/2 + j-1;
      for (i=j+1; i <= nrow ; ++i) {
          // calculate ip to avoid going off end of storage array
	  ip = m.begin() + (i-1)*i/2 + j-1;
	  if (fabs(*ip) > lambda) {
	      lambda = fabs(*ip);
	      pivrow = i;
	  }
      }	// for i
      if (lambda == 0 ) {
	  if (*mjj == 0) {
	      ifail = 1;
	      return;
	  }
	  s=1;
	  *mjj = 1./ *mjj;
      } else {	// lambda == 0
	  if (fabs(*mjj) >= lambda*alpha) {
	      s=1;
	      pivrow=j;
	  } else {	// fabs(*mjj) >= lambda*alpha
	      sigma = 0;  // compute sigma = max A(pivrow, j:pivrow-1)
	      ip = m.begin() + pivrow*(pivrow-1)/2+j-1;
	      for (k=j; k < pivrow; k++) {
		  if (fabs(*ip) > sigma) sigma = fabs(*ip);
		  ip++;
	      }	// for k
	      if (sigma * fabs(*mjj) >= alpha * lambda * lambda) {
		  s=1;
		  pivrow = j;
	      } else if (fabs(*(m.begin()+pivrow*(pivrow-1)/2+pivrow-1)) 
			    >= alpha * sigma) {
		s=1;
	      } else {
		s=2;
	      }	// if sigma...
	  }	// fabs(*mjj) >= lambda*alpha
	  if (pivrow == j) { // no permutation neccessary
	      piv[j-1] = pivrow;
	      if (*mjj == 0) {
		  ifail=1;
		  return;
	      }
	      temp2 = *mjj = 1./ *mjj; // invert D(j,j)

	      // update A(j+1:n, j+1,n)
	      for (i=j+1; i <= nrow; i++) {
		  temp1 = *(m.begin() + i*(i-1)/2 + j-1) * temp2;
		  ip = m.begin()+i*(i-1)/2+j;
		  for (k=j+1; k<=i; k++) {
		      *ip -= temp1 * *(m.begin() + k*(k-1)/2 + j-1);
		      if (fabs(*ip) <= epsilon)
			*ip=0;
		      ip++;
		  }
	      }	// for i
	      // update L 
	      //ip = m.begin() + (j+1)*j/2 + j-1; 
	      for (i=j+1; i <= nrow; ++i) {
        	  // calculate ip to avoid going off end of storage array
		  ip = m.begin() + (i-1)*i/2 + j-1;
	          *ip *= temp2;
	      }
	  } else if (s==1) { // 1x1 pivot 
	      piv[j-1] = pivrow;

	      // interchange rows and columns j and pivrow in
	      // submatrix (j:n,j:n)
	      ip = m.begin() + pivrow*(pivrow-1)/2 + j;
	      for (i=j+1; i < pivrow; i++, ip++) {
		  temp1 = *(m.begin() + i*(i-1)/2 + j-1);
		  *(m.begin() + i*(i-1)/2 + j-1)= *ip;
		  *ip = temp1;
	      }	// for i
	      temp1 = *mjj;
	      *mjj = *(m.begin()+pivrow*(pivrow-1)/2+pivrow-1);
	      *(m.begin()+pivrow*(pivrow-1)/2+pivrow-1) = temp1;
	      ip = m.begin() + (pivrow+1)*pivrow/2 + j-1;
	      iq = ip + pivrow-j;
	      for (i = pivrow+1; i <= nrow; ip += i, iq += i++) {
		  temp1 = *iq;
		  *iq = *ip;
		  *ip = temp1;
	      }	// for i

	      if (*mjj == 0) {
		  ifail = 1;
		  return;
	      }	// *mjj == 0
	      temp2 = *mjj = 1./ *mjj; // invert D(j,j)

	      // update A(j+1:n, j+1:n)
	      for (i = j+1; i <= nrow; i++) {
		  temp1 = *(m.begin() + i*(i-1)/2 + j-1) * temp2;
		  ip = m.begin()+i*(i-1)/2+j;
		  for (k=j+1; k<=i; k++) {
		      *ip -= temp1 * *(m.begin() + k*(k-1)/2 + j-1);
		      if (fabs(*ip) <= epsilon)
			*ip=0;
		      ip++;
		  }	// for k
	      }	// for i
	      // update L 
	      //ip = m.begin() + (j+1)*j/2 + j-1; 
	      for (i=j+1; i <= nrow; ++i) {
        	  // calculate ip to avoid going off end of storage array
		  ip = m.begin() + (i-1)*i/2 + j-1;
	          *ip *= temp2;
	      }
	  } else { // s=2, ie use a 2x2 pivot
	      piv[j-1] = -pivrow;
	      piv[j] = 0; // that means this is the second row of a 2x2 pivot

	      if (j+1 != pivrow) {
		  // interchange rows and columns j+1 and pivrow in
		  // submatrix (j:n,j:n) 
		  ip = m.begin() + pivrow*(pivrow-1)/2 + j+1;
		  for (i=j+2; i < pivrow; i++, ip++) {
		      temp1 = *(m.begin() + i*(i-1)/2 + j);
		      *(m.begin() + i*(i-1)/2 + j) = *ip;
		      *ip = temp1;
		  }	// for i
		  temp1 = *(mjj + j + 1);
		  *(mjj + j + 1) = 
		    *(m.begin() + pivrow*(pivrow-1)/2 + pivrow-1);
		  *(m.begin() + pivrow*(pivrow-1)/2 + pivrow-1) = temp1;
		  temp1 = *(mjj + j);
		  *(mjj + j) = *(m.begin() + pivrow*(pivrow-1)/2 + j-1);
		  *(m.begin() + pivrow*(pivrow-1)/2 + j-1) = temp1;
		  ip = m.begin() + (pivrow+1)*pivrow/2 + j;
		  iq = ip + pivrow-(j+1);
		  for (i = pivrow+1; i <= nrow; ip += i, iq += i++) {
		      temp1 = *iq;
		      *iq = *ip;
		      *ip = temp1;
		  }	// for i
	      }	//  j+1 != pivrow
	      // invert D(j:j+1,j:j+1)
	      temp2 = *mjj * *(mjj + j + 1) - *(mjj + j) * *(mjj + j); 
	      if (temp2 == 0) {
		std::cerr
		  << "SymMatrix::bunch_invert: error in pivot choice" 
		  << std::endl;
              }
	      temp2 = 1. / temp2;
	      // this quotient is guaranteed to exist by the choice 
	      // of the pivot
	      temp1 = *mjj;
	      *mjj = *(mjj + j + 1) * temp2;
	      *(mjj + j + 1) = temp1 * temp2;
	      *(mjj + j) = - *(mjj + j) * temp2;

	      if (j < nrow-1) { // otherwise do nothing
		  // update A(j+2:n, j+2:n)
		  for (i=j+2; i <= nrow ; i++) {
		      ip = m.begin() + i*(i-1)/2 + j-1;
		      temp1 = *ip * *mjj + *(ip + 1) * *(mjj + j);
		      if (fabs(temp1 ) <= epsilon) temp1 = 0;
		      temp2 = *ip * *(mjj + j) + *(ip + 1) * *(mjj + j + 1);
		      if (fabs(temp2 ) <= epsilon) temp2 = 0;
		      for (k = j+2; k <= i ; k++) {
			  ip = m.begin() + i*(i-1)/2 + k-1;
			  iq = m.begin() + k*(k-1)/2 + j-1;
			  *ip -= temp1 * *iq + temp2 * *(iq+1);
			  if (fabs(*ip) <= epsilon)
			    *ip = 0;
		      }	// for k
		  }	// for i
		  // update L
		  for (i=j+2; i <= nrow ; i++) {
		      ip = m.begin() + i*(i-1)/2 + j-1;
		      temp1 = *ip * *mjj + *(ip+1) * *(mjj + j);
		      if (fabs(temp1) <= epsilon) temp1 = 0;
		      *(ip+1) = *ip * *(mjj + j) 
			        + *(ip+1) * *(mjj + j + 1);
		      if (fabs(*(ip+1)) <= epsilon) *(ip+1) = 0;
		      *ip = temp1;
		  }	// for k
	      }	// j < nrow-1
	  }
      }
  } // end of main loop over columns

  if (j == nrow) { // the the last pivot is 1x1
      mjj = m.begin() + j*(j-1)/2 + j-1;
      if (*mjj == 0) {
	  ifail = 1;
	  return;
      } else { *mjj = 1. / *mjj; }
  } // end of last pivot code

  // computing the inverse from the factorization
	 
  for (j = nrow ; j >= 1 ; j -= s) // loop over columns
  {
      mjj = m.begin() + j*(j-1)/2 + j-1;
      if (piv[j-1] > 0) { // 1x1 pivot, compute column j of inverse
	  s = 1; 
	  if (j < nrow) {
	      //ip = m.begin() + (j+1)*j/2 + j-1;
	      //for (i=0; i < nrow-j; ip += 1+j+i++) x[i] = *ip;
	      ip = m.begin() + (j+1)*j/2 - 1;
	      for (i=0; i < nrow-j; ++i) {
	          ip += i + j;
	          x[i] = *ip;
	      }
	      for (i=j+1; i<=nrow ; i++) {
		  temp2=0;
		  ip = m.begin() + i*(i-1)/2 + j;
		  for (k=0; k <= i-j-1; k++) temp2 += *ip++ * x[k];
		  // avoid setting ip outside the bounds of the storage array
		  ip -= 1;
		  // using the value of k from the previous loop
		  for ( ; k < nrow-j; ++k) {
		      ip += j+k;
		      temp2 += *ip * x[k];
		  }
		  *(m.begin()+ i*(i-1)/2 + j-1) = -temp2;
	      }	// for i
	      temp2 = 0;
	      //ip = m.begin() + (j+1)*j/2 + j-1;
	      //for (k=0; k < nrow-j; ip += 1+j+k++)
		//temp2 += x[k] * *ip;
	      ip = m.begin() + (j+1)*j/2 - 1;
	      for (k=0; k < nrow-j; ++k) {
	        ip += j+k;
		temp2 += x[k] * *ip;
	      }
	      *mjj -= temp2;
	  }	// j < nrow
      } else { //2x2 pivot, compute columns j and j-1 of the inverse
	  if (piv[j-1] != 0)
	    std::cerr << "error in piv" << piv[j-1] << std::endl;
	  s=2; 
	  if (j < nrow) {
	      //ip = m.begin() + (j+1)*j/2 + j-1;
	      //for (i=0; i < nrow-j; ip += 1+j+i++) x[i] = *ip;
	      ip = m.begin() + (j+1)*j/2 - 1;
	      for (i=0; i < nrow-j; ++i) {
	          ip += i + j;
	          x[i] = *ip;
	      }
	      for (i=j+1; i<=nrow ; i++) {
		  temp2 = 0;
		  ip = m.begin() + i*(i-1)/2 + j;
		  for (k=0; k <= i-j-1; k++)
		    temp2 += *ip++ * x[k];
		  for (ip += i-1; k < nrow-j; ip += 1+j+k++)
		    temp2 += *ip * x[k];
		  *(m.begin()+ i*(i-1)/2 + j-1) = -temp2;
	      }	// for i   
	      temp2 = 0;
	      //ip = m.begin() + (j+1)*j/2 + j-1;
	      //for (k=0; k < nrow-j; ip += 1+j+k++) temp2 += x[k] * *ip;
	      ip = m.begin() + (j+1)*j/2 - 1;
	      for (k=0; k < nrow-j; ++k) {
	          ip += k + j;
	          temp2 += x[k] * *ip;
	      }
	      *mjj -= temp2;
	      temp2 = 0;
	      //ip = m.begin() + (j+1)*j/2 + j-2;
	      //for (i=j+1; i <= nrow; ip += i++) temp2 += *ip * *(ip+1);
	      ip = m.begin() + (j+1)*j/2 - 2;
	      for (i=j+1; i <= nrow; ++i) {
	          ip += i - 1;
	          temp2 += *ip * *(ip+1);
	      }
	      *(mjj-1) -= temp2;
	      //ip = m.begin() + (j+1)*j/2 + j-2;
	      //for (i=0; i < nrow-j; ip += 1+j+i++) x[i] = *ip;
	      ip = m.begin() + (j+1)*j/2 - 2;
	      for (i=0; i < nrow-j; ++i) {
	          ip += i + j;
	          x[i] = *ip;
	      }
	      for (i=j+1; i <= nrow ; i++) {
		  temp2 = 0;
		  ip = m.begin() + i*(i-1)/2 + j;
		  for (k=0; k <= i-j-1; k++)
		      temp2 += *ip++ * x[k];
		  for (ip += i-1; k < nrow-j; ip += 1+j+k++)
		      temp2 += *ip * x[k];
		  *(m.begin()+ i*(i-1)/2 + j-2)= -temp2;
	      }	// for i
	      temp2 = 0;
	      //ip = m.begin() + (j+1)*j/2 + j-2;
	      //for (k=0; k < nrow-j; ip += 1+j+k++)
		//  temp2 += x[k] * *ip;
	      ip = m.begin() + (j+1)*j/2 - 2;
	      for (k=0; k < nrow-j; ++k) {
	          ip += k + j;
		  temp2 += x[k] * *ip;
	      }
	      *(mjj-j) -= temp2;
	  }	// j < nrow
      }	// else  piv[j-1] > 0

      // interchange rows and columns j and piv[j-1] 
      // or rows and columns j and -piv[j-2]

      pivrow = (piv[j-1]==0)? -piv[j-2] : piv[j-1];
      ip = m.begin() + pivrow*(pivrow-1)/2 + j;
      for (i=j+1;i < pivrow; i++, ip++) {
	  temp1 = *(m.begin() + i*(i-1)/2 + j-1);
	  *(m.begin() + i*(i-1)/2 + j-1) = *ip;
	  *ip = temp1;
      }	// for i
      temp1 = *mjj;
      *mjj = *(m.begin() + pivrow*(pivrow-1)/2 + pivrow-1);
      *(m.begin() + pivrow*(pivrow-1)/2 + pivrow-1) = temp1;
      if (s==2) {
	  temp1 = *(mjj-1);
	  *(mjj-1) = *( m.begin() + pivrow*(pivrow-1)/2 + j-2);
	  *( m.begin() + pivrow*(pivrow-1)/2 + j-2) = temp1;
      }	// s==2

      // problem right here
      if( pivrow < nrow ) {
	  ip = m.begin() + (pivrow+1)*pivrow/2 + j-1;  // &A(i,j)
	  // adding parenthesis for VC++
	  iq = ip + (pivrow-j);
	  for (i = pivrow+1; i <= nrow; i++) {
	      temp1 = *iq;
	      *iq = *ip;
	      *ip = temp1;
	      if( i < nrow ) {
	      ip += i;
	      iq += i;
	      }
	  }	// for i 
      }	// pivrow < nrow
  } // end of loop over columns (in computing inverse from factorization)

  return; // inversion successful

}

}  // namespace CLHEP
