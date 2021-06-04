#include "DxLib.h"
#include <math.h>

const char TITLE[] = "TestProgram";

//�E�B���h�E�T�C�Y
const int WIN_WIDTH = 600;
const int WIN_HEIGHT = 600;

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//�E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	/*�E�B���h�E�T�C�Y���蓮�ł͕ύX�����A
	�E�B���h�E�T�C�Y�ɕ����Ċg��ł��Ȃ��悤�ɂ���*/
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	//�^�C�g����ύX
	SetMainWindowText(TITLE);

	//��ʃT�C�Y�̍ő�T�C�Y,�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	//�摜�T�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	//��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x00, 0x00, 0xAF);

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//(�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	//�摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
	int back1 = LoadGraph("background.png");
	int back2 = LoadGraph("background2.png");
	int player = LoadGraph("ico.jpg");

	//�Q�[�����[�v�Ŏg���ϐ��̐錾
	int PlayerX = 250, PlayerY = 250;

	int Enemy1X = 10, Enemy1Y = 10, Enemy1R = 16;
	int Enemy2X = 500, Enemy2Y = 100, Enemy2R = 16;
	int E_getpos, E_getpos2, E_getpos3, isEnemyAlive = 1, enemyTymer = 1;
	int Speed = 2, Speed2 = 2;

	int bulx = -10, buly = -10, bulr = 10, isbulFlag = 0;
	int bul2x = -10, bul2y = -10, bul2r = 10, isbulFlag2 = 0;
	int bul3x = -10, bul3y = -10, bul3r = 10, isbulFlag3 = 0;
	
	int bgX = 0, bgY = 0;
	int bg2X = 0, bg2Y = -400;

	//�ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	//1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	//�Q�[�����[�v
	while (1)
	{
		//�ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		for (int i = 0; i < 256; ++i)
		{
			oldkeys[i] = keys[i];
		}
		//�ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		//��ʃN���A
		ClearDrawScreen();

		//�X�V����
		//�v���C���[
		if (keys[KEY_INPUT_UP] == 1 || keys[KEY_INPUT_W] == 1)
		{
			PlayerY -= 5;
		}
		if (keys[KEY_INPUT_DOWN] == 1 || keys[KEY_INPUT_S] == 1)
		{
			PlayerY += 5;
		}
		if (keys[KEY_INPUT_RIGHT] == 1 || keys[KEY_INPUT_D] == 1)
		{
			PlayerX += 5;
		}
		if (keys[KEY_INPUT_LEFT] == 1 || keys[KEY_INPUT_A] == 1)
		{
			PlayerX -= 5;
		}

		//�e�̏���
		if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 1)
		{
			if (isbulFlag == 0 || isbulFlag2 ==0 || isbulFlag == 0)
			{
				bulx = PlayerX + 15;
				buly = PlayerY + 15;

				bul2x = PlayerX + 15;
				bul2y = PlayerY + 15;
				
				bul3x = PlayerX + 15;
				bul3y = PlayerY + 15;

				isbulFlag = 1;
				isbulFlag2 = 1;
				isbulFlag3 = 1;
			}
		}

		if (isbulFlag == 1)
		{
			buly -= 10;

			if (buly < -20)
			{
				isbulFlag = 0;
			}
		}
		if (isbulFlag2 == 1)
		{
			bul2x -= 5;
			bul2y -= 10;
			if (bul2y < -20)
			{
				isbulFlag2 = 0;
			}
		} 
		if (isbulFlag3 == 1)
		{
			bul3x += 5;
			bul3y -= 10;
			if (bul3y < -20)
			{
				isbulFlag3 = 0;
			}
		}
		E_getpos = sqrt((Enemy1X - bulx) * (Enemy1X - bulx) + (Enemy1Y - buly) * (Enemy1Y - buly));
		E_getpos2 = sqrt((Enemy1X - bul2x) * (Enemy1X - bul2x) + (Enemy1Y - bul2y) * (Enemy1Y - bul2y));
		E_getpos3 = sqrt((Enemy1X - bul3x) * (Enemy1X - bul3x) + (Enemy1Y - bul3y) * (Enemy1Y - bul3y));
		if ( isbulFlag == 1 || isbulFlag2 == 1 || isbulFlag3 == 1 || isEnemyAlive == 1)
		{
			if (E_getpos < bulr + Enemy1R)
			{
				isbulFlag = 0;
				isEnemyAlive = 0;

				bulx = -100;
				buly = -100;
			}
			if (E_getpos2 < bul2r + Enemy1R)
			{
				isbulFlag2 = 0;
				isEnemyAlive = 0;
				bul2x = -100;
				bul2y = -100;
			}
			if (E_getpos3 < bul2r + Enemy1R)
			{
				isbulFlag3 = 0;
				isEnemyAlive = 0;
				bul3x = -100;
				bul3y = -100;
			}
		}
		if (isEnemyAlive == 0)
		{
			enemyTymer = enemyTymer - 1;
			if (enemyTymer < 0)
			{
				isEnemyAlive = 1;
				enemyTymer = 120;
			}
		}
		//�ǂɓ�����
		if (PlayerX < 0)
		{
			PlayerX = 0;
		}
		if (PlayerX + 32 > 600)
		{
			PlayerX = 568;
		}
		if (PlayerY < 0)
		{
			PlayerY = 0;
		}
		if (PlayerY + 32 > 400)
		{
			PlayerY = 368;
		}
		//�p�b�N�}���ǒ[�ړ�-
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

		//�G�̓����蔻��
		

		//�G�̍s��(���b�h)
		if (Enemy1X + 30 >= 600 || Enemy1X <= 0)
		{
			Speed *= -1;
		}
		Enemy1X += Speed;

		//�G�̍s��(�C�G���[)
		if (Enemy2X + 16 >= 600 || Enemy2X - 16 <= 0)
		{
			Speed2 *= -1;
		}
		Enemy2X += Speed2;

		//�`�揈��
		//�w�i�X�N���[��
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
		
		//���@,�v���C���[�`��
		DrawCircle(bulx, buly, bulr, GetColor(255, 255, 255), TRUE);
		DrawCircle(bul2x, bul2y, bul2r, GetColor(255, 255, 255), TRUE);
		DrawCircle(bul3x, bul3y, bul3r, GetColor(255, 255, 255), TRUE);
		
		DrawGraph(PlayerX,PlayerY,player,TRUE);
		//DrawBox(PlayerX, PlayerY, PlayerX + 32, PlayerY + 32, GetColor(255, 255, 255), TRUE);
		if (isEnemyAlive == 1)
		{
			DrawBox(Enemy1X, Enemy1Y, Enemy1X + 32, Enemy1Y + 32, GetColor(255, 0, 0), TRUE);
		}
		DrawCircle(Enemy2X, Enemy2Y, Enemy2R, GetColor(255, 255, 0),TRUE);
		
		//�f�o�b�N
		DrawBox(0, 400, 600, 600, GetColor(0, 0, 0), TRUE);
		DrawFormatString(0, 410, GetColor(255, 255, 255),
			"(���� PlayerX: %3dpx, PlayerY: %3dpx)", PlayerX, PlayerY);
		DrawFormatString(0, 430, GetColor(255, 255, 255),
			"(�E�� PlayerX: %3dpx, PlayerY: %3dpx)", PlayerX + 32, PlayerY);
		DrawFormatString(0, 450, GetColor(255, 255, 255),
			"(���� PlayerX: %3dpx, PlayerY: %3dpx)", PlayerX, PlayerY + 32);
		DrawFormatString(0, 470, GetColor(255, 255, 255),
			"(�E�� PlayerX: %3dpx, PlayerY: %3dpx)", PlayerX + 32, PlayerY + 32);
		DrawFormatString(0, 490, GetColor(255, 255, 255),
			"(Enemy1:X%3dpx,Y%3dpx)", Enemy1X, Enemy1Y);
		DrawFormatString(0, 510, GetColor(255, 255, 255),
			"(Enemy2:X%3dpx,Y%3dpx)", Enemy2X, Enemy2Y);
		DrawFormatString(0, 530, GetColor(255, 255, 255),
			"�w�i:Y%3dpx", bgY);
		DrawFormatString(110, 530, GetColor(255, 255, 255),
			"�w�i:Y%3dpx", bg2Y);
	
		//(�_�u���o�b�t�@)����
		ScreenFlip();

		//20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		//Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1)
		{
			break;
		}

		//ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	
	DxLib_End();// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}