#include "Title.h"
#include "SceneManager.h"
#include "ImageManager.h"
#include "Image.h"

HRESULT Title::Init()
{
	lpBuffer = ImageManager::GetSingleton()->AddImage("Title_Buffer", WINSIZE_WIDTH, WINSIZE_HEIGHT);
	lpBackImage = ImageManager::GetSingleton()->FindImage("title_background");

	return E_NOTIMPL;
}

void Title::Release()
{
}

void Title::Update(float deltaTime)
{
    if (KeyManager::GetSingleton()->IsKeyOnceDown('1'))
    {
        SceneManager::GetSingleton()->ChangeScene(SCENE_KIND::TITLE);
        return;
    }

    if (KeyManager::GetSingleton()->IsKeyOnceDown('2'))
    {
        SceneManager::GetSingleton()->ChangeScene(SCENE_KIND::INGAME);
        return;
    }
}

void Title::Render(HDC hdc)
{
	HDC hMemDC = lpBuffer->GetMemDC();
	lpBackImage->Render(hMemDC);

	lpBuffer->Render(hdc);
}
