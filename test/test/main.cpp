#include "DxLib.h"

const char TITLE[] = "TestProgram";

//ウィンドウサイズ
const int WIN_WIDTH = 600;
const int WIN_HEIGHT = 600;

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


	//ゲームループで使う変数の宣言
	int PlayerX = 250, PlayerY = 250;
	int EnemyX = 10, EnemyY = 10;

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

		if (PlayerX > 600)
		{
			PlayerX = -32;
		}
		if (PlayerX + 32 < 0)
		{
			PlayerX = 595;
		}
		if (PlayerY + 32 < 0)
		{
			PlayerY = 395;
		}
		if (PlayerY > 400)
		{
			PlayerY = -32;
		}
		//描画処理
		DrawBox(PlayerX, PlayerY, PlayerX + 32, PlayerY + 32, GetColor(255, 255, 255), TRUE);
		DrawBox(0, 400, 600, 600, GetColor(0, 0, 0), TRUE);
		DrawBox(EnemyX, EnemyY, EnemyX + 32, EnemyY + 32, GetColor(255, 0, 0),TRUE);
		DrawFormatString(0, 410, GetColor(255, 255, 255),
			"(左上 PlayerX: %3dpx, PlayerY: %3dpx)", PlayerX, PlayerY);
		DrawFormatString(0, 430, GetColor(255, 255, 255),
			"(右上 PlayerX: %3dpx, PlayerY: %3dpx)", PlayerX + 32, PlayerY);
		DrawFormatString(0, 450, GetColor(255, 255, 255),
			"(左下 PlayerX: %3dpx, PlayerY: %3dpx)", PlayerX, PlayerY + 32);
		DrawFormatString(0, 470, GetColor(255, 255, 255),
			"(右下 PlayerX: %3dpx, PlayerY: %3dpx)", PlayerX + 32, PlayerY + 32);
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