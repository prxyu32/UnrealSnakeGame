// SnakeGame project

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_StartGameWidget.generated.h"

class UButton;
class UComboBoxString;

UCLASS()
class SNAKEGAME_API USG_StartGameWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> StartGameButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> CloseGameButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UComboBoxString> GameSpeedComboBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UComboBoxString> GridSizeComboBox;

    virtual void NativeOnInitialized() override;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> GameLevel;

private:
    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnCloseGame();

    UFUNCTION()
    void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    void SaveSettings();
    void InitComboBox(                               //
        TObjectPtr<UComboBoxString>& ComboBox,       //
        TFunction<TArray<FString>()> OptionsGetter,  //
        TFunction<FString()> CurrentOptionGetter);
};
