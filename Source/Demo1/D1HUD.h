#pragma once 

#include <CoreMinimal.h>
#include <GameFramework/HUD.h>
#include "D1HUD.generated.h"

UCLASS()
class AD1HUD : public AHUD
{
	GENERATED_BODY()

public:
	AD1HUD();

	virtual void DrawHUD() override;

private:
	class UTexture2D* CrosshairTex;

};

