#include "SphAttachment.h"

//Create SphAttachment.
SphAttachment::SphAttachment(){
	m_attached = false;
}

//Add this attachment to avatar.
void SphAttachment::AddAttachment(SphAvatar* avatar){
	m_attached = true;
}

//Remove this attachment from avatar.
void SphAttachment::RemoveAttachment(SphAvatar* avatar){
	m_attached = false;
}

//return - true if this attachment is attached.
bool SphAttachment::IsAttached(){
	return m_attached;
}