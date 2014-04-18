/*
 *  polynomial.h
 *  rationalpolynomial
 *
 *  Created by Joe Dumoulin on 2/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include <cmath>
#include <stdexcept>
#include <vector>

#include "integer.h"

namespace num
{
	template<typename T>
	// Requires Ordered Field(T)
	class polynomial
	{
	public:
		typedef T value_type;
	private:
		std::vector<value_type> _coefficients;
		
		static void printCoefficient(std::ostream& out, size_t exp, value_type value)
		{
			if (value == value_type(1))
			{
				if (exp == 0)
					out << value;
				else if (exp == 1)
					out << "x";
				else // exp > 1
					out << "x^" << exp;
			}
			else if (value != value_type(1))
			{
				if (exp == 0)
					out << value;
				else if (exp == 1)
					out << value << "x";
				else // exp > 1
					out << value << "x^" << exp;
			}
		}
		
		void normalize()
		{
			// adjust the coefficient array so that highest order is nonzero
			//	This should happen after any arithmatic operation.
			typename std::vector<value_type>::reverse_iterator p = _coefficients.rbegin();
			// TODO: replace with:
			// while (p != _coefficients.rend() && *p == T(0)) ++p;
			while (p != _coefficients.rend())
			{
				if (*p != T(0))
					break;
				// else
				++p;
			}
			if (p != _coefficients.rbegin())
			{
				_coefficients.resize(_coefficients.size() 
									 - std::distance(_coefficients.rbegin(), p));
			}
		}
		
	public:
		// ctor/dtor/assignment
		polynomial(value_type* pV = 0, size_t size = 0)
			: _coefficients(pV, pV + size)
		{ normalize(); }
		polynomial(const std::vector<value_type>& v) 
		: _coefficients(v.begin(), v.end())
		{ normalize(); }
		polynomial(const polynomial& p) 
		: _coefficients(p._coefficients.begin(), p._coefficients.end())
		{} // already normalized
		virtual ~polynomial(){}
		
		polynomial& operator=(const polynomial& p)
		{
			if (p != *this) 
				_coefficients.assign(p._coefficients.begin(), p._coefficients.end());
			normalize();
			return *this;
		}
		
		// overloads
		// unary negate
		const polynomial operator-() const
		{
			polynomial p = *this;
			for (size_t s = 0; s < p._coefficients.size(); ++s)
			{
				p._coefficients[s] = -_coefficients[s];
			}
			return p;
		}
		
		polynomial& operator+=(const polynomial& p)
		{
			if (_coefficients.size() < p._coefficients.size())
			{
				_coefficients.resize(p._coefficients.size(), value_type(0));
			}
			for (size_t s = 0; s < p._coefficients.size(); ++s)
			{
				_coefficients[s] += p._coefficients[s];
			}
			// now trim if any top elements are zero;
			size_t t = _coefficients.size();
			while (t > 0 && _coefficients[t-1] == value_type(0)) --t;
			
			_coefficients.erase(_coefficients.begin() + t, _coefficients.end());
			normalize();
			return *this;
		}
		
		polynomial& operator-=(const polynomial& p)
		{
			polynomial t = p;
			*this += -t;
			return *this;
		}
		// value_type operator 
		polynomial& operator*=(const value_type& v)
		{
			if (v == value_type(0))
			{
				_coefficients.clear();
				return *this;
			}
			// multiply each element of this by v
			for (size_t s = 0; s < _coefficients.size(); ++s)
			{
				_coefficients[s] *= v;
			}
			normalize();
			return *this;
		}
		
		polynomial& operator*=(const polynomial p)
		{
			// if one element is zero then this is zero
			if (_coefficients.empty() || p._coefficients.empty())
			{
				_coefficients.clear();
				normalize();
				return *this;
			}
			
			// TODO:  there are some optimisations to the code below.
			// - assign is unneeded
			// - erase is redundant with normalise()
			// make a copy of this
			const polynomial r = *this;
			// resize this to support the result
			_coefficients.resize(_coefficients.size() + p._coefficients.size() - 1);
			_coefficients.assign(_coefficients.size(), value_type(0));
			for (size_t s = 0; s < p._coefficients.size(); ++s)
			{
				polynomial t = r;	// get a copy of old this
				t *= p._coefficients[s];
				for (size_t ss = 0; ss < t._coefficients.size(); ++ss)
					_coefficients[ss+s] += t._coefficients[ss];
			}
			// now trim if any top elements are zero;
			size_t t = _coefficients.size();
			while (t > 0 && _coefficients[t-1] == value_type(0)) --t;
			
			_coefficients.erase(_coefficients.begin() + t, _coefficients.end());
			normalize();
			return *this;
		}
		
		polynomial& operator/=(const value_type& v)
		{
			if (v == value_type(0))
			{
				throw std::runtime_error("polynomial::operator /= : attempted division by zero.");
			}
					
			for (size_t s = 0; s < _coefficients.size(); ++s)
			{
				_coefficients[s] /= v;
			}
			return *this;
		}
		
		polynomial& operator/=(polynomial p)
		{
			polynomial residue;
			reduce(p, *this, residue);
			return *this;
		}
		
		polynomial& operator%=(polynomial p)
		{
			polynomial quotient;
			reduce(p, quotient, *this);
			return *this;
		}
		
		value_type operator()(value_type v)
		{
			return evaluate(v);
		}
		
		// helpers
		void reduce(const polynomial& denom, polynomial& quotient, polynomial& residue) const
		{
			// check for division by zero
			if (denom._coefficients.size() == 0)
				throw std::runtime_error("polynomial reduce: attempt to divide by zero.");
	
			polynomial work = *this;
			quotient *= T(0); residue *= T(0);
			while (work._coefficients.size() >= denom._coefficients.size())
			{
				//find the exponent and the coefficient of the poly to multiply to the denom.
				size_t workSize = work._coefficients.size();
				size_t denomSize = denom._coefficients.size();
				size_t exp = workSize - denomSize;
				value_type coeff = work._coefficients[workSize-1]
										/denom._coefficients[denomSize-1];
				// now we know the values.  make the poly.
				std::vector<value_type> multiplier(exp+1, value_type(0));
				multiplier[exp] = coeff;
				
				// multiply this by the denom and the subtract from the working poly.
				work -= (denom * multiplier);
				// add the multiplier to the quotient.
				quotient += multiplier;
			}
			residue = work;
		}
		
		size_t order() const 
		{	// Wrong!  only if no zeros in the top of the coeffs.  FIX.
			int ret =  _coefficients.size() - 1;
			return (ret >= 0) ? ret : 0;
		}
		
		value_type evaluate(const value_type& a)
		{
			T r = T(0);
			for (int i = 0; i < _coefficients.size(); ++i)
			{
				r += _coefficients[i] * power(a, i, num::mult<value_type>());
			}
			return r;
		}
		
		polynomial derivative()
		{
			if (order() < 1) return polynomial();
			// get the coefficients from this polyniomial
			std::vector<value_type> coefficients(_coefficients.begin()+1, _coefficients.end());
			for (size_t s = 0; s != coefficients.size(); ++s)
			{
				coefficients[s] *= value_type(s+1);
			}
			return polynomial(coefficients);
		}
		
		polynomial integral(const value_type& constant = 0)
		{
			std::vector<value_type> coefficients(order() + 2, value_type(0));
						
			for (size_t s = 1; s < coefficients.size(); ++s)
			{
				coefficients[s] = _coefficients[s-1]/value_type(s);
			}
			
			coefficients[0] = constant;
			return polynomial(coefficients);
		}
		
		value_type definite_integral(value_type low, value_type high)
		{
			polynomial p = integral();
			return p(high) - p(low);
		}
		
		// friends
		friend const polynomial operator+(polynomial left, const polynomial& right)
		{
			return left += right;
		}
		
		friend const polynomial operator-(polynomial left, const polynomial& right)
		{
			return left -= right;
		}
		
		friend const polynomial operator*(polynomial left, const polynomial& right)
		{
			return left *= right;
		}
		
		friend const polynomial operator*(polynomial left, const polynomial::value_type& right)
		{
			return left *= right;
		}
		
		friend const polynomial operator*(const polynomial::value_type& left, polynomial right)
		{
			return right *= left;
		}
		
		friend const polynomial operator/(polynomial left, const polynomial& right)
		{
			return left /= right;
		}
		
		friend const polynomial operator/(polynomial left, const polynomial::value_type& right)
		{
			return left /= right;
		}
		
		friend const polynomial operator/(const polynomial::value_type& left, polynomial right)
		{
			value_type o[] = {T(1)};
			polynomial one(o);
			return one /= (right /= left);
		}
		
		friend const polynomial operator%(polynomial left, const polynomial& right)
		{
			return left %= right;
		}
		
		
		// predicates
		friend bool operator==(const polynomial& left, const polynomial& right)
		{
			if (left._coefficients.size() != right._coefficients.size())
				return false;
			for (size_t s = 0; s < left._coefficients.size(); ++s)
			{
				if (left._coefficients[s] != right._coefficients[s])
					return false;
			}
			return true;
		}
		
		friend bool operator!=(const polynomial& left, const polynomial& right)
		{
			return !(left == right);
		}
		
		// helpers
		friend std::ostream& operator<<(std::ostream& out, const polynomial& p)
		{
			if (p._coefficients.empty())
				out << 0;
			else 
			{	// do the first element
				int exp = p._coefficients.size() - 1;
				
				// the first entry.
				bool negative = (p._coefficients[exp] < 0);
				value_type value = (negative ? 
									-p._coefficients[exp] : p._coefficients[exp]);
				// print conditions apply
				if (negative)
					out << "-";
				polynomial::printCoefficient(out, exp, value);
				--exp;
				// the remaining entries
				while (exp >= 0)
				{
					bool negative = (p._coefficients[exp] < 0);
					value_type value = (negative ? -p._coefficients[exp] : p._coefficients[exp]);
					// print conditions apply
					if (value != 0)
					{
						if (negative)
							out << " - ";
						else 
							out << " + ";
						polynomial::printCoefficient(out, exp, value);
					}
					--exp;
				}
			}
			return out;
		}
	};
};
