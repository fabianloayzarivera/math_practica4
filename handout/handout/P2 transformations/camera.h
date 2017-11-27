//
//  camera.h
//  interpolation
//
//  Created by Jose Maria Benito on 28/10/14.
//  Copyright (c) 2014 Jose Maria. All rights reserved.
//

#ifndef interpolation_camera_h
#define interpolation_camera_h

typedef struct {
    VECTOR3D position;
    VECTOR3D direction;
    VECTOR3D up;
    
    double aperture;    // in degrees
    int screenwidth,screenheight;
    
} CAMERA;


typedef struct {
    double left;
    double right;
    double bottom;
    double top;
    double near;
    double far;
} FRUSTUM;

 // devuelve los valores de distancia de los planos a partir del fov horizontal
FRUSTUM makeFrustum(double fovX, double aspectRatio, double near, double far)
{
    const double DEG2RAD = 3.14159265 / 180;
    
    double tangent = tan(fovX/2 * DEG2RAD);     // tangent of half fovX
    double width = near * tangent;              // half width of near plane
    double height = width / aspectRatio;        // half height of near plane
    
    FRUSTUM ret;
    ret.left = -width;
    ret.right=  width;
    ret.bottom = -height;
    ret.top = height;
    ret.near = near;
    ret.far = far;
    return ret;
}

// TODO

MATRIX4 lookAt(VECTOR3D eyePosition, VECTOR3D target, VECTOR3D upVector) {
	VECTOR3D forward = Substract(eyePosition, target);
	forward = Normalize(forward);

	VECTOR3D left = CrossProduct(upVector, forward);
	left = Normalize(left);

	VECTOR3D up = CrossProduct(forward, left);
	
	MATRIX4 mat;

	mat.m[0] = left.x;
	mat.m[4] = left.y;
	mat.m[8] = left.z;
	mat.m[1] = up.x;
	mat.m[5] = up.y;
	mat.m[9] = up.z;
	mat.m[2] = forward.x;
	mat.m[6] = forward.y;
	mat.m[10] = forward.z;

	// set translation part
	mat.m[12] = -left.x * eyePosition.x - left.y * eyePosition.y - left.z * eyePosition.z;
	mat.m[13] = -up.x * eyePosition.x - up.y * eyePosition.y - up.z * eyePosition.z;
	mat.m[14] = -forward.x * eyePosition.x - forward.y * eyePosition.y - forward.z * eyePosition.z;

	mat.m[3] = 0;
	mat.m[7] = 0;
	mat.m[11] = 0;
	mat.m[15] = 1;

	return mat;


}


#endif
