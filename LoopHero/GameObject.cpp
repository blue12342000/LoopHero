#include "GameObject.h"

void GameObject::InsertChild(GameObject* lpChild, int index)
{
	lpChild->SetPos(lpChild->GetRealationPos(this));
	lpChild->lpParent = this;
	vChilds.insert(vChilds.begin() + index, lpChild);
}

void GameObject::AddChild(GameObject* lpChild)
{
	lpChild->SetParent(this);
	vChilds.push_back(lpChild);
}

void GameObject::RemoveChild(int index)
{
	if (index > -1 && index < vChilds.size())
	{
		GameObject* lpGame = (*vChilds.begin());
		vChilds.erase(vChilds.begin() + index);
	}
}

void GameObject::RemoveChild(GameObject* lpChild)
{
	if (!lpChild) return;

	for (int i = 0; i < vChilds.size(); ++i)
	{
		if (&(*vChilds[i]) == lpChild)
		{
			RemoveChild(i);
			return;
		}
	}
}

void GameObject::SetWorldPos(POINTFLOAT pos)
{
	if (lpParent)
	{
		RECT parentRc = lpParent->GetRect();
		pos.x -= parentRc.left;
		pos.y -= parentRc.top;
	}

	this->pos.x = pos.x;
	this->pos.y = pos.y;
}

POINTFLOAT GameObject::GetWorldPos()
{
	POINTFLOAT worldPos = pos;
	if (lpParent)
	{
		RECT parentRc = lpParent->GetRect();
		worldPos.x += parentRc.left;
		worldPos.y += parentRc.top;
	}

	return worldPos;
}

POINTFLOAT GameObject::GetRealationPos(GameObject* lpOther)
{
	POINTFLOAT localPos = GetWorldPos();
	if (lpParent)
	{
		RECT otherRc = lpOther->GetRect();
		localPos.x -= otherRc.left;
		localPos.y -= otherRc.top;
	}

	return localPos;
}

void GameObject::SetParent(GameObject* lpParent)
{
	POINTFLOAT worldPos = GetWorldPos();
	if (this->lpParent)
	{
		this->lpParent->RemoveChild(this);
	}
	this->lpParent = lpParent;
	SetWorldPos(worldPos);
}
