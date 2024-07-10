// SnakeGame project

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Utils/TestUtils.h"
#include "Framework/SG_GameMode.h"
#include "Framework/SG_Pawn.h"
#include "Misc/PathViews.h"
#include "Framework/SG_GameUserSettings.h"

BEGIN_DEFINE_SPEC(FSnakeFramework, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
USG_GameUserSettings* GameUserSettings;
END_DEFINE_SPEC(FSnakeFramework)

void FSnakeFramework::Define()
{
    using namespace LifeExe::Test;

    Describe("Framework", [this]() {  //
        It("GameMapShouldExist",
            [this]()
            {
                const TArray<FString> SnakeGameMaps = {"GameLevel", "MenuLevel"};

                TArray<FString> AllProjectMaps;
                IFileManager::Get().FindFilesRecursive(AllProjectMaps, *FPaths::ProjectContentDir(), TEXT("*.umap"), true, false);

                for (const auto& SnakeGameMap : SnakeGameMaps)
                {
                    const bool SnakeMapExists = AllProjectMaps.ContainsByPredicate(
                        [&](const FString& ProjectMap)
                        {
                            FStringView OutPath, OutName, OutExtension;
                            FPathViews::Split(FStringView(ProjectMap), OutPath, OutName, OutExtension);
                            return SnakeGameMap.Equals(FString(OutName));
                        });
                    TestTrueExpr(SnakeMapExists);
                }

                UE_LOG(LogTemp, Display, TEXT("------------------------"));
                for (const auto& MapName : AllProjectMaps)
                {
                    UE_LOG(LogTemp, Display, TEXT("Full path to disk: %s"), *MapName);

                    FStringView OutPath, OutName, OutExtension;
                    FPathViews::Split(FStringView(MapName), OutPath, OutName, OutExtension);
                    UE_LOG(LogTemp, Display, TEXT("Map name: %s"), *FString(OutName));
                }
                UE_LOG(LogTemp, Display, TEXT("++++++++++++++++++++++++"));
            });
    });

    Describe("Framework", [this]() {  //
        BeforeEach(
            [this]()
            {
                AutomationOpenMap("/Game/Levels/GameLevel");
                World = GetTestGameWorld();
            });

        It("ClassesMightBeSetupCorrectly",
            [this]()
            {
                TestNotNull("Snake game mode set up", Cast<ASG_GameMode>(World->GetAuthGameMode()));
                TestNotNull("Snake pawn set up", Cast<ASG_Pawn>(World->GetFirstPlayerController()->GetPawn()));
            });

        xIt("PawnLocationShouldBeAdjustedCorrectly", [this]() { unimplemented(); });
    });

    Describe("Framework.GameUserSettings",
        [this]()
        {
            BeforeEach([this]() { GameUserSettings = NewObject<USG_GameUserSettings>(); });

            It("SpeedOptionsMightExist",
                [this]()
                {
                    const TArray<FString> TestOptions{"Worm", "Snake", "Python"};
                    const auto SpeedOptions = GameUserSettings->GameSpeedOptions();

                    TestTrueExpr(TestOptions.Num() == SpeedOptions.Num());

                    for (const auto& Options : TestOptions)
                    {
                        TestTrueExpr(SpeedOptions.Find(Options) != INDEX_NONE);
                    }
                });

            It("SpeedElemCanBeFindByName",
                [this]()
                {
                    const TArray<TTuple<FString, EGameSpeed>> TestData{
                        {"Worm", EGameSpeed::Worm}, {"Snake", EGameSpeed::Snake}, {"Python", EGameSpeed::Python}};

                    for (const auto& [Name, Speed] : TestData)
                    {
                        TestTrueExpr(Speed == GameUserSettings->GameSpeedByName(Name));
                    }
                });
            It("SpeedElemMightBeDefaultIfnameDoesntExist",
                [this]() { TestTrueExpr(EGameSpeed::Snake == GameUserSettings->GameSpeedByName("xxxxxxxxxxxx")); });
            It("SpeedSettingsCanBeSaved",
                [this]()
                {
                    const TArray<TTuple<FString, EGameSpeed, float>> TestData{
                        {"Worm", EGameSpeed::Worm, 0.3f}, {"Snake", EGameSpeed::Snake, 0.1f}, {"Python", EGameSpeed::Python, 0.05f}};

                    for (const auto& [Name, SpeedType, Speed] : TestData)
                    {
                        GameUserSettings->SaveSnakeSettings(SpeedType, EGridSize::Size_30x10);
                        TestTrueExpr(GameUserSettings->CurrentGameSpeedOption().Equals(Name));
                        TestTrueExpr(GameUserSettings->GameSpeed() == Speed);
                    }
                });
        });
}
#endif
