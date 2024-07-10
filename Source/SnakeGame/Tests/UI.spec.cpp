// SnakeGame project

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Utils/TestUtils.h"
#include "UI/SG_StartGameWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"

BEGIN_DEFINE_SPEC(FSnakeUI, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
END_DEFINE_SPEC(FSnakeUI)

void FSnakeUI::Define()
{
    using namespace LifeExe::Test;

    Describe("UI",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap("/Game/Levels/MenuLevel");
                    World = GetTestGameWorld();
                });

            It("AllMenuComponentsShouldExist",
                [this]()
                {
                    auto* StartGameWidget = FindWidgetByClass<USG_StartGameWidget>();
                    check(StartGameWidget);

                    auto* StartGameButton = Cast<UButton>(FindWidgetByName(StartGameWidget, "StartGameButton"));
                    TestTrueExpr(StartGameButton != nullptr);

                    auto* CloseGameButton = Cast<UButton>(FindWidgetByName(StartGameWidget, "CloseGameButton"));
                    TestTrueExpr(CloseGameButton != nullptr);

                    auto* GameSpeedComboBox = Cast<UComboBoxString>(FindWidgetByName(StartGameWidget, "GameSpeedComboBox"));
                    TestTrueExpr(GameSpeedComboBox != nullptr);

                    auto* GridSizeComboBox = Cast<UComboBoxString>(FindWidgetByName(StartGameWidget, "GridSizeComboBox"));
                    TestTrueExpr(GridSizeComboBox != nullptr);
                });
        });
}

#endif
