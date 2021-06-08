#pragma once
#include "Scene.h"

enum class LOADING_STATE
{
	LOADING,
	FINISHED,
	DEACTIVE
};

class LoadingScene : public Scene
{
protected:
	LOADING_STATE loadingState;

public:
	virtual ~LoadingScene() {}

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

	virtual bool IsFinished() final { return loadingState == LOADING_STATE::FINISHED; }
	virtual bool IsLoading() final { return loadingState == LOADING_STATE::LOADING; }
};

