// --------------------------------------------------------------------------------------
// Die Pharao-Kopf Endgegner
//
// rammt seitlich gegen die Wände und versucht den Hurri zu zerquetschen
// lässt ggf Steine von der Decke rieseln
// --------------------------------------------------------------------------------------

#include "Boss_PharaoKopf.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerPharaoKopf::GegnerPharaoKopf(int Wert1, int Wert2, bool Light) {
    Handlung = GEGNER_NOTVISIBLE;
    BlickRichtung = LINKS;

    Energy = 6000;
    Value1 = Wert1;
    Value2 = Wert2;
    AnimPhase = 0;
    ChangeLight = Light;
    AnimCount = 100.0f;  // Counter für Spezial Aktion
    Destroyable = true;
    TestBlock = false;
}

bool GegnerPharaoKopf::Links() {
    if (static_cast<int>(xPos) < Value1 + BORDER) {
        xPos = static_cast<float>(Value1 + BORDER);
        return true;
    }

    return false;
}

bool GegnerPharaoKopf::Rechts() {
    if (static_cast<int>(xPos) + GegnerRect[GegnerArt].right > Value1 + 652 - BORDER) {
        xPos = static_cast<float>(Value1 + 652 - BORDER - GegnerRect[GegnerArt].right);
        return true;
    }

    return false;
}

bool GegnerPharaoKopf::Unten() {
    if (static_cast<int>(yPos) + GegnerRect[GegnerArt].bottom > Value2 + 480 - BORDER2) {
        yPos = static_cast<float>(Value2 + 480 - BORDER2 - GegnerRect[GegnerArt].bottom);
        return true;
    }

    return false;
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerPharaoKopf::DoKI() {
    // Energie anzeigen
    if (Handlung != GEGNER_NOTVISIBLE && Handlung != GEGNER_EXPLODIEREN)
        HUD.ShowBossHUD(6000, Energy);

    //----- Pharao dient auch als Plattform
    PlattformTest(GegnerRect[GegnerArt]);

    //----- Pharao Bewegungs Code

    // Levelausschnitt auf den PharaoKopf zentrieren, sobald dieser sichtbar wird
    if (Active == true && TileEngine.Zustand == TileStateEnum::SCROLLBAR) {
        TileEngine.ScrollLevel(static_cast<float>(Value1), static_cast<float>(Value2),
                               TileStateEnum::SCROLLTOLOCK);  // Level auf die Faust zentrieren

        SoundManager.FadeSong(MUSIC_STAGEMUSIC, -2.0f, 0, true);  // Ausfaden und pausieren
    }

    // Zwischenboss blinkt nicht so lange wie die restlichen Gegner
    if (DamageTaken > 0.0f)
        DamageTaken -= 100 SYNC;  // Rotwerden langsam ausfaden lassen
    else
        DamageTaken = 0.0f;  // oder ganz anhalten

    // Hat der PharaoKopf keine Energie mehr ? Dann explodiert er
    if (Energy <= 100.0f && Handlung != GEGNER_EXPLODIEREN) {
        Handlung = GEGNER_EXPLODIEREN;
        xSpeed = 0.0f;
        ySpeed = 0.0f;
        xAcc = 0.0f;
        yAcc = 0.0f;
        AnimCount = 20.0f;

        SoundManager.PlayWave(100, 128, 11025, SOUND_EXPLOSION1);
        SoundManager.PlayWave(100, 128, 11025, SOUND_PHARAODIE);

        // Endboss-Musik ausfaden und abschalten
        SoundManager.FadeSong(MUSIC_BOSS, -2.0f, 0, false);
    }

    // Kopf zieht dem Spieler Energie an wenn er ihn berührt
    TestDamagePlayers(2.0f SYNC);

    if (Energy >= 3000 && AnimPhase > 1)
        AnimPhase = 0;

    // Hälfte der Energie verloren ? Dann Risse Zeigen und kurz Brocken fliegen lassen
    if (Energy < 3000 && AnimPhase < 2) {
        AnimPhase = 2;

        // Brocken
        for (int i = 0; i < 100; i++) {
            PartikelSystem.PushPartikel(xPos + random(224), yPos + random(224), ROCKSPLITTER);
            PartikelSystem.PushPartikel(xPos + random(224), yPos + random(224), ROCKSPLITTERSMALL);
        }

        SoundManager.PlayWave(100, 128, 11025, SOUND_PHARAORAMM);
        SoundManager.PlayWave(100, 128, 11025, SOUND_PHARAODIE);
    }

    // Je nach Handlung richtig verhalten
    switch (Handlung) {
        case GEGNER_NOTVISIBLE:  // Warten bis der Screen zentriert wurde
        {
            if (TileEngine.Zustand == TileStateEnum::LOCKED) {
                // Zwischenboss-Musik abspielen, sofern diese noch nicht gespielt wird
                // DKS - Added function SongIsPlaying() to SoundManagerClass:
                if (!SoundManager.SongIsPlaying(MUSIC_BOSS))
                    SoundManager.PlaySong(MUSIC_BOSS, false);

                // Und Boss erscheinen lassen
                Handlung = GEGNER_EINFLIEGEN;

                AnimCount = 1.0f;
            }
        } break;

        case GEGNER_EINFLIEGEN:  // Gegner kommt aus dem Boden raus
        {
            Energy = 6000;
            DamageTaken = 0.0f;

            yPos -= float(4.5 SYNC);  // Pharao erscheint langsam

            AnimCount -= 1.0f SYNC;

            // Boden brodeln lassen wo er auftaucht
            if (AnimCount <= 0.0f) {
                PartikelSystem.PushPartikel(xPos + float(random(230)) - 30, float(Value2 + 400 + random(12)),
                                            SMOKEBIG);
                PartikelSystem.PushPartikel(xPos + float(random(230)) - 12, float(Value2 + 405 + random(12)),
                                            ROCKSPLITTER);
                PartikelSystem.PushPartikel(xPos + float(random(230)) - 12, float(Value2 + 410 + random(12)),
                                            ROCKSPLITTERSMALL);

                AnimCount += 0.2f;
            }

            if (yPos <=
                static_cast<float>(Value2) + 480 - BORDER2 - GegnerRect[GegnerArt].bottom)  // Weit genug unten ?
            {
                for (int p = 0; p < NUMPLAYERS; p++)
                    if (Player[p].AufPlattform == this) {
                        ySpeed = -10.0f;
                        yAcc = -3.0f;
                        goto _weiter;
                    }

                if (pAim->xpos < xPos) {
                    xSpeed = -1.0f;
                    xAcc = -3.0f;
                } else {
                    xSpeed = 1.0f;
                    xAcc = 3.0f;
                }
            _weiter:
                yPos = static_cast<float>(Value2) + 480 - BORDER2 - GegnerRect[GegnerArt].bottom;
                SoundManager.PlayWave(100, 128, 8000, SOUND_STONEFALL);

                Handlung = GEGNER_LAUFEN;
            }
        } break;

        case GEGNER_LAUFEN:  // In Richtung Spieler rutschen
        {
            xSpeed = 0.0f;
            ySpeed = 0.0f;

            // Spieler steht auf dem Pharao Kopf ?
            for (int p = 0; p < NUMPLAYERS; p++)
                if (Player[p].AufPlattform == this) {
                    ySpeed = -10.0f;
                    yAcc = -3.0f;
                    goto _weiter2;
                }

                // Spieler ist am Boden
                else {
                    if (pAim->xpos < xPos) {
                        xSpeed = -1.0f;
                        xAcc = -3.0f;
                    } else {
                        xSpeed = 1.0f;
                        xAcc = 3.0f;
                    }

                    for (p = 0; p < NUMPLAYERS; p++)
                        if (SpriteCollision(xPos, yPos, GegnerRect[GegnerArt], Player[p].xpos, Player[p].ypos,
                                            Player[p].CollideRect) == true) {
                            xSpeed = -xSpeed;
                            xAcc = -xAcc;
                        }
                }
        _weiter2:

            Handlung = GEGNER_CRUSHEN;
        } break;

        case GEGNER_CRUSHEN:  // An die Wand klatschen und an der Wand anhalten =)
        {
            // Partikel am Boden
            if (ySpeed == 0)
                PartikelSystem.PushPartikel(xPos + random(224), yPos + 210, ROCKSPLITTERSMALL);

            // Links an die Wand rutschen ?
            if (xSpeed < 0) {
                // Hurri verschieben
                for (int p = 0; p < NUMPLAYERS; p++)
                    if (SpriteCollision(xPos, yPos, GegnerRect[GegnerArt], Player[p].xpos, Player[p].ypos,
                                        Player[p].CollideRect) == true &&
                        !(TileEngine.BlockLinks(Player[p].xpos, Player[p].ypos, Player[p].xposold, Player[p].yposold,
                                                Player[p].CollideRect) &
                          BLOCKWERT_WAND))
                        Player[p].xpos += xSpeed SYNC;

                // an die Wand gekommen?
                if (Links()) {
                    xSpeed = 0.0f;
                    xAcc = 0.0f;

                    for (int p = 0; p < NUMPLAYERS; p++)
                        DirectInput.Joysticks[Player[p].JoystickIndex].ForceFeedbackEffect(FFE_MEDIUMRUMBLE);

                    for (int i = 0; i < 20; i++) {
                        PartikelSystem.PushPartikel(xPos - 10, yPos - 30 + random(224), SMOKEBIG);
                        PartikelSystem.PushPartikel(xPos - 10, yPos + random(224), ROCKSPLITTER);
                        PartikelSystem.PushPartikel(xPos - 10, yPos + random(224), ROCKSPLITTERSMALL);
                    }

                    SoundManager.PlayWave(100, 128, 11025, SOUND_PHARAORAMM);
                    Handlung = GEGNER_CRUSHENERHOLEN;
                    AnimCount = 10.0f;

                    // Hurri dabei erwischt ?
                    TestDamagePlayers(1000.0f);

                    // Screen Wackeln lassen
                    ShakeScreen(3);
                }
            }

            // Rechts an die Wand rutschen ?
            if (xSpeed > 0) {
                // Hurri verschieben
                for (int p = 0; p < NUMPLAYERS; p++)
                    if (SpriteCollision(xPos, yPos, GegnerRect[GegnerArt], Player[p].xpos, Player[p].ypos,
                                        Player[p].CollideRect) == true &&
                        !(blockr & BLOCKWERT_WAND))
                        Player[p].xpos += xSpeed SYNC;

                // an die Wand gekommen?
                if (Rechts()) {
                    for (int p = 0; p < NUMPLAYERS; p++)
                        DirectInput.Joysticks[Player[p].JoystickIndex].ForceFeedbackEffect(FFE_MEDIUMRUMBLE);

                    xSpeed = 0.0f;
                    xAcc = 0.0f;
                    for (int i = 0; i < 20; i++) {
                        PartikelSystem.PushPartikel(xPos + 175, yPos + random(224) - 30, SMOKEBIG);
                        PartikelSystem.PushPartikel(xPos + 212, yPos + random(224), ROCKSPLITTER);
                        PartikelSystem.PushPartikel(xPos + 212, yPos + random(224), ROCKSPLITTERSMALL);
                    }

                    SoundManager.PlayWave(100, 128, 11025, SOUND_PHARAORAMM);
                    Handlung = GEGNER_CRUSHENERHOLEN;
                    AnimCount = 10.0f;

                    // Hurri dabei erwischt ?
                    TestDamagePlayers(1000.0f);

                    // Screen Wackeln lassen
                    ShakeScreen(3);
                }
            }

            // An die Decke gekommen ?
            if (ySpeed < 0) {
                if (yPos < static_cast<float>(Value2)) {
                    for (int i = 0; i < 20; i++) {
                        PartikelSystem.PushPartikel(xPos - 30 + random(224), yPos, SMOKEBIG);
                        PartikelSystem.PushPartikel(xPos + random(224), yPos + 10, ROCKSPLITTER);
                        PartikelSystem.PushPartikel(xPos + random(224), yPos + 10, ROCKSPLITTERSMALL);
                    }

                    SoundManager.PlayWave(100, 128, 11025, SOUND_PHARAORAMM);
                    yPos = static_cast<float>(Value2);
                    ySpeed = 5.0f;
                    yAcc = 10.0f;

                    for (int p = 0; p < NUMPLAYERS; p++) {
                        if (Player[p].AufPlattform == this)
                            Player[p].DamagePlayer(200.0f);

                        Player[p].AufPlattform = nullptr;
                    }

                    // Screen Wackeln lassen
                    ShakeScreen(3);
                }
            } else

                // An den Boden gekommen ?
                if (ySpeed > 0.0f) {
                // Hurri dabei erwischt ?
                for (int p = 0; p < NUMPLAYERS; p++)
                    if (Player[p].Energy > 0 && Player[p].AufPlattform == nullptr &&
                        SpriteCollision(xPos, yPos, GegnerRect[GegnerArt], Player[p].xpos, Player[p].ypos,
                                        Player[p].CollideRect) == true) {
                        Player[p].DamagePlayer(25.0f);

                        // Wieder hoch fliegen
                        ySpeed = -10.0f;
                        yAcc = -3.0f;
                    }

                if (Unten()) {
                    ySpeed = 0.0f;
                    yAcc = 0.0f;
                    for (int i = 0; i < 20; i++) {
                        PartikelSystem.PushPartikel(xPos - 30 + random(224), yPos + 170, SMOKEBIG);
                        PartikelSystem.PushPartikel(xPos + random(224), yPos + 220, ROCKSPLITTER);
                        PartikelSystem.PushPartikel(xPos + random(224), yPos + 220, ROCKSPLITTERSMALL);
                    }

                    SoundManager.PlayWave(100, 128, 11025, SOUND_PHARAORAMM);
                    TileEngine.BlockUnten(xPos, yPos, xPos, yPos, GegnerRect[GegnerArt]);
                    AnimCount = 20.0f;
                    Handlung = GEGNER_CRUSHENERHOLEN;
                }
                // Screen Wackeln lassen
                ShakeScreen(3);
            }
        } break;

        case GEGNER_CRUSHENERHOLEN: {
            // ErholungsCounter runterzählen
            AnimCount -= 1.0f SYNC;

            if (AnimCount <= 0.0f) {
                int j = random(3);

                // Schiessen ?
                if (j == 0 && AnimPhase > 1) {
                    Handlung = GEGNER_SCHIESSEN;
                    AnimPhase = 3;
                    AnimCount = 8.0f;
                } else
                    j = random(2) + 1;

                // Steine von der Decke rieseln lassen ?
                if (j == 1) {
                    // Springen
                    ySpeed = -10.0f;
                    yAcc = 2.0f;

                    // böse kucken
                    if (AnimPhase == 0)
                        AnimPhase = 1;
                    else
                        AnimPhase = 3;

                    Handlung = GEGNER_SPECIAL;
                    TileEngine.BlockUnten(xPos, yPos, xPosOld, yPosOld, GegnerRect[GegnerArt]);
                }

                // Wieder zerquetschen
                bool PlayerOn = false;
                for (int p = 0; p < NUMPLAYERS; p++)
                    if (Player[p].AufPlattform == this)
                        PlayerOn = true;

                if (j == 2 || PlayerOn) {
                    Handlung = GEGNER_LAUFEN;

                    if (j == 2)
                        SoundManager.PlayWave(100, 128, 8000, SOUND_STONEFALL);
                }
            }
        } break;

        case GEGNER_SPECIAL:  // Boss hopft auf und ab und lässt Steine von der Decke rieseln
        {
            if (Unten() && ySpeed > 0.0f) {
                // Stein erscheinen lassen
                Gegner.PushGegner(pAim->xpos, float(Value2), FALLINGROCK, 0, 0, false);

                // Partikel unter Boss
                for (int i = 0; i < 20; i++) {
                    PartikelSystem.PushPartikel(xPos - 30 + random(224), yPos + 170, SMOKEBIG);
                    PartikelSystem.PushPartikel(xPos + random(224), yPos + 220, ROCKSPLITTER);
                    PartikelSystem.PushPartikel(xPos + random(224), yPos + 220, ROCKSPLITTERSMALL);
                }

                SoundManager.PlayWave(100, 128, 11025, SOUND_PHARAORAMM);

                ySpeed = 0.0f;
                yAcc = 0.0f;
                AnimCount = 10.0f;
                Handlung = GEGNER_CRUSHENERHOLEN;

                // Normal kucken
                if (AnimPhase == 1)
                    AnimPhase = 0;
                else
                    AnimPhase = 2;
            }
        } break;

        case GEGNER_SCHIESSEN:  // Gegner schiesst aus den Augen
        {
            AnimCount -= 1.0f SYNC;

            if (AnimCount <= 0) {
                // Augen leuchten lassen
                PartikelSystem.PushPartikel(xPos + 68, yPos + 77, LASERFLAMEPHARAO);
                PartikelSystem.PushPartikel(xPos + 68, yPos + 77, LASERFLAMEPHARAO);
                PartikelSystem.PushPartikel(xPos + 112, yPos + 77, LASERFLAMEPHARAO);
                PartikelSystem.PushPartikel(xPos + 112, yPos + 77, LASERFLAMEPHARAO);

                // Schüsse erzeugen
                Projectiles.PushProjectile(xPos + 68, yPos + 77, PHARAOLASER, pAim);
                Projectiles.PushProjectile(xPos + 112, yPos + 77, PHARAOLASER, pAim);

                // Sound ausgeben
                SoundManager.PlayWave(50, 128, 22050, SOUND_PHARAODIE);
                SoundManager.PlayWave(70, 128, 11025, SOUND_LASERSHOT);

                // Je nach SchwierigkeitsGrad die Schuss Frequenz setzen
                switch (Skill) {
                    case SKILL_EASY:     AnimCount = 10.0f; break;
                    case SKILL_MEDIUM:   AnimCount =  8.5f; break;
                    case SKILL_HARD:     AnimCount =  7.0f; break;
                    case SKILL_HURRICAN: AnimCount =  6.0f; break;
                }

                if (random(2) == 0) {
                    AnimPhase = 2;
                    Handlung = GEGNER_CRUSHENERHOLEN;
                }
            }
        } break;

        // Pharao Kopf explodiert
        case GEGNER_EXPLODIEREN: {
            AnimCount -= SpeedFaktor;
            Energy = 100.0f;

            // brodeln lassen
            PartikelSystem.PushPartikel(xPos + random(224), yPos + random(224), ROCKSPLITTER);
            PartikelSystem.PushPartikel(xPos + random(224), yPos + random(224), ROCKSPLITTERSMALL);

            // Fertig explodiert ? Dann wird er ganz zerlegt
            if (AnimCount <= 0.0f)
                Energy = 0.0f;
        } break;

        default:
            break;
    }  // switch
}

// --------------------------------------------------------------------------------------
// PharaoKopf explodiert
// --------------------------------------------------------------------------------------

void GegnerPharaoKopf::GegnerExplode() {
    for (int p = 0; p < NUMPLAYERS; p++) {
        if (Player[p].AufPlattform == this)
            Player[p].AufPlattform = nullptr;

        DirectInput.Joysticks[Player[p].JoystickIndex].ForceFeedbackEffect(FFE_BIGRUMBLE);
    }

    // Splitter
    for (int i = 0; i < 50; i++) {
        PartikelSystem.PushPartikel(xPos + random(224), yPos + random(224), ROCKSPLITTER);
        PartikelSystem.PushPartikel(xPos + random(224), yPos + random(224), ROCKSPLITTERSMALL);
        PartikelSystem.PushPartikel(xPos + random(224), yPos + random(224), SMOKEBIG);
    }

    Player[0].Score += 8000;

    SoundManager.PlayWave(100, 128, 11025, SOUND_EXPLOSION2);
    SoundManager.PlayWave(100, 128, 11025, SOUND_PHARAORAMM);

    ShakeScreen(4);

    HUD.BossHUDActive = false;

    ScrolltoPlayeAfterBoss();
}
