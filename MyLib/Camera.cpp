#include	"Camera.h"
#include	"framework.h"

Camera camera;

/*************************************************************************
	Camera�N���X
*************************************************************************/
Camera::Camera()
{
	pos = DirectX::XMFLOAT3(0.0f, 0.0f,-5.0f);
	target = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	angle = 0.0f;

	float	fov = DirectX::XMConvertToRadians(30.f);
	float	aspect = static_cast<float>(framework::SCREEN_WIDTH) / static_cast<float>(framework::SCREEN_HEIGHT);
	SetPerspectiveMatrix(fov, aspect, 0.1f, 1000.0f);

	mode = MODE::WATCH;
}

//���s���e
DirectX::XMMATRIX	Camera::SetOrthographicMatrix(float width, float height, float zNear, float zFar)
{
	projection = DirectX::XMMatrixOrthographicLH(width, height, zNear, zFar);
	return projection;
}

//�������e
DirectX::XMMATRIX	Camera::SetPerspectiveMatrix(float fov, float aspect, float zNear, float zFar)
{
	projection = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, zNear, zFar);
	return projection;
}

//�r���[�s��擾
DirectX::XMMATRIX	Camera::GetViewMatrix()
{
	DirectX::XMVECTOR	p = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
	DirectX::XMVECTOR	t = DirectX::XMVectorSet(target.x, target.y, target.z, 1.0f);
	DirectX::XMVECTOR	up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	return DirectX::XMMatrixLookAtLH(p, t, up);
}

//�X�V�֐�
void		Camera::Update()
{
	switch (mode)
	{
	case WATCH:		Watch();		break;		//	�Ď��J����
	}
}

//�Ď��J�����֐�
void		Camera::Watch()
{

	static float dist = 10;

	if (GetAsyncKeyState('W')<0)
		dist -= 0.02f;
	else if (GetAsyncKeyState('A')<0)
		angle -= 0.4f;
	else if (GetAsyncKeyState('S')<0)
		dist += 0.02f;
	else if (GetAsyncKeyState('D') < 0)
		angle += 0.4f;

	if (dist <= 1.0f)dist = 1.0f;

	pos.x = sinf(angle*0.01745f)*dist;
	pos.z = cosf(angle*0.01745f)*-dist;

	if (GetAsyncKeyState('J')<0)
		pos.y += 0.05f;
	else if (GetAsyncKeyState('K')<0)
		pos.y -= 0.05f;
}