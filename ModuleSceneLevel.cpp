#include "Application.h"
#include "ModuleSceneLevel.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleUI.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneHighscore.h"
#include "ModuleAudio.h"
#include "Road.h"

using namespace std;

ModuleSceneLevel::ModuleSceneLevel(bool active) : Module(active)
{
}

ModuleSceneLevel::~ModuleSceneLevel()
{}

// Load assets
bool ModuleSceneLevel::Start()
{
	LOG("Loading level scene");
	road = new Road();
	actualState = COUNTDOWN;
	countdownTimer = 5.f;
	gameoverTimer = 5.f;
	tick_timer = clock();
	App->player->Enable();
	App->ui->Enable();
	road->Start();
	road->InitRoad();
	const char* m = App->musicLevel;
	App->audio->PlayMusic(App->musicLevel, 0.f);
	return true;
}

// UnLoad assets
bool ModuleSceneLevel::CleanUp()
{
	LOG("Unloading level scene");
	/*** TODO !!***/

	App->player->Disable();
	App->ui->Disable();
	road->CleanUp();
	RELEASE(road);

	return true;
}

// Update: draw background
update_status ModuleSceneLevel::Update()
{
	time = (float)((clock() - tick_timer) / (double)CLOCKS_PER_SEC);
	tick_timer = clock();

	switch (actualState)
	{
	case COUNTDOWN:
		if (countdownTimer < 4.0f)
			App->player->preparingAnim.speed = 0.1f;
		if (countdownTimer < 2.0f)
			road->ActivateAnims();

		countdownTimer -= time;

		if (countdownTimer < 0)
		{
			actualState = RUNNING;
			App->player->playerState = ONROAD;
			App->ui->uiState = UIPlay;
		}
		road->DrawRoad();
		break;
	case RUNNING:
		road->UpdateRoad(time);
		road->DrawRoad();
		if (road->ending)
		{
			actualState = FINISH;
			App->ui->uiState = UIFinish;
		}
		if (App->ui->timeLeft < 0.f)
		{
			actualState = GAMEOVER;
			App->ui->uiState = UIGameOver;
			App->player->playerState = PlayerGAMEOVER;
		}
		break;
	case GAMEOVER:
		road->DrawRoad();
		gameoverTimer -= time;
		if (gameoverTimer < 0.f)
		{
			App->audio->StopFx();
			App->audio->StopMusic(0.f);
			App->score = App->player->score;
			App->totalTime = App->ui->totalTime + App->ui->lapTime;
			App->fade->FadeToBlack(App->highscore, this, 0.f);
		}
		break;
	case FINISH:
		road->UpdateRoadEnding(time);
		road->DrawRoad();
		if (App->player->endSequence.Finished())
		{
			App->audio->StopFx();
			App->audio->StopMusic(0.f);
			App->score = App->player->score;
			App->totalTime = App->ui->totalTime + App->ui->lapTime;
			App->fade->FadeToBlack(App->highscore, this, 0.f);
		}
		break;
	}

	return UPDATE_CONTINUE;
}