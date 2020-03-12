#ifndef _CAMERA_H_
#define _CAMERA_H_

#include	<DirectXMath.h>

class Camera
{
public:
	enum MODE
	{
		WATCH,				//	�Ď��J����	
	};

private:
	int					mode;		//	�J�����̏��

public:
	DirectX::XMFLOAT3	pos;		//	�ʒu
	DirectX::XMFLOAT3	target;		//	�����_
	DirectX::XMMATRIX	projection;	//	���e�s��
	float				angle;		//	�J�����̊p�x

public:
	Camera();
	void				Update();	//	�X�V�֐�

private:
	void				Watch();	//	�Ď��J����

public:
	DirectX::XMMATRIX	SetOrthographicMatrix(float width, float height, float zNear, float zFar);			//	���s���e�s��ݒ�֐�
	DirectX::XMMATRIX	SetPerspectiveMatrix(float fov, float aspect, float zNear, float zFar);				//	�������e�s��ݒ�֐�
	DirectX::XMMATRIX	GetViewMatrix();																	//	�r���[�s��擾�֐�
	DirectX::XMMATRIX	GetProjectionMatrix() { return	projection; }										//	���e�s��擾�֐�
	
};

extern Camera camera;


#endif //!_CAMERA_H_
