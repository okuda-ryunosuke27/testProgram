#include "DxLib.h"

const char TITLE[] = "TestProgram";

//�E�B���h�E�T�C�Y
const int WIN_WIDTH = 600;
const int WIN_HEIGHT = 600;

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


	//�Q�[�����[�v�Ŏg���ϐ��̐錾
	int PlayerX = 250, PlayerY = 250;
	int EnemyX = 10, EnemyY = 10;

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
		//�`�揈��
		DrawBox(PlayerX, PlayerY, PlayerX + 32, PlayerY + 32, GetColor(255, 255, 255), TRUE);
		DrawBox(0, 400, 600, 600, GetColor(0, 0, 0), TRUE);
		DrawBox(EnemyX, EnemyY, EnemyX + 32, EnemyY + 32, GetColor(255, 0, 0),TRUE);
		DrawFormatString(0, 410, GetColor(255, 255, 255),
			"(���� PlayerX: %3dpx, PlayerY: %3dpx)", PlayerX, PlayerY);
		DrawFormatString(0, 430, GetColor(255, 255, 255),
			"(�E�� PlayerX: %3dpx, PlayerY: %3dpx)", PlayerX + 32, PlayerY);
		DrawFormatString(0, 450, GetColor(255, 255, 255),
			"(���� PlayerX: %3dpx, PlayerY: %3dpx)", PlayerX, PlayerY + 32);
		DrawFormatString(0, 470, GetColor(255, 255, 255),
			"(�E�� PlayerX: %3dpx, PlayerY: %3dpx)", PlayerX + 32, PlayerY + 32);
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