// --------------------------------------------------------------------------------------
// Der Lava Ball Spawner
//
// ERZEUGT Lava Bälle, die aus der Lava hopsen
// --------------------------------------------------------------------------------------

#include "Gegner_LavaBallSpawner.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerLavaBallSpawner::GegnerLavaBallSpawner(int Wert1, int Wert2, bool Light) {
    Active = true;
    Handlung = GEGNER_LAUFEN;
    Energy = 50;
    Value1 = Wert1;
    Value2 = Wert2;
    ChangeLight = Light;
    Destroyable = false;
    Delay = float(Wert2);
    DontMove = true;
    OwnDraw = true;
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerLavaBallSpawner::DoKI() {
    // Warten bis ein neuer geschossen wird
    //
    int pa = PlayerAbstand();
    if (pa < 2000) {
        Delay -= 1.0f SYNC;
    }

    if (Delay <= 0.0f) {
        Delay = float(Value2 + random(20));
        Gegner.PushGegner(xPos, yPos, LAVABALL, Value1, Value2, ChangeLight);
    }
}

// --------------------------------------------------------------------------------------
// LavaBall explodiert
// --------------------------------------------------------------------------------------

void GegnerLavaBallSpawner::GegnerExplode() {}
