#include "UI.h"
#include "Resource.h"
#include "easing.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

GameUI provisionalGameUI;

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/

void GameUI::Init()
{
	sprGameUI = RESOURCE->GetSpriteData(Resource::Texture::UI);
}

void GameUI::Uninit()
{

}

void GameUI::Update()
{
	ShowChainNumUpdate();
}

void GameUI::Draw()
{
	DirectX::XMFLOAT2 basicSize(36, 60);
	DirectX::XMFLOAT2 basicTex(0, 696);
	DirectX::XMFLOAT2 basicCenter(0.0f, 0.0f);
	DirectX::XMFLOAT4 basicColor(1.0f, 1.0f, 1.0f, 1.0f);


	sprGameUI->Begin();

	// スコア
	{
		DirectX::XMFLOAT2 scorePos = DirectX::XMFLOAT2(1463.0f, 879.0f);
		TexData scoreBoard{};
		scoreBoard.pos = DirectX::XMFLOAT2(1416.0f, 774.0f);
		scoreBoard.tex = DirectX::XMFLOAT2(0.0f, 312.0f);
		scoreBoard.size = DirectX::XMFLOAT2(312.0f, 192.0f);


		int hundredThousand = totalScore % 1000000 / 100000;
		int tenThousand = totalScore % 100000 / 10000;
		int thousand = totalScore % 10000 / 1000;
		int hundred = totalScore % 1000 / 100;
		int ten = totalScore % 100 / 10;
		int one = totalScore % 10 / 1;

		// Board
		sprGameUI->Draw(scoreBoard.pos, scoreBoard.size, scoreBoard.tex, scoreBoard.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, basicColor);

		// 十万
		sprGameUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 0, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(basicTex.x + 36 * hundredThousand/*ここに桁入れる*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
		// 万
		sprGameUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 1, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(basicTex.x + 36 * tenThousand/*ここに桁入れる*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
		// 千
		sprGameUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 2, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(basicTex.x + 36 * thousand/*ここに桁入れる*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
		// 百
		sprGameUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 3, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(basicTex.x + 36 * hundred/*ここに桁入れる*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
		// 十
		sprGameUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 4, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(basicTex.x + 36 * ten/*ここに桁入れる*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
		// 一
		sprGameUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 5, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(basicTex.x + 36 * one/*ここに桁入れる*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
	}

	// チェイン数
	{
		if (isShowChainNum)
		{
			//nowChainNum = 10;// TODO : test用の仮変数
			basicTex = DirectX::XMFLOAT2(0, 816);
			basicColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, showChainAlpha);

			if (nowChainNum < 10)
			{
				// コンボ数が二桁未満
				// 一
				sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + ADJUST + SINGLE_CORRECTION_X, showChainNumPos.y), DirectX::XMFLOAT2(basicSize.x * 2, basicSize.y * 2), DirectX::XMFLOAT2(basicTex.x + 36 * nowChainNum, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
				// 連！
				sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + 81 + ADJUST + SINGLE_CORRECTION_X, showChainNumPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(basicTex.x, basicTex.y + 60), DirectX::XMFLOAT2(66, 60), basicCenter, 0.0f, basicColor);//2
			}
			else
			{
				// コンボが二桁以上
				int ten = nowChainNum % 100 / 10;
				int one = nowChainNum % 10 / 1;

				// 十
				sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + ADJUST + SINGLE_CORRECTION_X, showChainNumPos.y), DirectX::XMFLOAT2(basicSize.x * 2, basicSize.y * 2), DirectX::XMFLOAT2(basicTex.x + 36 * ten, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);//1
				// 一
				sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + 81 + ADJUST + SINGLE_CORRECTION_X, showChainNumPos.y), DirectX::XMFLOAT2(basicSize.x * 2, basicSize.y * 2), DirectX::XMFLOAT2(basicTex.x + 36 * one, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);//2
				//連！
				sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + 162 + ADJUST + SINGLE_CORRECTION_X, showChainNumPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(basicTex.x, basicTex.y + 60), DirectX::XMFLOAT2(66, 60), basicCenter, 0.0f, basicColor);

			}
		}
	}

	sprGameUI->End();

}

void GameUI::SetShowChainNumInit(DirectX::XMFLOAT2 _pos)
{
	showChainNumPos = _pos;
	showChainNumPos.y += 57;
	showChainTimer = 0;
	showChainAlpha = 0.0f;
	showChainNumFirstPos = showChainNumPos.y;

	isShowChainNum = true;
}

void GameUI::ShowChainNumUpdate()
{
	if (!isShowChainNum) return;

	if (showChainTimer < 60)
	{
		if (showChainTimer <= 10)
		{
			showChainAlpha += 0.1f;
		}
		showChainNumPos.y = easing::OutExp(static_cast<float>(showChainTimer), 60.0f, showChainNumFirstPos - 57.0f, showChainNumFirstPos);
		++showChainTimer;
	}
	else
	{
		showChainAlpha -= 0.1f;
		if (showChainAlpha <= 0.0f)
		{
			showChainAlpha = 0.0f;
		}
		++showChainTimer;
	}

	if (showChainTimer >= 120)
	{
		showChainTimer = 0;
		isShowChainNum = false;
	}
}

void GameUI::CalcScore(int _eraseNum, int _chainCount)
{
	float nowPlusScore{};

	// 6 コンボ以上は基礎点が変わらないため
	if (_chainCount >= 5)
	{
		_chainCount = 5;
	}

	if (_eraseNum - 2 == 0)
	{
		nowPlusScore = static_cast<float>(baseScore_tbl[_chainCount]);
	}
	else
	{
		nowPlusScore = baseScore_tbl[_chainCount] * (_eraseNum - 2) * 1.5f;
	}

	totalScore += nowPlusScore;
}
