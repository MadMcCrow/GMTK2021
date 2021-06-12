// Fill out your copyright notice in the Description page of Project Settings.

#include "FadeViewportClient.h"
#include "Engine/Canvas.h"

void UFadeViewportClient::PostRender(UCanvas* Canvas)
{
        Super::PostRender(Canvas);

        // Fade if requested, you could use the same DrawScreenFade method from any canvas such as the HUD
        if (bFading)
        {
              DrawScreenFade(Canvas);
        }
}

void UFadeViewportClient::ClearFade()
{
        bFading = false;
}

void UFadeViewportClient::Fade(const float Duration, const bool bInToBlack)
{
        if (const UWorld* CurrentWorld = GetWorld())
        {
                bFading = true;
                bToBlack = bInToBlack;
                FadeDuration = Duration;
                FadeStartTime = CurrentWorld->GetTimeSeconds();
        }
}

void UFadeViewportClient::DrawScreenFade(UCanvas* Canvas)
{
        if (bFading)
        {
                if (const UWorld* CurrentWorld = GetWorld())
                {
                        const float Time = World->GetTimeSeconds();
                        const float Alpha = FMath::Clamp((Time - FadeStartTime) / FadeDuration, 0.f, 1.f);

                        // Make sure that we stay black in a fade to black
                        if (Alpha == 1.f && !bToBlack)
                        {
                                bFading = false;
                        }
                        else
                        {
                                FColor OldColor = Canvas->DrawColor;
                                FLinearColor FadeColor = FLinearColor::Black;
                                FadeColor.A = bToBlack ? Alpha : 1 - Alpha;
                                Canvas->DrawColor = FadeColor.ToFColor(true); // TheJamsh: "4.10 cannot convert directly to FColor, so need to use FLinearColor::ToFColor() :)
                                Canvas->DrawTile(Canvas->DefaultTexture, 0, 0, Canvas->ClipX, Canvas->ClipY, 0, 0, Canvas->DefaultTexture->GetSizeX(), Canvas->DefaultTexture->GetSizeY());
                                Canvas->DrawColor = OldColor;
                        }
                }
        }
}