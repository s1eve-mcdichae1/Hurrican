// --------------------------------------------------------------------------------------
// Der Wandkrabbler links
//
// krabbelt an der Wand entlang bis er auf eine (Gegner) Wand stösst und dreht dann um
// Schiesst in gleichen Abständen zur Seite
// Hat Speed == Value2 und SchussDelay == Value1
// --------------------------------------------------------------------------------------

#include "Gegner_KrabblerLinks.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerKrabblerLinks::GegnerKrabblerLinks(int Wert1, int Wert2, bool Light) {
    Handlung = GEGNER_LAUFEN;
    Energy = 75;
    Value1 = Wert1;
    Value2 = Wert2;
    ySpeed = float(Value2);
    BlickRichtung = LINKS;
    ShotDelay = float(Value1);
    AnimSpeed = 0.5f;
    AnimStart = 0;
    AnimEnde = 9;
    ChangeLight = Light;
    Destroyable = true;
}

// --------------------------------------------------------------------------------------
// Bewegungs KI
// --------------------------------------------------------------------------------------

void GegnerKrabblerLinks::DoKI() {
    BlickRichtung = LINKS;

    SimpleAnimation();

    // An der Decke bzw dem Boden
    if (blocko & BLOCKWERT_WAND || blocko & BLOCKWERT_GEGNERWAND)
        ySpeed = float(Value2);

    if (blocku & BLOCKWERT_WAND || blocku & BLOCKWERT_GEGNERWAND)
        ySpeed = float(-Value2);

    // Schuss Delay verzögern
    ShotDelay -= 1.0f SYNC;

    // Je nach Handlung richtig verhalten
    switch (Handlung) {
        case GEGNER_LAUFEN:  // Krabbler krabbelt an der Decke entlang
        {
            // Schiessen ?
            if (ShotDelay <= 0.0f) {
                ShotDelay = float(Value1);
                Handlung = GEGNER_SCHIESSEN;
                Projectiles.PushProjectile(xPos + 24, yPos + 23, KRABBLERLASER2);
                PartikelSystem.PushPartikel(xPos + 48, yPos + 3, LASERFLAME);
                SoundManager.PlayWave3D(int(xPos + 25), int(yPos + 37), 44100, SOUND_LASERSHOT);
                AnimPhase = 10;
                AnimEnde = 14;
                AnimStart = 0;
                AnimSpeed = 0.75f;
            }
        } break;

        case GEGNER_SCHIESSEN: {
            // Fertig mit schiessen ?
            if (AnimPhase == AnimStart) {
                AnimEnde = 9;
                AnimPhase = 0;
                AnimSpeed = 0.5f;
                Handlung = GEGNER_LAUFEN;
            }
        } break;

        default:
            break;
    }  // switch
}

// --------------------------------------------------------------------------------------
// WandKrabbler explodiert
// --------------------------------------------------------------------------------------

void GegnerKrabblerLinks::GegnerExplode() {
    // Explosion
    for (int i = 0; i < 5; i++)
        PartikelSystem.PushPartikel(float(xPos - 15) + random(20), float(yPos - 15) + random(40), EXPLOSION_MEDIUM3);

    SoundManager.PlayWave(100, 128, -random(2000) + 11025, SOUND_EXPLOSION1);  // Sound ausgeben

    Player[0].Score += 300;
}
