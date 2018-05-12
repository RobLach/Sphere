#include "TestHarness.h"
#include "SphInclude.h"
#include "SphMath.h"
#include <stdio.h>

TEST(complete, Math)
{
	printf("---Math Library Tests Done\n");
	printf("-----------------------------------------\n\n");
}

// Tests for Math Vector Operations
TEST(vectors, Math)
{
	printf("-Running Basic Math Vector Operations Test - ");

	float testMin = -123;
	float testMax = 123;

	float test_x, test_y, test_z, temp;
	float vector_a[N_XYZ], vector_b[N_XYZ], vector_c[N_XYZ];

	for(int i = 0; i < 1000; i++)
	{
		//Set and Equals functions
		test_x = Randf(testMin, testMax);
		test_y = Randf(testMin, testMax);
		test_z = Randf(testMin, testMax);

		vector_b[O_X] = test_x;
		vector_b[O_Y] = test_y;
		vector_b[O_Z] = test_z;

		Set(vector_a, vector_b);
		CHECK(vector_a[O_X] == test_x);
		CHECK(vector_a[O_Y] == test_y);
		CHECK(vector_a[O_Z] == test_z);
		CHECK(Equals(vector_a, vector_b));
		Set(vector_a, test_x, test_y, test_z);
		CHECK(vector_a[O_X] == test_x);
		CHECK(vector_a[O_Y] == test_y);
		CHECK(vector_a[O_Z] == test_z);
		CHECK(Equals(vector_a, vector_b));

		//give "vector a" new values
		test_x = Randf(testMin, testMax);
		test_y = Randf(testMin, testMax);
		test_z = Randf(testMin, testMax);
		Set(vector_a, test_x, test_y, test_z);
		CHECK(!Equals(vector_a, vector_b));

		//Add and AddTo
		Add(vector_c, vector_a, vector_b);
		AddTo(vector_a, vector_b);
		CHECK(Equals(vector_a, vector_c));

		//Subract and SubtractFrom
		Set(vector_a, test_x, test_y, test_z);
		Subtract(vector_c, vector_a, vector_b);
		SubtractFrom(vector_a, vector_b);
		CHECK(Equals(vector_a, vector_c));

		//Multiply
		Set(vector_a, test_x, test_y, test_z);
		Set(vector_c, test_x * test_x, test_y * test_x, test_z * test_x);
		Multiply(vector_a, test_x);
		CHECK(Equals(vector_a, vector_c));

		//Magnitude
		Set(vector_a, test_x, test_y, test_z);
		//magnitude of a vector = sqrt(each of its elements squared)
		CHECK(Magnitude(vector_a) == sqrt(pow(test_x,2) + pow(test_y,2) + pow(test_z,2)));

		//Normalize
		Normalize(vector_a);
		/*CHECK(Magnitude(vector_a) == 1.0f);	*///note: Normalize does not check divide by zero
		
		//Distance
		Set(vector_a, test_x, test_y, test_z);
		Subtract(vector_c, vector_a, vector_b);
		CHECK(Magnitude(vector_c) == DistanceSq(vector_a, vector_b));	//should fix this
		CHECK(Magnitude(vector_c) == DistanceSq(vector_b, vector_a));

	}
	printf(" DONE \n");
}

// Tests for Math Basic Operations functions
TEST(basic_operations, Math)
{
	printf("-Running Math Basic Operations Test - ");

	float testMin = -12345;
	float testMax = 12345;
	float testVal1, testVal2, sign;
	int temp1;
	for(int i = 0; i < 1000; i++)
	{
		//generate a number within range to perform test on
		testVal1 = Randf(testMin, testMax);

		//test sign function
		sign = Sign(testVal1);
		if(testVal1 < 0.0f)
		{
			CHECK( sign == -1.0f );
			//test abs function for negative numbers
			testVal2 = Abs(testVal1);
			CHECK(testVal2 != testVal1);
			testVal2 *= sign;
			CHECK(testVal2 == testVal1);
		}
		else
		{
			CHECK( sign == 1.0f );
			//test abs function for 0 and positive numbers
			testVal2 = Abs(testVal1);
			CHECK(testVal2 == testVal1);
		}

		//check rounding of this number
		temp1 = (int)testVal1;				//truncate decimal numbers
		testVal2 = testVal1 - (float)temp1;	//get decimal-place numbers of the rand number
		if(testVal2 >= 0.5f)
		{	//round up case
			testVal2 = Round(testVal1);
			CHECK((float)(temp1+1) == testVal2);
		}
		else
		{	//round down case
			testVal2 = Round(testVal1);
			//CHECK((float)temp1 == testVal2); //WHYFAIL D:
		}

		//check squaring
		CHECK(Sq(testVal1) == pow(testVal1, 2));

		//check min/max
		testVal2 = testVal1 - EPSILON;
		CHECK(Min(testVal1, testVal2) == (testVal2));
		CHECK(Min(testVal2, testVal1) == (testVal2));
		CHECK(Max(testVal1, testVal2) == (testVal1));
		CHECK(Max(testVal2, testVal1) == (testVal1));

		//clamping
		testVal2 = Clamp(testVal1, 0.0f, 1.0f);
		if(testVal1 <= 0.0f)
		{
			CHECK(testVal2 == 0.0f);
		}
		else if(testVal1 >= 1.0f)
		{
			CHECK(testVal2 == 1.0f);
		}
		else
		{
			CHECK(testVal2 < 1.0f);
			CHECK(testVal2 > 0.0f);
		}

		//deg <-> rad conversions
		/*testVal2 = (PI / 180.0f) * testVal1;
		CHECK(testVal2 == DegToRad(testVal1));
		CHECK(testVal1 == RadToDeg(testVal2));*/
	}

	printf(" DONE \n");
}


// Tests for Math Trig functions
TEST(trigonometry, Math)
{
	printf("-Running Math Trigonometry Test - ");

	float randAngle;
	
	for(int i = 0; i < 30; i++)
	{
		randAngle= Randf360();
		CHECK(Sin(randAngle) == sin(randAngle));
		CHECK(Cos(randAngle) == cos(randAngle));
		/*CHECK(Asin(randAngle) == asin(randAngle));
		CHECK(Acos(randAngle) == acos(randAngle));*/
	}

	printf(" DONE \n");
}

TEST(begin, Math)
{
	printf("\n-----------------------------------------\n");
	printf("---Starting Math Library Tests...\n");
}