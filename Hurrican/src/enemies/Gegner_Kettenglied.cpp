// --------------------------------------------------------------------------------------
// Das Kettenglied
//
// hängt zb. an dem Rollmops
// --------------------------------------------------------------------------------------

#include "Gegner_Kettenglied.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerKettenglied::GegnerKettenglied(int Wert1, int Wert2, bool Light) {
    Handlung = GEGNER_STEHEN;
    Energy = 1000;
    Value1 = Wert1;
    Value2 = Wert2;
    ChangeLight = Light;
    Destroyable = true;
    OwnDraw = true;
    AnimCount = 255.0f;
    AnimSpeed = -1.0f;
}

void GegnerKettenglied::DoDraw() {
    // rendern
    //
    D3DCOLOR Color;

    // Gegner getroffen? Dann nochmal in weiss aufleuchten lassen
    int Wert = 255 - static_cast<int>(DamageTaken);

    if (AnimSpeed < 0.0f) {
        if (DirectGraphics.BlendMode == BlendModeEnum::ADDITIV)
            Color = D3DCOLOR_RGBA(255, 255, 255, Wert);
        else
            Color = 0xFFFFFFFF;
    } else {
        Wert = static_cast<int>(AnimSpeed);
        Color = D3DCOLOR_RGBA(255, 255, 255, Wert);
    }

    pGegnerGrafix[GegnerArt]->RenderSpriteRotated(static_cast<float>(xPos - TileEngine.XOffset),
                                                  static_cast<float>(yPos - TileEngine.YOffset), AnimCount, AnimPhase,
                                                  Color);
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerKettenglied::DoKI() {
    // Je nach Handlung richtig verhalten
    //
    switch (Handlung) {
        // Stein wartet bis der Spieler darüber läuft
        case GEGNER_STEHEN: {
            // Schaden genommen? Dann diesen Schaden dem Parent zufügen
            if (Energy < 1000.0f) {
                if (pParent->AnimCount <= 0.0f) {
                    pParent->AnimCount = 0.1f;
                    pParent->Energy -= (1000.0f - Energy);
                }

                Energy = 1000.0f;
            }
        } break;

        // Kettenglied fliegt los
        case GEGNER_SPECIAL: {
            AnimCount = static_cast<float>(random(360));

            Destroyable = false;
            xSpeed = static_cast<float>(random(40) - 20);
            ySpeed = -static_cast<float>(random(40) - 20);
            yAcc = 5.0f;

            Handlung = GEGNER_SPECIAL2;

            AnimSpeed = 255.0f;
        } break;

        case GEGNER_SPECIAL2: {
            uint32_t bu = TileEngine.BlockUnten(xPos, yPos, xPosOld, yPosOld, GegnerRect[GegnerArt]);

            if (bu & BLOCKWERT_WAND && ySpeed > 0.0f) {
                ySpeed *= -0.3f;

                if (ySpeed > -0.1f)
                    ySpeed = 0.0f;
            }

            AnimSpeed -= 5.0f SYNC;

            if (AnimSpeed < 0.0f)
                Energy = 0.0f;

            if (ySpeed != 0.0f)
                AnimCount += 50.0f SYNC;

            if (AnimCount > 360.0f)
                AnimCount = 0.0f;

        } break;

        default:
            break;
    }  // switch
}

// --------------------------------------------------------------------------------------
// Explodieren
// --------------------------------------------------------------------------------------

void GegnerKettenglied::GegnerExplode() {}
