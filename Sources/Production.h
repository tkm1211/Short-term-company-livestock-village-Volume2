#ifndef _PRODUCTION_H_
#define _PRODUCTION_H_

#include <memory>
#include "sprite.h"
#include"Scene.h"
#include"Singleton.h"


class Production:public Singleton<Production>
{
public:
	std::unique_ptr<SpriteBatch> sprBlackCurtainL;
	std::unique_ptr<SpriteBatch> sprBlackCurtainR;
	std::unique_ptr<SpriteBatch> sprRule;
    float           time;           //�C�[�W���O�����Ɏg���ϐ�
    int             state;          //���
	int				tutorialState;	//�`���[�g���A���`����
	float			tutorialTime;	//�`���[�g���A���̌o�ߎ���
	bool			tutorialNext;	//���ւ�\������
    unsigned char   checkFlag;      //�t���O�Ǘ��p�ϐ�
	float			easingPosX1;	//���̍��W
	float			easingPosX2;	//�E�̍��W
	float			tutorialPosY;	//�`���[�g���A���̘gY���W
	float			tutorialAlpha;	//�{�[�h�̃A���t�@�l

    enum
    {
        INITIALIZE,
        FIRST_EASING,
        CHANGE,
        INTERBAL,
        SECOND_EASING,
        END,
    };
	enum
	{
		INITIALIZE_T,
		FIRST_EASING_T,
		INTERBAL_T,
		SECOND_EASING_T,
		CHANGE_T,
	};

public:

    void        Init();         //������
    void        Start();        //�؂�ւ��̎n�߂ɌĂяo��
    void        End();          //�C�[�W���O�I�����ɏo��
    void        Draw();         //�`�揈��

    /*�e��֐�*/
    void        CSOH(int, bool _isDisplayTutorial = false);					//�V�[���؂�ւ�����(�c��OutExp�C�[�W���O)
    bool        CheckFlag(unsigned char);							//�t���O�������Ă��邩���m�F����
    void        SetOn(unsigned char flag) { checkFlag |= flag; }	//�t���O�𗧂Ă�
    void        SetOff(unsigned char flag) { checkFlag &= ~flag; }	//�t���O��off��


    /*�g���ĂȂ�'s*/
    void        Update(){}      //�i�X�V�����j�g���ĂȂ�
    void        Uninit() {}     //�i�I�������j�g���ĂȂ�


};

//�C���X�^���X�擾�}�N��
#define PRODUCTION Production::getInstance()


//�t���O�萔
#define GO_TITLE		0b00000001		//�^�C�g����ʂɈȍ~����
#define GO_PLAYERSTILE	0b00000010		//�V���O�����}���`�����߂�
#define GO_GAME			0b00000100		//�Q�[����ʂɈȍ~����
#define GO_TUTORIAL		0b00001000		//�`���[�g���A����ʂɈȍ~����

#endif //_PRODUCTION_H_
