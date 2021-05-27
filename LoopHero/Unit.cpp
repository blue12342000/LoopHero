#include "Unit.h"
#include "LoopHero.h"
#include "Trait.h"
#include "EquipItem.h"
#include "Image.h"
#include "Animation.h"

void Unit::Init()
{
	lpIcon = nullptr;
}

void Unit::Release()
{
	lpTrait = nullptr;

	if (lpIcon)
	{
		delete lpIcon;
		lpIcon = nullptr;
	}
}

void Unit::Update(float deltaTime)
{
	if (lpIcon) lpIcon->Update(deltaTime);
}

void Unit::Render(HDC hdc)
{
	if (lpIcon) lpIcon->Render(hdc, GetWorldPos().x, GetWorldPos().y, IMAGE_ALIGN::CENTER);
}

void Unit::Idle()
{
}

void Unit::Hit(float dmg)
{
	currHp -= dmg;
	if (currHp <= 0)
	{
		currHp = GetStatus(UNIT_STATUS::MAX_HP);
	}
}

void Unit::Attack()
{

}

void Unit::Revive()
{

}

void Unit::Death()
{

}

void Unit::SetTrait(Trait& trait)
{
	Release();
	this->lpTrait = &trait;

	name = lpTrait->GetTraitId();
	mStatus = lpTrait->GetInitStatus();
	currHp = GetStatus(UNIT_STATUS::MAX_HP);
	for (const auto& pair : lpTrait->GetUnitSlot())
	{
		mEquip.insert(make_pair(pair.first, EquipSlot{ pair.second, nullptr }));
	}
	lpIcon = new Animation();
	lpIcon->Init(name + "_icon", ANIMATION_TYPE::LOOP, 5);
	lpIcon->Play();
}

string Unit::ToString()
{
	string desc = "";

	int n = (int)UNIT_STATUS::NONE;
	UNIT_STATUS status;
	string var, content = "";
	for (int i = 0; i < n; ++i)
	{
		status = (UNIT_STATUS)i;
		if (mStatus.find(status) != mStatus.end())
		{
			if (content.size() > 0) content += "\n";
			var = to_string(mStatus[status]);
			content += GameData::GetSingleton()->GetLang(status) + ": " + var.substr(0, var.find_first_of('.') + 2);
		}
	}

	return name + "|" + content;
}
