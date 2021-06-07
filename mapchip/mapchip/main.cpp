#include "DxLib.h"
#include <algorithm>

const char TITLE[] = "TestProgram";

//ウィンドウサイズ
const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 640;

//マップチップロード
const int size_map_x = 32;
const int size_map_y = 32;
const int num_chip_x = 10;
const int num_chip_y = 4;
const int num_chip_all = num_chip_x * num_chip_y;
int chip[num_chip_all];

const int num_map_x = 20;
const int num_map_y = 15;
int data[15][20] = {
	// 横20x15
	{10, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 10 },
	{10, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 10 },
	{10, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 10 },
	{10, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 10 },
	{10, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 10 },
	{10, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 10 },
	{10, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 10 },
	{10, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 10 },
	{10, 0, 0, 0, 0,  10, 10, 10, 10, 10,  10, 10, 10, 10, 10,  0, 0, 0, 0, 10 },
	{10, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 10 },
	{10, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 10 },
	{10, 0, 0, 10, 10,  10, 0, 0, 0, 0,  0, 0, 0, 0, 10,  10, 10, 0, 0, 10 },
	{10, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 10 },
	{10, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 10 },
	{10, 10, 10, 10, 10,  10, 10, 10, 10, 10,  10, 10, 10, 10, 10,  10, 10, 10, 10, 10 }
};

int CheckHit(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	/*ウィンドウサイズを手動では変更せず、
	ウィンドウサイズに併せて拡大できないようにする*/
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	//タイトルを変更
	SetMainWindowText(TITLE);

	//画面サイズの最大サイズ,カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	//画像サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	//画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0xAF);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//(ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	//画像などのリソースデータの変数宣言と読み込み
	LoadDivGraph("mapchip.png", num_chip_all, num_chip_x, num_chip_y, size_map_x, size_map_y, chip);

	//ゲームループで使う変数の宣言
	float PlayerX = 250.0f, PlayerY = 250.0f;
	float g = 0.0f;
	//最新のキーボード情報用
	char keys[256] = { 0 };

	//1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	//ゲームループ
	while (1)
	{
		//最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		for (int i = 0; i < 256; ++i)
		{
			oldkeys[i] = keys[i];
		}
		//最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		//画面クリア
		ClearDrawScreen();

		//更新処理
		float old_x = PlayerX;
		float old_y = PlayerY;
		if (keys[KEY_INPUT_UP] == 1)
		{
			PlayerY -= 5;
		}
		if (keys[KEY_INPUT_DOWN] == 1)
		{
			PlayerY += 5;
		}
		if (keys[KEY_INPUT_RIGHT] == 1)
		{
			PlayerX += 5;
		}
		if (keys[KEY_INPUT_LEFT] == 1)
		{
			PlayerX -= 5;
		}
		for (int i = 0; i < num_map_y; i++)
		{
			for (int j = 0; j < num_map_x; j++)
			{
				int no = data[i][j];
				DrawGraph(j * size_map_x, i * size_map_y, chip[no], FALSE);
			}
		}

		if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0)
		{
			g = -5.0f;
		}
		PlayerY += g;
		g += 0.2f;

		for (int i = 0; i < num_map_y; i++)
		{
			for (int j = 0; j < num_map_x; j++)
			{
				int No = data[i][j];
				if (No >= 10 && No < 20)
				{
					if (CheckHit(j * size_map_x,i * size_map_y, size_map_x, size_map_y, PlayerX, PlayerY, 32, 32))
					{
						if (PlayerY >= old_y)
						{
							if(i * size_map_y - 32 >= old_y)
							{
								old_y = i * size_map_y - 32;
								PlayerY = old_y;
								g = 0.0f;
							}
							else
							{
								int _x = (int)max(PlayerX, old_x);
								_x /= size_map_x;
								_x *= size_map_x;
								PlayerX = _x;
								old_x = PlayerX;
							}
						}
						else if (PlayerY < old_y)
						{
							if ((i + 1) * size_map_y <= old_y)
							{
								old_y = (i + 1) * size_map_y;
								PlayerY = old_y;
								g = 0.0f;
							}
							else
							{
								int _x = (int)max(PlayerX, old_x);
								_x /= size_map_x;
								_x *= size_map_x;
								PlayerX = _x;
								old_x = PlayerX;
							}
						}
					}
				}
			}
		}
		//描画処理
		DrawBox(PlayerX, PlayerY, PlayerX + 32, PlayerY + 32, GetColor(255, 255, 255), TRUE);

		//(ダブルバッファ)裏面
		ScreenFlip();

		//20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		//Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1)
		{
			break;
		}

		//ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

int CheckHit(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	int L1 = x1;      // 左
	int R1 = x1 + w1; // 右(左+横幅)
	int L2 = x2;      // 左
	int R2 = x2 + w2; // 右(左+横幅)

	if (R1 < L2) return 0;
	if (R2 < L1) return 0;

	int U1 = y1;      // 上
	int D1 = y1 + h1; // 下(上+縦幅)
	int U2 = y2;      // 上
	int D2 = y2 + h2; // 下(上+縦幅)

	if (D1 < U2) return 0;
	if (D2 < U1) return 0;

	// それ以外の場合は当たっている
	return 1;
}