// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGameInstance.h"
#include "MoviePlayer.h"
#include "Kismet/GameplayStatics.h"

void ULevelGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ULevelGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ULevelGameInstance::EndLoadingScreen);
}

void ULevelGameInstance::OpenLevel(TSoftObjectPtr<UWorld> World)
{
	if (World.IsNull())
	{
		OpenLevel(TSoftObjectPtr<UWorld>(GetWorld()));
		return;
	}

	BeginLoadingScreen(World.ToSoftObjectPath().GetAssetPathName().ToString());
	UGameplayStatics::OpenLevel(this,FName(*GetPathFromAsset(World)));
}

void ULevelGameInstance::OpenNextLevel()
{
	UpdateCurrentLevel();
	int32 Found = LegitLevels.Find(CurrentLevel);
	if (Found != INDEX_NONE)
	{
		if (LegitLevels.IsValidIndex(Found+1))
			OpenLevel(LegitLevels[Found + 1]);
		else
			OpenLevel(MainMenu);
		return;
	}

	if (LegitLevels.IsValidIndex(0))
	{
		OpenLevel(LegitLevels[0]);
	}

}

void ULevelGameInstance::RestartLevel()
{
	UpdateCurrentLevel();
	OpenLevel(CurrentLevel);
}

void ULevelGameInstance::UpdateCurrentLevel()
{
	CurrentLevel = GetAssetFromPath(UGameplayStatics::GetCurrentLevelName(GetWorld()));
}

void ULevelGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
	if (LoadingScreenWidgetClass)
	{
		LoadingWidget = CreateWidget<UUserWidget>(this, LoadingScreenWidgetClass);
		if (LoadingWidget)
		{
			LoadingWidget->AddToViewport();
		}
	}
}

void ULevelGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	UpdateCurrentLevel();
	if (LoadingWidget)
	{
		LoadingWidget->RemoveFromViewport();
		LoadingWidget = nullptr;
	}
}

TSoftObjectPtr<UWorld> ULevelGameInstance::GetAssetFromPath(const FString& Path)
{
	for (auto itr : LegitLevels)
	{
		if ( GetPathFromAsset(itr) == Path)
		{
			return itr;
		}
	}
	return TSoftObjectPtr<UWorld>(GetWorld());
}

FString ULevelGameInstance::GetPathFromAsset(const TSoftObjectPtr<UWorld>& Asset)
{
	FString PathPart, FilenamePart, ExtensionPart;
	FPaths::Split(Asset.GetLongPackageName(),PathPart, FilenamePart, ExtensionPart);
	return FilenamePart;
}