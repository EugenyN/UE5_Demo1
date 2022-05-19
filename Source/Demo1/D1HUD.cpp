#include "Demo1/D1HUD.h"

#include <Engine/Canvas.h>
#include <Engine/Texture2D.h>
#include <TextureResource.h>
#include <CanvasItem.h>
#include <UObject/ConstructorHelpers.h>

AD1HUD::AD1HUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/Demo/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AD1HUD::DrawHUD()
{
	Super::DrawHUD();

	const FVector2D CrosshairDrawPosition(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->GetResource(), FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}
