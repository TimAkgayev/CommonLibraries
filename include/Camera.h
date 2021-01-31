#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();

	void Move(XMFLOAT3 dir);
	void Move(XMVECTOR dir);

	void MoveAlongLookAt(float amount);
	void MoveAlongRight(float amount);
	void MoveAlongUp(float amount);


	XMFLOAT3 GetLookAtVector() const;
	XMFLOAT3 GetRightVector() const;
	XMFLOAT3 GetUpVector() const;

	XMMATRIX GetViewTranslationMatrix() const;
	XMMATRIX GetViewRotationMatrix() const;
	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetPerspectiveProjectionMatrix() const;
	XMMATRIX GetOrthographicProjectionMatrix(float viewWidth, float viewHeight) const;
	XMFLOAT3 GetPosition() const;


	void Roll(float degrees);
	XMMATRIX Pitch(float degrees);
	XMMATRIX Yaw(float degrees);


	virtual void SetPosition(XMFLOAT3& position);

	virtual void Lock();
	virtual void Unlock();

protected:

	bool mIsLocked;
	XMVECTOR mUp;
	XMVECTOR mRight;
	XMVECTOR mLookAt;
	XMVECTOR mPosition;
	XMMATRIX mProjectionMatrix;
};