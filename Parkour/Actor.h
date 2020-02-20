#pragma once
#include <vector>
#include <SDL/SDL_stdinc.h>
#include "Math.h"
#include <string>

enum class ActorState
{
	Active,
	Paused,
	Destroy
};

class Actor
{
public:
	Actor(class Game* game, Actor* parent = nullptr);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const Uint8* keyState);

	// Getters/setters
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }
	
	ActorState GetState() const { return mState; }
	void SetState(ActorState state) { mState = state; }

	class Game* GetGame() { return mGame; }
    
    Quaternion GetQua(){ return mQua; }
    void SetQua(Quaternion qua){ mQua = qua; }
    Vector3 GetQuatForwar();
    
    void CalcWorldTransform();
    Matrix4 CalcWorldRotTrans();
    
    Vector3 GetWorldPosition();
    Vector3 GetWorldForward();
    
    Vector3 GetForward(){ return Vector3(cos(mRotation), sin(mRotation), 0.0f); }
    
    const Matrix4& GetWorldTransform() const {return mWorldTransform; }
    
    Vector3 GetRight(){ return Vector3(cos(mRotation + Math::PiOver2), sin(mRotation + Math::PiOver2), 0.0f); }
    
    void AddChild(Actor* child){ mChildren.emplace_back(child); }
    void RemoveChild(Actor* child);
    
    Actor* GetParent(){return mParent;}
	
	// Adds component to Actor (this is automatically called
	// in the component constructor)
	void AddComponent(class Component* c);
	
	// Returns component of type T, or null if doesn't exist
	template <typename T>
	T* GetComponent() const
	{
		for (auto c : mComponents)
		{
			T* t = dynamic_cast<T*>(c);
			if (t != nullptr)
			{
				return t;
			}
		}
		
		return nullptr;
	}
protected:
	// Any actor-specific update code (overridable)
	virtual void OnUpdate(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void OnProcessInput(const Uint8* keyState);
	
	class Game* mGame;
	// Actor's state
	ActorState mState;

	// Transform
	Vector3 mPosition;
	float mScale;
	float mRotation;
	
	// Components
	std::vector<class Component*> mComponents;
    
    Matrix4 mWorldTransform;
    
    Quaternion mQua;
    
    class Actor* mParent;
    std::vector<class Actor*> mChildren;
    bool mInheritScale;
    
    void Set_Mesh(const std::string& filename, bool alpha = false);
};
