#ifndef _BOSS_FAHRSTUHLBOSS_HPP_
#define _BOSS_FAHRSTUHLBOSS_HPP_

#include "GegnerClass.hpp"
#include "enemies/Gegner_Stuff.hpp"

class GegnerFahrstuhlBoss : public GegnerClass {
  private:
    DirectGraphicsSprite Head;   // Grafiken für den "Kopf" des Gegners
    DirectGraphicsSprite Canon;  // Grafiken für die zwei Kanonen
    float ShotDelay;             // Schussverzögerung
    float TempY;                 // yPosition relativ zum Fahrstuhl
    float x1, y1, x2, y2, x3;    // Koordinaten für Kopf und Kanonen
    float dx1, dx2;              // Speed für Kopf und Kanonen
    float CanonAnim;             // AnimationsPhase der Kanone

    bool Activated;  // Boss kommt schon rein
    int Shots;       // Wieviele Schüsse abgeben

  public:
    GegnerFahrstuhlBoss(int Wert1,
                        int Wert2,  // Konstruktor
                        bool Light);
    void GegnerExplode();  // Gegner explodiert
    void DoKI();           // Gegner individuell mit seiner eigenen kleinen KI bewegen
    void DoDraw();         // Gegner individuell rendern
};

#endif
