#include "UIBattleWindow.h"
#include "UIBattleUnit.h" 
#include "BattleUnit.h"
#include "BattleField.h"
#include "Animation.h"
#include "Image.h"
#include "Hero.h"
#include "Utill.h"
#include "ParticleSystem.h"

void UIBattleWindow::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
	GameUI::Init(anchor, pos, width, height);
	lpBackground = ImageManager::GetSingleton()->FindImage("battle_background");

	lpParticleSystem = PoolingManager::GetSingleton()->GetClass<ParticleSystem>();

	ParticleInfo info;
	info.angle = -PI * 0.5f;
	info.deltaAngle = PI * 0.4f;
	info.force = 100.0f;
	info.deltaForce = 50.0f;
	info.spread = PARTICLE_SPREAD::CONE;
	info.movement = PARTICLE_MOVEMENT::GRAVITY;
	info.type = PATTICLE_TYPE::TEXT;
	info.align = PARTICLE_ALIGN::CENTER;
	info.lifeCycle = 1.0f;
	lpParticleSystem->Init(info);

	ParticleManager::GetSingleton()->AddParticleSystem("Battle_ParticleSystem", lpParticleSystem);

	AddEventHandler("BattleStart", bind(&UIBattleWindow::BattleStart, this, placeholders::_1));
	AddEventHandler("BattleEnd", bind(&UIBattleWindow::BattleEnd, this, placeholders::_1));
}

void UIBattleWindow::Release()
{
	if (lpParticleSystem)
	{
		lpParticleSystem->Release();
		lpParticleSystem = nullptr;
		ParticleManager::GetSingleton()->RemoveParticleSystem("Battle_ParticleSystem");
	}
	GameUI::Release();
}

void UIBattleWindow::Update(float deltaTime)
{
	lpParticleSystem->Update(deltaTime);

	GameUI::Update(deltaTime);
}

void UIBattleWindow::Render(HDC hdc)
{
	//RenderRectangle(hdc, rc, RGB(200, 200, 200));
	lpBackground->Render(hdc, rc.left, rc.top);

	GameUI::Render(hdc);

	lpParticleSystem->Render(hdc);
}

void UIBattleWindow::BattleStart(ObserverHandler* lpCaller)
{
	isVisible = true;
	if (typeid(*lpCaller) == typeid(BattleField))
	{
		lpBattleField = (BattleField*)lpCaller;

		int index = 0, group = 0;
		if (lpBattleField->GetHeroParty().size() == 1)
		{
			// 네크로멘서는 무조건 여길 타선 안된다.
			UIBattleUnit* lpUIBattleUnit = GameUI::Create<UIBattleUnit>(this);
			lpUIBattleUnit->Init(UI_ANCHOR::BOTTOM_MIDDLE, { -135.0f, 150.0f }, lpBattleField->GetHeroParty().back()->GetWidth(), lpBattleField->GetHeroParty().back()->GetHeight());
			lpUIBattleUnit->SetBattleUnit(lpBattleField->GetHeroParty().back());
			lpBattleField->GetHeroParty().back()->SetPos(lpUIBattleUnit->GetWorldPos());
		}
		else
		{
			for (BattleUnit*& lpBattleUnit : lpBattleField->GetHeroParty())
			{
				UIBattleUnit* lpUIBattleUnit = GameUI::Create<UIBattleUnit>();
				if (index % 2 == 0) InsertChild(lpUIBattleUnit, 0);
				else AddChild(lpUIBattleUnit);
				lpUIBattleUnit->Init(UI_ANCHOR::BOTTOM_MIDDLE, { -65.0f - (group % 2) * 140.0f, 150.0f + 60.0f * (1 + (-2) * (index % 2)) * group }, lpBattleUnit->GetWidth(), lpBattleUnit->GetHeight());
				lpUIBattleUnit->SetBattleUnit(lpBattleUnit);
				lpBattleUnit->SetPos(lpUIBattleUnit->GetWorldPos());
				if (index % 2 == 0) ++group;
				++index;
			}
		}
		
		index = 0;
		group = 0;
		if (lpBattleField->GetEnemyParty().size() == 1)
		{
			UIBattleUnit* lpUIBattleUnit = GameUI::Create<UIBattleUnit>(this);
			lpUIBattleUnit->Init(UI_ANCHOR::BOTTOM_MIDDLE, { 135.0f, 150.0f }, lpBattleField->GetEnemyParty().back()->GetWidth(), lpBattleField->GetEnemyParty().back()->GetHeight());
			lpUIBattleUnit->SetBattleUnit(lpBattleField->GetEnemyParty().back());
			lpBattleField->GetEnemyParty().back()->SetPos(lpUIBattleUnit->GetWorldPos());
		}
		else
		{
			for (BattleUnit*& lpBattleUnit : lpBattleField->GetEnemyParty())
			{
				UIBattleUnit* lpUIBattleUnit = GameUI::Create<UIBattleUnit>();
				if (index % 2 == 0) InsertChild(lpUIBattleUnit, 0);
				else AddChild(lpUIBattleUnit);
				lpUIBattleUnit->Init(UI_ANCHOR::BOTTOM_MIDDLE, { 65.0f + (group % 2) * 140.0f, 150.0f + 60.0f * (1 + (-2) * (index % 2)) * group }, lpBattleUnit->GetWidth(), lpBattleUnit->GetHeight());
				lpUIBattleUnit->SetBattleUnit(lpBattleUnit);
				lpBattleUnit->SetPos(lpUIBattleUnit->GetWorldPos());
				if (index % 2 == 0) ++group;
				++index;
			}
		}
	}
}

void UIBattleWindow::BattleEnd(ObserverHandler* lpCaller)
{
	isVisible = false;

	while (!vChilds.empty())
	{
		RemoveChild(0);
	}

	ObserverManager::GetSingleton()->Notify("Resume_Loop", this);
}

void UIBattleWindow::OnClick(EventData& data)
{
	ParticleManager::GetSingleton()->SpreadParticle("Battle_ParticleSystem", POINTFLOAT{ (float)data.point.x, (float)data.point.y }, "#0000FF|tile_appear");
}
