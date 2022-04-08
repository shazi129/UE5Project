// Fill out your copyright notice in the Description page of Project Settings.


#include "./BasePlayerController.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

ABasePlayerController::ABasePlayerController()
{
}

void ABasePlayerController::OnNetCleanup(UNetConnection* Connection)
{
	OnBeforeNetCleanUp(Connection);
	
	Super::OnNetCleanup(Connection);
}
