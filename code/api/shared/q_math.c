/*
===========================================================================

Wolfenstein: Enemy Territory GPL Source Code
Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company.

Enemy Territory Fortress
Copyright (C) 2000-2006 Quake III Fortress (Q3F) Development Team / Splash Damage Ltd.
Copyright (C) 2005-2018 Enemy Territory Fortress Development Team

OpenJK
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors

This file is part of Enemy Territory Fortress (ETF).

ETF is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ETF is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ETF. If not, see <http://www.gnu.org/licenses/>.

In addition, the Wolfenstein: Enemy Territory GPL Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the ETF Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "q_math.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>


///////////////////////////////////////////////////////////////////////////
//
//      DIRECTION ENCODING
//
///////////////////////////////////////////////////////////////////////////
const vec3_t bytedirs[NUMVERTEXNORMALS] =
{
	{-0.525731f, 0.000000f, 0.850651f}, {-0.442863f, 0.238856f, 0.864188f},
	{-0.295242f, 0.000000f, 0.955423f}, {-0.309017f, 0.500000f, 0.809017f},
	{-0.162460f, 0.262866f, 0.951056f}, {0.000000f, 0.000000f, 1.000000f},
	{0.000000f, 0.850651f, 0.525731f}, {-0.147621f, 0.716567f, 0.681718f},
	{0.147621f, 0.716567f, 0.681718f}, {0.000000f, 0.525731f, 0.850651f},
	{0.309017f, 0.500000f, 0.809017f}, {0.525731f, 0.000000f, 0.850651f},
	{0.295242f, 0.000000f, 0.955423f}, {0.442863f, 0.238856f, 0.864188f},
	{0.162460f, 0.262866f, 0.951056f}, {-0.681718f, 0.147621f, 0.716567f},
	{-0.809017f, 0.309017f, 0.500000f},{-0.587785f, 0.425325f, 0.688191f},
	{-0.850651f, 0.525731f, 0.000000f},{-0.864188f, 0.442863f, 0.238856f},
	{-0.716567f, 0.681718f, 0.147621f},{-0.688191f, 0.587785f, 0.425325f},
	{-0.500000f, 0.809017f, 0.309017f}, {-0.238856f, 0.864188f, 0.442863f},
	{-0.425325f, 0.688191f, 0.587785f}, {-0.716567f, 0.681718f, -0.147621f},
	{-0.500000f, 0.809017f, -0.309017f}, {-0.525731f, 0.850651f, 0.000000f},
	{0.000000f, 0.850651f, -0.525731f}, {-0.238856f, 0.864188f, -0.442863f},
	{0.000000f, 0.955423f, -0.295242f}, {-0.262866f, 0.951056f, -0.162460f},
	{0.000000f, 1.000000f, 0.000000f}, {0.000000f, 0.955423f, 0.295242f},
	{-0.262866f, 0.951056f, 0.162460f}, {0.238856f, 0.864188f, 0.442863f},
	{0.262866f, 0.951056f, 0.162460f}, {0.500000f, 0.809017f, 0.309017f},
	{0.238856f, 0.864188f, -0.442863f},{0.262866f, 0.951056f, -0.162460f},
	{0.500000f, 0.809017f, -0.309017f},{0.850651f, 0.525731f, 0.000000f},
	{0.716567f, 0.681718f, 0.147621f}, {0.716567f, 0.681718f, -0.147621f},
	{0.525731f, 0.850651f, 0.000000f}, {0.425325f, 0.688191f, 0.587785f},
	{0.864188f, 0.442863f, 0.238856f}, {0.688191f, 0.587785f, 0.425325f},
	{0.809017f, 0.309017f, 0.500000f}, {0.681718f, 0.147621f, 0.716567f},
	{0.587785f, 0.425325f, 0.688191f}, {0.955423f, 0.295242f, 0.000000f},
	{1.000000f, 0.000000f, 0.000000f}, {0.951056f, 0.162460f, 0.262866f},
	{0.850651f, -0.525731f, 0.000000f},{0.955423f, -0.295242f, 0.000000f},
	{0.864188f, -0.442863f, 0.238856f}, {0.951056f, -0.162460f, 0.262866f},
	{0.809017f, -0.309017f, 0.500000f}, {0.681718f, -0.147621f, 0.716567f},
	{0.850651f, 0.000000f, 0.525731f}, {0.864188f, 0.442863f, -0.238856f},
	{0.809017f, 0.309017f, -0.500000f}, {0.951056f, 0.162460f, -0.262866f},
	{0.525731f, 0.000000f, -0.850651f}, {0.681718f, 0.147621f, -0.716567f},
	{0.681718f, -0.147621f, -0.716567f},{0.850651f, 0.000000f, -0.525731f},
	{0.809017f, -0.309017f, -0.500000f}, {0.864188f, -0.442863f, -0.238856f},
	{0.951056f, -0.162460f, -0.262866f}, {0.147621f, 0.716567f, -0.681718f},
	{0.309017f, 0.500000f, -0.809017f}, {0.425325f, 0.688191f, -0.587785f},
	{0.442863f, 0.238856f, -0.864188f}, {0.587785f, 0.425325f, -0.688191f},
	{0.688191f, 0.587785f, -0.425325f}, {-0.147621f, 0.716567f, -0.681718f},
	{-0.309017f, 0.500000f, -0.809017f}, {0.000000f, 0.525731f, -0.850651f},
	{-0.525731f, 0.000000f, -0.850651f}, {-0.442863f, 0.238856f, -0.864188f},
	{-0.295242f, 0.000000f, -0.955423f}, {-0.162460f, 0.262866f, -0.951056f},
	{0.000000f, 0.000000f, -1.000000f}, {0.295242f, 0.000000f, -0.955423f},
	{0.162460f, 0.262866f, -0.951056f}, {-0.442863f, -0.238856f, -0.864188f},
	{-0.309017f, -0.500000f, -0.809017f}, {-0.162460f, -0.262866f, -0.951056f},
	{0.000000f, -0.850651f, -0.525731f}, {-0.147621f, -0.716567f, -0.681718f},
	{0.147621f, -0.716567f, -0.681718f}, {0.000000f, -0.525731f, -0.850651f},
	{0.309017f, -0.500000f, -0.809017f}, {0.442863f, -0.238856f, -0.864188f},
	{0.162460f, -0.262866f, -0.951056f}, {0.238856f, -0.864188f, -0.442863f},
	{0.500000f, -0.809017f, -0.309017f}, {0.425325f, -0.688191f, -0.587785f},
	{0.716567f, -0.681718f, -0.147621f}, {0.688191f, -0.587785f, -0.425325f},
	{0.587785f, -0.425325f, -0.688191f}, {0.000000f, -0.955423f, -0.295242f},
	{0.000000f, -1.000000f, 0.000000f}, {0.262866f, -0.951056f, -0.162460f},
	{0.000000f, -0.850651f, 0.525731f}, {0.000000f, -0.955423f, 0.295242f},
	{0.238856f, -0.864188f, 0.442863f}, {0.262866f, -0.951056f, 0.162460f},
	{0.500000f, -0.809017f, 0.309017f}, {0.716567f, -0.681718f, 0.147621f},
	{0.525731f, -0.850651f, 0.000000f}, {-0.238856f, -0.864188f, -0.442863f},
	{-0.500000f, -0.809017f, -0.309017f}, {-0.262866f, -0.951056f, -0.162460f},
	{-0.850651f, -0.525731f, 0.000000f}, {-0.716567f, -0.681718f, -0.147621f},
	{-0.716567f, -0.681718f, 0.147621f}, {-0.525731f, -0.850651f, 0.000000f},
	{-0.500000f, -0.809017f, 0.309017f}, {-0.238856f, -0.864188f, 0.442863f},
	{-0.262866f, -0.951056f, 0.162460f}, {-0.864188f, -0.442863f, 0.238856f},
	{-0.809017f, -0.309017f, 0.500000f}, {-0.688191f, -0.587785f, 0.425325f},
	{-0.681718f, -0.147621f, 0.716567f}, {-0.442863f, -0.238856f, 0.864188f},
	{-0.587785f, -0.425325f, 0.688191f}, {-0.309017f, -0.500000f, 0.809017f},
	{-0.147621f, -0.716567f, 0.681718f}, {-0.425325f, -0.688191f, 0.587785f},
	{-0.162460f, -0.262866f, 0.951056f}, {0.442863f, -0.238856f, 0.864188f},
	{0.162460f, -0.262866f, 0.951056f}, {0.309017f, -0.500000f, 0.809017f},
	{0.147621f, -0.716567f, 0.681718f}, {0.000000f, -0.525731f, 0.850651f},
	{0.425325f, -0.688191f, 0.587785f}, {0.587785f, -0.425325f, 0.688191f},
	{0.688191f, -0.587785f, 0.425325f}, {-0.955423f, 0.295242f, 0.000000f},
	{-0.951056f, 0.162460f, 0.262866f}, {-1.000000f, 0.000000f, 0.000000f},
	{-0.850651f, 0.000000f, 0.525731f}, {-0.955423f, -0.295242f, 0.000000f},
	{-0.951056f, -0.162460f, 0.262866f}, {-0.864188f, 0.442863f, -0.238856f},
	{-0.951056f, 0.162460f, -0.262866f}, {-0.809017f, 0.309017f, -0.500000f},
	{-0.864188f, -0.442863f, -0.238856f}, {-0.951056f, -0.162460f, -0.262866f},
	{-0.809017f, -0.309017f, -0.500000f}, {-0.681718f, 0.147621f, -0.716567f},
	{-0.681718f, -0.147621f, -0.716567f}, {-0.850651f, 0.000000f, -0.525731f},
	{-0.688191f, 0.587785f, -0.425325f}, {-0.587785f, 0.425325f, -0.688191f},
	{-0.425325f, 0.688191f, -0.587785f}, {-0.425325f, -0.688191f, -0.587785f},
	{-0.587785f, -0.425325f, -0.688191f}, {-0.688191f, -0.587785f, -0.425325f}
};

// this isn't a real cheap function to call!
int DirToByte( vec3_t dir )
{
	int		i, best;
	float	d, bestd;

	if ( !dir ) {
		return 0;
	}

	bestd = 0;
	best = 0;
	for (i=0 ; i<NUMVERTEXNORMALS ; i++)
	{
		d = DotProduct(dir, bytedirs[i]);
		if (d > bestd)
		{
			bestd = d;
			best = i;
		}
	}

	return best;
}

void ByteToDir( int b, vec3_t dir )
{
	if ( b < 0 || b >= NUMVERTEXNORMALS ) {
		VectorCopy( vec3_origin, dir );
		return;
	}
	VectorCopy(bytedirs[b], dir);
}

///////////////////////////////////////////////////////////////////////////
//
//      MATH UTILITIES
//
///////////////////////////////////////////////////////////////////////////
signed char ClampChar( int i )
{
	if ( i < -128 ) {
		return -128;
	}
	if ( i > 127 ) {
		return 127;
	}
	return i;
}

signed short ClampShort( int i )
{
	if ( i < -32768 ) {
		return -32768;
	}
	if ( i > 0x7fff ) {
		return 0x7fff;
	}
	return i;
}

int Com_Clampi( int min, int max, int value )
{
	if ( value < min )
	{
		return min;
	}
	if ( value > max )
	{
		return max;
	}
	return value;
}

float Com_Clamp( float min, float max, float value ) {
	if ( value < min ) {
		return min;
	}
	if ( value > max ) {
		return max;
	}
	return value;
}

int Com_AbsClampi( int min, int max, int value )
{
	if( value < 0 )
	{
		return Com_Clampi( -max, -min, value );
	}
	else
	{
		return Com_Clampi( min, max, value );
	}
}

float Com_AbsClamp( float min, float max, float value )
{
	if( value < 0.0f )
	{
		return Com_Clamp( -max, -min, value );
	}
	else
	{
		return Com_Clamp( min, max, value );
	}
}


float Q_rsqrt( float number )
{
	byteAlias_t t;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	t.f  = number;
	t.i  = 0x5f3759df - ( t.i >> 1 );               // what the fuck?
	y  = t.f;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
												//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	assert( !Q_isnan(y) );
	return y;
}

float Q_fabs( float f )
{
	byteAlias_t fi;
	fi.f = f;
	fi.i &= 0x7FFFFFFF;
	return fi.f;
}

/*
=====================
Q_acos

the msvc acos doesn't always return a value between -PI and PI:

int i;
i = 1065353246;
acos(*(float*) &i) == -1.#IND0

This should go in q_math but it is too late to add new traps
to game and ui
=====================
*/
float Q_acos(float c) {
	float angle;

	angle = acosf(c);

	if (angle > M_PI) {
		return (float)M_PI;
	}
	if (angle < -M_PI) {
		return (float)M_PI;
	}
	return angle;
}

float Q_asin(float c)
{
	float angle;

	angle = asinf(c);

	if (angle > M_PI) {
		return (float)M_PI;
	}
	if (angle < -M_PI) {
		return (float)M_PI;
	}
	return angle;
}

float Q_powf ( float x, int y )
{
	float r = x;
	for ( y--; y>0; y-- )
		r *= x;
	return r;
}

qboolean Q_isnan (float f)
{
#ifdef _MSC_VER
	return (qboolean)(_isnan (f) != 0);
#else
	return (qboolean)(isnan (f) != 0);
#endif
}

int Q_log2( int val )
{
	int answer;

	answer = 0;
	while ( ( val>>=1 ) != 0 ) {
		answer++;
	}
	return answer;
}

float LerpAngle(float from, float to, float frac)
{
	float	a;

	if ( to - from > 180 ) {
		to -= 360;
	}
	if ( to - from < -180 ) {
		to += 360;
	}
	a = from + frac * (to - from);

	return a;
}

/*void LerpPosition( vec3_t start, vec3_t end, float frac, vec3_t out) {
	vec3_t dist;

	VectorSubtract( end, start, dist );
	VectorMA(start, frac, dist, out);
}*/

/*
=================
AngleSubtract

Always returns a value from -180 to 180
=================
*/
float AngleSubtract( float a1, float a2 ) {
	float	a;

	a = a1 - a2;
	a=fmodf(a,360);//chop it down quickly, then level it out
	while ( a > 180 ) {
		a -= 360;
	}
	while ( a < -180 ) {
		a += 360;
	}
	return a;
}

void AnglesSubtract( vec3_t v1, vec3_t v2, vec3_t v3 ) {
	v3[0] = AngleSubtract( v1[0], v2[0] );
	v3[1] = AngleSubtract( v1[1], v2[1] );
	v3[2] = AngleSubtract( v1[2], v2[2] );
}

float AngleMod(float a) {
	a = (360.0f/65536) * ((int)(a*(65536/360.0f)) & 65535);
	return a;
}

/*
=================
AngleNormalize360

returns angle normalized to the range [0 <= angle < 360]
=================
*/
float AngleNormalize360 ( float angle ) {
	return (360.0f / 65536) * ((int)(angle * (65536 / 360.0f)) & 65535);
}

/*
=================
AngleNormalize180

returns angle normalized to the range [-180 < angle <= 180]
=================
*/
float AngleNormalize180 ( float angle ) {
	angle = AngleNormalize360( angle );
	if ( angle > 180.0 ) {
		angle -= 360.0;
	}
	return angle;
}

/*
=================
AngleDelta

returns the normalized delta from angle1 to angle2
=================
*/
float AngleDelta ( float angle1, float angle2 ) {
	return AngleNormalize180( angle1 - angle2 );
}


///////////////////////////////////////////////////////////////////////////
//
//      GEOMETRIC UTILITIES
//
///////////////////////////////////////////////////////////////////////////
/*
=====================
PlaneFromPoints

Returns false if the triangle is degenrate.
The normal will point out of the clock for clockwise ordered points
=====================
*/
qboolean PlaneFromPoints( vec4_t plane, const vec3_t a, const vec3_t b, const vec3_t c ) {
	vec3_t	d1, d2;

	VectorSubtract( b, a, d1 );
	VectorSubtract( c, a, d2 );
	CrossProduct( d2, d1, plane );
	if ( VectorNormalize( plane ) == 0 ) {
		return qfalse;
	}

	plane[3] = DotProduct( a, plane );
	return qtrue;
}

/*
===============
RotatePointAroundVector

From q3mme
===============
*/
void RotatePointAroundVector( vec3_t dst, const vec3_t dir, const vec3_t point, float degrees ) {
	float   m[3][3];
	float   c, s, t;

	degrees = -DEG2RAD( degrees );
	s = sinf( degrees );
	c = cosf( degrees );
	t = 1 - c;

	m[0][0] = t*dir[0]*dir[0] + c;
	m[0][1] = t*dir[0]*dir[1] + s*dir[2];
	m[0][2] = t*dir[0]*dir[2] - s*dir[1];

	m[1][0] = t*dir[0]*dir[1] - s*dir[2];
	m[1][1] = t*dir[1]*dir[1] + c;
	m[1][2] = t*dir[1]*dir[2] + s*dir[0];

	m[2][0] = t*dir[0]*dir[2] + s*dir[1];
	m[2][1] = t*dir[1]*dir[2] - s*dir[0];
	m[2][2] = t*dir[2]*dir[2] + c;
	VectorRotate( point, m, dst );
}

/*
===============
RotatePointAroundVertex

Rotate a point around a vertex
===============
*/
#if defined(_MSC_VER)
#pragma warning (push)
// disable double to float warning for now
#pragma warning (disable : 4244)
#endif
void RotatePointAroundVertex ( vec3_t pnt, float rot_x, float rot_y, float rot_z, const vec3_t origin ) {
	// move pnt to rel{0,0,0}
	VectorSubtract( pnt, origin, pnt );
	if (rot_x) {
		float sinval = sin( rot_x );float cosval = cos( rot_x );
		float temp1 = pnt[1];float temp2 = pnt[2];
		pnt[1] = temp1 * cosval - temp2 * sinval;
        pnt[2] = temp1 * sinval + temp2 * cosval;
	}
	if (rot_y) {
		float sinval = sin( rot_y );float cosval = cos( rot_y );
		float temp1 = pnt[2];float temp2 = pnt[0];
		pnt[2] = temp1 * cosval - temp2 * sinval;
        pnt[0] = temp1 * sinval + temp2 * cosval;
	}
	if (rot_z) {
		float sinval = sin( rot_z );float cosval = cos( rot_z );
		float temp1 = pnt[0];float temp2 = pnt[1];
		pnt[0] = temp1 * cosval - temp2 * sinval;
        pnt[1] = temp1 * sinval + temp2 * cosval;
	}
	// move pnt back
	VectorAdd( pnt, origin, pnt );
}
#if defined(_MSC_VER)
#pragma warning (pop)
#endif

// Keeg from ET code
/*
=================
GetPerpendicularViewVector

  Used to find an "up" vector for drawing a sprite so that it always faces the view as best as possible
=================
*/
void GetPerpendicularViewVector( const vec3_t point, const vec3_t p1, const vec3_t p2, vec3_t up )
{
	vec3_t	v1, v2;

	VectorSubtract( point, p1, v1 );
	VectorNormalize( v1 );

	VectorSubtract( point, p2, v2 );
	VectorNormalize( v2 );

	CrossProduct( v1, v2, up );
	VectorNormalize( up );
}

/*
================
ProjectPointOntoVector
================
*/
void ProjectPointOntoVector( vec3_t point, vec3_t vStart, vec3_t vEnd, vec3_t vProj )
{
	vec3_t pVec, vec;

	VectorSubtract( point, vStart, pVec );
	VectorSubtract( vEnd, vStart, vec );
	VectorNormalize( vec );
	// project onto the directional vector for this segment
	VectorMA( vStart, DotProduct( pVec, vec ), vec, vProj );
}

void RotateAroundDirection( matrix3_t axis, float yaw ) {

	// create an arbitrary axis[1]
	PerpendicularVector( axis[1], axis[0] );

	// rotate it around axis[0] by yaw
	if ( yaw ) {
		vec3_t	temp;

		VectorCopy( axis[1], temp );
		RotatePointAroundVector( axis[1], axis[0], temp, yaw );
	}

	// cross to get axis[2]
	CrossProduct( axis[0], axis[1], axis[2] );
}

void vectoangles( const vec3_t value1, vec3_t angles ) {
	float	forward;
	float	yaw, pitch;

	if ( value1[1] == 0 && value1[0] == 0 ) {
		yaw = 0;
		if ( value1[2] > 0 ) {
			pitch = 90;
		}
		else {
			pitch = 270;
		}
	}
	else {
		if ( value1[0] ) {
			yaw = ( atan2f ( value1[1], value1[0] ) * 180 / M_PI );
		}
		else if ( value1[1] > 0 ) {
			yaw = 90;
		}
		else {
			yaw = 270;
		}
		if ( yaw < 0 ) {
			yaw += 360;
		}

		forward = sqrtf ( value1[0]*value1[0] + value1[1]*value1[1] );
		pitch = ( atan2f(value1[2], forward) * 180 / M_PI );
		if ( pitch < 0 ) {
			pitch += 360;
		}
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0;
}

vec_t GetYawForDirection( const vec3_t p1, const vec3_t p2 ) {
	vec3_t v, angles;

	VectorSubtract( p2, p1, v );
	vectoangles( v, angles );

	return angles[YAW];
}

void GetAnglesForDirection( const vec3_t p1, const vec3_t p2, vec3_t out ) {
	vec3_t v;

	VectorSubtract( p2, p1, v );
	vectoangles( v, out );
}

void ProjectPointOnPlane( vec3_t dst, const vec3_t p, const vec3_t normal )
{
	float d;
	vec3_t n;
	float inv_denom;

	inv_denom =  DotProduct( normal, normal );
	//assert( Q_fabs(inv_denom) != 0.0f );
	inv_denom = 1.0f / inv_denom;

	d = DotProduct( normal, p ) * inv_denom;

	n[0] = normal[0] * inv_denom;
	n[1] = normal[1] * inv_denom;
	n[2] = normal[2] * inv_denom;

	dst[0] = p[0] - d * n[0];
	dst[1] = p[1] - d * n[1];
	dst[2] = p[2] - d * n[2];
}

qboolean G_FindClosestPointOnLineSegment( const vec3_t start, const vec3_t end, const vec3_t from, vec3_t result )
{
	vec3_t	vecStart2From, vecStart2End, vecEnd2Start, vecEnd2From;
	float	distEnd2From, distEnd2Result, theta, cos_theta, dot;

	//Find the perpendicular vector to vec from start to end
	VectorSubtract( from, start, vecStart2From);
	VectorSubtract( end, start, vecStart2End);

	dot = DotProductNormalize( vecStart2From, vecStart2End );

	if ( dot <= 0 )
	{
		//The perpendicular would be beyond or through the start point
		VectorCopy( start, result );
		return qfalse;
	}

	if ( dot == 1 )
	{
		//parallel, closer of 2 points will be the target
		if( (VectorLengthSquared( vecStart2From )) < (VectorLengthSquared( vecStart2End )) )
		{
			VectorCopy( from, result );
		}
		else
		{
			VectorCopy( end, result );
		}
		return qfalse;
	}

	//Try other end
	VectorSubtract( from, end, vecEnd2From);
	VectorSubtract( start, end, vecEnd2Start);

	dot = DotProductNormalize( vecEnd2From, vecEnd2Start );

	if ( dot <= 0 )
	{//The perpendicular would be beyond or through the start point
		VectorCopy( end, result );
		return qfalse;
	}

	if ( dot == 1 )
	{//parallel, closer of 2 points will be the target
		if( (VectorLengthSquared( vecEnd2From )) < (VectorLengthSquared( vecEnd2Start )))
		{
			VectorCopy( from, result );
		}
		else
		{
			VectorCopy( end, result );
		}
		return qfalse;
	}

	//		      /|
	//		  c  / |
	//		    /  |a
	//	theta  /)__|
	//		      b
	//cos(theta) = b / c
	//solve for b
	//b = cos(theta) * c

	//angle between vecs end2from and end2start, should be between 0 and 90
	theta = 90 * (1 - dot);//theta

						   //Get length of side from End2Result using sine of theta
	distEnd2From = VectorLength( vecEnd2From );//c
	cos_theta = cosf(DEG2RAD(theta));//cos(theta)
	distEnd2Result = cos_theta * distEnd2From;//b

											  //Extrapolate to find result
	VectorNormalize( vecEnd2Start );
	VectorMA( end, distEnd2Result, vecEnd2Start, result );

	//perpendicular intersection is between the 2 endpoints
	return qtrue;
}

float G_PointDistFromLineSegment( const vec3_t start, const vec3_t end, const vec3_t from )
{
	vec3_t	vecStart2From, vecStart2End, vecEnd2Start, vecEnd2From, intersection;
	float	distEnd2From, distStart2From, distEnd2Result, theta, cos_theta, dot;

	//Find the perpendicular vector to vec from start to end
	VectorSubtract( from, start, vecStart2From);
	VectorSubtract( end, start, vecStart2End);
	VectorSubtract( from, end, vecEnd2From);
	VectorSubtract( start, end, vecEnd2Start);

	dot = DotProductNormalize( vecStart2From, vecStart2End );

	distStart2From = Distance( start, from );
	distEnd2From = Distance( end, from );

	if ( dot <= 0 )
	{
		//The perpendicular would be beyond or through the start point
		return distStart2From;
	}

	if ( dot == 1 )
	{
		//parallel, closer of 2 points will be the target
		return ((distStart2From<distEnd2From)?distStart2From:distEnd2From);
	}

	//Try other end

	dot = DotProductNormalize( vecEnd2From, vecEnd2Start );

	if ( dot <= 0 )
	{//The perpendicular would be beyond or through the end point
		return distEnd2From;
	}

	if ( dot == 1 )
	{//parallel, closer of 2 points will be the target
		return ((distStart2From<distEnd2From)?distStart2From:distEnd2From);
	}

	//		      /|
	//		  c  / |
	//		    /  |a
	//	theta  /)__|
	//		      b
	//cos(theta) = b / c
	//solve for b
	//b = cos(theta) * c

	//angle between vecs end2from and end2start, should be between 0 and 90
	theta = 90 * (1 - dot);//theta

						   //Get length of side from End2Result using sine of theta
	cos_theta = cosf(DEG2RAD(theta));//cos(theta)
	distEnd2Result = cos_theta * distEnd2From;//b

											  //Extrapolate to find result
	VectorNormalize( vecEnd2Start );
	VectorMA( end, distEnd2Result, vecEnd2Start, intersection );

	//perpendicular intersection is between the 2 endpoints, return dist to it from from
	return Distance( intersection, from );
}

void MatrixMultiply(float in1[3][3], float in2[3][3], float out[3][3]) {
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
		in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
		in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
		in1[0][2] * in2[2][2];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
		in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
		in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
		in1[1][2] * in2[2][2];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
		in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
		in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
		in1[2][2] * in2[2][2];
}


///////////////////////////////////////////////////////////////////////////
//
//      BOUNDING BOX
//
///////////////////////////////////////////////////////////////////////////
float RadiusFromBounds( const vec3_t mins, const vec3_t maxs ) {
	int		i;
	vec3_t	corner;
	float	a, b;

	for (i=0 ; i<3 ; i++) {
		a = fabsf( mins[i] );
		b = fabsf( maxs[i] );
		corner[i] = a > b ? a : b;
	}

	return VectorLength (corner);
}

void ClearBounds( vec3_t mins, vec3_t maxs ) {
	mins[0] = mins[1] = mins[2] = 100000;
	maxs[0] = maxs[1] = maxs[2] = -100000;
}

void AddPointToBounds( const vec3_t v, vec3_t mins, vec3_t maxs ) {
	if ( v[0] < mins[0] ) {
		mins[0] = v[0];
	}
	if ( v[0] > maxs[0]) {
		maxs[0] = v[0];
	}

	if ( v[1] < mins[1] ) {
		mins[1] = v[1];
	}
	if ( v[1] > maxs[1]) {
		maxs[1] = v[1];
	}

	if ( v[2] < mins[2] ) {
		mins[2] = v[2];
	}
	if ( v[2] > maxs[2]) {
		maxs[2] = v[2];
	}
}


///////////////////////////////////////////////////////////////////////////
//
//      PLANE
//
///////////////////////////////////////////////////////////////////////////
#if 0
void SetPlaneSignbits( cplane_t *out )
{
	int	bits, j;

	// for fast box on planeside test
	bits = 0;
	for (j=0 ; j<3 ; j++) {
		if (out->normal[j] < 0) {
			bits |= 1<<j;
		}
	}
	out->signbits = bits;
}

int	PlaneTypeForNormal( const vec3_t normal )
{
	if ( normal[0] == 1.0 )
		return PLANE_X;
	if ( normal[1] == 1.0 )
		return PLANE_Y;
	if ( normal[2] == 1.0 )
		return PLANE_Z;
	if ( normal[0] == 0.0 && normal[1] == 0.0 && normal[2] == 0.0 )
		return PLANE_NON_PLANAR;

	return PLANE_NON_AXIAL;
}

/*
==================
BoxOnPlaneSide

Returns 1, 2, or 1 + 2
==================
*/
int BoxOnPlaneSide(vec3_t emins, vec3_t emaxs, cplane_t* p)
{
	float	dist[2];
	int		sides, b, i;

	// fast axial cases
	if (p->type < 3)
	{
		if (p->dist <= emins[p->type])
			return 1;
		if (p->dist >= emaxs[p->type])
			return 2;
		return 3;
	}

	// general case
	dist[0] = dist[1] = 0;
	if (p->signbits < 8) // >= 8: default case is original code (dist[0]=dist[1]=0)
	{
		for (i=0 ; i<3 ; i++)
		{
			b = (p->signbits >> i) & 1;
			dist[ b] += p->normal[i]*emaxs[i];
			dist[!b] += p->normal[i]*emins[i];
		}
	}

	sides = 0;
	if (dist[0] >= p->dist)
		sides = 1;
	if (dist[1] < p->dist)
		sides |= 2;

	return sides;
}
#endif



///////////////////////////////////////////////////////////////////////////
//
//      BOX & CYLINDER TRACE
//
///////////////////////////////////////////////////////////////////////////
static ID_INLINE qboolean BoxTraceTestResult( int axis, float dist, const vec3_t start, const vec3_t forward, const vec3_t mins, const vec3_t maxs, vec3_t result ) {
	result[0] = start[0] + forward[0] * dist;
	result[1] = start[1] + forward[1] * dist;
	result[2] = start[2] + forward[2] * dist;

	if (axis != 0 && (result[0] < mins[0] || result[0] > maxs[0]))
		return qfalse;
	if (axis != 1 && (result[1] < mins[1] || result[1] > maxs[1]))
		return qfalse;
	if (axis != 2 && (result[2] < mins[2] || result[2] > maxs[2]))
		return qfalse;
	return qtrue;
}

static ID_INLINE qboolean BoxTraceTestSides( int axis, const vec3_t start, const vec3_t forward, const vec3_t mins, const vec3_t maxs, vec3_t result ) {
	if (forward[axis] > 0 && start[axis] <= mins[axis]) {
		float dist = ( mins[axis] - start[axis] ) / forward[axis];
		if (BoxTraceTestResult(axis, dist, start, forward, mins, maxs, result))
			return qtrue;
	}
	if (forward[axis] < 0 && start[axis] >= maxs[axis]) {
		float dist = ( maxs[axis] - start[axis] ) / forward[axis];
		if (BoxTraceTestResult(axis, dist, start, forward, mins, maxs, result))
			return qtrue;
	}
	return qfalse;
}

qboolean BoxTraceImpact(const vec3_t start, const vec3_t forward, const vec3_t mins, const vec3_t maxs, vec3_t result ) {
	if (BoxTraceTestSides(0, start, forward, mins, maxs, result))
		return qtrue;
	if (BoxTraceTestSides(1, start, forward, mins, maxs, result))
		return qtrue;
	if (BoxTraceTestSides(2, start, forward, mins, maxs, result))
		return qtrue;
	return qfalse;
}

qboolean CylinderTraceImpact( const vec3_t start, const vec3_t forward, const vec3_t container, vec3_t result ) {
	float a = forward[0] * forward[0] + forward[1]*forward[1];
	float b = 2*(forward[0]*start[0] + forward[1]*start[1]);
	float c = start[0]*start[0] + start[1]*start[1] - container[2]*container[2];
	float base = b*b - 4*a*c;
	float t;//,r;

	/* Test if we intersect with the top or bottom */
	if ( (forward[2] > 0 && start[2] <= container[0]) || (forward[2] < 0 && start[2] >= container[1] )) {
		t = (container[0] - start[2] / forward[2]);
		VectorMA( start, t, forward, result );
		if ( (result[0]*result[0] + result[1]*result[1]) < ( container[2] * container[2]))
			return qtrue;
	}

	if (base >= 0 ) {
		t = (-b - sqrtf( base )) / 2*a;
		VectorMA( start, t, forward, result );
		//r = result[0] * result[0] + result[1]*result[1];
		if (result[2] >= container[0] && result[2]<= container[1])
			return qtrue;
	}
	return qfalse;
}



///////////////////////////////////////////////////////////////////////////
//
//      AXIS
//
///////////////////////////////////////////////////////////////////////////
matrix3_t	axisDefault = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };

void AxisClear( matrix3_t axis ) {
	axis[0][0] = 1;
	axis[0][1] = 0;
	axis[0][2] = 0;
	axis[1][0] = 0;
	axis[1][1] = 1;
	axis[1][2] = 0;
	axis[2][0] = 0;
	axis[2][1] = 0;
	axis[2][2] = 1;
}

void AxisCopy( matrix3_t in, matrix3_t out ) {
	VectorCopy( in[0], out[0] );
	VectorCopy( in[1], out[1] );
	VectorCopy( in[2], out[2] );
}

void AnglesToAxis( const vec3_t angles, matrix3_t axis ) {
	vec3_t right;

	// angle vectors returns "right" instead of "y axis"
	AngleVectors( angles, axis[0], right, axis[2] );
	VectorSubtract( vec3_origin, right, axis[1] );
}

void AxisToAngles( matrix3_t axis, vec3_t angles ) {
	float	length1;
	float	yaw, pitch, roll = 0;
	
	if ( axis[0][1] == 0 && axis[0][0] == 0 ) {
		yaw = 0;
		if ( axis[0][2] > 0 ) {
			pitch = 90;
		}
		else {
			pitch = 270;
		}
	}
	else {
		if ( axis[0][0] ) {
			yaw = ( atan2f ( axis[0][1], axis[0][0] ) * 180 / M_PI );
		}
		else if ( axis[0][1] > 0 ) {
			yaw = 90;
		}
		else {
			yaw = 270;
		}
		if ( yaw < 0 ) {
			yaw += 360;
		}

		length1 = sqrtf ( axis[0][0]*axis[0][0] + axis[0][1]*axis[0][1] );
		pitch = ( atan2f( axis[0][2], length1) * 180 / M_PI );
		if ( pitch < 0 ) {
			pitch += 360;
		}

		roll = ( atan2f( axis[1][2], axis[2][2] ) * 180 / M_PI );
		if ( roll < 0 ) {
			roll += 360;
		}
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
	angles[ROLL] = roll;
}


///////////////////////////////////////////////////////////////////////////
//
//      VEC2
//
///////////////////////////////////////////////////////////////////////////
vec2_t		vec2_zero = {0,0};

void VectorAdd2( const vec2_t vec1, const vec2_t vec2, vec2_t vecOut )
{
	vecOut[0] = vec1[0]+vec2[0];
	vecOut[1] = vec1[1]+vec2[1];
}

void VectorSubtract2( const vec2_t vec1, const vec2_t vec2, vec2_t vecOut )
{
	vecOut[0] = vec1[0]-vec2[0];
	vecOut[1] = vec1[1]-vec2[1];
}

void VectorScale2( const vec2_t vecIn, float scale, vec2_t vecOut )
{
	vecOut[0] = vecIn[0]*scale;
	vecOut[1] = vecIn[1]*scale;
}

void VectorMA2( const vec2_t vec1, float scale, const vec2_t vec2, vec2_t vecOut )
{
	vecOut[0] = vec1[0] + scale*vec2[0];
	vecOut[1] = vec1[1] + scale*vec2[1];
}

void VectorSet2( vec2_t vec, float x, float y )
{
	vec[0]=x; vec[1]=y;
}

void VectorClear2( vec2_t vec )
{
	vec[0] = vec[1] = 0.0f;
}

void VectorCopy2( const vec2_t vecIn, vec2_t vecOut )
{
	vecOut[0] = vecIn[0];
	vecOut[1] = vecIn[1];
}


///////////////////////////////////////////////////////////////////////////
//
//      VEC3
//
///////////////////////////////////////////////////////////////////////////
vec3_t		vec3_origin = {0,0,0};

void VectorAdd( const vec3_t vec1, const vec3_t vec2, vec3_t vecOut )
{
	vecOut[0] = vec1[0]+vec2[0];
	vecOut[1] = vec1[1]+vec2[1];
	vecOut[2] = vec1[2]+vec2[2];
}

void VectorSubtract( const vec3_t vec1, const vec3_t vec2, vec3_t vecOut )
{
	vecOut[0] = vec1[0]-vec2[0];
	vecOut[1] = vec1[1]-vec2[1];
	vecOut[2] = vec1[2]-vec2[2];
}

void VectorScale( const vec3_t vecIn, float scale, vec3_t vecOut )
{
	vecOut[0] = vecIn[0]*scale;
	vecOut[1] = vecIn[1]*scale;
	vecOut[2] = vecIn[2]*scale;
}

void VectorMA( const vec3_t vec1, float scale, const vec3_t vec2, vec3_t vecOut )
{
	vecOut[0] = vec1[0] + scale*vec2[0];
	vecOut[1] = vec1[1] + scale*vec2[1];
	vecOut[2] = vec1[2] + scale*vec2[2];
}

void VectorSet( vec3_t vec, float x, float y, float z )
{
	vec[0]=x; vec[1]=y; vec[2]=z;
}

void VectorClear( vec3_t vec )
{
	vec[0] = vec[1] = vec[2] = 0.0f;
}

void VectorCopy( const vec3_t vecIn, vec3_t vecOut )
{
	vecOut[0] = vecIn[0];
	vecOut[1] = vecIn[1];
	vecOut[2] = vecIn[2];
}

float VectorLength( const vec3_t vec )
{
	return (float)sqrt( vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2] );
}

float VectorLengthSquared( const vec3_t vec )
{
	return (vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

float Distance( const vec3_t p1, const vec3_t p2 )
{
	vec3_t	v;

	VectorSubtract( p2, p1, v );
	return VectorLength( v );
}

float DistanceSquared( const vec3_t p1, const vec3_t p2 )
{
	vec3_t	v;

	VectorSubtract( p2, p1, v );
	return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
}

// fast vector normalize routine that does not check to make sure
// that length != 0, nor does it return length, uses rsqrt approximation
void VectorNormalizeFast( vec3_t vec )
{
	float ilength;

	ilength = Q_rsqrt( DotProduct( vec, vec ) );

	vec[0] *= ilength;
	vec[1] *= ilength;
	vec[2] *= ilength;
}

float VectorNormalize( vec3_t vec )
{
	float	length, ilength;

	length = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
	length = sqrtf( length );

	if ( length ) {
		ilength = 1/length;
		vec[0] *= ilength;
		vec[1] *= ilength;
		vec[2] *= ilength;
	}

	return length;
}

float VectorNormalize2( const vec3_t vec, vec3_t vecOut )
{
	float	length, ilength;

	length = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
	length = sqrtf( length );

	if ( length ) {
		ilength = 1/length;
		vecOut[0] = vec[0]*ilength;
		vecOut[1] = vec[1]*ilength;
		vecOut[2] = vec[2]*ilength;
	}
	else
		VectorClear( vecOut );

	return length;
}

void VectorAdvance( const vec3_t veca, const float scale, const vec3_t vecb, vec3_t vecc)
{
	vecc[0] = veca[0] + (scale * (vecb[0] - veca[0]));
	vecc[1] = veca[1] + (scale * (vecb[1] - veca[1]));
	vecc[2] = veca[2] + (scale * (vecb[2] - veca[2]));
}

void VectorInc( vec3_t vec ) {
	vec[0] += 1.0f; vec[1] += 1.0f; vec[2] += 1.0f;
}

void VectorDec( vec3_t vec ) {
	vec[0] -= 1.0f; vec[1] -= 1.0f; vec[2] -= 1.0f;
}

void VectorInverse( vec3_t vec ) {
	vec[0] = -vec[0]; vec[1] = -vec[1]; vec[2] = -vec[2];
}

void CrossProduct( const vec3_t vec1, const vec3_t vec2, vec3_t vecOut ) {
	vecOut[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1];
	vecOut[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
	vecOut[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];
}

float DotProduct( const vec3_t vec1, const vec3_t vec2 ) {
	return vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2];
}

qboolean VectorCompare( const vec3_t vec1, const vec3_t vec2 )
{
	return (qboolean)(vec1[0] == vec2[0] && vec1[1] == vec2[1] && vec1[2] == vec2[2]);
}

qboolean VectorCompare2( const vec3_t v1, const vec3_t v2 )
{
	if ( v1[0] > (v2[0] + 0.0001f) || v1[0] < (v2[0] - 0.0001f) ||
			v1[1] > (v2[1] + 0.0001f) || v1[1] < (v2[1] + 0.0001f) ||
			v1[2] > (v2[2] + 0.0001f) || v1[2] < (v2[2] + 0.0001f) )
	{
		return qfalse;
	}

	return qtrue;
}

/*
=============
TempVector

This is just a convenience function
for making temporary vectors for function calls
=============
*/
float	*tv( float x, float y, float z ) {
	static	int		index;
	static	vec3_t	vecs[8];
	float	*v;

	// use an array so that multiple tempvectors won't collide
	// for a while
	v = vecs[index];
	index = (index + 1)&7;

	v[0] = x;
	v[1] = y;
	v[2] = z;

	return v;
}

#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning (disable : 4244)
#endif
void SnapVector( float *v )
{
#if defined(_MSC_VER) && id386
	// pitiful attempt to reduce _ftol2 calls -rww
	static int i;
	static float f;

	f = *v;
	__asm fld f
	__asm fistp	i
	*v = (float)i;
	v++;
	f = *v;
	__asm fld f
	__asm fistp i
	*v = (float)i;
	v++;
	f = *v;
	__asm fld f
	__asm fistp i
	*v = (float)i;
#else // mac, linux, mingw
	v[0] = (int)v[0];
	v[1] = (int)v[1];
	v[2] = (int)v[2];
#endif
}

/*
======================
SnapVectorTowards

Round a vector to integers for more efficient network
transmission, but make sure that it rounds towards a given point
rather than blindly truncating.  This prevents it from truncating 
into a wall.
======================
*/
void SnapVectorTowards( vec3_t v, const vec3_t to ) {
	int		i;
	for ( i = 0 ; i < 3 ; i++ ) {
		if ( to[i] < v[i])
			v[i] = floor(v[i]);
		else
			v[i] = ceil(v[i]);
	}
}
#if defined(_MSC_VER)
#pragma warning (pop)
#endif

float DistanceHorizontal( const vec3_t p1, const vec3_t p2 )
{
	vec3_t	v;

	VectorSubtract( p2, p1, v );
	return sqrtf( v[0]*v[0] + v[1]*v[1] ); //Leave off the z component
}

float DistanceHorizontalSquared( const vec3_t p1, const vec3_t p2 )
{
	vec3_t	v;

	VectorSubtract( p2, p1, v );
	return v[0]*v[0] + v[1]*v[1];	//Leave off the z component
}

/*
================
MakeNormalVectors

Given a normalized forward vector, create two
other perpendicular vectors
================
*/
void MakeNormalVectors( const vec3_t forward, vec3_t right, vec3_t up) {
	float		d;

	// this rotate and negate guarantees a vector
	// not colinear with the original
	right[1] = -forward[0];
	right[2] = forward[1];
	right[0] = forward[2];

	d = DotProduct(right, forward);
	VectorMA(right, -d, forward, right);
	VectorNormalize (right);
	CrossProduct (right, forward, up);
}

void VectorRotate( const vec3_t in, matrix3_t matrix, vec3_t out )
{
	out[0] = DotProduct( in, matrix[0] );
	out[1] = DotProduct( in, matrix[1] );
	out[2] = DotProduct( in, matrix[2] );
}

void AngleVectors( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up) {
	float		angle;
	static float		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = angles[YAW] * (M_PI*2 / 360);
	sy = sinf(angle);
	cy = cosf(angle);
	angle = angles[PITCH] * (M_PI*2 / 360);
	sp = sinf(angle);
	cp = cosf(angle);
	angle = angles[ROLL] * (M_PI*2 / 360);
	sr = sinf(angle);
	cr = cosf(angle);

	if (forward)
	{
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}
	if (right)
	{
		right[0] = (-1*sr*sp*cy+-1*cr*-sy);
		right[1] = (-1*sr*sp*sy+-1*cr*cy);
		right[2] = -1*sr*cp;
	}
	if (up)
	{
		up[0] = (cr*sp*cy+-sr*-sy);
		up[1] = (cr*sp*sy+-sr*cy);
		up[2] = cr*cp;
	}
}

/*
** assumes "src" is normalized
*/
void PerpendicularVector( vec3_t dst, const vec3_t src )
{
	int	pos;
	int i;
	float minelem = 1.0F;
	vec3_t tempvec;

	/*
	** find the smallest magnitude axially aligned vector
	*/
	for ( pos = 0, i = 0; i < 3; i++ )
	{
		if ( fabs( src[i] ) < minelem )
		{
			pos = i;
			minelem = fabsf( src[i] );
		}
	}
	tempvec[0] = tempvec[1] = tempvec[2] = 0.0F;
	tempvec[pos] = 1.0F;

	/*
	** project the point onto the plane defined by src
	*/
	ProjectPointOnPlane( dst, tempvec, src );

	/*
	** normalize the result
	*/
	VectorNormalize( dst );
}

float DotProductNormalize( const vec3_t inVec1, const vec3_t inVec2 )
{
	vec3_t	v1, v2;

	VectorNormalize2( inVec1, v1 );
	VectorNormalize2( inVec2, v2 );

	return DotProduct(v1, v2);
}


///////////////////////////////////////////////////////////////////////////
//
//      VEC4
//
///////////////////////////////////////////////////////////////////////////
void VectorScale4( const vec4_t vecIn, float scale, vec4_t vecOut )
{
	vecOut[0] = vecIn[0]*scale;
	vecOut[1] = vecIn[1]*scale;
	vecOut[2] = vecIn[2]*scale;
	vecOut[3] = vecIn[3]*scale;
}

void VectorCopy4( const vec4_t vecIn, vec4_t vecOut )
{
	vecOut[0] = vecIn[0];
	vecOut[1] = vecIn[1];
	vecOut[2] = vecIn[2];
	vecOut[3] = vecIn[3];
}

void VectorSet4( vec4_t vec, float x, float y, float z, float w )
{
	vec[0]=x; vec[1]=y; vec[2]=z; vec[3]=w;
}

void VectorClear4( vec4_t vec )
{
	vec[0] = vec[1] = vec[2] = vec[3] = 0;
}


///////////////////////////////////////////////////////////////////////////
//
//      VEC5
//
///////////////////////////////////////////////////////////////////////////
void VectorSet5( vec5_t vec, float x, float y, float z, float w, float u ) {
	vec[0]=x; vec[1]=y; vec[2]=z; vec[3]=w; vec[4]=u;
}


///////////////////////////////////////////////////////////////////////////
//
//      ROTATION MATRIX
//
///////////////////////////////////////////////////////////////////////////
/*
================
CreateRotationMatrix
================
*/
void CreateRotationMatrix(const vec3_t angles, matrix3_t matrix) {
	AngleVectors(angles, matrix[0], matrix[1], matrix[2]);
	VectorInverse(matrix[1]);
}

/*
================
TransposeMatrix
================
*/
void TransposeMatrix(matrix3_t matrix, matrix3_t transpose) {
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			transpose[i][j] = matrix[j][i];
		}
	}
}

/*
================
RotatePoint
================
*/
/* Ensiform - Borrowed from ET */
/*void RotatePoint(vec3_t point, const matrix3_t matrix) {
	vec3_t tvec;

	VectorCopy(point, tvec);
	point[0] = DotProduct(matrix[0], tvec);
	point[1] = DotProduct(matrix[1], tvec);
	point[2] = DotProduct(matrix[2], tvec);
}*/






