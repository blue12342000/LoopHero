#pragma once
#include "GameUI.h"

enum class UI_BAR_TYPE
{
	HORIZON,
	VERTICAL,
	RANGE
};

enum class UI_BAR_TARGET
{
	VARIABLE,
	FUNC
};

class Image;
class UITextField;
class UIProgressBar : public GameUI
{
private:
	UI_BAR_TYPE type;
	UI_BAR_TARGET target;
	UI_BAR_TARGET maxTarget;

	Image* lpBackground;
	Image* lpBar;

	float tick;
	float lastVar;

	function<float()> lpTargetFunc;
	float* lpTargetVar;
	
	function<float()> lpMaxFunc;
	float* lpMaxVar;

	// Range 전용 변수
	float var;
	float minVar;
	float maxVar;
	bool isDrag;

	vector<function<void(float)>> vChangedFuncs;

	// 라벨출력이 필요하다면 외부에서 연결시켜준다면 해당 라벨에 데이터를 넣어준다.
	UITextField* lpLabel;

public:
	virtual ~UIProgressBar() {}

	virtual void Init(UI_ANCHOR anchor, POINTFLOAT pos, int width, int height, UI_BAR_TYPE type, string back, string bar);
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	void SetTrackingData(float& lpTargetVar);
	void SetTrackingData(function<float()> lpTargetFunc);
	void SetTrackingMaxData(float& lpTargetVar);
	void SetTrackingMaxData(function<float()> lpTargetFunc);

	float GetVar();
	float GetMaxVar();

	void SetVar(float var);
	void SetRange(float min, float max);
	void ClearFunc();
	void PushBackFunc(function<void(float)> func);
	void PopFunc();

	void OnBeginDrag(EventData& data) override;
	void OnDrag(EventData& data) override;
	void OnEndDrag(EventData& data) override;

	inline void SetTick(float tick) { this->tick = tick; }
	inline float GetTick() { return this->tick; }
	inline int GetVarTick() { return (int)(var / tick + FLT_EPSILON); }
	inline int GetTotalTick() { return (int)((maxVar - minVar) / tick + FLT_EPSILON); }
	inline void SetLabel(UITextField* lpLabel) { this->lpLabel = lpLabel; }
};

