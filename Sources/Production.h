#ifndef _PRODUCTION_H_
#define _PRODUCTION_H_

#include <memory>
#include "sprite.h"
#include"Scene.h"
#include"Singleton.h"


class Production:public Singleton<Production>
{
public:
	std::shared_ptr<SpriteBatch> sprCurtain;
	std::shared_ptr<SpriteBatch> sprDescriptionRule;
    float           time;           //イージング処理に使う変数
    int             state;          //状態
	int				tutorialState;	//チュートリアル描画状態
	float			tutorialTime;	//チュートリアルの経過時間
	bool			tutorialNext;	//次へを表示する
    unsigned char   checkFlag;      //フラグ管理用変数
	float			easingPosX1;	//左の座標
	float			easingPosX2;	//右の座標
	float			tutorialPosY;	//チュートリアルの枠Y座標
	float			tutorialAlpha;	//ボードのアルファ値

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

    void        Init();         //初期化
    void        Start();        //切り替えの始めに呼び出す
    void        End();          //イージング終了時に出す
    void        Draw();         //描画処理

    /*各種関数*/
    void        CSOH(int, bool _isDisplayTutorial = false);					//シーン切り替え処理(縦のOutExpイージング)
    bool        CheckFlag(unsigned char);							//フラグが立っているかを確認する
    void        SetOn(unsigned char flag) { checkFlag |= flag; }	//フラグを立てる
    void        SetOff(unsigned char flag) { checkFlag &= ~flag; }	//フラグをoffる


    /*使ってない's*/
    void        Update(){}      //（更新処理）使ってない
    void        Uninit() {}     //（終了処理）使ってない


};

//インスタンス取得マクロ
#define PRODUCTION Production::getInstance()


//フラグ定数
#define GO_TITLE		0b00000001		//タイトル画面に以降する
#define GO_PLAYERSTILE	0b00000010		//シングルかマルチか決める
#define GO_SINGLEGAME	0b00000100		//ゲーム画面に以降する シングル
#define GO_CPUGAME		0b00001000		//ゲーム画面に以降する CPU
#define GO_MULTIGAME	0b00010000		//ゲーム画面に以降する マルチ
#define GO_TUTORIAL		0b00100000		//チュートリアル画面に以降する

#endif //_PRODUCTION_H_
