// --------------------------------------------------------------------------------------
// Die Riesen Raupe
//
// kriecht auf den Spieler zu und schiesst 2 Suchschüsse bei richtigem Abstand
// --------------------------------------------------------------------------------------

#include "Boss_RiesenRaupe.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerRiesenRaupe::GegnerRiesenRaupe(int Wert1, int Wert2, bool Light) {
    Handlung = GEGNER_LAUFEN;
    Energy = 800;
    Value1 = Wert1;
    Value2 = Wert2;
    ChangeLight = Light;
    Destroyable = true;
    AnimEnde = 10;
    AnimSpeed = 1.5f;
    BlickRichtung = LINKS;
    ShotCount = 0;
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerRiesenRaupe::DoKI() {
    // animieren
    AnimCount += SpeedFaktor;   // Animationscounter weiterzählen
    if (AnimCount > AnimSpeed)  // Grenze überschritten ?
    {
        AnimCount = 0;  // Dann wieder auf Null setzen
        AnimPhase++;    // Und nächste Animationsphase

        if (AnimPhase > AnimEnde)  // Animation zu Ende	?
        {
            AnimPhase = AnimStart;
            xSpeed = 0.0f;

            if (PlayerAbstand() > 400 || ShotCount == 0) {
                ShotCount = random(5);
                AnimEnde = 10;
                AnimSpeed = 1.5f;
            }

            // evtl schiessen?
            //
            else {
                // evtl schiessen wenn Spieler zu nahe
                ShotCount--;

                AnimPhase = 10;
                AnimEnde = 13;
                AnimSpeed = 1.5f;

                PartikelSystem.PushPartikel(xPos + 30, yPos - 2, SMOKE);
                PartikelSystem.PushPartikel(xPos + 120, yPos - 2, SMOKE);

                Projectiles.PushProjectile(xPos + 30, yPos - 2, PFLANZESHOT, pAim);
                Projectiles.PushProjectile(xPos + 120, yPos - 2, PFLANZESHOT, pAim);

                SoundManager.PlayWave(100, 128, 11025, SOUND_ROCKET);
            }
        }
    }

    if (AnimPhase < 10) {
        if (BlickRichtung == LINKS)
            xSpeed = -1.0f;
        else
            xSpeed = 1.0f;
    }

    // runterfallen ?
    if (!(blocku & BLOCKWERT_WAND) && !(blocku & BLOCKWERT_PLATTFORM)) {
        yAcc = 3.0f;

        if (ySpeed > 20.0f)
            ySpeed = 20.0f;
    } else {
        ySpeed = 0.0f;
        yAcc = 0.0f;
    }

    // umdrehen ?
    if ((blockl & BLOCKWERT_WAND || blockl & BLOCKWERT_GEGNERWAND) && xSpeed < 0.0f) {
        BlickRichtung = RECHTS;
        xSpeed *= -1;
    }

    if ((blockr & BLOCKWERT_WAND || blockr & BLOCKWERT_GEGNERWAND) && xSpeed > 0.0f) {
        BlickRichtung = LINKS;
        xSpeed *= -1;
    }

    // Testen, ob der Spieler die Raupe berührt hat
    TestDamagePlayers(5.0f SYNC);
}

// --------------------------------------------------------------------------------------
// RiesenRaupe explodiert
// --------------------------------------------------------------------------------------

void GegnerRiesenRaupe::GegnerExplode() {
    // Explosionen erzeugen
    for (int i = 0; i < 10; i++) {
        PartikelSystem.PushPartikel(xPos + random(180) - 30, yPos + random(60) - 30, EXPLOSION_BIG);
        PartikelSystem.PushPartikel(xPos + random(180) - 30, yPos + random(60) - 30, SPIDERSPLITTER);
    }

    for (int i = 0; i < 20; i++)
        PartikelSystem.PushPartikel(xPos + random(180) - 10, yPos + random(60) - 10, EXPLOSION_MEDIUM2);

    for (int i = 0; i < 300; i++)
        PartikelSystem.PushPartikel(xPos + random(180) - 10, yPos + random(50), ROCKETSMOKE);

    SoundManager.PlayWave(100, 128, 11025 + random(2000), SOUND_EXPLOSION3);  // Sound ausgeben

    Player[0].Score += 250;
}
