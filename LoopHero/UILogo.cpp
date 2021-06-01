#include "UILogo.h"
#include "UISprite.h"

void UILogo::Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height)
{
    GameUI::Init(anchor, pos, width, height);

    lpLogo = GameUI::Create<UISprite>(this);
    lpLogo->Init(UI_ANCHOR::MIDDLE, { 0.0f, 0.0f }, 195 * 2, 195 * 2);
    lpLogo->SetTexture(ImageManager::GetSingleton()->FindImage("title_logo"));
}
