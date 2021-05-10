#pragma once
template <typename T>
class Singleton
{
protected:
	static T* lpInstance;

protected:
	Singleton() {};
	virtual ~Singleton() {};

public:
	static T* GetSingleton();
	void ReleaseSingleton();
};

template <typename T>
T* Singleton<T> ::lpInstance = nullptr;

template<typename T>
inline T* Singleton<T> ::GetSingleton()
{
	if (lpInstance == nullptr)
	{
		lpInstance = new T;
	}
	return lpInstance;
}

template<typename T>
inline void Singleton<T>::ReleaseSingleton()
{
	if (lpInstance)
	{
		delete lpInstance;
		lpInstance = nullptr;
	}
}