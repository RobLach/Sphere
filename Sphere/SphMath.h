#pragma once
#include <stdlib.h>
#include <math.h>
#include "SphInclude.h"

#define PI   (3.1415926536f)
#define PI2  (6.2831853072f)
#define PId2 (1.5707963268f)
#define PId3 (1.0471975512f)

#define MAX_FLOAT (3.4E38f)
#define EPSILON   (0.000001f)
#define BIG_EPSILON (0.001f)

//Useful variables.
cfloat g_zeroes[] = {0.0f, 0.0f, 0.0f, 0.0f};
cfloat g_ones[]   = {1.0f, 1.0f, 1.0f, 1.0f};
cfloat g_unitQuat[] = {1.0f, 0.0f, 0.0f, 0.0f};

//Methods for generating random numbers
inline int Rand(){									//i: [0, RAND_MAX)
    return rand();
}
inline float Randf(){								//f: [0, 1)
	return (float)Rand() / (float)RAND_MAX;
}
inline float Randf(cfloat min, cfloat max){	//f: [min, max)
	return Randf() * (max - min) + min;
}
inline float Randf2(){								//f: [-1, 1)
	return Randf(-1.0f, 1.0f);
}
inline float Randf2pi(){							//f: [0, 2 * pi)
	return Randf(0.0f, PI2);
}
inline float Randf360(){							//f: [0, 360)
	return Randf(0.0f, 360.0f);
}
inline int Randi(cint limit){						//i: [0, limit)
	return (int)(limit * Randf());
}
inline bool Randb(){									//b: true/false
	return Randf() >= 0.5f;
}

//Trig functions
template <typename T>
inline T Sin(T angle){
	return (T)sin(angle);
}
template <typename T>
inline T Cos(T angle){
	return (T)cos(angle);
}
template <typename T>
inline T Asin(T value){
	return (T)asin(Clamp(value, (T)-1.0, (T)1.0));
}
template <typename T>
inline T Acos(T value){
	return (T)acos(Clamp(value, (T)-1.0, (T)1.0));
}

//Some helpful math methods

template <typename T>
inline T Abs(T value){
	return (value < (T)(0)) ? (-value) : (value);
}

//Return -1,0, or 1 depending on the sign of the value.
template <typename T>
inline T Sign(T value){
	return (value == (T)(0)) ? (value) : ((value > (T)(0)) ? (T)(1) : (T)(-1));
}

//Round a value to its closest integer representation.
template <typename T>
inline int Round(T value){
	return (int)(value + (T)(0.5) * Sign(value));
}

//Round a value to the closest integer multiple of the second value.
template <typename T>
inline T RoundToMultiple(T value, T multiple){
	return (T)(multiple * Round(value / multiple));
}

//Square a value.
template <typename T>
inline T Sq(T value){
	return value * value;
}

//Find minimum.
template<typename T>
inline T Min(T x, T y){
	return (x < y) ? (x) : (y);
}

//Find maximum.
template<typename T>
inline T Max(T x, T y){
	return (x > y) ? (x) : (y);
}

//Clamp x value between xmin and xmax.
template<typename T>
inline T Clamp(T x, T xmin, T xmax){
	return Min(Max(x, xmin), xmax);
}

//If x is less than xmin, increase x by step.
template<typename T>
inline T BringUp(T x, T xmin, T step){
	return (x < xmin) ? (x + step) : (x);
}

//If x is greater than xmax, decrease x by step.
template<typename T>
inline T BringDown(T x, T xmax, T step){
	return (x > xmax) ? (x - step) : (x);
}

//Apply bringUp() and bringDown().
template<typename T>
inline T BringUpDown(T x, T xmin, T xmax, T step){
	return BringUp(BringDown(x, xmax, step), xmin, step);
}

//Convert 0.0-1.0 value into a smoother value suited for interpolation.
template<typename T>
inline T SmoothStep(T x){
	return (x * x * ((T)(3.0) - (T)(2.0) * x));
}

//Angle converstions
template<typename T>
inline T DegToRad(T deg){
	return deg / 180.0f * PI;
}
template<typename T>
inline T RadToDeg(T rad){
	return rad * 180.0f / PI;
}

//Convert an angle to a (0, full) range
template<typename T>
inline T BringFullRad(T angle){
	return BringUpDown(angle, (T)(0.0f), (T)(PI2), (T)(PI2));
}
template<typename T>
inline T BringFullDeg(T angle){
	return BringUpDown(angle, (T)(0.0f), (T)(360.0f), (T)(360.0f));
}
//Convert an angle to a (-full/2, full/2) range
template<typename T>
inline T BringHalfRad(T angle){
	return BringUpDown(angle, (T)(-PI), (T)(PI), (T)(PI2));
}
template<typename T>
inline T BringHalfDeg(T angle){
	return BringUpDown(angle, (T)(-180.0f), (T)(180.0f), (T)(360.0f));
}
//Calculate the shortest distance between two angles using (-full/2, full/2) range
template<typename T>
inline T MinAngleRad(T angle1, T angle2){
	return BringHalfRad(angle1 - angle2);
}
template<typename T>
inline T MinAngleDeg(T angle1, T angle2){
	return BringHalfDeg(angle1 - angle2);
}

//================== VECTOR FUNCTIONS ====================

//return - true iff a == b.
template <typename T>
inline bool Equals(const T* a, const T* b){
	return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

//Add vector a and b and store it into result.
template <typename T1, typename T2, typename T3>
inline void Add2(T1* result, const T2* a, const T3* b){
	result[0] = (T1)(a[0] + b[0]);
	result[1] = (T1)(a[1] + b[1]);
}

//Add vector a and b and store it into result.
template <typename T1, typename T2, typename T3>
inline void Add(T1* result, const T2* a, const T3* b){
	result[0] = (T1)(a[0] + b[0]);
	result[1] = (T1)(a[1] + b[1]);
	result[2] = (T1)(a[2] + b[2]);
}

//Add vector a to result.
template <typename T1, typename T2>
inline void AddTo2(T1* result, const T2* a){
	result[0] += (T1)(a[0]);
	result[1] += (T1)(a[1]);
}

//Add vector a to result.
template <typename T1, typename T2>
inline void AddTo(T1* result, const T2* a){
	result[0] += (T1)(a[0]);
	result[1] += (T1)(a[1]);
	result[2] += (T1)(a[2]);
}

//Add vector a to result.
template <typename T1, typename T2>
inline void AddTo4(T1* result, const T2* a){
	result[0] += (T1)(a[0]);
	result[1] += (T1)(a[1]);
	result[2] += (T1)(a[2]);
	result[3] += (T1)(a[3]);
}

//Add vector a to result.
template <typename T1, typename T2>
inline void AddToN(T1* result, const T2* a, cint count){
	for(int n = 0; n < count; n++){
		result[n] += (T1)(a[n]);
	}
}

//Subtract vector a from b and store it into result.
template <typename T1, typename T2, typename T3>
inline void Subtract2(T1* result, const T2* a, const T3* b){
	result[0] = (T1)(a[0] - b[0]);
	result[1] = (T1)(a[1] - b[1]);
}

//Subtract vector a from b and store it into result.
template <typename T1, typename T2, typename T3>
inline void Subtract(T1* result, const T2* a, const T3* b){
	result[0] = (T1)(a[0] - b[0]);
	result[1] = (T1)(a[1] - b[1]);
	result[2] = (T1)(a[2] - b[2]);
}

//Subtract vector a from result.
template <typename T1, typename T2>
inline void SubtractFrom2(T1* result, const T2* a){
	result[0] -= (T1)(a[0]);
	result[1] -= (T1)(a[1]);
}

//Subtract vector a from result.
template <typename T1, typename T2>
inline void SubtractFrom(T1* result, const T2* a){
	result[0] -= (T1)(a[0]);
	result[1] -= (T1)(a[1]);
	result[2] -= (T1)(a[2]);
}

//Multiply the vector by a number.
template <typename T1, typename T2>
inline void Multiply2(T1* vector, const T2 number){
	vector[0] *= (T1)(number);
	vector[1] *= (T1)(number);
}

//Multiply the vector by a number.
template <typename T1, typename T2>
inline void Multiply(T1* vector, const T2 number){
	vector[0] *= (T1)(number);
	vector[1] *= (T1)(number);
	vector[2] *= (T1)(number);
}

//Multiply the vector by a number.
template <typename T1, typename T2>
inline void Multiply4(T1* vector, const T2 number){
	vector[0] *= (T1)(number);
	vector[1] *= (T1)(number);
	vector[2] *= (T1)(number);
	vector[3] *= (T1)(number);
}

//Multiply the vector by a number.
template <typename T1, typename T2, typename T3>
inline void MultiplyN(T1* vector, const T2 number, const T3 count){
	for(T3 n = 0; n < count; n++){
		vector[n] *= (T1)(number);
	}
}

//Linearly interpolate two vectors, taking percentA from a and the rest from b.
template <typename T1, typename T2, typename T3, typename T4>
inline void LinInterp2(T1* result, const T2 percentA, const T3* a, const T4* b){
	result[0] = (T1)(percentA * a[0] + ((T2)(1.0) - percentA) * b[0]);
	result[1] = (T1)(percentA * a[1] + ((T2)(1.0) - percentA) * b[1]);
}

//Linearly interpolate two vectors, taking percentA from a and the rest from b.
template <typename T1, typename T2, typename T3, typename T4>
inline void LinInterp(T1* result, const T2 percentA, const T3* a, const T4* b){
	result[0] = (T1)(percentA * a[0] + ((T2)(1.0) - percentA) * b[0]);
	result[1] = (T1)(percentA * a[1] + ((T2)(1.0) - percentA) * b[1]);
	result[2] = (T1)(percentA * a[2] + ((T2)(1.0) - percentA) * b[2]);
}

//Return the magnitude of the vector.
template <typename T>
inline T Magnitude2(const T* vector){
	return (T)sqrt(Sq(vector[0]) + Sq(vector[1]));
}

//Return the square of the magnitude of the vector.
template <typename T>
inline T MagnitudeSq2(const T* vector){
	return Sq(vector[0]) + Sq(vector[1]);
}

//Return the magnitude of the vector.
template <typename T>
inline T Magnitude(const T* vector){
	return (T)sqrt(Sq(vector[0]) + Sq(vector[1]) + Sq(vector[2]));
}

//Return the magnitude of the vector.
template <typename T>
inline T Magnitude4(const T* vector){
	return (T)sqrt(Sq(vector[0]) + Sq(vector[1]) + Sq(vector[2]) + Sq(vector[3]));
}

//Return the square of the magnitude of the vector.
template <typename T>
inline T MagnitudeSq(const T* vector){
	return Sq(vector[0]) + Sq(vector[1]) + Sq(vector[2]);
}

//Return the magnitude of the vector.
template <typename T>
inline float Magnitude(T v0, T v1, T v2){
	return (T)sqrt(Sq(v0) + Sq(v1) + Sq(v2));
}

//Return the square of the magnitude of the vector.
template <typename T>
inline T MagnitudeSq(T v0, T v1, T v2){
	return Sq(v0) + Sq(v1) + Sq(v2);
}

//Normalize a vector.
template <typename T>
inline void Normalize2(T* vector){
	Multiply2(vector, 1.0f / Magnitude2(vector));
}

//Normalize a vector.
template <typename T>
inline void Normalize(T* vector){
	Multiply(vector, 1.0f / Magnitude(vector));
}

//Normalize a vector.
template <typename T>
inline void Normalize4(T* vector){
	Multiply4(vector, 1.0f / Magnitude4(vector));
}

//Return the magnitude of the vector.
template <typename T1, typename T2>
inline T1 Distance2(const T1* vector1, const T2* vector2){
	return Sq(vector1[0] - vector2[0]) + Sq(vector1[1] - vector2[1]);
}

//Return the magnitude of the vector.
template <typename T1, typename T2>
inline T1 Distance(const T1* vector1, const T2* vector2){
	return Sq(vector1[0] - vector2[0]) + Sq(vector1[1] - vector2[1]) + Sq(vector1[2] - vector2[2]);
}

//Return the magnitude of the vector.
template <typename T1, typename T2>
inline T1 Distance2Sq(const T1* vector1, const T2* vector2){
	return sqrt(Sq(vector1[0] - vector2[0]) + Sq(vector1[1] - vector2[1]));
}

//Return the magnitude of the vector.
template <typename T1, typename T2>
inline T1 DistanceSq(const T1* vector1, const T2* vector2){
	return sqrt(Sq(vector1[0] - vector2[0]) + Sq(vector1[1] - vector2[1]) + Sq(vector1[2] - vector2[2]));
}

//Compute the dot product of two vectors.
template <typename T1, typename T2>
inline T1 Dot2(const T1* vector1, const T2* vector2){
	return vector1[0] * vector2[0] + vector1[1] * vector2[1];
}

//Compute the dot product of two vectors.
template <typename T>
inline T Dot(const T* vector1, const T* vector2){
	return vector1[0] * vector2[0] + vector1[1] * vector2[1] + vector1[2] * vector2[2];
}

//Set the dest vector to source vector.
template <typename T1, typename T2>
inline void Set2(T1* dest, const T2* source){
	dest[0] = (T1)source[0];
	dest[1] = (T1)source[1];
}

//Set the dest vector to source vector.
template <typename T1, typename T2>
inline void Set(T1* dest, const T2* source){
	dest[0] = (T1)source[0];
	dest[1] = (T1)source[1];
	dest[2] = (T1)source[2];
}

//Set the dest vector to source vector.
template <typename T1, typename T2>
inline void Set4(T1* dest, const T2* source){
	dest[0] = (T1)source[0];
	dest[1] = (T1)source[1];
	dest[2] = (T1)source[2];
	dest[3] = (T1)source[3];
}

//Set the dest vector to source vector.
template <typename T1, typename T2, typename T3>
inline void SetN(T1* dest, const T2 value, const T3 count){
	for(T3 n = 0; n < count; n++){
		dest[n] = (T1)value;
	}
}

//Set the dest vector to {value0, value1}.
template <typename T1, typename T2>
inline void Set(T1* dest, const T2 value0, const T2 value1){
	dest[0] = (T1)value0;
	dest[1] = (T1)value1;
}

//Set the dest vector to {value0, value1, value2}.
template <typename T1, typename T2>
inline void Set(T1* dest, const T2 value0, const T2 value1, const T2 value2){
	dest[0] = (T1)value0;
	dest[1] = (T1)value1;
	dest[2] = (T1)value2;
}

//Set the dest vector to {value0, value1, value2, value3}.
template <typename T1, typename T2>
inline void Set(T1* dest, const T2 value0, const T2 value1, const T2 value2, const T2 value3){
	dest[0] = (T1)value0;
	dest[1] = (T1)value1;
	dest[2] = (T1)value2;
	dest[3] = (T1)value3;
}

//Set the dest vector + offset to {value0, value1} and move offset by 2.
template <typename T1, typename T2, typename T3>
inline void SetAndGo(T1* dest, T2& offset, const T3 value0, const T3 value1){
	dest[offset++] = (T1)value0;
	dest[offset++] = (T1)value1;
}

//Set the dest vector + offset to {value0, value1, value2} and move offset by 3.
template <typename T1, typename T2, typename T3>
inline void SetAndGo(T1* dest, T2& offset, const T3 value0, const T3 value1, const T3 value2){
	dest[offset++] = (T1)value0;
	dest[offset++] = (T1)value1;
	dest[offset++] = (T1)value2;
}

//Set the dest vector + offset to {value0, value1, value2, value3} and move offset by 4.
template <typename T1, typename T2, typename T3>
inline void SetAndGo(T1* dest, T2 &offset, const T3 value0, const T3 value1, const T3 value2, const T3 value3){
	dest[offset++] = (T1)value0;
	dest[offset++] = (T1)value1;
	dest[offset++] = (T1)value2;
	dest[offset++] = (T1)value3;
}

//Set the dest vector + offset to first three values of source and move offset by 3.
template <typename T1, typename T2, typename T3>
inline void SetAndGo2(T1* dest, T2& offset, const T3* source){
	dest[offset++] = (T1)source[0];
	dest[offset++] = (T1)source[1];
}

//Set the dest vector + offset to first three values of source and move offset by 3.
template <typename T1, typename T2, typename T3>
inline void SetAndGo(T1* dest, T2& offset, const T3* source){
	dest[offset++] = (T1)source[0];
	dest[offset++] = (T1)source[1];
	dest[offset++] = (T1)source[2];
}

//Set the dest to the cross product of u and v.
template <typename T1, typename T2, typename T3>
inline void Cross(T1* dest, const T2* u, const T3* v){
	T2 u0 = u[0], u1 = u[1], u2 = u[2];
	T3 v0 = v[0], v1 = v[1], v2 = v[2];
	dest[0] = u1 * v2 - u2 * v1;
	dest[1] = u2 * v0 - u0 * v2;
	dest[2] = u0 * v1 - u1 * v0;
}

//dest = u projected onto v
template <typename T1, typename T2, typename T3>
inline void VectorProject(T1* dest, const T2* u, const T3* v){
	float factor = Dot(u, v) / MagnitudeSq(v);
	Set(dest, v);
	Multiply(dest, factor);
}

//dest = vector1 + factor * vector2;
template <typename T1, typename T2, typename T3, typename T4>
inline void VectorPlusFactorVector(T1* dest, const T2* vector1, T3 factor, const T4* vector2){
	T1 temp[N_XYZ];
	Set(temp, vector2);
	Multiply(temp, factor);
	Add(dest, vector1, temp);
}

//Set result to the result of rotating vector around axis by angle in radians.
template <typename T1, typename T2, typename T3, typename T4>
inline void Rotate(T1* result, const T2* vector, const T3* axis, const T4 angle){
	float q0 = cos(angle / 2.0f), q1 = sin(angle / 2.0f) * axis[0], q2 = sin(angle / 2.0f) * axis[1], q3 = sin(angle / 2.0f) * axis[2];
	float q00 = (q0  + q1  - q2  - q3);
	float q01 = 2.0f * (q1 * q2 - q0 * q3);
	float q02 = 2.0f * (q1 * q3 + q0 * q2);
	float q10 = 2.0f * (q2 * q1 + q0 * q3);
	float q11 = (q0  - q1  + q2  - q3 );
	float q12 = 2.0f * (q2 * q3 - q0 * q1);
	float q20 = 2.0f * (q3 * q1 - q0 * q2);
	float q21 = 2.0f * (q3 * q2 + q0 * q1);
	float q22 = (q0  - q1  - q2  + q3);
	T2 v0 = vector[0];
	T2 v1 = vector[1];
	T2 v2 = vector[2];
	result[0] = q00 * v0 +  q01 * v1 + q02 * v2;
	result[1] = q10 * v0 +  q11 * v1 + q12 * v2;
	result[2] = q20 * v0 +  q21 * v1 + q22 * v2;
}

//result = [rotation angle in degress, 3D normalized axis of rotation]
template <typename T1, typename T2>
inline void QuatToRotation(T1* result, const T2* quat){
	if(MagnitudeSq(quat + O_QX) > EPSILON){
		result[O_QW] = RadToDeg(2.0f * acos(quat[O_QW]));
		Set(result + O_QX, quat + O_QX);
		Normalize(result + O_QX);
	} else {
		Set4(result, g_zeroes);
	}
}

//result = result * quat, where result is quaternion.
template <typename T>
inline void QuatMultiplyBy(T* result, const T* quat){
	T w = result[O_QW] * quat[O_QW] - result[O_QX] * quat[O_QX] - result[O_QY] * quat[O_QY] - result[O_QZ] * quat[O_QZ];
	T x = result[O_QW] * quat[O_QX] + result[O_QX] * quat[O_QW] + result[O_QY] * quat[O_QZ] - result[O_QZ] * quat[O_QY];
	T y = result[O_QW] * quat[O_QY] - result[O_QX] * quat[O_QZ] + result[O_QY] * quat[O_QW] + result[O_QZ] * quat[O_QX];
	T z = result[O_QW] * quat[O_QZ] + result[O_QX] * quat[O_QY] - result[O_QY] * quat[O_QX] + result[O_QZ] * quat[O_QW];
	Set(result, w, x, y, z);
}

//result = result * quat, where result is quaternion.
template <typename T>
inline void QuatMultiply(T* result, const T* quat1, const T* quat2){
	T w = quat1[O_QW] * quat2[O_QW] - quat1[O_QX] * quat2[O_QX] - quat1[O_QY] * quat2[O_QY] - quat1[O_QZ] * quat2[O_QZ];
	T x = quat1[O_QW] * quat2[O_QX] + quat1[O_QX] * quat2[O_QW] + quat1[O_QY] * quat2[O_QZ] - quat1[O_QZ] * quat2[O_QY];
	T y = quat1[O_QW] * quat2[O_QY] - quat1[O_QX] * quat2[O_QZ] + quat1[O_QY] * quat2[O_QW] + quat1[O_QZ] * quat2[O_QX];
	T z = quat1[O_QW] * quat2[O_QZ] + quat1[O_QX] * quat2[O_QY] - quat1[O_QY] * quat2[O_QX] + quat1[O_QZ] * quat2[O_QW];
	Set(result, w, x, y, z);
}

//result = quaternion representation of rotation around y-axis by the given angle.
template <typename T1, typename T2>
inline void QuaternionY(T1* result, T2 radians){
	Set(result, Cos(radians / (T2)(2.0)), 0.0f, (T2)Sin(radians / (T2)(2.0)), 0.0f);
}

//result = log(quat)
template <typename T1, typename T2>
inline void QuatLog(T1* result, const T2* quat){
	float angle = (float)acos(quat[O_QW]);
	float sina = (float)sin(angle);
	if(Abs(sina) <= EPSILON){
		Set4(result, g_zeroes);
	} else {
		float factor = angle / sina;
		Set(result, 0.0f, factor * quat[O_QX], factor * quat[O_QY], factor * quat[O_QZ]);
	}
}

//result = e^quat
//return - angle of rotation represented by result.
template <typename T1, typename T2>
inline T2 QuatExp(T1* result, const T2* quat){
	T2 magn = (T2)Magnitude(quat + O_QX);
	T2 sina = (T2)sin(magn);
	T2 cosa = (T2)cos(magn);

	if (magn <= EPSILON){
		Set(result, cosa, 0.0f, 0.0f, 0.0f);
	} else {
		T2 factor = sina / magn;
		Set(result, cosa, factor * quat[O_QX], factor * quat[O_QY], factor * quat[O_QZ]);
	}
	return T2(2.0) * magn;
}

//result = quat applied to vector.
template <typename T1, typename T2, typename T3>
inline void QuatApplyTo(T1* result, const T2* vector, const T3* quat){
	if(quat[O_QW] >= (T3)(1.0) - (T3)(EPSILON)){
		Set(result, vector);
	} else {
		T3 theta = (T3)(2.0) * Acos(quat[O_QW]);
		T3 cosTheta = Cos(theta);
		T3 sinTheta = Sin(theta);
		T3 axis[N_XYZ]; Set(axis, quat + O_QX); Normalize(axis);
		T3 uXuDOTv[N_XYZ], temp[N_XYZ], temp2[N_XYZ];
		Set(uXuDOTv, axis);
		Multiply(uXuDOTv, Dot(axis, vector));
		Subtract(temp, vector, uXuDOTv);
		VectorPlusFactorVector(temp, uXuDOTv, cosTheta, temp);
		Cross(temp2, axis, vector);
		VectorPlusFactorVector(result, temp, sinTheta, temp2);
	}
}

//Converts an axis and angle of rotation around that axis to a quaternion
template <typename T1, typename T2, typename T3>
inline void AxisAngleToQuat(T1* result, const T2* axis, const T3 angle){
	float s = sin(angle/2);
	result[O_QX] = axis[O_X]*s;
	result[O_QY] = axis[O_Y]*s;
	result[O_QZ] = axis[O_Z]*s;
	result[O_QW] = cos(angle/2);
	NormalizeQuat(result);
}

//Normalize a quaternion.
template <typename T>
inline void NormalizeQuat(T* quat){
	float magnitude = sqrt(Sq(quat[O_QW]) + Sq(quat[O_QX]) + Sq(quat[O_QY]) + Sq(quat[O_QZ]));
	quat[O_QW] = quat[O_QW]/magnitude;
	quat[O_QX] = quat[O_QX]/magnitude;
	quat[O_QY] = quat[O_QY]/magnitude;
	quat[O_QZ] = quat[O_QZ]/magnitude;

}