﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<random>

using namespace DirectX;
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() 
{ 
	delete model_; 
}

void GameScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();

	//乱数シード生成器
	random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲（回転角用）
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲（座標用）
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	//for (size_t i = 0; i < _countof(worldTransfrom_); i++) {
	//	/// X,Y,Z,方向のスケーリングを設定
	//	worldTransfrom_[i].scale_ = {1.0f, 1.0f, 1.0f};
	//	/// X,Y,Z,軸周りの回転角の設定
	//	worldTransfrom_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
	//	/// X,Y,Z,軸周りの平行移動の設定
	//	worldTransfrom_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

	//	//ワールドトランスフォームの初期化
	//	worldTransfrom_[i].Initialize();
	//}

	////カメラ視点座標を設定
	//viewProjection_.eye = {0, 0, -
	// 10};
	////カメラ注視点座標を認定
	//viewProjection_.target = {10, 0, 0};
	////カメラ上方向ベクトルを設定（右上45度指定）
	//viewProjection_.up = {cosf(XM_PI / 4.0f), sinf(XM_PI/4.0f),0.0f};

	////カメラ垂直方向視野角を設定
	//viewProjection_.fovAngleY = XMConvertToRadians(10.0f);
	////アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;
	////ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	////ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//親（0番）
	worldTransform_[PartId::Root].Initialize();
	//子（1番）
	worldTransform_[PartId::Spine].translation_ = {0, 2.5f, 0};
	worldTransform_[PartId::Spine].parent_ = &worldTransform_[0];
	worldTransform_[PartId::Spine].Initialize();
	//上半身
	worldTransform_[PartId::Spine].Initialize();
	worldTransform_[PartId::Chest].translation_ = {0, 0, 0};
	worldTransform_[PartId::Chest].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Chest].Initialize();
	worldTransform_[PartId::Head].translation_ = {0, 2.5f, 0};
	worldTransform_[PartId::Head].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::Head].Initialize();
	worldTransform_[PartId::AlmL].translation_ = {2.5f, 0, 0};
	worldTransform_[PartId::AlmL].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::AlmL].Initialize();
	worldTransform_[PartId::AlmR].translation_ = {-2.5f, 0, 0};
	worldTransform_[PartId::AlmR].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::AlmR].Initialize();
	//下半身

	worldTransform_[PartId::Hip].translation_ = {0, -2.5f, 0};
	worldTransform_[PartId::Hip].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Hip].Initialize();
	worldTransform_[PartId::LegL].translation_ = {2.5f, -2.5f, 0};
	worldTransform_[PartId::LegL].parent_ = &worldTransform_[PartId ::Hip];
	worldTransform_[PartId::LegL].Initialize();
	worldTransform_[PartId::LegR].translation_ = {-2.5f, -2.5f, 0};
	worldTransform_[PartId::LegR].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegR].Initialize();
}

void GameScene::Update() {
	/*debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "translation:(%f,%f,%f)",
	    worldTransform_.translation_.x,
	    worldTransform_.translation_.y,
	    worldTransform_.translation_.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "rotation:(%f,%f,%f)",
	    worldTransform_.rotation_.x,
	    worldTransform_.rotation_.y,
	    worldTransform_.rotation_.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "scale:(%f,%f,%f)",
	    worldTransform_.scale_.x,
	    worldTransform_.scale_.y,
	    worldTransform_.scale_.z);*/

	////視点移動処理
	//{
	//	//視点の移動ベクトル
	//	XMFLOAT3 move = {0, 0, 0};
	//	//視点の移動の速さ
	//	const float kEyeSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_W)) {
	//		move = {0, 0, kEyeSpeed};
	//	} else if (input_->PushKey(DIK_S)) {
	//		move = {0, 0, -kEyeSpeed};
	//	}

	//	//視点移動（ベクトルの加算）
	//	viewProjection_.eye.x += move.x;
	//	viewProjection_.eye.y += move.y;
	//	viewProjection_.eye.z += move.z;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//	debugText_->SetPos(50, 50);
	//	debugText_->Printf(
	//	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	//}

	////注意点移動処理
	//{
	//	//注意点の移動ベクトル
	//	XMFLOAT3 move = {0, 0, 0};
	//	//注意点の移動速さ
	//	const float kTagetspeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_LEFT)) {
	//		move = {-kTagetspeed, 0, 0};
	//	} else if (input_->PushKey(DIK_RIGHT)) {
	//		move = {kTagetspeed, 0, 0};
	//	}

	//	//視点移動（ベクトルの加算）
	//	viewProjection_.target.x += move.x;
	//	viewProjection_.target.y += move.y;
	//	viewProjection_.target.z += move.z;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//	debugText_->SetPos(50, 70);
	//	debugText_->Printf(
	//	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	//	  viewProjection_.target.z);
	//}

	////上方向回転処理
	//{
	//	//上方向の回転速さ[ラジアン/frame]
	//	const float kUpRotSpeed = 0.05f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_SPACE)) {
	//		viewAngle += kUpRotSpeed;
	//		//2πを超えたら0に戻す
	//		viewAngle = fmodf(viewAngle, XM_2PI);
	//	}

	//	//上方向ベクトルを計算（半径1の円周上の座標）
	//	viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//	debugText_->SetPos(50, 90);
	//	debugText_->Printf(
	//	  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y,
	//	  viewProjection_.up.z);
	//}

	////FoV変更処理
	//{
	//	//Wキーで　視野角が広がる
	//	if (input_->PushKey(DIK_W))
	//	{
	//		viewProjection_.fovAngleY += 0.01f;
	//		viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
	//	}
	//	//Sキーで　視野角が広がる
	//	if (input_->PushKey(DIK_S)) {
	//		viewProjection_.fovAngleY -= 0.01f;
	//		viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0, 01f);
	//	}

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();
	//	//デバッグ用表示
	//	debugText_->SetPos(50, 110);
	//	debugText_->Printf("fovAngleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));
	//}
	//
	////クリップ距離変更を増減
	//{
	//	//上下キーでニアクリップ距離を増減
	//	if (input_->PushKey(DIK_UP)) {
	//		viewProjection_.nearZ += 0.1f;
	//	} else if (input_->PushKey(DIK_DOWN)) {
	//		viewProjection_.nearZ -= 0.1f;
	//	}

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//	debugText_->SetPos(50, 130);
	//	debugText_->Printf("nesrZ:%f", viewProjection_.nearZ);

	//}

	//キャラクターの移動処理
	{
		//キャラクターの移動ベクトル
		XMFLOAT3 move = {0, 0, 0};
		//キャラクターの移動の速さ
		const float kCharacterSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = {-kCharacterSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kCharacterSpeed, 0, 0};
		}

		//注視点移動（ベクトル加算）
		worldTransform_[PartId::Root].translation_.x += move.x;
		worldTransform_[PartId::Root].translation_.y += move.y;
		worldTransform_[PartId::Root].translation_.z += move.z;
		// デバッグ用表示
		debugText_->SetPos(50, 150);
		debugText_->Printf("Root:(%f,%f,%f)", 
			worldTransform_[PartId::Root].translation_.x,
			worldTransform_[PartId::Root].translation_.y, 
			worldTransform_[PartId::Root].translation_.z);
	}
	worldTransform_[PartId::Root].UpdateMatrix();
	worldTransform_[PartId::Spine].UpdateMatrix();
	worldTransform_[PartId::Chest].UpdateMatrix();
	worldTransform_[PartId::Head].UpdateMatrix();
	worldTransform_[PartId::AlmL].UpdateMatrix();
	worldTransform_[PartId::AlmR].UpdateMatrix();
	worldTransform_[PartId::Hip].UpdateMatrix();
	worldTransform_[PartId::LegL].UpdateMatrix();
	worldTransform_[PartId::LegR].UpdateMatrix();
	
	//上半身の回転処理
	{
		//上半身の回転の速さ[ラジアン/frame]
		const float kChestRotSpeed = 0.05f;

		//押した方向で移動ベクトルを移動
		if (input_->PushKey(DIK_U)) {
			worldTransform_[Chest].rotation_.y -= kChestRotSpeed;
		} else if (input_->PushKey(DIK_I)) {
			worldTransform_[Chest].rotation_.y += kChestRotSpeed;
		}
	}
	//下半身の回転処理
	{
		//下半身の回転の速さ[ラジアン/frame]
		const float kChestRotSpeed = 0.05f;

		//押した方向で移動ベクトルを移動
		if (input_->PushKey(DIK_J)) {
			worldTransform_[Hip].rotation_.y -= kChestRotSpeed;
		} else if (input_->PushKey(DIK_K)) {
			worldTransform_[Hip].rotation_.y += kChestRotSpeed;
		}
	}
}
	

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//3Dモデル描画
	/*for (size_t i = 0; i < _countof(worldTransform_); i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}*/
	/*model_->Draw(worldTransform_[PartId::Root], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::Spine], viewProjection_, textureHandle_);*/
	model_->Draw(worldTransform_[PartId::Chest], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::Head], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::AlmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::AlmR], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::Hip], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::LegL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::LegR], viewProjection_, textureHandle_);
	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
