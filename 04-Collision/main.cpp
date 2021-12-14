/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work

	Key functions: 
		CGame::SweptAABB
		CGameObject::SweptAABBEx
		CGameObject::CalcPotentialCollisions
		CGameObject::FilterCollision

		CGameObject::GetBoundingBox
		
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "ScreenManager.h"

#include "Mario.h"
#include "Brick.h"
#include "Goomba.h"
#include "BatTrap.h"
#include "EnemyBall.h"
#include "Map.h"
#include "Sophia.h"
#include "Bullet.h"
#include "Brick2.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define MAX_FRAME_RATE 120

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_NES_ENEMY 30
#define ID_TEX_MAP 40
#define ID_TEX_PLAYER 50
#define ID_TEX_BRICK 60
#define ID_TEX_BRICK2 70
CGame *game;

CMario *mario;
CGoomba *goomba;
CBrick *brick;
CBrick2 *brick2;

vector<LPGAMEOBJECT> objects;

class CSampleKeyHander: public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler; 

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		mario->SetState(SOPHIA_STATE_JUMP);
		break;
	case DIK_A: // reset
		mario->SetState(SOPHIA_STATE_IDLE);
		mario->SetLevel(MARIO_LEVEL_BIG);
		mario->SetPosition(50.0f,0.0f);
		mario->SetSpeed(0, 0);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// disable control key when Mario die 
	if (mario->GetState() == SOPHIA_STATE_DIE) return;

	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(SOPHIA_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(SOPHIA_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_DOWN))
		mario->SetState(SOPHIA_STATE_WALKING_DOWN);
	else if (game->IsKeyDown(DIK_UP))
		mario->SetState(SOPHIA_STATE_JUMP);
	else
		mario->SetState(SOPHIA_STATE_IDLE);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MAP, L"textures\\level3-side.png", D3DCOLOR_XRGB(255, 255, 255));
	//brick
	textures->Add(ID_TEX_BRICK, L"textures\\brick.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_BRICK2, L"textures\\brick-1.png", D3DCOLOR_XRGB(255, 255, 255));

	textures->Add(ID_TEX_PLAYER, L"textures\\player.png", D3DCOLOR_XRGB(0, 57, 115));
	textures->Add(ID_TEX_MARIO, L"textures\\mario.png",D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_MISC, L"textures\\misc.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_ENEMY, L"textures\\enemies.png", D3DCOLOR_XRGB(3, 26, 110));
	textures->Add(ID_TEX_NES_ENEMY, L"textures\\NESBlaster MasterEnemies.png", D3DCOLOR_XRGB(3, 26, 110));
	textures->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));


	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	// map 
	LPDIRECT3DTEXTURE9 texMap = textures->Get(ID_TEX_MAP);
	sprites->Add(00001, 0, 0, 1551, 780, texMap);
	
	LPDIRECT3DTEXTURE9 texMario = textures->Get(ID_TEX_MARIO);

	// big
	sprites->Add(10001, 246, 154, 260, 181, texMario);		// idle right

	sprites->Add(10002, 275, 154, 290, 181, texMario);		// walk
	sprites->Add(10003, 304, 154, 321, 181, texMario);

	sprites->Add(10011, 186, 154, 200, 181, texMario);		// idle left
	sprites->Add(10012, 155, 154, 170, 181, texMario);		// walk
	sprites->Add(10013, 125, 154, 140, 181, texMario);

	sprites->Add(10099, 215, 120, 231, 135, texMario);		// die 

	// small
	sprites->Add(10021, 247, 0, 259, 15, texMario);			// idle small right
	sprites->Add(10022, 275, 0, 291, 15, texMario);			// walk 
	sprites->Add(10023, 306, 0, 320, 15, texMario);			// 

	sprites->Add(10031, 187, 0, 198, 15, texMario);			// idle small left

	sprites->Add(10032, 155, 0, 170, 15, texMario);			// walk
	sprites->Add(10033, 125, 0, 139, 15, texMario);	
	
	//brick
	LPDIRECT3DTEXTURE9 texBrick = textures->Get(ID_TEX_BRICK);
	sprites->Add(20001, 0, 0, 16, 16, texBrick);

	//brick2
	LPDIRECT3DTEXTURE9 texBrick2 = textures->Get(ID_TEX_BRICK2);
	sprites->Add(20002, 0, 0, 32, 32, texBrick2);


	//LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);
	//sprites->Add(30001, 5, 14, 21, 29, texEnemy);
	//sprites->Add(30002, 25, 14, 41, 29, texEnemy);

	//sprites->Add(30003, 45, 21, 61, 29, texEnemy); // die sprite

	LPDIRECT3DTEXTURE9 texNESEnemy = textures->Get(ID_TEX_NES_ENEMY);
	sprites->Add(40001, 5, 14, 21, 29, texNESEnemy);
	sprites->Add(40002, 25, 14, 41, 29, texNESEnemy);

	sprites->Add(40003, 75, 1485, 100, 1505, texNESEnemy); // BatTrap static sprite

	LPDIRECT3DTEXTURE9 texPlayer = textures->Get(ID_TEX_PLAYER);

	sprites->Add(98001, 91, 21, 112, 28, texPlayer); // bullet

	sprites->Add(98005, 140, 101, 148, 109, texPlayer); // explore 1
	sprites->Add(98006, 149, 97, 165, 113, texPlayer);	// explore 2
	sprites->Add(98007, 166, 97, 182, 113, texPlayer);	// explore 3
	sprites->Add(98008, 183, 93, 207, 117, texPlayer);	// explore 4


	sprites->Add(99001, 3, 12, 11, 20, texPlayer);		// than xe

	sprites->Add(99002, 3, 21 , 11, 29, texPlayer);		// banh xe 1
	sprites->Add(99003, 12, 21, 20, 29, texPlayer);		// banh xe 2
	sprites->Add(99004, 21, 21, 29, 29, texPlayer);		// banh xe 3
	sprites->Add(99005, 30, 21, 38, 29, texPlayer);		// banh xe 4

	sprites->Add(99006, 3, 3, 11, 11, texPlayer);		// sung 1
	sprites->Add(99007, 12, 3, 20, 11, texPlayer);		// sung 2
	sprites->Add(99008, 21, 3, 29, 11, texPlayer);		// sung 3
	sprites->Add(99009, 30, 3, 38, 11, texPlayer);		// sung 4

	sprites->Add(99010, 39, 3, 55, 11, texPlayer);		// khung 1
	sprites->Add(99011, 56, 3, 72, 11, texPlayer);		// khung 2

	sprites->Add(99012, 73 , 3, 89, 19, texPlayer);		// khung 3
	sprites->Add(99013, 90, 3, 98, 19, texPlayer);		// khung 4
	sprites->Add(99014, 99, 3, 107, 19, texPlayer);		// khung 5
	sprites->Add(99015, 108, 3, 124, 19, texPlayer);	// khung 5

	// enemyball

	sprites->Add(50001, 78, 385, 98, 405, texNESEnemy); // walking left sprite
	sprites->Add(50002, 98, 385, 118, 405, texNESEnemy);
	sprites->Add(50003, 118, 385, 138, 405, texNESEnemy);

	sprites->Add(50011, 138, 385, 158, 405, texNESEnemy); // walking right sprite
	sprites->Add(50012, 158, 385, 178, 405, texNESEnemy);
	sprites->Add(50013, 118, 385, 138, 405, texNESEnemy);


	LPANIMATION ani;

	ani = new CAnimation(100);	// idle big right
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	// idle big left
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(100);	// idle small right
	ani->Add(10021);
	animations->Add(402, ani);

	ani = new CAnimation(100);	// idle small left
	ani->Add(10031);
	animations->Add(403, ani);

	ani = new CAnimation(100);	// walk right big
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);

	ani = new CAnimation(100);	// // walk left big
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(501, ani);

	ani = new CAnimation(100);	// walk right small
	ani->Add(10021);
	ani->Add(10022);
	ani->Add(10023);
	animations->Add(502, ani);

	ani = new CAnimation(100);	// walk left small
	ani->Add(10031);
	ani->Add(10032);
	ani->Add(10033);
	animations->Add(503, ani);


	ani = new CAnimation(100);		// Mario die
	ani->Add(10099);
	animations->Add(599, ani);

	ani = new CAnimation(100);		// brick
	ani->Add(20001);
	animations->Add(601, ani);

	ani = new CAnimation(100);		// brick2
	ani->Add(20002);
	animations->Add(602, ani);

	ani = new CAnimation(100);		// map
	ani->Add(00001);
	animations->Add(001, ani);

	//ani = new CAnimation(300);		// Goomba walk
	//ani->Add(30001);
	//ani->Add(30002);
	//animations->Add(701, ani);

	//ani = new CAnimation(1000);		// Goomba dead
	//ani->Add(30003);
	//animations->Add(702, ani);


	ani = new CAnimation(300);		// BatTrap
	ani->Add(40003);
	animations->Add(801, ani);

	ani = new CAnimation(200);		// enemyball walk 
	ani->Add(50001);
	ani->Add(50002);
	ani->Add(50003);
	ani->Add(50011);
	ani->Add(50012);
	animations->Add(901, ani);

	ani = new CAnimation(200);		// sophia point
	ani->Add(99001);
	animations->Add(1001, ani);

	ani = new CAnimation(200);		// sophia banh xe trc chay
	ani->Add(99002);
	ani->Add(99003);
	ani->Add(99004);
	ani->Add(99005);
	animations->Add(1201, ani);

	ani = new CAnimation(200);		// sophia banh xe trc di 
	ani->Add(99002);
	ani->Add(99003);
	animations->Add(1202, ani);

	ani = new CAnimation(200);		// sophia banh xe sau chay
	ani->Add(99005);
	ani->Add(99002);
	ani->Add(99003);
	ani->Add(99004);
	animations->Add(1211, ani);

	ani = new CAnimation(200);		// sophia banh xe sau di
	ani->Add(99005);
	ani->Add(99002);
	animations->Add(1212, ani);

	ani = new CAnimation(200);
	ani->Add(99003);
	animations->Add(1003, ani);

	ani = new CAnimation(200);		// sophia gun
	ani->Add(99007);
	animations->Add(1004, ani);

	ani = new CAnimation(200);		// sophia body
	ani->Add(99010);
	animations->Add(1005, ani);

	ani = new CAnimation(200);		// sophia bullet
	ani->Add(98001);
	animations->Add(2001, ani);

	ani = new CAnimation(100);		// sophia bullet explore
	ani->Add(98005);
	ani->Add(98006);
	ani->Add(98007);
	ani->Add(98006);
	animations->Add(2002, ani);

	ani = new CAnimation(1000);		// Goomba dead
	ani->Add(30003);
	animations->Add(702, ani);

	Map *map = new Map();			//Map
	map->AddAnimation(001);
	map->SetPosition(0, 780.f);
	objects.push_back(map);

	mario = new CMario();
	//than duoi
	mario->AddAnimation(1001);
	//banh trc
	mario->AddAnimation(1201);
	//banh sau
	mario->AddAnimation(1211);
	//gun
	mario->AddAnimation(1004);
	//body
	mario->AddAnimation(1005);

	//mario->AddAnimation(400);		// idle right big
	//mario->AddAnimation(401);		// idle left big
	//mario->AddAnimation(402);		// idle right small
	//mario->AddAnimation(403);		// idle left small

	//mario->AddAnimation(500);		// walk right big
	//mario->AddAnimation(501);		// walk left big
	//mario->AddAnimation(502);		// walk right small
	//mario->AddAnimation(503);		// walk left big

	//mario->AddAnimation(599);		// die

	//mario->SetPosition(1400.0f, 750.0f);
	mario->SetPosition(50.0f, 250.0f);

	objects.push_back(mario);


	brick = new CBrick(30);
	brick->AddAnimation(601);
	brick->SetPosition(0 , 145);
	objects.push_back(brick);

	brick = new CBrick(5, false);
	brick->AddAnimation(601);
	brick->SetPosition(90.0f, 145 - 16.0f * 5);
	objects.push_back(brick);

	brick2 = new CBrick2(3);
	brick2->AddAnimation(602);
	brick2->SetPosition(834.0f, 380.0f);
	objects.push_back(brick2);

	brick2 = new CBrick2(4);
	brick2->AddAnimation(602);
	brick2->SetPosition(818.0f, 348.0f);
	objects.push_back(brick2);

	BatTrap *batTrap = new BatTrap();
	batTrap->AddAnimation(801);
	batTrap->SetPosition(70.0f, 100.0f);
	objects.push_back(batTrap);

	//Sophia *sophia = new Sophia();
	//sophia->AddAnimation(1001);
	//sophia->AddAnimation(1002);
	//sophia->AddAnimation(1003);
	//sophia->AddAnimation(1004);
	//sophia->AddAnimation(1005);
	//sophia->SetPosition(100.0f, 30.0f);
	//objects.push_back(sophia);


	CBullet *bullet = new CBullet();
	bullet->AddAnimation(2001);
	bullet->AddAnimation(2002);
	bullet->SetPosition(150.0f, 30.0f);
	bullet->SetState(BULLET_STATE_WALKING);
	objects.push_back(bullet);


	EnemyBall *enemyBall = new EnemyBall();
	enemyBall->AddAnimation(901);
	enemyBall->AddAnimation(902);
	enemyBall->SetPosition(40.0f, 250.0f);
	enemyBall->SetState(ENEMYBALL_STATE_WALKING);
	objects.push_back(enemyBall);

	float cx, cy;

	mario->GetPosition(cx, cy);

	ScreenManager::GetInstance()->setScreenManagerPosition(cx - SCREEN_WIDTH/2, SCREEN_HEIGHT);
	// and Goombas 

	//for (int i = 0; i < 4; i++)
	//{
	//	goomba = new CGoomba();
	//	goomba->AddAnimation(701);
	//	goomba->AddAnimation(702);
	//	goomba->SetPosition(200 + i*60, 135);
	//	goomba->SetState(GOOMBA_STATE_WALKING);
	//	objects.push_back(goomba);
	//}

}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;

	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt,&coObjects);
	}


	// Update camera to follow mario

	float cx, cy;

	float sX, sY, changeX, changeY;

	mario->GetPosition(cx, cy);

	ScreenManager::GetInstance()->getScreenManagerPosition(sX, sY);

	int isChangeScreen = 0;
	changeX = sX;
	changeY = sY;
	if (cx - sX < 40) { changeX = cx - 40; isChangeScreen = 1; }
	
	if (cx - sX >= SCREEN_WIDTH - 40 - 24) 
	{ 
		changeX = sX + 40 + 24 + (cx - (sX + SCREEN_WIDTH));
		isChangeScreen = 1;
	}

	if (sY - cy < 40) { changeY = cy + 40; isChangeScreen = 1; }

	if (sY - (cy - 16) >= SCREEN_HEIGHT - 40 )
	{
		changeY = sY - 40 - 16 - (sY - ( SCREEN_HEIGHT - 40 - 16 ) - cy - 16 );
		isChangeScreen = 1;
	}
	
	if (isChangeScreen == 1)
	{
		ScreenManager::GetInstance()->setScreenManagerPosition(changeX, changeY);
	}
}

/*
	Render a frame 
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();
			
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();

	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();

	game->InitKeyboard(keyHandler);

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT*2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}