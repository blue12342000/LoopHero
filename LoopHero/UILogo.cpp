#include "UILogo.h"
#include "UISprite.h"
#include "AnimationUIController.h"
#include "AnimationMove.h"
#include "Utill.h"

void UILogo::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
    GameUI::Init(anchor, pos, width, height);
    CreateAnimController("logo_anim");

    lpLogo = GameUI::Create<UISprite>(this);
    lpLogo->Init(UI_ANCHOR::MIDDLE, { 0.0f, 0.0f }, 195 * 2, 195 * 2);
    lpLogo->SetTexture(ImageManager::GetSingleton()->FindImage("title_logo"));
    lpLogo->SetEventCatch(EVENT_CATCH::PASS);
}

void UILogo::Release()
{
    GameUI::Release();
}

void UILogo::Render(HDC hdc)
{
    GameUI::Render(hdc);
}

void UILogo::OnClick(EventData& data)
{
    //ObserverManager::GetSingleton()->Notify("OpenAnimController", this);
}
