// --------------------------------------------------------------------------------------
// Der RoboMan1
//
// fliegt und schiesst Raketen
// --------------------------------------------------------------------------------------

#include "Gegner_RoboMan1.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerRoboMan1::GegnerRoboMan1(int Wert1, int Wert2, bool Light) {
    Handlung = GEGNER_LAUFEN;
    Energy = 150;
    Value1 = Wert1;
    Value2 = Wert2;
    ChangeLight = Light;
    Destroyable = true;
    AnimEnde = 0;
    AnimSpeed = 1.0f;

    ShotCount = 1.0f;
    SmokeCount = 0.2f;
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerRoboMan1::DoKI() {
    blocko = TileEngine.BlockOben(xPos, yPos, xPosOld, yPosOld, GegnerRect[GegnerArt], true);
    blocku = TileEngine.BlockUnten(xPos, yPos, xPosOld, yPosOld, GegnerRect[GegnerArt], true);
    blockl = TileEngine.BlockLinks(xPos, yPos, xPosOld, yPosOld, GegnerRect[GegnerArt], true);
    blockr = TileEngine.BlockRechts(xPos, yPos, xPosOld, yPosOld, GegnerRect[GegnerArt], true);

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
                if (AnimPhase == 14)
                    BlickRichtung = -BlickRichtung;

                AnimEnde = 0;
                AnimPhase = 0;
            }
        }
    }  // animieren

    // Rauch ausgeben
    SmokeCount -= 1.0f SYNC;
    if (SmokeCount < 0.0f) {
        SmokeCount += 0.1f;

        PartikelSystem.PushPartikel(xPos + 29 + random(5), yPos + 90, ROBOMANSMOKE);
        PartikelSystem.PushPartikel(xPos + 40 + random(5), yPos + 90, ROBOMANSMOKE);
    }

    // Immer den Spieler im Auge behalten
    //
    if (AnimPhase == 0 && BlickRichtung == RECHTS && pAim->xpos + 45 < xPos + 45) {
        AnimPhase = 3;
        AnimStart = 0;
        AnimEnde = 14;
        AnimSpeed = 0.5f;
    }

    if (AnimPhase == 0 && BlickRichtung == LINKS && pAim->xpos + 45 > xPos + 45) {
        AnimPhase = 3;
        AnimStart = 0;
        AnimEnde = 14;
        AnimSpeed = 0.5f;
    }

    // Vor dem Spieler wegfliegen ?
    if (PlayerAbstand() < 200 && Handlung != GEGNER_FALLEN) {
        if (pAim->xpos + 45 > xPos + 45)
            xPos -= 8.0f SYNC;
        if (pAim->xpos + 45 < xPos + 45)
            xPos += 8.0f SYNC;
    }

    // Zum Spieler hinfliegen ?
    if (PlayerAbstand() > 300 && Handlung != GEGNER_FALLEN) {
        if (pAim->xpos + 45 > xPos + 45)
            xPos += 8.0f SYNC;
        if (pAim->xpos + 45 < xPos + 45)
            xPos -= 8.0f SYNC;
    }

    // evtl schiessen
    if (PlayerAbstand() < 500 && PlayerAbstand() > 50 && Handlung != GEGNER_FALLEN && AnimEnde == 0) {
        ShotCount -= 1.0f SYNC;

        if (ShotCount <= 0.0f) {
            SoundManager.PlayWave(100, 128, 11025, SOUND_GRANATE);

            if (BlickRichtung == LINKS)
                Projectiles.PushProjectile(xPos - 20, yPos + 75, ROBOROCKET, pAim);
            else
                Projectiles.PushProjectile(xPos + 70, yPos + 75, ROBOROCKET, pAim);

            ShotCount = 15.0f;
            AnimEnde = 3;
            AnimStart = 0;
            AnimPhase = 0;
            AnimSpeed = 1.0f;
            AnimCount = 0.0f;
        }
    }

    // yPosition mit dem Spieler angleichen
    if (Handlung != GEGNER_FALLEN) {
        if (yPos < pAim->ypos - 40)
            yAcc = 2.5f;
        if (yPos > pAim->ypos - 40)
            yAcc = -2.5f;

        ySpeed = std::clamp(ySpeed, -10.0f, 10.0f);
    }

    if ((blocko & BLOCKWERT_WAND || blocko & BLOCKWERT_GEGNERWAND) && ySpeed < 0.0f)
        ySpeed *= -1.0f;

    if ((blocku & BLOCKWERT_WAND || blocku & BLOCKWERT_GEGNERWAND) && ySpeed > 0.0f)
        ySpeed *= -1.0f;

    switch (Handlung) {
        // Robo stürzt ab
        case GEGNER_FALLEN: {
            // An die Wand gekracht ?
            if (blockl & BLOCKWERT_WAND || blockr & BLOCKWERT_WAND || blocko & BLOCKWERT_WAND ||
                blocku & BLOCKWERT_WAND || blocku & BLOCKWERT_PLATTFORM)
                Energy = 0.0f;

            // Grenze der Fallgeschwindigkeit
            if (ySpeed > 35.0f)
                yAcc = 0.0f;

            // Robo rauchen lassen
            ShotCount -= 3.0f SYNC;
            if (ShotCount <= 0.0f) {
                PartikelSystem.PushPartikel(xPos + random(80), yPos + random(90), SMOKE);
                ShotCount = 1.0f;
            }
        } break;

        default:
            break;
    }  // switch

    // Testen, ob der Spieler den Robo berührt hat
    if (Handlung != GEGNER_FALLEN)
        TestDamagePlayers(4.0f SYNC);

    // Soviel Energie verloren, dass der Robo abstürzt ?
    if (Energy <= 0.0f && Handlung != GEGNER_FALLEN) {
        Handlung = GEGNER_FALLEN;
        Energy = 60.0f;
        ySpeed = 3.0f;
        yAcc = 2.0f;

        /*		if (BlickRichtung == LINKS)
                    xSpeed = -5.0f;
                else
                    xSpeed = 5.0f;*/
    }
}

// --------------------------------------------------------------------------------------
// RoboMan explodiert
// --------------------------------------------------------------------------------------

void GegnerRoboMan1::GegnerExplode() {
    SoundManager.PlayWave(100, 128, 8000 + random(4000), SOUND_EXPLOSION3);

    int i;

    for (i = 0; i < 3; i++)
        PartikelSystem.PushPartikel(xPos - 40 + random(70), yPos + random(40), EXPLOSION_BIG);
    for (i = 0; i < 10; i++)
        PartikelSystem.PushPartikel(xPos - 10 + random(70), yPos + random(60), EXPLOSION_MEDIUM2);
    for (i = 0; i < 10; i++)
        PartikelSystem.PushPartikel(xPos + random(70), yPos + 20 + random(60), SPIDERSPLITTER);
}
