/*
 *  FiniteIntegralType.h
 *  integralDomain
 *
 *  Created by Joe Dumoulin on 10/8/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FINITE_INTEGRAL_TYPE
#define FINITE_INTEGRAL_TYPE

#include <vector>
#include <set>
#include <stdexcept>

#include "num.h"

namespace num
{
	template<typename T, T BASE>
	//requires Integer(T) && Positive(BASE)
	class finite_integral_type
	{
	public:
		typedef T value_type;
		typedef finite_integral_type self_type;
	private:
		value_type _value;
		
		const T mod(T s)
		{
			return s % BASE;
		}
		
	public:
		finite_integral_type(T i = T(0)) { _value = mod(i); } //normalize(); }
		finite_integral_type(const finite_integral_type& i) { _value = i._value; }
		virtual ~finite_integral_type(){}
		
		finite_integral_type& operator=(const finite_integral_type& i) 
		{ 
			_value = i._value; 
			return *this;
		}
		
		// return a multiplicative inverse (if any) of *this
		const finite_integral_type invert() const
		{
			for (self_type a(T(1)); a.value() != T(0); ++a)
			{
				if ((a * (*this)) == self_type(T(1)))
				{
					return a;
				}
			}
			throw std::runtime_error("num::finite_integral_type::invert() - no inverse exists.");
		}
		
		// overloads
		const finite_integral_type operator-() const // unary
		{
			finite_integral_type a(*this);
			a._value = -a._value;
			return a.normalize();
		}
		
		finite_integral_type& operator+=(const finite_integral_type& a)
		{
			_value = mod(_value + a._value);
			return *this;
		}
		
		finite_integral_type& operator-=(const finite_integral_type& a)
		{
			*this += -a;
			return *this;
		}
		
		finite_integral_type& operator*=(const finite_integral_type& a)
		{
			_value = mod(_value * a._value);
			return *this;
		}
		
		finite_integral_type& operator/=(const finite_integral_type& a)
		{
			if (a._value == T(0))
				throw std::runtime_error("num::finite_integral_type::operator/= - Division by zero error.");
			self_type b(a);
			_value = mod(_value *= b.invert().value());
			return *this;
		}
		
		// this operation is not homomorphic.  a little tricky to test
		finite_integral_type& operator%=(const finite_integral_type& a)
		{
			if (a._value == T(0))
				throw std::runtime_error("num::finite_integral_type::operator%= - Division by zero error.");
			_value %= a.value();
			return *this;
		}
		
		finite_integral_type& operator++()
		{
			*this += T(1);
			return *this;
			
		}
		finite_integral_type operator++(int)
		{
			finite_integral_type t = *this;
			operator++();
			return t; 
		}
		
		//helpers
		const T& value() const { return _value; }
		 
		static const T base() { return BASE; }
		
		// normalize changes positive to negative or negative to positive
		//	when the number is higher(lower) than 1/2 base
		const finite_integral_type& normalize() 
		{ 
			if (_value == T(0)) return *this;
			if (_value < T(0))
			{
				if (_value < -BASE/T(2))
				{	// make _value positive
					_value = _value + BASE;
				}
			}
			else 
			{
				if (_value > BASE/T(2))
				{	// make _value negative
					_value = _value - BASE;
				}
			}
			return *this;
		}
		
		// friends
		friend const finite_integral_type operator+(finite_integral_type left, const finite_integral_type& right)
		{
			return left += right;
		}
		
		friend const finite_integral_type operator-(finite_integral_type left, const finite_integral_type& right)
		{
			return left -= right;
		}
		
		friend const finite_integral_type operator*(finite_integral_type left, const finite_integral_type& right)
		{
			return left *= right;
		}
		
		friend const finite_integral_type operator/(finite_integral_type left, const finite_integral_type& right)
		{
			return left /= right;
		}
		
		// this operation is not homomorphic.  a little tricky to test
		friend const finite_integral_type operator%(finite_integral_type left, const finite_integral_type& right)
		{
			return left %= right;
		}

		friend bool operator<(const finite_integral_type& left, const finite_integral_type& right)
		{ 
			self_type l(left);
			self_type r(right);
			return (l.normalize()).value() < (r.normalize()).value(); 
		}
		
		friend bool operator==(const finite_integral_type& left, const finite_integral_type& right)
		{
			self_type l(left);
			self_type r(right);
			return (l.normalize()).value() == (r.normalize()).value(); 
		}
		
		friend bool operator!=(const finite_integral_type& left, const finite_integral_type& right)
		{ return !(left == right); }
		
		friend bool operator>(const finite_integral_type& left, const finite_integral_type& right)
		{ return !(left < right) && !(left == right); }
		
		friend bool operator<=(const finite_integral_type& left, const finite_integral_type& right)
		{ return left < right || left == right; }
		
		friend bool operator>=(const finite_integral_type& left, const finite_integral_type& right)
		{ return !(left < right); }
		
		friend std::ostream& operator<<(std::ostream& out, const finite_integral_type& p)
		{
			out << p._value << "(mod " << BASE << ")";
			return out;
		}
	};
	
	// useful for associative containers.
	template<typename FIT>
	class less
	{
	public:
		bool operator()(const FIT& left, const FIT& right) 
		{
			return left < right;
		}
	};
	
	// return a cyle of unique values generated by an element of a 
	//	finite set of integers.
	template<typename FIT>
	std::vector<FIT> find_cycle_for_value(const typename FIT::value_type& value)
	{
		typedef typename FIT::value_type value_type;
		FIT f(value);
		FIT next = f;
		
		std::vector<FIT> testlist;
		
		for (value_type i = value_type(1); i < f.base(); ++i)
		{
			testlist.push_back(next);
			if (next == FIT(0))
			{	// divisor of zero.  we are done.
				break;
			}
			next *= f;
			if (next == *(testlist.begin()))
				break;  // completed a cycle
			if (next == value_type(1))
			{
				if (*testlist.begin() != value_type(1))
				{
					testlist.push_back(next);
				}
				break; 
			}
		}
		std::cout << "value = " << value << "=> ";
		print_container_as_set(std::cout, testlist) << std::endl;
		
		return testlist;
	}
	
	template<typename T, typename Container>
	void list_relatively_prime(const T& value, std::back_insert_iterator<Container> out)
	{
		for (T t = T(1); t < value; ++t)
		{
			if (gcd(t, value) == T(1))
			{
				out = t;
				++out;
			}
		}
	}
	
	
	template<typename FIT>
	std::set<FIT, less<FIT> > find_multiplicative_cycles()
	{
		typedef typename FIT::value_type value_type;
		
		std::set<FIT, less<FIT> > generators;
		// for each value less than BASE, try to catch every 
		//	number that generates the multiplicative group.
		//	return an array of numbers that generate the Base.
		for (value_type t = value_type(1); t < FIT::base(); ++t)
		{
			std::vector<FIT> testset = find_cycle_for_value<FIT>(t);
			// this cycle generates the finite integer set if all 
			//	elements of the set are in one cycle AND the last element
			//	of the set is 1.
			if (testset.size() == FIT::base() - value_type(1)
				&& *(testset.begin()+(testset.size()-1)) == FIT(1))
			{ 
				generators.insert(t);
				
				// once we've found one generator by brute force,
				// we can use the fact that every other generator can be found 
				// from this by checking for relative primality of the power 
				//	of each mult of the known generator.
				std::vector<value_type> indexes; 
				list_relatively_prime(FIT::base() - value_type(1), std::back_inserter(indexes));
				
				//std::cout << " indexes = ";
				//print_container_as_set(std::cout, indexes) << std::endl;
				for (size_t s = 0; s < indexes.size(); ++s)
				{
					generators.insert(testset[indexes[s]-1]);
				}
				break;
			}
		}
		return generators;
	}

	template<typename FIT>
	std::ostream& print_generators(std::ostream& out)
	{
		typedef typename FIT::value_type value_type;
		
		out << "print generators for " << FIT::base() << std::endl;
		std::set<FIT, less<FIT> > gen_set = find_multiplicative_cycles<FIT>();
		
		return print_container_as_set(out, gen_set);
	}
	
};

#endif // FINITE_INTEGRAL_TYPE
