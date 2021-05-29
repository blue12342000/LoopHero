#include "GameObject.h"

void GameObject::VaildChilds()
{
	auto it = vChilds.cbegin();
	while (it != vChilds.cend())
	{
		if ((*it)->lpParent != this)
		{
			it = vChilds.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GameObject::Init()
{
}

void GameObject::Release()
{
	if (lpParent)
	{
		lpParent->RemoveChild(this);
		return;
	}
	else
	{
		while (!vChilds.empty())
		{
			vChilds.back()->lpParent = nullptr;
			vChilds.back()->Release();
			vChilds.pop_back();
		}
	}
	ClearEventHandler();
	ObserverManager::GetSingleton()->RemoveObserver(this);
	PoolingManager::GetSingleton()->AddClass(this);
}

void GameObject::Update(float deltaTime)
{
	if (isVisible)
	{
		for (const auto& lpChild : vChilds)
		{
			if (lpChild->isVisible) lpChild->Update(deltaTime);
		}
	}
}

void GameObject::Render(HDC hdc)
{
	if (isVisible)
	{
		for (const auto& lpChild : vChilds)
		{
			if (lpChild->isVisible) lpChild->Render(hdc);
		}
	}
}

void GameObject::InsertChild(GameObject* lpChild, int index)
{
	lpChild->SetParent(this);
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
		auto it = (vChilds.begin() + index);
		(*it)->lpParent = nullptr;
		(*it)->Release();
		vChilds.erase(it);
	}
}

void GameObject::RemoveChild(GameObject* lpChild)
{
	if (!lpChild) return;

	RemoveChild(find(vChilds.begin(), vChilds.end(), lpChild) - vChilds.begin());
}

void GameObject::SetWorldPos(POINTFLOAT pos)
{
	if (lpParent)
	{
		pos.x -= lpParent->GetWorldPos().x;
		pos.y -= lpParent->GetWorldPos().y;
	}

	this->pos.x = pos.x;
	this->pos.y = pos.y;
}

POINTFLOAT GameObject::GetWorldPos()
{
	POINTFLOAT worldPos = pos;
	if (lpParent)
	{
		worldPos.x += lpParent->GetWorldPos().x;
		worldPos.y += lpParent->GetWorldPos().y;
	}

	return worldPos;
}

POINTFLOAT GameObject::GetRealationPos(GameObject* lpOther)
{
	POINTFLOAT localPos = GetWorldPos();
	if (lpParent)
	{
		localPos.x -= lpOther->pos.x;
		localPos.y -= lpOther->pos.x;
	}

	return localPos;
}

void GameObject::SetParent(GameObject* lpParent)
{
	POINTFLOAT worldPos = GetWorldPos();
	GameObject* lpPreParent = this->lpParent;
	this->lpParent = lpParent;
	if (lpPreParent)
	{
		lpPreParent->VaildChilds();
	}
	SetWorldPos(worldPos);
}
