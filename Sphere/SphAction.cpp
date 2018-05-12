#include "SphMath.h"
#include "SphAction.h"

cfloat l_framesPerSecond = 25.0f;//Blender default FPS
cfloat l_secondsPerFrame = 1.0f / l_framesPerSecond;

//Create SphAction.
SphAction::SphAction(){
	this->m_seconds = 0.0f;
	this->m_active = false;
	this->m_loop = false;
	this->m_reverse = false;
	this->m_persistent = false;
	this->m_overflow = 0.0f;
}

//Update SphAction.
//second - since last call.
void SphAction::Update(float seconds){
	m_seconds += seconds;
	m_overflow = 0.0;
	int frame = (int)(m_seconds * l_framesPerSecond);
	if(frame >= m_frames){
		float animationLength = m_frames * l_secondsPerFrame;
		if(m_loop){
			if(animationLength <= EPSILON){
				m_overflow = EPSILON;
			} else {
				while(m_seconds >= animationLength){ m_seconds -= animationLength; }
				m_overflow = m_seconds;
			}
			m_active = true;
		} else {
			m_overflow = m_seconds - animationLength;
			m_active = m_persistent;
		}
	} else {
		m_active = true;
	}
}

//Make this action active.
//offset - seconds into the animation.
//loop - true if the action should loop.
//persistent - true if the action should still be active when it reaches the end.
//return - length of this action in seconds.
float SphAction::Activate(float offset, bool loop, bool persistent){
	m_active = true;
	m_seconds = offset;
	m_loop = loop;
	m_persistent = persistent;
	m_overflow = 0.0f;
	return m_frames * l_secondsPerFrame;
}

//Disactivate this action as soon as it finishes.
//immediately - if true, then the action will be stopped this instant. If false, then wait until it's current loop is done.
//return - seconds until this action is deactivated.
float SphAction::Deactivate(bool immediately){
	if(!m_active) return 0.0f;
	m_loop = m_persistent = false;
	if(immediately){
		m_seconds = m_frames * l_secondsPerFrame;
		m_overflow = 0.0f;
		m_active = false;
	}
	return GetTimeToFinish();
}

//bone - for which this is computed.
//frame1, frame2 - will be set to the left and right keyframes from the current frame respectively.
void SphAction::FindKeyframes(int bone, int& frame1, int& frame2){
	SphKeyFrame* keyFrames = m_boneKeyFrames[bone];
	int keyFrameCount = m_boneKeyFrameCounts[bone];
	float curSeconds = (m_reverse) ? (m_frames * l_secondsPerFrame - m_seconds) : (m_seconds);
	frame1 = -1, frame2 = 0;
	for(int n = 0; n < keyFrameCount; n++){
		SphKeyFrame& keyFrame = keyFrames[n];
		if(curSeconds >= keyFrame.m_seconds){
			frame1 = frame2 = n;
		} else {
			frame2 = n;
			break;
		}
	}
}

//bone - index.
//rotation - add to this [N_QUAT] weighed logarithm of the current keyframe quaternion.
//return - true if the action applies to this bone.
bool SphAction::AddToRotation(int bone, float* rotation){
	SphKeyFrame* keyFrames = m_boneKeyFrames[bone];
	if(keyFrames == NULL || m_seconds < 0.0f) return false;

	int frame1, frame2;
	FindKeyframes(bone, frame1, frame2);
	if(frame1 == frame2){
		//we passed the last frame
		AddTo4(rotation, keyFrames[frame1].m_quat);
	} else {
		//we have to interpolate the rotation
		float time1 = (frame1 < 0) ? (0.0f) : keyFrames[frame1].m_seconds;
		float time2 = keyFrames[frame2].m_seconds;
		float curSeconds = (m_reverse) ? (m_frames * l_secondsPerFrame - m_seconds) : (m_seconds);
		float percent = (curSeconds - time1) / (time2 - time1);
		float quat[N_QUAT];

		if(frame1 >= 0){
			//we have reached first keyframe
			Set4(quat, keyFrames[frame1].m_quat);
			Multiply4(quat, 1.0f - percent);
			AddTo4(rotation, quat);
		}

		Set4(quat, keyFrames[frame2].m_quat);
		Multiply4(quat, percent);
		AddTo4(rotation, quat);
	}
	return true;
}

//offset - add to this offset the current offset of the main bone for this action
void SphAction::AddToOffset(float* offset){
	cint parentBone = 0;
	SphKeyFrame* keyFrames = m_boneKeyFrames[parentBone];
	if(keyFrames == NULL || m_seconds < 0.0f) return;
	int frame1, frame2;
	FindKeyframes(parentBone, frame1, frame2);

	if(frame1 == frame2){
		//we passed the last frame
		AddTo(offset, m_offsets + frame1 * N_XYZ);
	} else {
		//we have to interpolate the offsets
		float tempOffset[N_XYZ];
		float time1 = (frame1 < 0) ? (0.0f) : keyFrames[frame1].m_seconds;
		float time2 = keyFrames[frame2].m_seconds;
		float curSeconds = (m_reverse) ? (m_frames * l_secondsPerFrame - m_seconds) : (m_seconds);
		float percent = (curSeconds - time1) / (time2 - time1);

		if(frame1 >= 0){
			//we have reached first keyframe
			Set(tempOffset, m_offsets + frame1 * N_XYZ);
			Multiply(tempOffset, 1.0f - percent);
			AddTo(offset, tempOffset);
		}

		Set(tempOffset, m_offsets + frame2 * N_XYZ);
		Multiply(tempOffset, percent);
		AddTo(offset, tempOffset);
	}
}

//return - seconds until this action finishes current loop.
float SphAction::GetTimeToFinish(){
	return m_frames * l_secondsPerFrame - m_seconds - EPSILON;
}

//return - seconds overflowed from the previous animation cycle, 0.0 if last frame was in the same cycle
float SphAction::GetOverflowTime(){
	return m_overflow;
}

//return - true if the action is active.
bool SphAction::IsActive(){
	return m_active;
}

//return - true if the action is being actively used.
bool SphAction::IsUsed(){
	return IsActive() && m_seconds >= 0.0f;
}

//file - to load SphAction from.
//boneCount - number of bones in the skeleton.
void SphAction::Load(FILE* file, int boneCount){
	//set up the bone keyframes
	fscanf(file, "frames %d\n", &m_frames);
	m_boneKeyFrames = new SphKeyFrame*[boneCount];
	m_boneKeyFrameCounts = new int[boneCount];
	SetN(m_boneKeyFrames, (void*)NULL, boneCount);
	Set(m_boneKeyFrameCounts, 0, boneCount);

	//read all bone postions for this action
	int boneReadCount;
	fscanf(file, "bones %d\n", &boneReadCount);
	for(int b = 0; b < boneReadCount; b++){
		//load keyframes for a bone
		int boneIndex, keyFrames, keyFrameValue;
		fscanf(file, "bone %d\n", &boneIndex);
		fscanf(file, "keyframes %d\n", &keyFrames);
		m_boneKeyFrameCounts[boneIndex] = keyFrames;
		m_boneKeyFrames[boneIndex] = new SphKeyFrame[keyFrames];
		if(boneIndex == 0){
			m_offsets = new float[keyFrames * N_XYZ];
		}

		for(int f = 0; f < keyFrames; f++){
			//load one keyframe
			SphKeyFrame& keyFrame = m_boneKeyFrames[boneIndex][f];
			fscanf(file, "frame %d\n", &keyFrameValue);
			keyFrame.m_seconds = keyFrameValue * l_secondsPerFrame;
			fscanf(file, "quat %f %f %f %f\n", keyFrame.m_quat + O_QW, keyFrame.m_quat + O_QX, keyFrame.m_quat + O_QY, keyFrame.m_quat + O_QZ);
			QuatLog(keyFrame.m_quat, keyFrame.m_quat);

			//also read in offsets for the main bone.
			if(boneIndex == 0){
				float* offset = m_offsets + f * N_XYZ;
				fscanf(file, "off %f %f %f\n", offset + O_X, offset + O_Y, offset + O_Z);
			}
		}
	}
}
