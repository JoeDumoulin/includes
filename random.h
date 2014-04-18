/*
 *  random.h
 *  
 *
 *  Created by Joe Dumoulin on 10/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RANDOM_H
#define RANDOM_H


#include <iostream>
#include <vector>
#include <iterator>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <time.h>

#include "num.h"

namespace rnd
{
	// Generate random numbers with uniform distribution
	// TODO: generalize this class.
	template<typename Value_T>
	struct Uniform
	{
		Uniform(Value_T l, Value_T h, float seed=-1.0) : m_low(l), m_high(h), m_seed(seed) 
		{
			if (m_seed < 0.0)
				srand(time(NULL));
			else
				srand(m_seed);
		}
		
		Value_T operator()() 
		{
			double scaled_rand = (double) rand();
			scaled_rand /= RAND_MAX;
			return m_low + (Value_T) (scaled_rand * (m_high - m_low));
		}
	private:
		Value_T m_low;
		Value_T m_high;
		float m_seed;
	};
	
	// Generate numbers with Exponential distribution using inverse transform method
	template<typename Value_T>
	struct Exponential
	{
		Exponential(double lambda, float seed=-1.0) : m_lambda(lambda)
		, U(0.0, 1.0, seed)
		{}
		Value_T operator()() 
		{
			return -log(1-U())/m_lambda;
		}
	private:
		Uniform<Value_T> U;
		double m_lambda;
	};
	
	// Generate values with Normal distribution aprox. using Box-Muller method.
	//	This method returns a pair of pseudo random values that are normally 
	//	distributed with 0 mean and var = 0.
	template<typename Value_T>
	struct Box_Muller
	{
		typedef std::pair<Value_T, Value_T> NormalPair;
		
		Box_Muller(float seed=-1.0) :  U(0.0, 1.0, seed)
		{}
		NormalPair operator()()
		{
			Value_T U1 = U(); Value_T U2 = U();
			return NormalPair(
							  num::Sqrt(-2*log(U1))*cos(2*num::PI*U2),
							  num::Sqrt(-2*log(U1))*sin(2*num::PI*U2)
							  );
		}
	private:
		Uniform<Value_T> U;
	};
	
	// Generate values with Normal distribution aprox. using Polar Marsaglia method
	//	This method returns a pair of pseudo random values that are normally 
	//	distributed with 0 mean and var = 0.
	template<typename Value_T>
	struct Polar_Marsaglia
	{
		Polar_Marsaglia(float seed=-1.0) : U(0.0, 1.0, seed) {}
		std::pair<Value_T, Value_T> operator()()
		{
			double V1 = 1.0; double V2 = 1.0;
			double W;
			while ((W = (V1*V1+V2*V2)) > 1.0)
			{
				V1 = 2.0*U()-1; 
				V2 = 2.0*U()-1;
			}
			return std::pair<Value_T, Value_T>(
											   V1*num::Sqrt(-2*log(W)/W),
											   V2*num::Sqrt(-2*log(W)/W)
											   );
		}
	private:
		Uniform<Value_T> U;
		
	};
	
	// these are helpers.  not really needed.  They take the first entry in the pair and
	//	 return that one random value only.
	template<typename Value_T> 
	struct NormalBM
	{
		NormalBM(float seed = -1) : normal(seed) {}
		Value_T operator()()
		{
			std::pair<Value_T, Value_T> n = normal();
			return n.first;
		}
	private:
		Box_Muller<Value_T> normal;
	};
	
	template<typename Value_T> 
	struct NormalPM
	{
		NormalPM(Value_T m = 0, Value_T s = 1, float seed = -1) 
		: m_mean(m), m_stdev(s), m_normal(seed) {}
		Value_T operator()()
		{
			std::pair<Value_T, Value_T> n = m_normal();
			return n.first*m_stdev + m_mean;
		}
	private:
		Polar_Marsaglia<Value_T> m_normal;
		Value_T m_mean;
		Value_T m_stdev;
	};
	
	// Generate random values for a two-valued random variable.  The
	// constructor takes a value between 0 and 1 and the functor returns random
	//	values of 0 or one based on whenther the random number is < or greater 
	//	than the value passed in.
	template<typename Value_T>
	struct TwoPointRandomVariable
	{
		TwoPointRandomVariable(double p): point(p), random(0.0, 1.0){}
		Value_T operator()()
		{
			return (Value_T) (random() < point ? 0 : 1);
		}
	private:
		double point;
		Uniform<double> random;
	};
	
	// Generate random values for an N-valued random variable.  Initialize with a 
	//	vector of n-1 ordered probabilities (doubles between 0 and 1) and the functor 
	//	will return the numeric value of the bucket (0 to n) which gets the random 
	//	number.
	template<typename Value_T>
	struct NPointRandomVariable
	{
		NPointRandomVariable(std::vector<Value_T> p) : point(p), random(0.0, 1.0){}
		Value_T operator()()
		{
			Value_T r = random();
			int count = 0;
			while(count < point.size() && r < point[count])
			{
				count++;
			}
			return (Value_T) (count == 0 ? count : count-1);
		}
	private:
		std::vector<double> point;
		Uniform<double> random;
	};
	
	template<typename Value_T, class Strategy >
	struct DataSetBase
	{
	public:
		typedef Value_T value_t;
		typedef typename std::vector<value_t>::iterator data_iterator_t;
		
		DataSetBase(int N, Strategy& cdf) : m_data(N, 0), m_distribution(cdf) { generate(); }
		void generate()
		{
			m_mean = 0; m_variance = 0;
			data_iterator_t u = m_data.begin();
			for (; u != m_data.end(); ++u)
			{
				*u = m_distribution();
				m_mean += *u;
			}
			m_mean /= m_data.size();
			
			u = m_data.begin();
			for (; u != m_data.end(); ++u)
			{
				m_variance += (*u - m_mean)*(*u - m_mean);
			}
			m_variance /= (m_data.size() - 1);
		}
		
		void print_data()
		{
			std::copy(m_data.begin(), m_data.end(), std::ostream_iterator<value_t>(std::cout, ", "));
			std::cout << "\n";
		}
		
		data_iterator_t begin()
		{		return m_data.begin(); }
		
		data_iterator_t end()
		{		return m_data.end(); }
		
		const double mean() 
		{		return m_mean; }
		
		const double variance()
		{		return m_variance; }
		
	protected:
		std::vector<value_t> m_data;
		Strategy m_distribution;
		double m_mean;
		double m_variance;
		
	};
	
	// helpers
	template<typename Value_T>
	struct DataSetUniform : public DataSetBase<Value_T, Uniform<Value_T> >
	{
		typedef DataSetBase<Value_T, Uniform<Value_T> > super_t;
		DataSetUniform(int N, Uniform<Value_T> cdf) : super_t(N, cdf) {}
	};
	
	template<typename Value_T>
	struct DataSetTwoPoint : public DataSetBase<Value_T, TwoPointRandomVariable<Value_T> >
	{
		typedef DataSetBase<Value_T, TwoPointRandomVariable<Value_T> > super_t;
		DataSetTwoPoint(int N, TwoPointRandomVariable<Value_T> cdf) : super_t(N, cdf) {}
	};
	
	template<typename Value_T>
	struct DataSetExponential : public DataSetBase<Value_T, Exponential<Value_T> >
	{
		typedef DataSetBase<Value_T, Exponential<Value_T> > super_t;
		DataSetExponential(int N, DataSetExponential<Value_T> cdf) : super_t(N, cdf) {}
	};
	
	template<typename Value_T>
	struct DataSetNormal : public DataSetBase<Value_T, NormalPM<Value_T> >
	{
		typedef DataSetBase<Value_T, NormalPM<Value_T> > super_t;
		DataSetNormal(int N, NormalPM<Value_T> cdf) : super_t(N, cdf) {}
	};
};

#endif // RANDOM_H
