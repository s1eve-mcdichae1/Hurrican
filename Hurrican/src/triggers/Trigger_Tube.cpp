// --------------------------------------------------------------------------------------
// Die Röhre, aus der (evtl) ein Mutant hopft =)
// Value 1 gibt an, was genau gerendert werden soll
//		0 : Röhre ganz
//		1 : Röhre kaputt
//		2 : Röhre kaputt nur unten
//		3 : Röhre kaputt nur oben
//		4 : Röhre ganz mit Mutant
//		5 : Röhre ganz mit Mutant der rausspringt
// Value 2 gibt an, ob Flüssigkeit drinsteht oder nicht
// --------------------------------------------------------------------------------------

#include "Trigger_Tube.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerTube::GegnerTube(int Wert1, int Wert2, bool Light) {
    Handlung = GEGNER_STEHEN;
    Value1 = Wert1;
    Value2 = Wert2;
    ChangeLight = Light;
    Destroyable = false;
    ForceLight = true;
    Energy = 100;
    BackGround = true;
    OwnDraw = true;
    SinOff = 0.0f;
}

// --------------------------------------------------------------------------------------
// Drawroutine
// --------------------------------------------------------------------------------------

void GegnerTube::DoDraw() {
    if (Value1 == 0 || Value1 == 1 || Value1 == 4 || Value1 == 5)
        PlattformTest(GegnerRect[GegnerArt]);

    // Mutant drin?
    if (Value1 >= 4)
        pGegnerGrafix[MUTANT]->RenderSprite(
            xPos - static_cast<float>(TileEngine.XOffset) - 1,
            yPos - static_cast<float>(TileEngine.YOffset) + 60 + static_cast<float>(sin(SinOff)) * 1.8f, 0, 0xFFFFFFFF);

    // Flüssigkeit drin?
    if (Value2 == 1) {
        DirectGraphics.SetAdditiveMode();

        if (Value1 % 4 == 0 || Value1 == 5) {
            RenderRect4(xPos - static_cast<float>(TileEngine.XOffset) + 18,
                        yPos - static_cast<float>(TileEngine.YOffset) + 75, 62, 3, 0xFF008822, 0xFF008822, 0xFF004411,
                        0xFF004411);

            RenderRect4(xPos - static_cast<float>(TileEngine.XOffset) + 18,
                        yPos - static_cast<float>(TileEngine.YOffset) + 78, 62, 89, 0xFF004411, 0xFF004411, 0xFF00DD22,
                        0xFF00DD22);
        } else {
            RenderRect4(xPos - static_cast<float>(TileEngine.XOffset) + 18,
                        yPos - static_cast<float>(TileEngine.YOffset) + 145, 62, 3, 0xFF008822, 0xFF008822, 0xFF004411,
                        0xFF004411);

            RenderRect4(xPos - static_cast<float>(TileEngine.XOffset) + 18,
                        yPos - static_cast<float>(TileEngine.YOffset) + 148, 62, 19, 0xFF004411, 0xFF004411, 0xFF008822,
                        0xFF008822);
        }

        DirectGraphics.SetColorKeyMode();
    }

    // Und Röhre drüber
    int a;
    a = Value1 % 4;

    if (Value1 == 5)
        a = 0;
    pGegnerGrafix[GegnerArt]->RenderSprite(xPos - static_cast<float>(TileEngine.XOffset),
                                           yPos - static_cast<float>(TileEngine.YOffset), a, 0xFFFFFFFF);
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerTube::DoKI() {
    SinOff += 0.3f SYNC;

    if (SinOff > 2 * PI)
        SinOff = 0.0f;

    // Mutant kann rausspringen?
    if (Value1 == 5) {
        for (int p = 0; p < NUMPLAYERS; p++)
            if (PlayerAbstand() < 100 && Player[p].ypos > yPos) {
                Value1 = 1;

                for (int i = 0; i < 100; i++)
                    PartikelSystem.PushPartikel(xPos + 5 + random(55), yPos + 70 + random(60), GLASSPLITTER);

                Gegner.PushGegner(xPos - 1, yPos + 60, MUTANT, 1, 0, false);
                Gegner.PushGegner(xPos + 50 - 10, yPos + 150, PARTIKELSPAWN, 11, 80, false);

                SoundManager.PlayWave(100, 128, 10000 + random(2000), SOUND_GLASSBREAK);
            }
    }
}

// --------------------------------------------------------------------------------------
// LuefterGross explodiert
// --------------------------------------------------------------------------------------

void GegnerTube::GegnerExplode() {}
