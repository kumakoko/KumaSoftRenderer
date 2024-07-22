#include <cmath>
#include "ksr_quaternion.h"

namespace KSR
{
oid QUAT_Add(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qsum)
{
// this function adds two quaternions
qsum->x = q1->x + q2->x;
qsum->y = q1->y + q2->y;
qsum->z = q1->z + q2->z;
qsum->w = q1->w + q2->w;

} // end QUAT_Add

///////////////////////////////////////////////////////////////

 void QUAT_Sub(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qdiff)
{
// this function subtracts two quaternions, q1-q2
qdiff->x = q1->x - q2->x;
qdiff->y = q1->y - q2->y;
qdiff->z = q1->z - q2->z;
qdiff->w = q1->w - q2->w;

} // end QUAT_Sub

///////////////////////////////////////////////////////////////

void QUAT_Conjugate(QUAT_PTR q, QUAT_PTR qconj)
{
// this function computes the conjugate of a quaternion
qconj->x = -q->x;
qconj->y = -q->y;
qconj->z = -q->z;
qconj->w = q->w;
} // end QUAT_Conjugate

///////////////////////////////////////////////////////////////

void QUAT_Scale(QUAT_PTR q, float scale, QUAT_PTR qs)
{
// this function scales a quaternion and returns it 
qs->x = scale*q->x;
qs->y = scale*q->y;
qs->z = scale*q->z;
qs->w = scale*q->w;

} // end QUAT_Scale

///////////////////////////////////////////////////////////////

void QUAT_Scale(QUAT_PTR q, float scale)
{
// this function scales a quaternion in place
q->x*=scale;
q->y*=scale;
q->z*=scale;
q->w*=scale;

} // end QUAT_Scale

//////////////////////////////////////////////////////////////

float QUAT_Norm(QUAT_PTR q)
{
// returns the length or norm of a quaterion
return sqrtf(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);

} // end QUAT_Norm

//////////////////////////////////////////////////////////////

float QUAT_Norm2(QUAT_PTR q)
{
// returns the length or norm of a quaterion squared
return q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z;

} // end QUAT_Norm2

/////////////////////////////////////////////////////////////

void QUAT_Normalize(QUAT_PTR q, QUAT_PTR qn)
{
// this functions normalizes the sent quaternion and 
// returns it

// compute 1/length
float qlength_inv = 1.0f/(sqrtf(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z));

// now normalize
qn->w=q->w*qlength_inv;
qn->x=q->x*qlength_inv;
qn->y=q->y*qlength_inv;
qn->z=q->z*qlength_inv;

} // end QUAT_Normalize

/////////////////////////////////////////////////////////////

void QUAT_Normalize(QUAT_PTR q)
{
// this functions normalizes the sent quaternion in place

// compute length
float qlength_inv = 1.0f/(sqrtf(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z));

// now normalize
q->w*=qlength_inv;
q->x*=qlength_inv;
q->y*=qlength_inv;
q->z*=qlength_inv;

} // end QUAT_Normalize

//////////////////////////////////////////////////////////////

void QUAT_Unit_Inverse(QUAT_PTR q, QUAT_PTR qi)
{
// this function computes the inverse of a unit quaternion
// and returns the result
// the inverse of a unit quaternion is the conjugate :)
qi->w =  q->w;
qi->x = -q->x;
qi->y = -q->y;
qi->z = -q->z;
} // end QUAT_Unit_Inverse

//////////////////////////////////////////////////////////////

void QUAT_Unit_Inverse(QUAT_PTR q)
{
// this function computes the inverse of a unit quaternion
// in place
// the inverse of a unit quaternion is the conjugate :)
q->x = -q->x;
q->y = -q->y;
q->z = -q->z;
} // end QUAT_Unit_Inverse

/////////////////////////////////////////////////////////////

void QUAT_Inverse(QUAT_PTR q, QUAT_PTR qi)
{
// this function computes the inverse of a general quaternion
// and returns result
// in general, q-1 = *q/|q|2
// compute norm squared
float norm2_inv = 1.0f/(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);

// and plug in
qi->w =  q->w*norm2_inv;
qi->x = -q->x*norm2_inv;
qi->y = -q->y*norm2_inv;
qi->z = -q->z*norm2_inv;

} // end QUAT_Inverse

/////////////////////////////////////////////////////////////

void QUAT_Inverse(QUAT_PTR q)
{
// this function computes the inverse of a general quaternion
// in place
// in general, q-1 = *q/|q|2
// compute norm squared
float norm2_inv = 1.0f/(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);

// and plug in
q->w =  q->w*norm2_inv;
q->x = -q->x*norm2_inv;
q->y = -q->y*norm2_inv;
q->z = -q->z*norm2_inv;

} // end QUAT_Inverse

/////////////////////////////////////////////////////////////

void QUAT_Mul(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qprod)
{
// this function multiplies two quaternions

// this is the brute force method
//qprod->w = q1->w*q2->w - q1->x*q2->x - q1->y*q2->y - q1->z*q2->z;
//qprod->x = q1->w*q2->x + q1->x*q2->w + q1->y*q2->z - q1->z*q2->y;
//qprod->y = q1->w*q2->y - q1->x*q2->z + q1->y*q2->w - q1->z*q2->x;
//qprod->z = q1->w*q2->z + q1->x*q2->y - q1->y*q2->x + q1->z*q2->w;

// this method was arrived at basically by trying to factor the above
// expression to reduce the # of multiplies

float prd_0 = (q1->z - q1->y) * (q2->y - q2->z);
float prd_1 = (q1->w + q1->x) * (q2->w + q2->x);
float prd_2 = (q1->w - q1->x) * (q2->y + q2->z);
float prd_3 = (q1->y + q1->z) * (q2->w - q2->x);
float prd_4 = (q1->z - q1->x) * (q2->x - q2->y);
float prd_5 = (q1->z + q1->x) * (q2->x + q2->y);
float prd_6 = (q1->w + q1->y) * (q2->w - q2->z);
float prd_7 = (q1->w - q1->y) * (q2->w + q2->z);

float prd_8 = prd_5 + prd_6 + prd_7;
float prd_9 = 0.5f * (prd_4 + prd_8);

// and finally build up the result with the temporary products

qprod->w = prd_0 + prd_9 - prd_5;
qprod->x = prd_1 + prd_9 - prd_8;
qprod->y = prd_2 + prd_9 - prd_7;
qprod->z = prd_3 + prd_9 - prd_6;

} // end QUAT_Mul

///////////////////////////////////////////////////////////

void QUAT_Triple_Product(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR q3, QUAT_PTR qprod)
{
// this function computes q1*q2*q3 in that order and returns 
// the results in qprod

QUAT qtmp;
QUAT_Mul(q1,q2,&qtmp);
QUAT_Mul(&qtmp, q3, qprod);

} // end QUAT_Triple_Product

///////////////////////////////////////////////////////////

void VECTOR3D_Theta_To_QUAT(QUAT_PTR q, VECTOR3D_PTR v, float theta)
{
// initializes a quaternion based on a 3d direction vector and angle
// note the direction vector must be a unit vector
// and the angle is in rads

float theta_div_2 = 0.5f*theta; // compute theta/2

// compute the quaterion, note this is from chapter 4
// pre-compute to save time
float sinf_theta = sinf(theta_div_2);

q->x = sinf_theta * v->x;
q->y = sinf_theta * v->y;
q->z = sinf_theta * v->z;
q->w = cosf( theta_div_2 );

} // end VECTOR3D_Theta_To_QUAT

///////////////////////////////////////////////////////////////

 void VECTOR4D_Theta_To_QUAT(QUAT_PTR q, VECTOR4D_PTR v, float theta)
{
// initializes a quaternion based on a 4d direction vector and angle
// note the direction vector must be a unit vector
// and the angle is in rads

float theta_div_2 = 0.5f*theta; // compute theta/2

// compute the quaterion, note this is from chapter 4

// pre-compute to save time
float sinf_theta = sinf(theta_div_2);

q->x = sinf_theta * v->x;
q->y = sinf_theta * v->y;
q->z = sinf_theta * v->z;
q->w = cosf( theta_div_2 );

} // end VECTOR4D_Theta_to_QUAT

///////////////////////////////////////////////////////////////

void EulerZYX_To_QUAT(QUAT_PTR q, float theta_z, float theta_y, float theta_x)
{
// this function intializes a quaternion based on the zyx
// multiplication order of the angles that are parallel to the
// zyx axis respectively, note there are 11 other possibilities
// this is just one, later we may make a general version of the
// the function

// precompute values
float cos_z_2 = 0.5f*cosf(theta_z);
float cos_y_2 = 0.5f*cosf(theta_y);
float cos_x_2 = 0.5f*cosf(theta_x);

float sin_z_2 = 0.5f*sinf(theta_z);
float sin_y_2 = 0.5f*sinf(theta_y);
float sin_x_2 = 0.5f*sinf(theta_x);

// and now compute quaternion
q->w = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
q->x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
q->y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
q->z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;

} // EulerZYX_To_QUAT

///////////////////////////////////////////////////////////////

void QUAT_To_VECTOR3D_Theta(QUAT_PTR q, VECTOR3D_PTR v, float *theta)
{
// this function converts a unit quaternion into a unit direction
// vector and rotation angle about that vector

// extract theta
*theta = acosf(q->w);

// pre-compute to save time
float sinf_theta_inv = 1.0f/sinf(*theta);

// now the vector
v->x    = q->x*sinf_theta_inv;
v->y    = q->y*sinf_theta_inv;
v->z    = q->z*sinf_theta_inv;

// multiply by 2
*theta*=2;

} // end QUAT_To_VECTOR3D_Theta

////////////////////////////////////////////////////////////

void QUAT_Print(QUAT_PTR q, char *name="q")
{
// this function prints out a quaternion
//Write_Error("\n%s=%f+[(%f)i + (%f)j + (%f)k]", name, q->w, q->x, q->y, q->z);

} // end QUAT_Print

}