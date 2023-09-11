#include "ResultScene.h"
#include "FbxLoader.h"
#include "Vector3.h"
#include "StageManager.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"
#include <stdlib.h>
#include <time.h>

void ResultScene::Initialize()
{
	// nullptr�`�F�b�N
	dxCommon = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	scene_ = SceneManager::GetInstance();

	// �J��������
	cameraObject = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	srand(time(NULL));//����������

	// �e�N�X�`���ǂݍ���

	//�X�v���C�g����
	backGround = Sprite::Create(31, { 0.0f,0.0f });

	cursor = Sprite::Create(4, { 0.0f,0.0f });

	//�X�R�A�֘A
	scoreSprite = Sprite::Create(6, { 588.0f, 359.0f });//SCORE���S
	colonScore = Sprite::Create(20, { 752.0f, 359.0f });//�R����(:)
	for (int i = 0; i < 10; i++)
	{
		hunThouScore[i] = Sprite::Create(10 + i, { 782.0f, 359.0f });//[0]00000
	}
	for (int i = 0; i < 10; i++)
	{
		tenThouScore[i] = Sprite::Create(10 + i, { 814.0f, 359.0f });//0[0]0000
	}
	for (int i = 0; i < 10; i++)
	{
		thouScore[i] = Sprite::Create(10 + i, { 846.0f, 359.0f });//00[0]000
	}
	for (int i = 0; i < 10; i++)
	{
		hunScore[i] = Sprite::Create(10 + i, { 878.0f, 359.0f });//000[0]00
	}
	for (int i = 0; i < 10; i++)
	{
		tenScore[i] = Sprite::Create(10 + i, { 910.0f, 359.0f });//0000[0]0
	}
	for (int i = 0; i < 10; i++)
	{
		oneScore[i] = Sprite::Create(10 + i, { 942.0f, 359.0f });//00000[0]
	}

	//���U���g��ʊ֘A
	//score
	score = StageManager::GetInstance()->GetScore();
	//RESULT���S
	resultSprite = Sprite::Create(21, { 700.0f, 143.0f });
	//RANK���S
	rankSprite = Sprite::Create(22, { 613.0f, 525.0f });
	//A���S
	aSprite = Sprite::Create(23, { 900.0f, 525.0f });
	//B���S
	bSprite = Sprite::Create(24, { 900.0f, 525.0f });
	//C���S
	cSprite = Sprite::Create(25, { 900.0f, 525.0f });
	//RETRY���S
	retrySprite = Sprite::Create(8, { 312.0f, 715.0f });
	//TITLE���S
	titleSprite = Sprite::Create(9, { 1147.0f, 715.0f });

	// �w�i�X�v���C�g����

	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(cameraObject.get());
	particleMan->Clear();

	// �J���������_���Z�b�g
	cameraObject->SetTarget(cameraTarget);
	cameraObject->SetEye({ cameraEye[0],cameraEye[1],cameraEye[2] });

	// ���f�������w�肵�ăt�@�C���ǂݍ���

	// ���C�g����
	lightGroup = LightGroup::Create();
	lightGroup->SetCircleShadowActive(0, true);
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup.get());
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(cameraObject.get());

	// �J�������Z�b�g
	FbxObject3d::SetCamera(cameraObject.get());

	cameraCollider.center = XMLoadFloat3(&cameraObject->GetEye());
	cameraCollider.radius = 5.0f;

	skydomeCollider.center = { 0,0,0 };
	skydomeCollider.radius = 400.0f;

	cameraObject->Update();
	particleMan->Update();
}

void ResultScene::Finalize() {}

void ResultScene::Update()
{
	//�����N�\��
	if (80000 <= score)
	{
		rankPattern = 0.0f;
	}
	else if (40000 <= score && score < 80000)
	{
		rankPattern = 1.0f;
	}
	else
	{
		rankPattern = 2.0f;
	}

	XMFLOAT2 retryPos = retrySprite->GetPosition();
	XMFLOAT2 titlePos = titleSprite->GetPosition();
	XMFLOAT2 mousePos = input->GetMousePosition();

	if (retryPos.x < mousePos.x && retryPos.x + 170.0f > mousePos.x && retryPos.y< mousePos.y && retryPos.y + 50.0f > mousePos.y) {
		retrySprite->SetColor(Sprite::Color::GREEN);
		if (input->TriggerMouse(LeftButton)) {
			SceneManager::GetInstance()->ToGameScene();
		}
	}
	else
	{
		retrySprite->SetColor(Sprite::Color::DEF);
	}

	if (titlePos.x < mousePos.x && titlePos.x + 161.0f > mousePos.x && titlePos.y< mousePos.y && titlePos.y + 50.0f > mousePos.y) {
		titleSprite->SetColor(Sprite::Color::GREEN);
		if (input->TriggerMouse(LeftButton)) {
			SceneManager::GetInstance()->ToTitleScene();
		}
	}
	else
	{
		titleSprite->SetColor(Sprite::Color::DEF);
	}

	cursor->SetPosition(input->GetMousePosition());
	if (input->PushMouse(LeftButton)) {
		cursor->SetColor({ 1.0f,1.0f,0.0f,1.0f });
	}
	else {
		cursor->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}
	//�X�R�A�֘A
	drawScore[0] = (score / 100000) % 10;//[0]00000
	drawScore[1] = (score / 10000) % 10;//0[0]0000
	drawScore[2] = (score / 1000) % 10;//00[0]000
	drawScore[3] = (score / 100) % 10;//000[0]00
	drawScore[4] = (score / 10) % 10;//0000[0]0
	drawScore[5] = score % 10;//00000[0]
}

void ResultScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	//spriteBG->Draw();

	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	backGround->Draw();

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�`��
	// 3D�I�u�W�F�N�g�̕`��
	Object3d::PreDraw(cmdList);

	Object3d::PostDraw();

	particleMan->Draw(cmdList);

	//lightGroup->Draw(cmdList, 3);

	// �p�[�e�B�N���̕`��
	//particleMan->Draw(cmdList);
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	//�X�R�A�֘A
	scoreSprite->Draw();//SCORE���S
	colonScore->Draw();//�R����(:)
	hunThouScore[drawScore[0]]->Draw();//[0]00000
	tenThouScore[drawScore[1]]->Draw();//0[0]0000
	thouScore[drawScore[2]]->Draw();//00[0]000
	hunScore[drawScore[3]]->Draw();//000[0]00
	tenScore[drawScore[4]]->Draw();//0000[0]0
	oneScore[drawScore[5]]->Draw();//00000[0]

	//���U���g��ʊ֘A
	retrySprite->Draw();//RETRY���S
	titleSprite->Draw();//TITLE���S
	rankSprite->Draw();//RANK���S
	if (rankPattern == 0.0f)
	{
		aSprite->Draw();//A���S
	}
	else if (rankPattern == 1.0f)
	{
		bSprite->Draw();//B���S
	}
	else
	{
		cSprite->Draw();//C���S
	}
	resultSprite->Draw();//RESULT���S

	cursor->Draw();

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}