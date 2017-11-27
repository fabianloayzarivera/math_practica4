//
//  VECTOR3D.h
//  StereoRendering
//
//  Created by Jose Maria Benito on 09/12/13.
//  Copyright (c) 2013 Jose Maria. All rights reserved.
//

#ifndef StereoRendering_VECTOR3D_h
#define StereoRendering_VECTOR3D_h
#include <vector>
#define ABS(x) (x < 0 ? -(x) : (x))
#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)
#define TRUE  1
#define FALSE 0
#define ESC 27
#define PI 3.141592653589793238462643
#define DTOR            0.0174532925    // degrees to radians
#define RTOD            57.2957795      // radians to degrees


typedef struct {
    double x,y,z;
} VECTOR3D;


typedef struct {
    double r,g,b;
} COLOUR;

typedef struct {
    VECTOR3D column0;
    VECTOR3D column1;
    VECTOR3D column2;
} MATRIX3;


typedef struct {
    float m[16];
} MATRIX4;

typedef struct {
	std::vector<VECTOR3D> P;
} CURVE;

static const COLOUR grey = {0.7,0.7,0.7};
static const COLOUR red = {1,0,0};
static const COLOUR green = {0,1,0};
static const COLOUR blue = {0,0,1};

static const COLOUR darkred = {0.6,0,0};
static const COLOUR darkgreen = {0,0.6,0};

// TODO

VECTOR3D Add(VECTOR3D a, VECTOR3D b)
{
    VECTOR3D ret;
    ret.x = a.x+b.x;
    ret.y = a.y+b.y;
    ret.z = a.z+b.z;
    return ret;
}
//
VECTOR3D Substract(VECTOR3D a, VECTOR3D b) {
	VECTOR3D ret;
	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return ret;
}

VECTOR3D Multiply(VECTOR3D a, VECTOR3D b) {
	VECTOR3D ret;
	ret.x = a.x * b.x;
	ret.y = a.y * b.y;
	ret.z = a.z * b.z;
	return ret;

}

VECTOR3D MultiplyWithScalar(float scalar, VECTOR3D a) {
	VECTOR3D ret;
	ret.x = a.x * scalar;
	ret.y = a.y * scalar;
	ret.z = a.z * scalar;
	return ret;
}

double Magnitude(VECTOR3D a) {
	return sqrt(powf(a.x, 2) + powf(a.y, 2)+ powf(a.z, 2));
	
}

VECTOR3D Normalize(VECTOR3D a) {
	VECTOR3D ret;
	double mag = Magnitude(a);
	ret.x = a.x / mag;
	ret.y = a.y / mag;
	ret.z = a.z / mag;
	return ret;
}

VECTOR3D CrossProduct(VECTOR3D a, VECTOR3D b) {
	VECTOR3D ret;
	
	ret.x = (a.y * b.z) - (a.z * b.y);
	ret.y = (a.z * b.x) - (a.x * b.z);
	ret.z = (a.x * b.y) - (a.y * b.x);
	return ret;
}

double DotProduct(VECTOR3D a, VECTOR3D b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * a.z));
}

MATRIX3 Transpose(MATRIX3 m) {
	MATRIX3 mat;

	mat.column0.x = m.column0.x;
	mat.column0.y = m.column1.x;
	mat.column0.z = m.column2.x;

	mat.column1.x = m.column0.y;
	mat.column1.y = m.column1.y;
	mat.column1.z = m.column2.y;

	mat.column2.x = m.column0.z;
	mat.column2.y = m.column1.z;
	mat.column2.z = m.column2.z;

	return mat;

}

VECTOR3D Transform(MATRIX3 m, VECTOR3D a) {
	
	VECTOR3D res;

	res.x = m.column0.x*a.x + m.column1.x*a.y + m.column2.x*a.z;
	res.y = m.column0.y*a.x + m.column1.y*a.y + m.column2.y*a.z;
	res.z = m.column0.z*a.x + m.column1.z*a.y + m.column2.z*a.z;

	return res;

}

COLOUR randomColor() {
	COLOUR obj;
	float randomR = (float)rand() / (float)(RAND_MAX / 1);
	obj.r = randomR;
	float randomG = (float)rand() / (float)(RAND_MAX / 1);
	obj.g = randomG;
	float randomB = (float)rand() / (float)(RAND_MAX / 1);
	obj.b = randomB;
	return obj;
}

VECTOR3D Interpolate(CURVE curve, float t) {
	int pointAmount = curve.P.size();
	
	switch (pointAmount)
	{
		case 2:
			return Add(MultiplyWithScalar((1 - t), curve.P[0]) , (MultiplyWithScalar(t, curve.P[1])));
			break;
		case 3:
			return Add(Add((MultiplyWithScalar((1 - t)*(1  -t), curve.P[0])) , (MultiplyWithScalar((2 * t*(t - 1)), curve.P[1])) ) , (MultiplyWithScalar(t*t, curve.P[2])));
			break;
		default:
			break;
	}

	
}

CURVE generateCurve(CURVE curve) {
	
	float t = 0;
	int steps = 100;
	float dT = 1.0f / static_cast<float>(steps);
	CURVE resultCurve;
	for (int i = 0; i < steps; i++) {
		resultCurve.P.push_back(Interpolate(curve, t));
		t += dT;
	}
	return resultCurve;

}

//local res = [0, 0, 0];
//res.x = mat.row1.x*x + mat.row2.x*y + mat.row3.x*z + mat.row4.x*w;
//res.y = mat.row1.y*x + mat.row2.y*y + mat.row3.y*z + mat.row4.y*w;
//res.z = mat.row1.z*x + mat.row2.z*y + mat.row3.z*z + mat.row4.z*w;
//return res;

MATRIX4 InverseOrthogonalMatrix(MATRIX3 A, VECTOR3D t) {
	
	MATRIX3 aT = Transpose(A);
	VECTOR3D transf = Transform(aT, t);
	MATRIX4 mat = {1,2,4};

	mat.m[0] = aT.column0.x;
	mat.m[1] = aT.column0.y;
	mat.m[2] = aT.column0.z;
	mat.m[3] = 0;

	mat.m[4] = aT.column1.x;
	mat.m[5] = aT.column1.y;
	mat.m[6] = aT.column1.z;
	mat.m[7] = 0;

	mat.m[8]  =	aT.column2.x;
	mat.m[9]  =	aT.column2.y;
	mat.m[10] = aT.column2.z;
	mat.m[11] = 0;

	mat.m[12] = transf.x;
	mat.m[13] = transf.y;
	mat.m[14] = transf.z;
	mat.m[15] = 1;

	return mat;
}





#endif
