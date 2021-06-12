// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "LevelGameInstance.generated.h"

/**
 *
 */
UCLASS()
class GMTK2021_API ULevelGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category="Level")
	void OpenLevel(TSoftObjectPtr<UWorld> World);

	UFUNCTION(BlueprintCallable, Category="Level")
	void OpenNextLevel();

	UFUNCTION(BlueprintCallable, Category="Level")
	void RestartLevel();

	UFUNCTION(BlueprintCallable, Category="Level")
	void UpdateCurrentLevel();
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UWorld>> LegitLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> MainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass;

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);


private:

	UPROPERTY(Transient)
	TSoftObjectPtr<UWorld> CurrentLevel;

	UPROPERTY(Transient)
	UUserWidget* LoadingWidget;



	TSoftObjectPtr<UWorld> GetAssetFromPath(const FString& Path);

	FString GetPathFromAsset(const TSoftObjectPtr<UWorld>& Asset);


};