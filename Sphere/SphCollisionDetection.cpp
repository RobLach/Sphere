#include "SphMath.h"
#include "SphGame.h"
#include "SphCollisionDetection.h"
#include "SphObject.h"
#include "SphBound.h"
#include "SphPointBound.h"
#include "SphSphericBound.h"
#include "SphTightBound.h"

//Helpful functions.
void CollideLineSphere(SphCollisionInfo* info, cfloat* start, cfloat* end, cfloat* center, float radius);
void CollideLineTriangle(SphCollisionInfo* info, cfloat* start, cfloat* end, cfloat* v0, cfloat* v1, cfloat* v2);
float PointTriangleDist(SphCollisionInfo* info, cfloat* point, cfloat* v0, cfloat* v1, cfloat* v2);
float SegSegDist(SphCollisionInfo* info, cfloat* start0, cfloat* end0, cfloat* start1, cfloat* end1);

//Compute collision between two bounds.
//collisionsList - list to be filled with collision infos.
//o1 - bound for which to check collision.
//o2 - bound to check against.
void Collide(SphCollisionsList* collisionsList, SphBound* o1, SphBound* o2){
	if(o1 == NULL || o2 == NULL) return;
	if(o2->GetBoundType() == SphBound::Point){
		Collide(collisionsList, (SphBound*)o2, (SphBound*)o1);
	}
	if(o1->GetBoundType() == SphBound::Sphere){
		if(o2->GetBoundType() == SphBound::Sphere){
			Collide(collisionsList, (SphSphericBound*)o1, (SphSphericBound*)o2);
		} else if(o2->GetBoundType() == SphBound::Tight){
			Collide(collisionsList, (SphSphericBound*)o1, (SphTightBound*)o2);
		}
	} else if(o1->GetBoundType() == SphBound::Point){
		if(o2->GetBoundType() == SphBound::Sphere){
			Collide(collisionsList, (SphPointBound*)o1, (SphSphericBound*)o2);
		} else if(o2->GetBoundType() == SphBound::Tight){
			Collide(collisionsList, (SphPointBound*)o1, (SphTightBound*)o2);
		}
	}
	
}

//Compute collision between a point and a tight bounds.
void Collide(SphCollisionsList* collisionsList, SphPointBound* o1, SphTightBound* o2){
	if(o2->m_looseBound != NULL){
		SphCollisionsList tempList;
		Collide(&tempList, o1, o2->m_looseBound);
		if(tempList.size() == 0)return;
	}

	SphCollisionInfo bestInfo;
	cfloat* start = o1->GetOldPosition();
	cfloat* end = o1->GetObject()->GetPosition();
	cfloat* vertices = o2->GetVertices();
	cushort* indices = o2->GetIndices();
	
	float prevT = MAX_FLOAT;
	for(int i = 0; i < o2->GetIndexCount(); i += N_TRI){
		SphCollisionInfo tempInfo;
		CollideLineTriangle(&tempInfo, start, end, vertices + indices[i + 0] * N_XYZ, vertices + indices[i + 1] * N_XYZ, vertices + indices[i + 2] * N_XYZ);
		if(tempInfo.m_collision){
			if(tempInfo.m_temp[O_X] < prevT){
				prevT = tempInfo.m_temp[O_X];
				bestInfo = tempInfo;
			}
		}
	}
	if(bestInfo.m_collision){
		collisionsList->push_back(bestInfo);
	}
}

//Compute collision between a spheric and a tight bounds.
void Collide(SphCollisionsList* collisionsList, SphSphericBound* o1, SphTightBound* o2){
	if(o2->m_looseBound != NULL){
		SphCollisionsList tempList;
		Collide(&tempList, o1, o2->m_looseBound);
		if(tempList.size() == 0)return;
	}

	SphCollisionInfo tempInfo;
	cfloat radius = o1->m_radius;
	cfloat* center = o1->GetObject()->GetPosition();
	cfloat* vertices = o2->GetVertices();
	cushort* indices = o2->GetIndices();
	
	for(int i = 0; i < o2->GetIndexCount(); i += N_TRI){
		float dist = PointTriangleDist(&tempInfo, center, vertices + indices[i + 0] * N_XYZ, vertices + indices[i + 1] * N_XYZ, vertices + indices[i + 2] * N_XYZ);
		if(dist <= radius){
			tempInfo.m_collision = true;
			collisionsList->push_back(tempInfo);
		}
	}
}

//Compute collision between a point and a spherical bounds.
void Collide(SphCollisionsList* collisionsList, SphPointBound* o1, SphSphericBound* o2){
	SphCollisionInfo tempInfo;
	CollideLineSphere(&tempInfo, o1->GetOldPosition(), o1->GetObject()->GetPosition(), o2->GetObject()->GetPosition(), o2->m_radius);
	if(tempInfo.m_collision){
		collisionsList->push_back(tempInfo);
	}
}

//Compute collision between two spherical bounds.
void Collide(SphCollisionsList* collisionsList, SphSphericBound* o1, SphSphericBound* o2){
	SphCollisionInfo tempInfo;
	float distance = Distance(o1->GetObject()->GetPosition(), o2->GetObject()->GetPosition());
	float radiusSum = o1->m_radius + o2->m_radius;
	if(distance <= radiusSum * radiusSum){
		tempInfo.m_collision = true;
		Set(tempInfo.m_collisionPoint, o1->GetObject()->GetPosition());
		Multiply(tempInfo.m_collisionPoint, o1->m_radius / radiusSum);
		VectorPlusFactorVector(tempInfo.m_collisionPoint, tempInfo.m_collisionPoint, o2->m_radius / radiusSum, o2->GetObject()->GetPosition());
		collisionsList->push_back(tempInfo);
	}
}

//Compute collision between a line segment and a triangle.
void CollideLineTriangle(SphCollisionInfo* info, cfloat* start, cfloat* end, cfloat* v0, cfloat* v1, cfloat* v2){
	float origin[N_XYZ]; Add(origin, start, end); Multiply(origin, 0.5f);
	float direction[N_XYZ];
	Subtract(direction, end, start);
	float extent = 0.5f * Magnitude(direction);
	if(extent <= BIG_EPSILON) extent = BIG_EPSILON;
	Normalize(direction);

	float kDiff[N_XYZ]; Subtract(kDiff, origin, v0);
	float kEdge1[N_XYZ]; Subtract(kEdge1, v1, v0);
	float kEdge2[N_XYZ]; Subtract(kEdge2, v2, v0);
	float kNormal[N_XYZ]; Cross(kNormal, kEdge1, kEdge2);

	float fDdN = Dot(direction, kNormal);
	float fSign;
	if(fDdN > EPSILON){
		fSign = 1.0f;
	} else if(fDdN < -EPSILON) {
		fSign = -1.0f;
		fDdN = -fDdN;
	} else {
		return;
	}

	float temp[N_XYZ]; Cross(temp, kDiff, kEdge2);
	float fDdQxE2 = fSign * Dot(direction, temp);
	if(fDdQxE2 >= 0.0f){
		Cross(temp, kEdge1, kDiff);
		float fDdE1xQ = fSign * Dot(direction, temp);
		if(fDdE1xQ >= 0.0f){
			if(fDdQxE2 + fDdE1xQ <= fDdN){
				float fQdN = -fSign * Dot(kDiff, kNormal);
				float fExtDdN = extent * fDdN;
				if(-fExtDdN <= fQdN && fQdN <= fExtDdN){
					float fInv = 1.0f / fDdN;
					info->m_collision = true;
					VectorPlusFactorVector(info->m_collisionPoint, origin, fQdN * fInv, direction);
					info->m_temp[O_X] = fQdN * fInv;
				}
			}
		}
	}
}

//Compute collision between a line and a sphere.
void CollideLineSphere(SphCollisionInfo* info, cfloat* start, cfloat* end, cfloat* center, float radius){
	float origin[N_XYZ]; Add(origin, start, end); Multiply(origin, 0.5f);
	float direction[N_XYZ];
	Subtract(direction, end, start);
	float extent = 0.5f * Magnitude(direction);
	if(extent <= EPSILON) extent = BIG_EPSILON;
	Normalize(direction);

	float kDiff[N_XYZ]; Subtract(kDiff, origin, center);
	float fA0 = Dot(kDiff, kDiff) - radius * radius;
	float fA1 = Dot(direction, kDiff);
	float fDiscr = fA1 * fA1 - fA0;
	if(fDiscr < 0.0f) return;

	float fTmp0 = extent * extent + fA0;
	float fTmp1 = 2.0f * fA1 * extent;
	float fQM = fTmp0 - fTmp1;
	float fQP = fTmp0 + fTmp1;
	float fRoot;
	if(fQM * fQP <= 0.0f){
		fRoot = sqrt(fDiscr);
		float factor = (fQM > 0.0f) ? (-fA1 - fRoot) : (-fA1 + fRoot);
		VectorPlusFactorVector(info->m_collisionPoint, origin, factor, direction);
		info->m_collision = true;
	} else if(fQM > 0.0f && Abs(fA1) < extent){
		if(fDiscr >= EPSILON){
			fRoot = sqrt(fDiscr);
			float factor = -fA1 - fRoot;//+ fRoot
			VectorPlusFactorVector(info->m_collisionPoint, origin, factor, direction);
			info->m_collision = true;
		} else {
			VectorPlusFactorVector(info->m_collisionPoint, origin, -fA1, direction);
			info->m_collision = true;
		}
	}
}

//info - m_collisionPoint will contain the point on segment0 from which the shortest distance is measured.
//return - shortest distance from point to triangle.
float PointTriangleDist(SphCollisionInfo* info, cfloat* point, cfloat* v0, cfloat* v1, cfloat* v2){
	float kDiff[N_XYZ]; Subtract(kDiff, v0, point);
	float kEdge0[N_XYZ]; Subtract(kEdge0, v1, v0);
	float kEdge1[N_XYZ]; Subtract(kEdge1, v2, v0);
	float fA00 = MagnitudeSq(kEdge0);
	float fA01 = Dot(kEdge0, kEdge1);
	float fA11 = MagnitudeSq(kEdge1);
	float fB0 = Dot(kDiff, kEdge0);
	float fB1 = Dot(kDiff, kEdge1);
	float fC = MagnitudeSq(kDiff);
	float fDet = Abs(fA00 * fA11 - fA01 * fA01);
	float fS = fA01 * fB1 - fA11 * fB0;
	float fT = fA01 * fB0 - fA00 * fB1;
	float fSqrDistance;

	if(fS + fT <= fDet){
		if(fS < 0.0f){
			if(fT < 0.0f){  // region 4
				if(fB0 < 0.0f){
					fT = 0.0f;
					if(-fB0 >= fA00){
						fS = 1.0f;
						fSqrDistance = fA00+(2.0f)*fB0+fC;
					} else {
						fS = -fB0/fA00;
						fSqrDistance = fB0*fS+fC;
					}
				} else {
					fS = 0.0f;
					if(fB1 >= 0.0f){
						fT = 0.0f;
						fSqrDistance = fC;
					} else if(-fB1 >= fA11) {
						fT = 1.0f;
						fSqrDistance = fA11+(2.0f)*fB1+fC;
					} else {
						fT = -fB1/fA11;
						fSqrDistance = fB1*fT+fC;
					}
				}
			} else { // region 3
				fS = 0.0f;
				if(fB1 >= 0.0f){
					fT = 0.0f;
					fSqrDistance = fC;
				} else if(-fB1 >= fA11) {
					fT = 1.0f;
					fSqrDistance = fA11+(2.0f)*fB1+fC;
				} else {
					fT = -fB1/fA11;
					fSqrDistance = fB1*fT+fC;
				}
			}
		} else if(fT < 0.0f) { // region 5
			fT = 0.0f;
			if(fB0 >= 0.0f){
				fS = 0.0f;
				fSqrDistance = fC;
			} else if(-fB0 >= fA00) {
				fS = 1.0f;
				fSqrDistance = fA00+(2.0f)*fB0+fC;
			} else {
				fS = -fB0/fA00;
				fSqrDistance = fB0*fS+fC;
			}
		} else { // region 0
			// minimum at interior point
			float fInvDet = (1.0f)/fDet;
			fS *= fInvDet;
			fT *= fInvDet;
			fSqrDistance = fS*(fA00*fS+fA01*fT+(2.0f)*fB0) + fT*(fA01*fS+fA11*fT+(2.0f)*fB1)+fC;
		}
	} else {
		float fTmp0, fTmp1, fNumer, fDenom;
		if(fS < 0.0f) { // region 2
			fTmp0 = fA01 + fB0;
			fTmp1 = fA11 + fB1;
			if(fTmp1 > fTmp0){
				fNumer = fTmp1 - fTmp0;
				fDenom = fA00-2.0f*fA01+fA11;
				if(fNumer >= fDenom){
					fS = 1.0f;
					fT = 0.0f;
					fSqrDistance = fA00+(2.0f)*fB0+fC;
				} else {
					fS = fNumer/fDenom;
					fT = 1.0f - fS;
					fSqrDistance = fS*(fA00*fS+fA01*fT+2.0f*fB0) + fT*(fA01*fS+fA11*fT+(2.0f)*fB1)+fC;
				}
			} else {
				fS = 0.0f;
				if(fTmp1 <= 0.0f) {
					fT = 1.0f;
					fSqrDistance = fA11+(2.0f)*fB1+fC;
				} else if(fB1 >= 0.0f) {
					fT = 0.0f;
					fSqrDistance = fC;
				} else {
					fT = -fB1/fA11;
					fSqrDistance = fB1*fT+fC;
				}
			}
		} else if(fT < 0.0f) { // region 6
			fTmp0 = fA01 + fB1;
			fTmp1 = fA00 + fB0;
			if(fTmp1 > fTmp0) {
				fNumer = fTmp1 - fTmp0;
				fDenom = fA00-(2.0f)*fA01+fA11;
				if(fNumer >= fDenom){
					fT = 1.0f;
					fS = 0.0f;
					fSqrDistance = fA11+(2.0f)*fB1+fC;
				} else {
					fT = fNumer/fDenom;
					fS = 1.0f - fT;
					fSqrDistance = fS*(fA00*fS+fA01*fT+(2.0f)*fB0) +
						fT*(fA01*fS+fA11*fT+(2.0f)*fB1)+fC;
				}
			} else {
				fT = 0.0f;
				if(fTmp1 <= 0.0f){
					fS = 1.0f;
					fSqrDistance = fA00+(2.0f)*fB0+fC;
				} else if(fB0 >= 0.0f) {
					fS = 0.0f;
					fSqrDistance = fC;
				} else {
					fS = -fB0/fA00;
					fSqrDistance = fB0*fS+fC;
				}
			}
		} else { // region 1
			fNumer = fA11 + fB1 - fA01 - fB0;
			if(fNumer <= 0.0f){
				fS = 0.0f;
				fT = 1.0f;
				fSqrDistance = fA11+(2.0f)*fB1+fC;
			} else {
				fDenom = fA00-2.0f*fA01+fA11;
				if(fNumer >= fDenom) {
					fS = 1.0f;
					fT = 0.0f;
					fSqrDistance = fA00+(2.0f)*fB0+fC;
				} else {
					fS = fNumer/fDenom;
					fT = 1.0f - fS;
					fSqrDistance = fS*(fA00*fS+fA01*fT+(2.0f)*fB0) +
						fT*(fA01*fS+fA11*fT+(2.0f)*fB1)+fC;
				}
			}
		}
	}

	if(fSqrDistance < 0.0f){
		fSqrDistance = 0.0f;
	}

	Set(info->m_collisionPoint, v0);
	Multiply(kEdge0, fS);
	Multiply(kEdge1, fT);
	AddTo(info->m_collisionPoint, kEdge0);
	AddTo(info->m_collisionPoint, kEdge1);
	return sqrt(fSqrDistance);
}

//info - m_collisionPoint will contain the point on segment0 from which the shortest distance is measured.
//       m_temp will contain the same but for segment1.
//return - shortest distance between two line segments.
float SegSegDist(SphCollisionInfo* info, cfloat* start0, cfloat* end0, cfloat* start1, cfloat* end1){
	float extent0, extent1;
	float origin0[N_XYZ]; Add(origin0, start0, end0); Multiply(origin0, 0.5f);
	float origin1[N_XYZ]; Add(origin1, start1, end1); Multiply(origin1, 0.5f);

	float direction0[N_XYZ];
	Subtract(direction0, end0, start0);
	extent0 = 0.5f * Magnitude(direction0);
	if(extent0 <= EPSILON) {
		extent0 = BIG_EPSILON / 2.0f;
		direction0[O_X] = BIG_EPSILON;
	}
	Normalize(direction0);

	float direction1[N_XYZ];
	Subtract(direction1, end1, start1);
	extent1 = 0.5f * Magnitude(direction1);
	if(extent1 <= EPSILON) {
		extent1 = BIG_EPSILON / 2.0f;
		direction1[O_X] = BIG_EPSILON;
	}
	Normalize(direction1);

	float kDiff[N_XYZ]; Subtract(kDiff, origin0, origin1);
	float fA01 = -Dot(direction0, direction1);
	float fB0 = Dot(kDiff, direction0);
	float fB1 = -Dot(kDiff, direction1);
	float fC = MagnitudeSq(kDiff);
	float fDet = Abs(1.0f - fA01 * fA01);
	float fS0, fS1, fSqrDist, fExtDet0, fExtDet1, fTmpS0, fTmpS1;

	if(fDet >= BIG_EPSILON){// segments are not parallel
		fS0 = fA01 * fB1 - fB0;
		fS1 = fA01 * fB0 - fB1;
		fExtDet0 = extent0 * fDet;
		fExtDet1 = extent1 * fDet;

		if(fS0 >= -fExtDet0){
			if(fS0 <= fExtDet0){
				if(fS1 >= -fExtDet1){
					if(fS1 <= fExtDet1){  // region 0 (interior)
						float fInvDet = 1.0f / fDet;
						fS0 *= fInvDet;
						fS1 *= fInvDet;
						fSqrDist = fS0 * (fS0 + fA01 * fS1 + 2.0f * fB0) + fS1 * (fA01 * fS0 + fS1 + 2.0f * fB1) + fC;
					} else { // region 3 (side)
						fS1 = extent1;
						fTmpS0 = -(fA01 * fS1 + fB0);
						if(fTmpS0 < -extent0){
							fS0 = -extent0;
							fSqrDist = fS0 * (fS0 - 2.0f * fTmpS0) + fS1 * (fS1 + 2.0f * fB1) + fC;
						} else if(fTmpS0 <= extent0) {
							fS0 = fTmpS0;
							fSqrDist = -fS0 * fS0 + fS1 * (fS1 + 2.0f * fB1) + fC;
						} else {
							fS0 = extent0;
							fSqrDist = fS0 * (fS0 - 2.0f * fTmpS0) + fS1 * (fS1 + 2.0f * fB1) + fC;
						}
					}
				} else { // region 7 (side)
					fS1 = -extent1;
					fTmpS0 = -(fA01 * fS1 + fB0);
					if(fTmpS0 < -extent0){
						fS0 = -extent0;
						fSqrDist = fS0 * (fS0 - 2.0f * fTmpS0) + fS1 * (fS1 + 2.0f * fB1) + fC;
					} else if(fTmpS0 <= extent0) {
						fS0 = fTmpS0;
						fSqrDist = -fS0 * fS0 + fS1 * (fS1 + 2.0f * fB1) + fC;
					} else {
						fS0 = extent0;
						fSqrDist = fS0 * (fS0 - 2.0f * fTmpS0) + fS1 * (fS1 + 2.0f * fB1)+fC;
					}
				}
			} else {
				if(fS1 >= -fExtDet1){
					if(fS1 <= fExtDet1){  // region 1 (side)
						fS0 = extent0;
						fTmpS1 = -(fA01*fS0+fB1);
						if(fTmpS1 < -extent1){
							fS1 = -extent1;
							fSqrDist = fS1*(fS1-2.0f*fTmpS1) + fS0*(fS0+2.0f*fB0)+fC;
						} else if(fTmpS1 <= extent1) {
							fS1 = fTmpS1;
							fSqrDist = -fS1*fS1+fS0*(fS0+2.0f*fB0)+fC;
						} else {
							fS1 = extent1;
							fSqrDist = fS1*(fS1-2.0f*fTmpS1) + fS0*(fS0+2.0f*fB0)+fC;
						}
					} else { // region 2 (corner)
						fS1 = extent1;
						fTmpS0 = -(fA01*fS1+fB0);
						if(fTmpS0 < -extent0){
							fS0 = -extent0;
							fSqrDist = fS0*(fS0-2.0f*fTmpS0) + fS1*(fS1+2.0f*fB1)+fC;
						} else if(fTmpS0 <= extent0) {
							fS0 = fTmpS0;
							fSqrDist = -fS0*fS0+fS1*(fS1+2.0f*fB1)+fC;
						} else {
							fS0 = extent0;
							fTmpS1 = -(fA01*fS0+fB1);
							if(fTmpS1 < -extent1) {
								fS1 = -extent1;
								fSqrDist = fS1*(fS1-2.0f*fTmpS1) + fS0*(fS0+2.0f*fB0)+fC;
							} else if(fTmpS1 <= extent1) {
								fS1 = fTmpS1;
								fSqrDist = -fS1*fS1+fS0*(fS0+2.0f*fB0)+ fC;
							} else {
								fS1 = extent1;
								fSqrDist = fS1*(fS1-2.0f*fTmpS1) + fS0*(fS0+2.0f*fB0)+fC;
							}
						}
					}
				} else { // region 8 (corner)
					fS1 = -extent1;
					fTmpS0 = -(fA01*fS1+fB0);
					if(fTmpS0 < -extent0) {
						fS0 = -extent0;
						fSqrDist = fS0*(fS0-2.0f*fTmpS0) + fS1*(fS1+2.0f*fB1)+fC;
					} else if(fTmpS0 <= extent0) {
						fS0 = fTmpS0;
						fSqrDist = -fS0*fS0+fS1*(fS1+2.0f*fB1)+fC;
					} else {
						fS0 = extent0;
						fTmpS1 = -(fA01*fS0+fB1);
						if(fTmpS1 > extent1) {
							fS1 = extent1;
							fSqrDist = fS1*(fS1-2.0f*fTmpS1) + fS0*(fS0+2.0f*fB0)+fC;
						} else if(fTmpS1 >= -extent1) {
							fS1 = fTmpS1;
							fSqrDist = -fS1*fS1+fS0*(fS0+2.0f*fB0)+ fC;
						} else {
							fS1 = -extent1;
							fSqrDist = fS1*(fS1-2.0f*fTmpS1) + fS0*(fS0+2.0f*fB0)+fC;
						}
					}
				}
			}
		} else  {
			if(fS1 >= -fExtDet1) {
				if(fS1 <= fExtDet1){ // region 5 (side)
					fS0 = -extent0;
					fTmpS1 = -(fA01*fS0+fB1);
					if(fTmpS1 < -extent1){
						fS1 = -extent1;
						fSqrDist = fS1*(fS1-2.0f*fTmpS1) + fS0*(fS0+2.0f*fB0)+fC;
					} else if(fTmpS1 <= extent1) {
						fS1 = fTmpS1;
						fSqrDist = -fS1*fS1+fS0*(fS0+2.0f*fB0)+fC;
					} else {
						fS1 = extent1;
						fSqrDist = fS1*(fS1-2.0f*fTmpS1) + fS0*(fS0+2.0f*fB0)+fC;
					}
				} else { // region 4 (corner)
					fS1 = extent1;
					fTmpS0 = -(fA01*fS1+fB0);
					if(fTmpS0 > extent0){
						fS0 = extent0;
						fSqrDist = fS0*(fS0-2.0f*fTmpS0) + fS1*(fS1+2.0f*fB1)+fC;
					} else if(fTmpS0 >= -extent0) {
						fS0 = fTmpS0;
						fSqrDist = -fS0*fS0+fS1*(fS1+2.0f*fB1)+fC;
					} else {
						fS0 = -extent0;
						fTmpS1 = -(fA01*fS0+fB1);
						if(fTmpS1 < -extent1) {
							fS1 = -extent1;
							fSqrDist = fS1*(fS1-2.0f*fTmpS1) + fS0*(fS0+2.0f*fB0)+fC;
						} else if(fTmpS1 <= extent1) {
							fS1 = fTmpS1;
							fSqrDist = -fS1*fS1+fS0*(fS0+2.0f*fB0) + fC;
						} else {
							fS1 = extent1;
							fSqrDist = fS1*(fS1-2.0f*fTmpS1) + fS0*(fS0+2.0f*fB0)+fC;
						}
					}
				}
			} else { // region 6 (corner)
				fS1 = -extent1;
				fTmpS0 = -(fA01*fS1+fB0);
				if(fTmpS0 > extent0){
					fS0 = extent0;
					fSqrDist = fS0*(fS0-2.0f*fTmpS0) + fS1*(fS1+2.0f*fB1)+fC;
				} else if(fTmpS0 >= -extent0) {
					fS0 = fTmpS0;
					fSqrDist = -fS0*fS0+fS1*(fS1+2.0f*fB1)+fC;
				} else {
					fS0 = -extent0;
					fTmpS1 = -(fA01*fS0+fB1);
					if(fTmpS1 < -extent1){
						fS1 = -extent1;
						fSqrDist = fS1*(fS1-2.0f*fTmpS1) + fS0*(fS0+2.0f*fB0)+fC;
					} else if(fTmpS1 <= extent1) {
						fS1 = fTmpS1;
						fSqrDist = -fS1*fS1+fS0*(fS0+2.0f*fB0) + fC;
					} else {
						fS1 = extent1;
						fSqrDist = fS1*(fS1-2.0f*fTmpS1) + fS0*(fS0+2.0f*fB0)+fC;
					}
				}
			}
		}
	} else {
		float fE0pE1 = extent0 + extent1;
		float fSign = (fA01 > 0.0f ? -1.0f : 1.0f);
		float fB0Avr = 0.5f * (fB0 - fSign*fB1);
		float fLambda = -fB0Avr;
		if(fLambda < -fE0pE1){
			fLambda = -fE0pE1;
		} else if(fLambda > fE0pE1) {
			fLambda = fE0pE1;
		}

		fS1 = -fSign * fLambda * extent1 / fE0pE1;
		fS0 = fLambda + fSign * fS1;
		fSqrDist = fLambda * (fLambda + 2.0f * fB0Avr) + fC;
	}

	Set(info->m_collisionPoint, direction0);
	Multiply(info->m_collisionPoint, fS0);
	AddTo(info->m_collisionPoint, origin0);

	Set(info->m_temp, direction1);
	Multiply(info->m_temp, fS1);
	AddTo(info->m_temp, origin1);

	return sqrt(fSqrDist);
}