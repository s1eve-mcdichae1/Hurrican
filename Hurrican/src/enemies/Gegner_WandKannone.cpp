// --------------------------------------------------------------------------------------
// Die WandKannone im FlugSack Level
//
// Hängt an der Wand und ballert regelmäsig (Value2)
// Value1 = 0 Gegner blickt links
// Value1 = 1 Gegner blickt rechts
// --------------------------------------------------------------------------------------

#include "Gegner_WandKannone.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerWandKannone::GegnerWandKannone(int Wert1, int Wert2, bool Light) {
    Handlung = GEGNER_LAUFEN;
    Energy = 30;
    Value1 = Wert1;
    Value2 = Wert2;
    AnimStart = 0;
    AnimEnde = 0;
    AnimSpeed = 0.0f;
    AnimCount = float(Value1);
    ChangeLight = Light;
    Destroyable = true;
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerWandKannone::DoKI() {
    // Richtung setzen
    //
    if (Value2 == 0)
        BlickRichtung = LINKS;
    else
        BlickRichtung = RECHTS;

    // Animieren
    if (AnimEnde > 0)  // Soll überhaupt anmiert werden ?
    {
        AnimCount += SpeedFaktor;   // Animationscounter weiterzählen
        if (AnimCount > AnimSpeed)  // Grenze überschritten ?
        {
            AnimCount = 0;              // Dann wieder auf Null setzen
            AnimPhase++;                // Und nächste Animationsphase
            if (AnimPhase >= AnimEnde)  // Animation von zu Ende	?
            {
                AnimPhase = AnimStart;  // Dann wieder von vorne beginnen
                AnimEnde = 0;
                AnimCount = static_cast<float>(Value1);
                Handlung = GEGNER_LAUFEN;
            }
        }
    }  // animieren

    // Je nach Handlung richtig verhalten
    //
    switch (Handlung) {
        case GEGNER_LAUFEN: {
            AnimCount -= 1.0f SYNC;

            if (AnimCount <= 0.0) {
                AnimCount = 0.0f;
                Handlung = GEGNER_SCHIESSEN;
                AnimEnde = 3;
                AnimSpeed = 0.5f;
                AnimStart = 0;
                AnimPhase = 0;

                // Schuss abgeben
                //
                if (PlayerAbstand() < 600) {
                    SoundManager.PlayWave(100, 128, 22050, SOUND_LASERSHOT);
                    if (BlickRichtung == LINKS) {
                        Projectiles.PushProjectile(xPos - 18, yPos + 15, WALKER_LASER);
                        PartikelSystem.PushPartikel(xPos - 24, yPos - 10, LASERFLAME);

                    } else {
                        Projectiles.PushProjectile(xPos + 30, yPos + 15, WALKER_LASER2);
                        PartikelSystem.PushPartikel(xPos + 21, yPos - 10, LASERFLAME);
                    }
                }
            }
        } break;

        default:
            break;

    }  // switch
}

// --------------------------------------------------------------------------------------
// WandKannnone explodiert
// --------------------------------------------------------------------------------------

void GegnerWandKannone::GegnerExplode() {
    for (int i = 0; i < 10; i++)
        PartikelSystem.PushPartikel(xPos - 30 + random(42), yPos - 30 + random(32), EXPLOSION_MEDIUM2);

    SoundManager.PlayWave(100, 128, 8000 + random(4000), SOUND_EXPLOSION1);

    Player[0].Score += 150;  // Punkte geben
}
