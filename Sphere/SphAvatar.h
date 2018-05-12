#pragma once
#include "SphInclude.h"
#include "SphModel.h"
#include "SphMesh.h"
#include "SphKey.h"
#include "SphTeleportParticles.h"
#include "SphFireParticles.h"

class SphAvatarController;
class SphAttachment;

//SphAvatar represents the avatar's model in the game world. It's controlled by the player.
class SphAvatar : public SphModel {

public:
	enum ClosedType { Normal, Speed, Rubber, Metal };
	enum Attachment { LaserGun, SprayGun, Legs, AttachmentCount };

protected:
	static SphMesh s_meshNormal[N_PAIR], s_meshSpeed[N_PAIR], s_meshRubber[N_PAIR], s_meshMetal[N_PAIR];
	static void LoadMeshes();

	SphAttachment* m_attachments[AttachmentCount];
	SphAvatarController* m_controller;
	ClosedType m_closedType;
	float m_moveForce;
	float m_rotationFactor;
	bool m_keys[sizeof(SphKey::keyType)];
	bool m_cfa[sizeof(ClosedType)];
	int m_cacti;

	float m_frictionFactor;
	bool m_onGround;

	SphTeleportParticles* m_teleParticles;
	SphFireParticles* m_fireParticles;

	void ProcessCollisions();
	void ProcessCollision(SphCollisionInfo* info);

public:
	SphAvatar();
	void Render();
	void Update(float seconds);
	void AddClosedType(ClosedType type);
	void SetClosedType(ClosedType type);
	void RemoveClosedType(ClosedType type);
	ClosedType GetClosedType();
	void AddAttachment(Attachment attachment);
	void RemoveAttachment(Attachment attachment);
	bool HasAttachment(Attachment attachment);
	void AddKey(SphKey::keyType color);
	void RemoveKey(SphKey::keyType color);
	bool HasKey(SphKey::keyType color);
	bool HasClosedType(ClosedType type);
	void AddCactus();
	int GetCacti();
	void Teleporting();
	void SetOnFire();

	float GetMoveForce();
	float GetRotationFactor();
	float GetBounceFactor();
	bool IsClosed();
	bool IsOnGround();
	SphObject::SphObjectType GetObjType();
	float m_timeSinceTeleport;
	float m_timeSinceFire;
	float m_currentTime;

	void Quit();
	void Reset();
};
