/*
 *  quaternion.h
 *  quaternion
 *
 *  Created by Joe Dumoulin on 3/7/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef QUATERNION_H
#define QUATERNION_H

#include <ostream>
#include <stdexcept>
#include <vector>

namespace num
{
	template<class T>
	class quaternion
	{
	public:
		typedef typename T value_type;
	private:
		value_type _coefficient[4]; // a, b, c, d;
		static void printCoefficient(std::ostream& out, size_t index, value_type value)
		{
			if (index > 0 && value != value_type(1))
			{
				out << value;
			}
			switch(index)
			{
				case 0:
					out << value;
					break;
				case 1:
					out << "i";
					break;
				case 2:
					out << "j";
					break;
				case 3:
					out << "k";
					break;
			}
		}
	public:
		quaternion(const value_type& a = 0, const value_type& b = 0
				   , const value_type& c = 0, const value_type& d = 0)
		{
		_coefficient[0] = a;
		_coefficient[1] = b;
		_coefficient[2] = c;
		_coefficient[3] = d;
	}
		quaternion(const quaternion& q)
		{
		_coefficient[0] = q._coefficient[0];
		_coefficient[1] = q._coefficient[1];
		_coefficient[2] = q._coefficient[2];
		_coefficient[3] = q._coefficient[3];
	}
		template<typename Iter>
		quaternion(Iter begin, Iter end) : _coefficient(begin, end) {}
		
		virtual ~quaternion(){}
	
		quaternion& operator=(const quaternion& q)
		{
		_coefficient[0] = q._coefficient[0];
		_coefficient[1] = q._coefficient[1];
		_coefficient[2] = q._coefficient[2];
		_coefficient[3] = q._coefficient[3];
		return *this;
	}
	
		quaternion& operator+=(const quaternion& q)
		{
			_coefficient[0] += q._coefficient[0];
			_coefficient[1] += q._coefficient[1];
			_coefficient[2] += q._coefficient[2];
			_coefficient[3] += q._coefficient[3];
			return *this;
		}
	
		const quaternion operator-() const
		{
			quaternion q;
			q._coefficient[0] = -_coefficient[0];
			q._coefficient[1] = -_coefficient[1];
			q._coefficient[2] = -_coefficient[2];
			q._coefficient[3] = -_coefficient[3];
			return q;
		}
	
		quaternion& operator-=(quaternion q)
		{
			return *this += -q;
		}
	
		quaternion& operator*=(const quaternion& q)
		{
			const quaternion p = *this;
			_coefficient[0] 
			= p._coefficient[0]*q._coefficient[0]
			- p._coefficient[1]*q._coefficient[1] 
			- p._coefficient[2]*q._coefficient[2]
			- p._coefficient[3]*q._coefficient[3];
			_coefficient[1]
			= p._coefficient[0]*q._coefficient[1]
			+ p._coefficient[1]*q._coefficient[0] 
			+ p._coefficient[2]*q._coefficient[3]
			- p._coefficient[3]*q._coefficient[2];
			_coefficient[2]
			= p._coefficient[0]*q._coefficient[2]
			+ p._coefficient[2]*q._coefficient[0] 
			+ p._coefficient[3]*q._coefficient[1]
			- p._coefficient[1]*q._coefficient[3];
			_coefficient[3]
			= p._coefficient[0]*q._coefficient[3]
			+ p._coefficient[3]*q._coefficient[0] 
			+ p._coefficient[1]*q._coefficient[2]
			- p._coefficient[2]*q._coefficient[1];
			
			return *this;
		}
		
		quaternion& operator*=(const value_type& v)
		{
			_coefficient[0] *= v;
			_coefficient[1] *= v;
			_coefficient[2] *= v;
			_coefficient[3] *= v;
			return *this;
		}
		
		quaternion& operator/=(const quaternion& q)
		{
			quaternion denom(q.conjugate());
			value_type v = (q * denom)._coefficient[0];
			if (v == value_type(0))
				throw std::runtime_error("division by zero.");
			*this *= denom * (value_type(1)/v);
			return *this;
		}
		
		quaternion& operator/=(const value_type& v)
		{
			if (v == value_type(0))
				throw std::runtime_error("division by zero.");
			*this *= (value_type(1)/v);
			return *this;
		}
		
		//helpers
		const quaternion conjugate() const
		{
			quaternion p;
			p._coefficient[0] = _coefficient[0];
			p._coefficient[1] = -_coefficient[1];
			p._coefficient[2] = -_coefficient[2];
			p._coefficient[3] = -_coefficient[3];
			return p;
		}
		
		value_type normSquared() const
		{ 
			quaternion q(*this);
			q *= q.conjugate();
			return q.real();
		}
		
		value_type real() const
		{ return _coefficient[0]; }
		
		// get the vector portion of the quaternion
		std::vector<quaternion::value_type> imag() const 
		{ return std::vector<quaternion::value_type>(_coefficient + 1, _coefficient + 4); }

		
		// friends
		friend const quaternion operator+(quaternion l, const quaternion& r)
		{ return l += r; }
		
		friend const quaternion operator-(quaternion l, const quaternion& r)
		{ return l -= r; }
		
		friend const quaternion operator*(quaternion l, const quaternion& r)
		{ return l *= r; }
		
		friend const quaternion operator*(quaternion l, const value_type& r)
		{ return l *= r; }
		
		friend const quaternion operator*(const value_type& l, quaternion r)
		{ return r *= l; }
		
		friend const quaternion operator/(quaternion l, const quaternion& r)
		{ return l /= r; }
		
		friend const quaternion operator/(quaternion l, const value_type& r)
		{ return l /= r; }
		
		friend const quaternion operator/(const value_type& l, quaternion r)
		{ return r /= l; }
		
		friend bool operator==(const quaternion& l, const quaternion& r)
		{
			return l._coefficient[0] == r._coefficient[0]
				&& l._coefficient[1] == r._coefficient[1]
				&& l._coefficient[2] == r._coefficient[2]
				&& l._coefficient[3] == r._coefficient[3];
		}
		
		friend bool operator!=(const quaternion& l, const quaternion& r)
		{
			return !(l == r);
		}
		
		friend bool operator>(const quaternion& l, const quaternion& r)
		{
			return (l * l.conjugate() > r * r.conjugate());
		}
		
		friend bool operator<(const quaternion& l, const quaternion& r)
		{
			return (l * l.conjugate() > r * r.conjugate());
		}
		
		friend bool operator<=(const quaternion& l, const quaternion& r)
		{
			return !(l > r);
		}
		
		friend bool operator>=(const quaternion& l, const quaternion& r)
		{
			return !(l < r);
		}
		
		friend std::ostream& operator<<(std::ostream& out, const quaternion& p)
		{
			// the first entry.
			int entry = 0; // array index
			bool negative = (p._coefficient[entry] < 0);
			quaternion::value_type value = (negative ? 
											-p._coefficient[entry] : p._coefficient[entry]);
			// print conditions apply
			if (negative)
				out << "-";
			quaternion::printCoefficient(out, entry, value);
			++entry;
			// the remaining entries
			while (entry < 4)
			{
				bool negative = (p._coefficient[entry] < 0);
				value_type value = (negative ? -p._coefficient[entry] : p._coefficient[entry]);
				// print conditions apply
				if (value != 0)
				{
					if (negative)
						out << " - ";
					else 
						out << " + ";
					quaternion::printCoefficient(out, entry, value);
				}
				++entry;
			}
			
			return out;
		}
	};
};

#endif // QUATERNION_H