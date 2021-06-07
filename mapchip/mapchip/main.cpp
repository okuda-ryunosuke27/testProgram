#include "DxLib.h"
#include <algorithm>

const char TITLE[] = "TestProgram";

//�E�B���h�E�T�C�Y
const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 640;

//�}�b�v�`�b�v���[�h
const int size_map_x = 32;
const int size_map_y = 32;
const int num_chip_x = 10;
const int num_chip_y = 4;
const int num_chip_all = num_chip_x * num_chip_y;
int chip[num_chip_all];

const int num_map_x = 20;
const int num_map_y = 15;
int data[15][20] = {
	// ��20x15
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

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
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
	LoadDivGraph("mapchip.png", num_chip_all, num_chip_x, num_chip_y, size_map_x, size_map_y, chip);

	//�Q�[�����[�v�Ŏg���ϐ��̐錾
	float PlayerX = 250.0f, PlayerY = 250.0f;
	float g = 0.0f;
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
		//�`�揈��
		DrawBox(PlayerX, PlayerY, PlayerX + 32, PlayerY + 32, GetColor(255, 255, 255), TRUE);

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
	
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}

int CheckHit(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	int L1 = x1;      // ��
	int R1 = x1 + w1; // �E(��+����)
	int L2 = x2;      // ��
	int R2 = x2 + w2; // �E(��+����)

	if (R1 < L2) return 0;
	if (R2 < L1) return 0;

	int U1 = y1;      // ��
	int D1 = y1 + h1; // ��(��+�c��)
	int U2 = y2;      // ��
	int D2 = y2 + h2; // ��(��+�c��)

	if (D1 < U2) return 0;
	if (D2 < U1) return 0;

	// ����ȊO�̏ꍇ�͓������Ă���
	return 1;
}