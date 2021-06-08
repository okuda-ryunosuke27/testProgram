#include "DxLib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
const char TITLE[] = "TestProgram";

//ウィンドウサイズ
const int WIN_WIDTH = 600;
const int WIN_HEIGHT = 600;

//ゲームループで使う変数の宣言
typedef struct {
	int PlayerX;
	int PlayerY;
}Player;
typedef struct {
	int EnemyX;
	int EnemyY;
	int EnemyR;
	int isEnemyAlive;
	int enemyTymer;
	int Speed;
}Enemy;
typedef struct {
	int EnemyX;
	int EnemyY;
	int EnemyR;
	int isEnemyAlive;
	int enemyTymer;
	int Speed;
}Enemy2;
typedef struct {
	int bulX;
	int bulY;
	int bulR;
	int isbulFlag;
}Bullet;
typedef struct {
	int bulX;
	int bulY;
	int bulR;
	int isbulFlag;
}Bullet2;
typedef struct {
	int bulX;
	int bulY;
	int bulR;
	int isbulFlag;
}Bullet3;
Player play;
Enemy enemy;
Enemy2 enemy2;
Bullet bul;
Bullet2 bul2;
Bullet3 bul3;


int E_getpos, E_getpos2, E_getpos3;
int E_getpos4, E_getpos5, E_getpos6;
int enemycount = 0;
int timer = 0;

int bgX = 0, bgY = 0;
int bg2X = 0, bg2Y = -400;
int rad;
int scene = 0;

//最新のキーボード情報用
char keys[256] = { 0 };
//1ループ(フレーム)前のキーボード情報
char oldkeys[256] = { 0 };

void (*pl)();
void (*en)();
void (*bu)();
void PlayerUpdata();
void EnemyUpdata();
void BulletUpdata();
void Initialize();
// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
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
	SetBackgroundColor(0x00, 0x00, 0x00);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//(ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	//画像などのリソースデータの変数宣言と読み込み
	int back1 = LoadGraph("background.png");
	int back2 = LoadGraph("background2.png");
	int player = LoadGraph("ico.jpg");
	int title = LoadGraph("title.png");
	int ent = LoadGraph("Enter.png");

	srand(time(NULL));
	Initialize();

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
		if (scene == 0)
		{
			DrawGraph(0, 0, title, TRUE);
			
			timer++;
			if (timer % 30)
			{
				DrawGraph(160, 200, ent, TRUE);
			}
			if (keys[KEY_INPUT_RETURN] == 1 && oldkeys[KEY_INPUT_RETURN] == 0)
			{
				scene = 1;
			}
			
		}
		if (scene == 1)
		{
			pl = PlayerUpdata;
			en = EnemyUpdata;
			bu = BulletUpdata;
			(*pl)();
			(*en)();
			(*bu)();

			//パックマン壁端移動
			/*if (PlayerX > 600)
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
			}*/
			timer--;
			if (timer <= 0)
			{
				scene = 2;
			}
			
			//描画処理
			//背景スクロール
			DrawGraph(bgX, bgY, back1, FALSE);
			DrawGraph(bg2X, bg2Y, back2, FALSE);
			bgY += 2;
			bg2Y += 2;
			if (bgY > 400)
			{
				bgY = -395;
			}
			if (bg2Y > 400)
			{
				bg2Y = -395;
			}

			//実機,プレイヤー描画
			DrawCircle(bul.bulX, bul.bulY, bul.bulR, GetColor(255, 255, 255), TRUE);
			DrawCircle(bul2.bulX, bul2.bulY, bul2.bulR, GetColor(255, 255, 255), TRUE);
			DrawCircle(bul3.bulX, bul3.bulY, bul3.bulR, GetColor(255, 255, 255), TRUE);

			DrawGraph(play.PlayerX, play.PlayerY, player, TRUE);
			//DrawBox(PlayerX, PlayerY, PlayerX + 32, PlayerY + 32, GetColor(255, 255, 255), TRUE);
			if (enemy.isEnemyAlive == 1)
			{
				DrawBox(enemy.EnemyX, enemy.EnemyY, enemy.EnemyX + 32, enemy.EnemyY + 32, GetColor(255, 0, 0), TRUE);
			}
			if (enemy2.isEnemyAlive == 1)
			{
				DrawCircle(enemy2.EnemyX, enemy2.EnemyY, enemy2.EnemyR, GetColor(255, 255, 0), TRUE);
			}
			//デバック
			DrawBox(0, 400, 600, 600, GetColor(0, 0, 0), TRUE);
			DrawFormatString(0, 410, GetColor(255, 255, 255),
				"(左上 PlayerX: %3dpx, PlayerY: %3dpx)", play.PlayerX, play.PlayerY);
			DrawFormatString(0, 430, GetColor(255, 255, 255),
				"(右上 PlayerX: %3dpx, PlayerY: %3dpx)", play.PlayerX + 32, play.PlayerY);
			DrawFormatString(0, 450, GetColor(255, 255, 255),
				"(左下 PlayerX: %3dpx, PlayerY: %3dpx)", play.PlayerX, play.PlayerY + 32);
			DrawFormatString(0, 470, GetColor(255, 255, 255),
				"(右下 PlayerX: %3dpx, PlayerY: %3dpx)", play.PlayerX + 32, play.PlayerY + 32);
			DrawFormatString(0, 490, GetColor(255, 255, 255),
				"(Enemy1:X%3dpx,Y%3dpx)", enemy.EnemyX, enemy.EnemyY);
			DrawFormatString(0, 510, GetColor(255, 255, 255),
				"(Enemy2:X%3dpx,Y%3dpx)", enemy2.EnemyX, enemy2.EnemyY);
			DrawFormatString(0, 530, GetColor(255, 255, 255),
				"背景:Y%3dpx", bgY);
			DrawFormatString(110, 530, GetColor(255, 255, 255),
				"背景:Y%3dpx", bg2Y);
			DrawFormatString(230, 530, GetColor(255, 255, 255),
				"復活カウント:%3d", enemy.enemyTymer);
			DrawFormatString(0, 550, GetColor(255, 255, 255),
				"score:%3dpt", enemycount);
			DrawFormatString(110, 550, GetColor(255, 255, 255),
				"score:%3dpt", timer);
		}
		if (scene == 2)
		{
			DrawGraph(0, 0, back1, TRUE);
			SetFontSize(20);
			DrawFormatString(220, 200, GetColor(255, 255, 255),
				"スコアは：%3d点", enemycount);
			if (keys[KEY_INPUT_RETURN] == 1)
			{
				Initialize();
				scene = 0;
				enemycount = 0;
			}
		}
	
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
	
	DxLib_End();// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
void PlayerUpdata()
{
	if (keys[KEY_INPUT_UP] == 1 || keys[KEY_INPUT_W] == 1)
	{
		play.PlayerY -= 5;
	}
	if (keys[KEY_INPUT_DOWN] == 1 || keys[KEY_INPUT_S] == 1)
	{
		play.PlayerY += 5;
	}
	if (keys[KEY_INPUT_RIGHT] == 1 || keys[KEY_INPUT_D] == 1)
	{
		play.PlayerX += 5;
	}
	if (keys[KEY_INPUT_LEFT] == 1 || keys[KEY_INPUT_A] == 1)
	{
		play.PlayerX -= 5;
	}

	//壁に当たる
	if (play.PlayerX < 0)
	{
		play.PlayerX = 0;
	}
	if (play.PlayerX + 32 > 600)
	{
		play.PlayerX = 568;
	}
	if (play.PlayerY < 0)
	{
		play.PlayerY = 0;
	}
	if (play.PlayerY + 32 > 400)
	{
		play.PlayerY = 368;
	}
}
void EnemyUpdata()
{
	//敵の行動(レッド)
	if (enemy.EnemyX + 30 >= 600 || enemy.EnemyX <= 0)
	{
		enemy.Speed *= -1;
	}
	enemy.EnemyX += enemy.Speed;

	//敵の行動(イエロー)
	if (enemy2.EnemyX + 16 >= 600 || enemy2.EnemyX - 16 <= 0)
	{
		enemy2.Speed *= -1;
	}
	enemy2.EnemyX += enemy2.Speed;
}
void BulletUpdata()
{
	if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0)
	{
		if (bul.isbulFlag == 0 || bul2.isbulFlag == 0 || bul3.isbulFlag == 0)
		{
			bul.bulX = play.PlayerX + 15;
			bul.bulY = play.PlayerY + 15;

			bul2.bulX = play.PlayerX + 15;
			bul2.bulY = play.PlayerY + 15;

			bul3.bulX = play.PlayerX + 15;
			bul3.bulY = play.PlayerY + 15;

			bul.isbulFlag = 1;
			bul2.isbulFlag = 1;
			bul3.isbulFlag = 1;
		}
	}

	if (bul.isbulFlag == 1)
	{
		bul.bulY -= 10;

		if (bul.bulY < -20)
		{
			bul.isbulFlag = 0;
		}
	}
	if (bul2.isbulFlag == 1)
	{
		bul2.bulX -= 5;
		bul2.bulY -= 10;
		if (bul2.bulY < -20)
		{
			bul2.isbulFlag = 0;
		}
	}
	if (bul3.isbulFlag == 1)
	{
		bul3.bulX += 5;
		bul3.bulY -= 10;
		if (bul3.bulY < -20)
		{
			bul3.isbulFlag = 0;
		}
	}
	E_getpos = sqrt((enemy.EnemyX - bul.bulX) * (enemy.EnemyX - bul.bulX) + (enemy.EnemyY - bul.bulY) * (enemy.EnemyY - bul.bulY));
	E_getpos2 = sqrt((enemy.EnemyX - bul2.bulX) * (enemy.EnemyX - bul2.bulX) + (enemy.EnemyY - bul2.bulY) * (enemy.EnemyY - bul2.bulY));
	E_getpos3 = sqrt((enemy.EnemyX - bul3.bulX) * (enemy.EnemyX - bul3.bulX) + (enemy.EnemyY - bul3.bulY) * (enemy.EnemyY - bul3.bulY));
	if (bul.isbulFlag == 1 || bul2.isbulFlag == 1 || bul3.isbulFlag == 1 || enemy.isEnemyAlive == 1)
	{
		if (E_getpos < bul.bulR + enemy.EnemyR)
		{
			bul.isbulFlag = 0;
			enemy.isEnemyAlive = 0;
			enemy.EnemyX = -200;
			enemy.EnemyY = -200;
			enemy.Speed = 0;
			enemycount += 10;

			bul.bulX = -100;
			bul.bulY = -100;
		}
		if (E_getpos2 < bul2.bulR + enemy.EnemyR)
		{
			bul2.isbulFlag = 0;
			enemy.isEnemyAlive = 0;
			enemy.EnemyX = -200;
			enemy.EnemyY = -200;
			enemy.Speed = 0;
			enemycount += 10;

			bul2.bulX = -100;
			bul2.bulY = -100;
		}
		if (E_getpos3 < bul3.bulR + enemy.EnemyR)
		{
			bul3.isbulFlag = 0;
			enemy.isEnemyAlive = 0;
			enemy.EnemyX = -200;
			enemy.EnemyY = -200;
			enemy.Speed = 0;
			enemycount += 10;

			bul3.bulX = -100;
			bul3.bulY = -100;
		}
	}
	if (enemy.isEnemyAlive == 0)
	{
		enemy.enemyTymer = enemy.enemyTymer - 1;
		if (enemy.enemyTymer < 0)
		{
			enemy.isEnemyAlive = 1;
			enemy.enemyTymer = 120;
			enemy.EnemyX = rand() % 200 + 100;
			enemy.EnemyY = 10;
			if (rand() % 2)
			{
				enemy.Speed = 2;
			}
			else
			{
				enemy.Speed = -2;
			}

		}
	}
	E_getpos4 = sqrt((enemy2.EnemyX - bul.bulX) * (enemy2.EnemyX - bul.bulX) + (enemy2.EnemyY - bul.bulY) * (enemy2.EnemyY - bul.bulY));
	E_getpos5 = sqrt((enemy2.EnemyX - bul2.bulX) * (enemy2.EnemyX - bul2.bulX) + (enemy2.EnemyY - bul2.bulY) * (enemy2.EnemyY - bul2.bulY));
	E_getpos6 = sqrt((enemy2.EnemyX - bul3.bulX) * (enemy2.EnemyX - bul3.bulX) + (enemy2.EnemyY - bul3.bulY) * (enemy2.EnemyY - bul3.bulY));
	if (bul.isbulFlag == 1 || bul2.isbulFlag == 1 || bul3.isbulFlag == 1 || enemy2.isEnemyAlive == 1)
	{
		if (E_getpos4 < bul.bulR + enemy2.EnemyR)
		{
			bul.isbulFlag = 0;
			enemy2.isEnemyAlive = 0;
			enemy2.EnemyX = -200;
			enemy2.EnemyY = -200;
			enemy2.Speed = 0;
			enemycount += 15;

			bul.bulX = -100;
			bul.bulY = -100;
		}
		if (E_getpos5 < bul2.bulR + enemy2.EnemyR)
		{
			bul2.isbulFlag = 0;
			enemy2.isEnemyAlive = 0;
			enemy2.EnemyX = -200;
			enemy2.EnemyY = -200;
			enemy2.Speed = 0;
			enemycount += 15;

			bul2.bulX = -100;
			bul2.bulY = -100;
		}
		if (E_getpos6 < bul3.bulR + enemy2.EnemyR)
		{
			bul3.isbulFlag = 0;
			enemy2.isEnemyAlive = 0;
			enemy2.EnemyX = -200;
			enemy2.EnemyY = -200;
			enemy2.Speed = 0;
			enemycount += 15;

			bul3.bulX = -100;
			bul3.bulY = -100;
		}
	}
	if (enemy2.isEnemyAlive == 0)
	{
		enemy2.enemyTymer = enemy2.enemyTymer - 1;
		if (enemy2.enemyTymer < 0)
		{
			enemy2.isEnemyAlive = 1;
			enemy2.enemyTymer = 120;
			enemy2.EnemyX = rand() % 200 + 100;
			enemy2.EnemyY = 100;
			if (rand() % 2)
			{
				enemy2.Speed = 2;
			}
			else
			{
				enemy2.Speed = -2;
			}
		}
	}
}
void Initialize()
{
	play.PlayerX = 250, play.PlayerY = 250;
	enemy.EnemyX = 10, enemy.EnemyY = 10, enemy.EnemyR = 18, enemy.isEnemyAlive = 1, enemy.enemyTymer = 120, enemy.Speed = 2;
	enemy2.EnemyX = 500, enemy2.EnemyY = 100, enemy2.EnemyR = 16, enemy2.isEnemyAlive = 1, enemy2.enemyTymer = 120, enemy2.Speed = 2;
	bul.bulX = -10, bul.bulY = -10, bul.bulR = 10, bul.isbulFlag = 0;
	bul2.bulX = -10, bul2.bulY = -10, bul2.bulR = 10, bul2.isbulFlag = 0;
	bul3.bulX = -10, bul3.bulY = -10, bul3.bulR = 10, bul3.isbulFlag = 0;
	timer = 1200;

}