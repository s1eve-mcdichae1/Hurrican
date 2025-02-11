#ifndef _BOSS_RIESENSPINNE_HPP_
#define _BOSS_RIESENSPINNE_HPP_

#include "GegnerClass.hpp"
#include "enemies/Gegner_Stuff.hpp"

class GegnerRiesenSpinne : public GegnerClass {
  private:
    DirectGraphicsSprite Head;     // Grafiken des Kopfs
    DirectGraphicsSprite Legs[3];  // Grafiken der Beine
    float LegsAnim[8];             // Animationsphasen der 8 Beine
    float HeadWinkel;              // Winkel des Kopfes
    float HeadXOffset;             // X-Offset des Kopfse
    float WalkDir;                 // Laufrichtung
    float WalkCount;               // Zähler fürs Laufen
    float DamageWackel;            // Spinne wurde verletzt
    float LastEnergy;
    float ShotDelay;
    float yBody;
    int ShotCount;
    int ShotMode;

  public:
    GegnerRiesenSpinne(int Wert1,
                       int Wert2,  // Konstruktor
                       bool Light);

    void StopCurrentAction();
    void GegnerExplode();  // Gegner explodiert
    void DoKI();           // Gegner individuell mit seiner eigenen kleinen KI bewegen
    void DoDraw();         // Gegner individuell rendern
    void DrawLeg(float x, float y, float winkel, int anim, int off, D3DCOLOR col);
    void RandomHandlung();
};

#endif
