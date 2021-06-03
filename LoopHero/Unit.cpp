#include "Unit.h"
#include "LoopHero.h"
#include "Trait.h"
#include "EquipItem.h"
#include "Image.h"
#include "Animation.h"

void Unit::Init()
{
	lpIcon = nullptr;
	state = UNIT_STATE::ALIVE;
	isPossibleLevelUp = true;
	AddEventHandler("LevelUp", bind(&Unit::LevelUp, this, placeholders::_1));
}

void Unit::Release()
{
	if (lpIcon)
	{
		lpIcon->Release();
		lpIcon = nullptr;
	}
	GameObject::Release();
}

void Unit::Update(float deltaTime)
{
	if (lpIcon) lpIcon->Update(deltaTime);

	if (state == UNIT_STATE::ALIVE)
	{
		currHp += GetStatus(UNIT_STATUS::HP_REGEN) * deltaTime;
		if (currHp > GetStatus(UNIT_STATUS::MAX_HP))
		{
			currHp = GetStatus(UNIT_STATUS::MAX_HP);
		}
	}
}

void Unit::Render(HDC hdc)
{
	if (lpIcon) lpIcon->Render(hdc, GetWorldPos().x, GetWorldPos().y, IMAGE_ALIGN::CENTER);
}

bool Unit::Hit(float dmg)
{
	float evasion = GetStatus(UNIT_STATUS::EVASION);
	if (rand() % 1000 < (int)(evasion * 10))
	{
		// È¸ÇÇ
		return false;
	}

	currHp -= dmg;
	if (currHp <= 0)
	{
		state = UNIT_STATE::DEATH;
	}
	return true;
}

float Unit::Attack()
{
	int atk = rand() % (int)((GetStatus(UNIT_STATUS::MAX_DMG) - GetStatus(UNIT_STATUS::MIN_DMG)) * 10 + FLT_EPSILON);
	return atk / 10.0f + GetStatus(UNIT_STATUS::MIN_DMG);
}

void Unit::SetTrait(Trait& trait)
{
	this->lpTrait = &trait;

	name = lpTrait->GetTraitId();
	mStatus = lpTrait->GetInitStatus();
	for (int i = 1; i < GameData::GetSingleton()->GetLoopLevel(); ++i)
	{
		LevelUp(this);
	}

	currHp = GetStatus(UNIT_STATUS::MAX_HP);
	for (const auto& pair : lpTrait->GetUnitSlot())
	{
		mEquip.insert(make_pair(pair.first, EquipSlot{ pair.second, nullptr }));
	}
	lpIcon = PoolingManager::GetSingleton()->GetClass<Animation>();
	lpIcon->Init(name + "_icon", ANIMATION_TYPE::LOOP, 5);
	lpIcon->Play();
}

void Unit::UseEquipItem(UNIT_SLOT slot, EquipItem* lpEquipItem)
{
	float hpRatio = currHp / GetStatus(UNIT_STATUS::MAX_HP);
	if (mEquip[slot].lpEquip)
	{
		for (const auto& pair : mEquip[slot].lpEquip->GetStatus())
		{
			mStatus[pair.first] -= pair.second;
		}
	}

	for (const auto& pair : lpEquipItem->GetStatus())
	{
		mStatus[pair.first] += pair.second;
	}

	currHp = GetStatus(UNIT_STATUS::MAX_HP) * hpRatio;
	mEquip[slot].lpEquip = lpEquipItem;
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
			if (abs(mStatus[status]) < FLT_EPSILON) continue;

			if (content.size() > 0) content += "\n";
			var = to_string(mStatus[status]);
			content += GameData::GetSingleton()->GetLang(status) + ": " + var.substr(0, var.find_first_of('.') + 2);
		}
	}

	return name + "|" + content;
}

void Unit::LevelUp(ObserverHandler* lpCaller)
{
	if (isPossibleLevelUp)
	{
		for (auto& pair : mStatus)
		{
			switch (pair.first)
			{
			case UNIT_STATUS::VAMP:
			case UNIT_STATUS::COUNTER:
			case UNIT_STATUS::EVASION:
			case UNIT_STATUS::MOVE_SPEED:
				pair.second *= 1.10f;
				if (pair.second > 50)
				{
					pair.second = 50;
				}
				break;
			case UNIT_STATUS::ATK_SPEED:
				if (pair.second > 0)
				{
					pair.second *= 1.10f;
					if (pair.second > 80)
					{
						pair.second = 80;
					}
				}
				break;
			default:
				pair.second *= 1.25f;
				break;
			}
		}
		currHp = GetStatus(UNIT_STATUS::MAX_HP);
	}
}
