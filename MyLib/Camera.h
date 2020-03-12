#ifndef _CAMERA_H_
#define _CAMERA_H_

#include	<DirectXMath.h>

class Camera
{
public:
	enum MODE
	{
		WATCH,				//	監視カメラ	
	};

private:
	int					mode;		//	カメラの状態

public:
	DirectX::XMFLOAT3	pos;		//	位置
	DirectX::XMFLOAT3	target;		//	注視点
	DirectX::XMMATRIX	projection;	//	投影行列
	float				angle;		//	カメラの角度

public:
	Camera();
	void				Update();	//	更新関数

private:
	void				Watch();	//	監視カメラ

public:
	DirectX::XMMATRIX	SetOrthographicMatrix(float width, float height, float zNear, float zFar);			//	平行投影行列設定関数
	DirectX::XMMATRIX	SetPerspectiveMatrix(float fov, float aspect, float zNear, float zFar);				//	透視投影行列設定関数
	DirectX::XMMATRIX	GetViewMatrix();																	//	ビュー行列取得関数
	DirectX::XMMATRIX	GetProjectionMatrix() { return	projection; }										//	投影行列取得関数
	
};

extern Camera camera;


#endif //!_CAMERA_H_
