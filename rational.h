/*
 *  rational.h
 *  rational
 *
 *  Created by Joe  Dumoulin on 1/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef RATIONAL_H
#define RATIONAL_H

#include <stdexcept>
#include <ostream>

#include "gcd.h"

namespace num
{
	template<class T>
	class rational
	{
	public:
		typedef T value_type;
	private:	
		value_type _num;
		value_type _denom;
		
		void normalize_negative()
		{ // positive denom always
			if (_denom < 0) { _num = -_num; _denom = -_denom; }
		}
		void reduce()
		{
			value_type factor = num::gcd(_num, _denom); 
			_num /= factor; _denom /= factor;
		}
	public:
		// ctor/dtor/assignment
		rational(const value_type& num = 0, const value_type& denom = 1) : _num(num), _denom(denom)
		{ 
			if (_denom == 0) throw std::runtime_error("rational ctor: zero denominator error."); 
			reduce();
			normalize_negative();
		}
		virtual ~rational(){}
		
		rational(const rational& r) : _num(r._num), _denom(r._denom)
		{ 
			reduce();
			normalize_negative();
		}

		rational& operator=(const rational& r)
		{
			if (this != &r)
			{
				_num = r._num; _denom = r._denom;
			}
			normalize_negative();
			return *this;
		}
		
		// accessors
		const value_type& numerator() { return _num; }
		const value_type& denominator() { return _denom; }
		
		// overloads
		const rational operator-() const
		{
			return rational(-_num, _denom);
		}
		
		rational& operator+=(const rational& r)
		{
			_num = _num*r._denom + r._num*_denom;
			_denom = _denom*r._denom;
			reduce();
			normalize_negative();
			return *this;
		}
		
		rational& operator-=(const rational& r)
		{
			rational t = r;
			*this += -t;
			return *this;
		}
		
		rational& operator*=(const rational& r)
		{
			_num *= r._num;
			_denom *= r._denom;
			reduce();
			normalize_negative();
			return *this;
		}
		
		rational& operator/=(const rational& r)
		{
			_num *= r._denom;
			_denom *= r._num;
			if (_denom == 0) throw std::runtime_error("rational operator/=(): zero denominator error.");			
			reduce();
			normalize_negative();
			return *this;
		}
		
		rational& operator%=(const rational& r)
		{	// division is always complete in the rationals.  no remainder.
			_num = T(0);
			_denom = T(1);
			return *this;
		}
		
		// helpers
		rational& invert()
		{
			if (_num == 0) throw std::runtime_error("rational invert(): zero denominator error.");			
			value_type swap = _num;
			_num = _denom;
			_denom = swap;
			normalize_negative();
			return *this;
		}
		
		// friends
		friend const rational operator+(rational l, const rational& r)
		{
			return l+=r;
		}
		
		friend const rational operator-(rational l, const rational& r)
		{
			return l-=r;
		}
		
		friend const rational operator*(rational l, const rational& r)
		{
			return l *= r;
		}
		
		friend const rational operator/(rational l, const rational& r)
		{
			return l /= r;
		}
		
		friend const rational operator%(rational l, const rational& r)
		{
			return l %= r;
		}
		
		friend std::ostream& operator<<(std::ostream& os, const rational& r)
		{ 
			os << "(" << r._num << ", " << r._denom << ")"; 
			return os;
		}
		
		friend bool operator==(const rational& l, const rational& r)
		{
			return (l._num == r._num && l._denom == r._denom);
		}
		
		friend bool operator!=(const rational& l, const rational& r)
		{
			return !(l == r);
		}
		
		friend bool operator<(const rational& l, const rational& r)
		{
			rational x = l-r;
			return (x._num < 0);
		}

		friend bool operator>(const rational& l, const rational& r)
		{
			rational x = l-r;
			return (x._num > 0);
		}

		friend bool operator<=(const rational& l, const rational& r)
		{
			return (l < r || l == r);
		}

		friend bool operator>=(const rational& l, const rational& r)
		{
			return (l > r || l == r);
		}

		friend T floor(const rational& r)
		{
			return r._num/r._denom;
		}
		
		friend T ceil(const rational& r)
		{
			return floor(r) + T(1);
		}
	};

}
#endif //RATIONAL_H

