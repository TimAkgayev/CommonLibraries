#pragma once
#include <DirectXApplication.h>
#include <ObjFile.h>
#include <TriangleMesh.h>
#include <Camera.h>
#include "Joint.h"
#include <Line.h>
#include "Materials.h"


struct CoordinateLocator
{
	Line* x;
	Line* y;
	Line* z;
};



class JointLocator
{
public:
	JointLocator(Joint*, ID3D11Device*);
	void Draw(ID3D11DeviceContext*, const XMFLOAT3&, LineMaterial*);
private:
	Line* mCircleLines;
	Line* mXaxisLine;
	Line* mYaxisLine;
	Line* mZaxisLine;

	Joint* mJoint;

	XMVECTOR mCircleNormal;

};

class MyApp : public DirectXApplication
{
public:
	virtual void Initialize() override;
	virtual void Update(float dt) override;
	virtual void Draw() override;

	virtual LRESULT MessageProc(UINT msg, WPARAM wParam, LPARAM lParam) override;


private:
	ObjFile mTerrainFile;
	TriangleMesh* mTerrainModel;
	Camera mCamera;
	float mMoveSpeed;
	float mTurnSpeed;

	Joint* mJointTree;
	std::vector<JointLocator*> mJointLocators;
	LineMaterial* lineMat;
	SimpleMaterial* simpleMat;

	Line* testLine;

private:
	void mUpdateCamera(float dt);
	void mInitializeMaterials();

};
