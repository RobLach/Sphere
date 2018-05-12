#pragma once
#include <stdio.h>
#include "SphInclude.h"

//SphAction controls the way skeleton bones behave during an action.
class SphAction{

private:
	//SphKeyFrame stores data for one keyframe for one skeleton bone.
	struct SphKeyFrame{
		float m_seconds;					//seconds since the start of animation
		float m_quat[N_QUAT];			//bone's quaternion value
	};

	SphKeyFrame** m_boneKeyFrames;	//we'll have one array for each bone, even if the bone doesn't participate in this action
	int* m_boneKeyFrameCounts;			//keep track of number of keyframes for each bone
	float* m_offsets;						//main bone's offset for each keyframe
	int m_frames;							//length of animation
	float m_seconds;
	float m_overflow;						//seconds overflowed from the previous animation cycle, 0.0 if last frame was in the same cycle
	bool m_active;
	bool m_loop;
	bool m_persistent;
	bool m_reverse;

	void FindKeyframes(int bone, int& frame1, int& frame2);

public:
	SphAction();
	void Load(FILE* file, int boneCount);
	void Update(float seconds);
	float Activate(float offset = 0.0f, bool loop = false, bool persistent = false);
	float Deactivate(bool immediately = false);
	float GetTimeToFinish();
	float GetOverflowTime();
	bool AddToRotation(int bone, float* rotation);
	void AddToOffset(float* offset);
	bool IsActive();
	bool IsUsed();
};
