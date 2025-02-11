// --------------------------------------------------------------------------------------
// Die Rakete, die die Riesenspinnen spuckt
//
// --------------------------------------------------------------------------------------

#include "Gegner_FetteRakete.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerFetteRakete::GegnerFetteRakete(int Wert1, int Wert2, bool Light) {
    Handlung = GEGNER_INIT;
    Energy = 50;
    ChangeLight = Light;
    Destroyable = true;
    Value1 = Wert1;
    Value2 = Wert2;
    SmokeDelay = 0.0f;
    AnimEnde = 10;
    AnimSpeed = 0.3f;
    OwnDraw = true;
    rot = static_cast<float>(Wert1);
    BlickRichtung = LINKS;

    // Rakete kommt vom Drachen? Dann verhält sie sich anders
    if (Wert2 == 99) {
        xSpeed = 0.0f;
        ySpeed = -10.0f;
        yAcc = 2.0f;
        Handlung = GEGNER_DREHEN;
        AnimCount = 12.0f;
    }
}

// --------------------------------------------------------------------------------------
// Eigene Draw Funktion
// --------------------------------------------------------------------------------------

void GegnerFetteRakete::DoDraw() {
    // Rakete rendern
    //
    pGegnerGrafix[GegnerArt]->RenderSpriteRotated(static_cast<float>(xPos - TileEngine.XOffset),
                                                  static_cast<float>(yPos - TileEngine.YOffset),
                                                  static_cast<float>(rot), AnimPhase, 0xFFFFFFFF);
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerFetteRakete::DoKI() {
    BlickRichtung = LINKS;

    switch (Handlung) {
        case GEGNER_INIT: {
            // Rakete kam nicht vom Drachen? Dann
            // enthält Wert1 den Winkel, in den die Raketen fliegen sollen

            // DKS - Support new trig sin/cos lookup table and use deg/rad versions of sin/cos:
            // xSpeed =  float (sin (Value1 * PI / 180.0f) * 35);
            // ySpeed = -float (cos (Value1 * PI / 180.0f) * 35);
            xSpeed = sin_deg(Value1) * 35.0f;
            ySpeed = -cos_deg(Value1) * 35.0f;

            Handlung = GEGNER_LAUFEN;
        } break;

        // Rakete dreht sich in Richtung Hurrican
        case GEGNER_DREHEN: {
            AnimCount -= 1.0f SYNC;

            // Gegner auf Spieler ausrichten
            //
            float dx, dy;

            // Abstände berechnen
            dx = (xPos + 12) - (pAim->xpos + 35);
            dy = (yPos + 17) - (pAim->ypos + 40);

            // Division durch Null verhinden
            if (dy == 0.0f)
                dy = 0.01f;

            // DKS - fixed uninitialized var warning:
            // float w, winkel;

            // DKS - Use new Rad/Deg macros, converted to float:
            // w = float(atan(dx / dy) * 360.0f / (D3DX_PI * 2));
            float w = RadToDeg(atanf(dx / dy));
            float winkel = w;

            if (dx >= 0 && dy >= 0)
                winkel = w;
            else if (dx > 0 && dy < 0)
                winkel = 180 + w;
            else if (dx < 0 && dy > 0)
                winkel = 360 + w;
            else if (dx < 0 && dy < 0)
                winkel = 180 + w;

            winkel = 360.0f - winkel;

            if (rot < winkel)
                rot += 25.0f SYNC;
            if (rot > winkel)
                rot -= 25.0f SYNC;

            // zuende gedreht? Dann losfliegen
            if (AnimCount < 0.0f) {
                Handlung = GEGNER_LAUFEN;
                SoundManager.PlayWave(100, 128, 9000 + random(2000), SOUND_ROCKET);

                xAcc = 0;
                yAcc = 0;
                // DKS - Support new trig sin/cos lookup table and use deg/rad versions of sin/cos:
                // xSpeed =  float (sin (rot * PI / 180.0f) * 35);
                // ySpeed = -float (cos (rot * PI / 180.0f) * 35);
                xSpeed = sin_deg(rot) * 35.0f;
                ySpeed = -cos_deg(rot) * 35.0f;
            }
        } break;

        // Rakete fliegt
        //
        case GEGNER_LAUFEN: {
            SimpleAnimation();

            SmokeDelay -= 1.0f SYNC;

            if (SmokeDelay < 0.0f) {
                SmokeDelay = 0.3f;

                PartikelSystem.PushPartikel(xPos + 2, yPos + 5, SMOKE);
                // PartikelSystem.PushPartikel(xPos + 8, yPos + 12, ROCKETSMOKE);
            }

            // Gegen die Wand geflogen? Dann explodieren lassen
            //
            if (blockl & BLOCKWERT_WAND || blockr & BLOCKWERT_WAND || blocko & BLOCKWERT_WAND ||
                blocku & BLOCKWERT_WAND)
                Energy = 0.0f;

        } break;

        default:
            break;
    }  // switch

    // Spieler getroffen?
    // Dann explodieren lassen und Energy abziehen
    //
    TestDamagePlayers(25.0f, true);
}

// --------------------------------------------------------------------------------------
// FetteRakete explodiert
// --------------------------------------------------------------------------------------

void GegnerFetteRakete::GegnerExplode() {
    SoundManager.PlayWave(100, 128, 8000 + random(4000), SOUND_EXPLOSION1);

    PartikelSystem.PushPartikel(xPos - 10, yPos - 20, EXPLOSION_MEDIUM2);

    for (int i = 0; i < 5; i++) {
        PartikelSystem.PushPartikel(xPos + random(20), yPos - 10 + random(20), MINIFLARE);
        PartikelSystem.PushPartikel(xPos + random(20), yPos - 10 + random(20), SPIDERSPLITTER);
    }

    Player[0].Score += 50;
}
