// --------------------------------------------------------------------------------------
// Der Geschützturm
//
// Dreht sich immer in Richtung Spieler und schiesst gerade aus
// --------------------------------------------------------------------------------------

#include "Gegner_GunTower.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerGunTower::GegnerGunTower(int Wert1, int Wert2, bool Light) {
    Handlung = GEGNER_STEHEN;
    AnimStart = 0;
    AnimEnde = 0;
    AnimSpeed = 1.0f;
    xSpeed = 0.0;
    ySpeed = 0.0f;
    xAcc = 0.0f;
    yAcc = 0.0f;
    Energy = 50;
    Value1 = Wert1;
    Value2 = Wert2;
    ChangeLight = Light;
    Destroyable = true;
    TestBlock = false;
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerGunTower::DoKI() {
    SimpleAnimation();

    // Je nach Handlung richtig verhalten
    switch (Handlung) {
        case GEGNER_STEHEN:  // Immer in Richtung Spieler kucken
        {
            if (AnimCount > 0.0f)
                AnimCount -= SpeedFaktor;
            else if (PlayerAbstandVert() < Value2 &&  // Schiessen, sobald der Spieler in den
                     PlayerAbstandHoriz() < 480)      // Toleranz Bereich kommt
            {
                if (BlickRichtung == LINKS)  // Schuss Links
                {
                    PartikelSystem.PushPartikel(xPos - 10, yPos + 6, SMOKE);
                    Projectiles.PushProjectile(xPos, yPos + 15, CANONBALL);
                } else  // Schuss Rechts
                {
                    PartikelSystem.PushPartikel(xPos + 35, yPos + 6, SMOKE);
                    Projectiles.PushProjectile(xPos + 35, yPos + 15, CANONBALL2);
                }
                SoundManager.PlayWave(100, 128, 11025, SOUND_CANON);
                Handlung = GEGNER_SCHIESSEN;
                AnimCount = 1.0f;
                AnimPhase = 1;
                AnimEnde = 5;
                AnimStart = 0;
            }

            if (BlickRichtung == LINKS && xPos + 30 < pAim->xpos) {
                Handlung = GEGNER_DREHEN;
                AnimStart = 12;
                AnimPhase = 5;
                AnimEnde = 13;
            } else if (BlickRichtung == RECHTS && xPos > pAim->xpos + 30) {
                Handlung = GEGNER_DREHEN;
                AnimStart = 12;
                AnimPhase = 5;
                AnimEnde = 13;
            }

        } break;

        case GEGNER_DREHEN:  // Turm dreht sich in Richtung Spieler
        {
            if (AnimPhase == AnimStart) {
                if (BlickRichtung == LINKS)
                    BlickRichtung = RECHTS;
                else
                    BlickRichtung = LINKS;
                AnimEnde = 0;
                AnimPhase = 0;
                Handlung = GEGNER_STEHEN;
            }
        } break;

        case GEGNER_SCHIESSEN:  // Turm schiesst auf den Spieler
        {
            if (AnimPhase == AnimStart)  // Letzte Animationsphase beim schiessen ?
            {
                AnimPhase = 0;
                AnimEnde = 0;
                AnimStart = 0;
                AnimCount = float(Value1);  // Je nach Editor-Einstellung die Reload-Sperre
                Handlung = GEGNER_STEHEN;   // setzen
            }
        } break;

        default:
            break;
    }  // switch
}

// --------------------------------------------------------------------------------------
// Explodieren
// --------------------------------------------------------------------------------------

void GegnerGunTower::GegnerExplode() {
    PartikelSystem.PushPartikel(float(xPos - 20), float(yPos - 40), EXPLOSION_BIG);
    for (int i = 0; i < 2; i++)
        PartikelSystem.PushPartikel(float(xPos - 10 + random(60)), float(yPos - 40 + random(20)), EXPLOSION_MEDIUM2);

    SoundManager.PlayWave(100, 128, -random(2000) + 11025, SOUND_EXPLOSION1);  // Sound ausgeben

    Player[0].Score += 200;
}
