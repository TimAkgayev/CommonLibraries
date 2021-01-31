#include "Main.h"
#include "Shaders.h"
#include "Joint.h"
#include <MeshGenerator.h>
#include "Materials.h"
#include "RasterizerStates.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MyApp app;

	app.CreateAppWindow(hInstance, 800, 600, L"Articulated Figure");

	return app.RunLoop();
}



LRESULT MyApp::MessageProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mAppPaused = true;
			mTimer.stop();
		}
		else
		{
			mAppPaused = false;
			mTimer.start();
		}
		return 0;
	}


	// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	}

	return DefWindowProc(mMainWindow, msg, wParam, lParam);

}

void MyApp::mUpdateCamera(float dt)
{

	XMVECTOR moveDirLook = XMVectorSet(mCamera.GetLookAtVector().x, 0, mCamera.GetLookAtVector().z, 1.0f);
	XMVECTOR moveDirRight = XMVectorSet(mCamera.GetRightVector().x, 0, mCamera.GetRightVector().z, 1.0f);
	XMVECTOR moveDirUp = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	if (GetAsyncKeyState(0x57)) //W key
		mCamera.Move(dt*moveDirLook * -1 * mMoveSpeed);

	if (GetAsyncKeyState(0x53)) //S key
		mCamera.Move(dt* moveDirLook*mMoveSpeed);


	if (GetAsyncKeyState(0x41)) //A key
		mCamera.Move(dt*moveDirRight * -1 * mMoveSpeed);


	if (GetAsyncKeyState(0x44)) //D key
		mCamera.Move(dt*moveDirRight*mMoveSpeed);



	if (GetAsyncKeyState(VK_UP))
		mCamera.Pitch(-1 * dt*mTurnSpeed);


	if (GetAsyncKeyState(VK_DOWN))
		mCamera.Pitch(1 * dt*mTurnSpeed);


	if (GetAsyncKeyState(VK_LEFT))
		mCamera.Yaw(-1 * dt*mTurnSpeed);


	if (GetAsyncKeyState(VK_RIGHT))
		mCamera.Yaw(1 * dt*mTurnSpeed);


	//page up and page down
	if (GetAsyncKeyState(VK_PRIOR))
		mCamera.Move(dt*moveDirUp*mMoveSpeed);

	if (GetAsyncKeyState(VK_NEXT))
		mCamera.Move(dt* -1 * moveDirUp*mMoveSpeed);

	
}


void MyApp::Initialize()
{
	//initializing DirectX
	mCreateSwapChainAndDevice();
	mCreateRenderTargetsAndDepthBuffers();
	mCreateViewport();

	//initialize the effects 
	mInitializeMaterials();
	
	
	//load the level
	mTerrainFile.LoadFile(mDevice, "FlatTerrain.obj");
	GenericObject terrain = ObjParser::ExtractIfSingle(mTerrainFile);
	mTerrainModel = new TriangleMesh(mDevice, terrain);

	XMFLOAT3 camPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mCamera.SetPosition(camPos);

	mMoveSpeed = 10.0f;
	mTurnSpeed = 200.0f;


	//create the joint tree
	Joint* rootJoint = new Joint;
	Joint* childJoint = new Joint;
	rootJoint->parentJoint = nullptr;
	rootJoint->childJoint = childJoint;
	
	rootJoint->position = XMFLOAT3(0.0f, 30.0f, 0.0f);
	XMFLOAT3 xb = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3 yb = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 zb = XMFLOAT3(0.0f, 0.0f, 1.0f);

	rootJoint->x_basis = XMLoadFloat3(&xb);
	rootJoint->y_basis = XMLoadFloat3(&yb);
	rootJoint->z_basis = XMLoadFloat3(&zb);

	childJoint->childJoint = nullptr;
	childJoint->parentJoint = rootJoint;
	childJoint->position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 xbc = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3 ybc = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 zbc = XMFLOAT3(0.0f, 0.0f, 1.0f);

	childJoint->x_basis = XMLoadFloat3(&xbc);
	childJoint->y_basis = XMLoadFloat3(&ybc);
	childJoint->z_basis = XMLoadFloat3(&zbc);

	mJointTree = rootJoint;

	//iterate over tree and create display shapes
	std::vector<Joint*> stack;
	stack.push_back(mJointTree);

	for (UINT i = 0; i < stack.size(); i++)
	{
		if (stack[i]->childJoint)
			stack.push_back(stack[i]->childJoint);

		JointLocator* l = new JointLocator(stack[i], mDevice);
		mJointLocators.push_back(l);
	}

	testLine = new Line(mDevice, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(10.0f, 10.0f, 10.0f));

}

void MyApp::Update(float dt)
{
	XMFLOAT3 c = mCamera.GetPosition();
	lineMat->SetLineWidthAndCameraPosition(mContext, 0.05f, c);
	
	//update our view matrices
	Shaders::ConstantBuffers::Common::SetViewProjectionMatrix(mContext, mCamera.GetViewMatrix(), mCamera.GetPerspectiveProjectionMatrix());

	

	mUpdateCamera(dt);
}

void MyApp::Draw()
{

	mContext->RSSetViewports(1, &mViewport);
	mContext->OMSetRenderTargets(1, &mBackBufferView, mDepthStencilView);
	mContext->OMSetDepthStencilState(mDepthStencilState, 0);

	float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
	mContext->ClearRenderTargetView(mBackBufferView, clearColor);
	mContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1, 0);

	simpleMat->StartMaterialPass(mContext);
	mTerrainModel->Draw(mContext);
	simpleMat->EndMaterialPass(mContext);


	lineMat->StartMaterialPass(mContext);
	for (auto j : mJointLocators)
		j->Draw(mContext, mCamera.GetPosition(), lineMat);

	lineMat->EndMaterialPass(mContext);

	mSwapChain->Present(0, 0);
}




void MyApp::mInitializeMaterials()
{
	Shaders::CreateShadersAndInputLayouts(mDevice);

	//initilize common shader matricies
	Shaders::ConstantBuffers::Common::SetViewProjectionMatrix(mContext, mCamera.GetViewMatrix(), mCamera.GetPerspectiveProjectionMatrix());

	RasterizerStates::CreateRasterizerStates(mDevice);

	lineMat = new LineMaterial();
	simpleMat = new SimpleMaterial();

	XMMATRIX iden = XMMatrixIdentity();
	XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	lineMat->SetWorldPosition(mContext, pos);
	lineMat->SetWorldRotation(mContext, iden);
	simpleMat->SetWorldPosition(mContext, iden);

}

JointLocator::JointLocator(Joint * j, ID3D11Device* device)
{
	mJoint = j;

	std::vector<LineVertex> vb;
	std::vector<UINT> ib;

	//create the circle
	MeshGenerator::Circle(1, 20, vb, ib);
	mCircleLines = new Line(device, vb, ib);

	//create the axes
	XMFLOAT3 xbasisF, ybasisF, zbasisF;
	XMStoreFloat3(&xbasisF, mJoint->x_basis);
	XMStoreFloat3(&ybasisF, mJoint->y_basis);
	XMStoreFloat3(&zbasisF, mJoint->z_basis);
	mXaxisLine = new Line(device, XMFLOAT3(0.0f, 0.0f, 0.0f), xbasisF, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	mYaxisLine = new Line(device, XMFLOAT3(0.0f, 0.0f, 0.0f), ybasisF, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	mZaxisLine = new Line(device, XMFLOAT3(0.0f, 0.0f, 0.0f), zbasisF, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

	//generate the normal vector for the circle
	XMVECTOR v1 = XMLoadFloat3(&vb[0].pos);
	XMVECTOR v2 = XMLoadFloat3(&vb[1].pos);
	XMVECTOR v3 = XMLoadFloat3(&vb[2].pos);
	XMVECTOR dv1 = v2 - v1;
	XMVECTOR dv2 = v3 - v1;
	mCircleNormal = XMVector3Normalize(XMVector3Cross(dv1, dv2));
 
}

void JointLocator::Draw(ID3D11DeviceContext* context, const XMFLOAT3& camPos, LineMaterial* mat)
{
	//update the axis cube
	XMFLOAT3 axisCubeTranslation = XMFLOAT3(mJoint->position.x, mJoint->position.y, mJoint->position.z);
	mat->SetWorldPosition(context, axisCubeTranslation);
	XMMATRIX ident = XMMatrixIdentity();
	mat->SetWorldRotation(context, ident);
	XMFLOAT3 camPos2 = camPos;
	mat->SetLineWidthAndCameraPosition(context, 0.05f, camPos2);

	//draw the axis cube
	mXaxisLine->Draw(context);
	mYaxisLine->Draw(context);
	mZaxisLine->Draw(context);


	//get the direction we should be facing
	XMVECTOR camPosV = XMLoadFloat3(&camPos);
	XMVECTOR jointPosV = XMLoadFloat3(&mJoint->position);
	XMVECTOR dirV = XMVector3Normalize(camPosV - jointPosV);

	//get the angle between normal and direction vectors
	XMVECTOR dirVLen = XMVector3Length(dirV);
	XMFLOAT3 dirVLenF;
	XMStoreFloat3(&dirVLenF, dirVLen);

	//make sure a direction vector exists (player is probably standing on the locator) 
	XMMATRIX rotationMatrix = XMMatrixIdentity();
	if (dirVLenF.x != 0)
	{
		XMVECTOR angle = XMVector3AngleBetweenVectors(dirV, mCircleNormal);
		XMFLOAT3 angleF;
		XMStoreFloat3(&angleF, angle);

		//if the angle is not zero re-orient the circle to face the camera
		
		if (angleF.x != 0)
		{
			//get the normal of a rotation plane
			XMVECTOR rotationAxis = XMVector3Cross(mCircleNormal, dirV);

			//make sure rotation vector exists (otherwise normal and direction are in opposite directions)

			//generate the rotation matrix around the plane normal
			rotationMatrix = XMMatrixRotationAxis(rotationAxis, angleF.x);

		}
	}

	//rotate and position the circle 
	XMFLOAT3 translationF = XMFLOAT3(mJoint->position.x, mJoint->position.y, mJoint->position.z);
	mat->SetWorldPosition(context, translationF);
	mat->SetWorldRotation(context, rotationMatrix);
	mat->SetLineWidthAndCameraPosition(context, 0.05f, camPos2);
	mCircleLines->Draw(context);
}
