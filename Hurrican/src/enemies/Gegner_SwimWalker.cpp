// --------------------------------------------------------------------------------------
// Der schwimmende Walker
//
// Schwimmt im Wasser rum und ballert Torpedos auf den Hurri
// --------------------------------------------------------------------------------------

#include "Gegner_SwimWalker.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerSwimWalker::GegnerSwimWalker(int Wert1, int Wert2, bool Light) {
    Handlung = GEGNER_LAUFEN;
    Energy = 25;
    Value1 = Wert1;
    Value2 = Wert2;
    ChangeLight = Light;
    Destroyable = true;
    xSpeed = 7.0f;
    ySpeed = 0.0f;
    yAcc = 2.0f;
    AnimSpeed = 1.5f;
    AnimEnde = 10;
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerSwimWalker::DoKI() {
    SimpleAnimation();

    // Nach links bzw rechts auf Kollision prüfen und dann ggf umkehren
    if (BlickRichtung == LINKS)
        if (blockl & BLOCKWERT_WAND || blockl & BLOCKWERT_GEGNERWAND) {
            BlickRichtung = RECHTS;
            xSpeed = 7.0f;
        }

    if (BlickRichtung == RECHTS)
        if (blockr & BLOCKWERT_WAND || blockr & BLOCKWERT_GEGNERWAND) {
            BlickRichtung = LINKS;
            xSpeed = -7.0f;
        }

    // auf und ab schwimmen
    if (ySpeed > 9.0f)
        yAcc = -2.0f;
    if (ySpeed < -9.0f)
        yAcc = 2.0f;

    // Je nach Handlung richtig verhalten
    switch (Handlung) {
        case GEGNER_LAUFEN:  // Normal laufen und dabei ab und zu schiessen
        {
            // Bei bestimmten Mindestabstand schiessen lassen
            if (PlayerAbstand() <= 250 && random(200) == 0 &&
                ((BlickRichtung == LINKS && pAim->xpos + 45 <= xPos) ||
                 (BlickRichtung == RECHTS && pAim->xpos - 45 >= xPos))) {
                SoundManager.PlayWave(100, 128, 11025, SOUND_ROCKET);

                if (BlickRichtung == LINKS)
                    Projectiles.PushProjectile(xPos - 25, yPos + 10, TORPEDO, pAim);
                else
                    Projectiles.PushProjectile(xPos + 55, yPos + 10, TORPEDO, pAim);

                Handlung = GEGNER_SCHIESSEN;
            }
        } break;

        case GEGNER_SCHIESSEN: {
            if (AnimPhase > AnimEnde - 2)
                Handlung = GEGNER_LAUFEN;
        } break;

        default:
            break;
    }  // switch

    // Testen, ob der Spieler den Walker berührt hat
    TestDamagePlayers(4.0f SYNC);
}

// --------------------------------------------------------------------------------------
// Explodieren
// --------------------------------------------------------------------------------------

void GegnerSwimWalker::GegnerExplode() {
    // blubbern
    PartikelSystem.PushPartikel(xPos + random(20), yPos + random(20), EXPLOSION_MEDIUM3);

    PartikelSystem.PushPartikel(xPos, yPos, EXPLOSION_MEDIUM2);

    for (int i = 0; i < 30; i++) {
        PartikelSystem.PushPartikel(xPos - 10 + random(85), yPos - 10 + random(65), BUBBLE);
    }

    SoundManager.PlayWave(100, 128, 11025 + random(2000), SOUND_EXPLOSION1);  // Sound ausgeben

    Player[0].Score += 100;
}
