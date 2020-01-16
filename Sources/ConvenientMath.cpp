#include "ConvenientMath.h"

namespace ConvMath
{
	void InitializeArray(int* _array, int num, int val)
	{
		for (int i = 0; i < num; i++)
		{
			*_array = val;
			_array++;
		}
	}
	void InitializeArray(float* _array, int num, float val)
	{
		for (int i = 0; i < num; i++)
		{
			*_array = val;
			_array++;
		}
	}
	void InitializeArray(double* _array, int num, double val)
	{
		for (int i = 0; i < num; i++)
		{
			*_array = val;
			_array++;
		}
	}
}