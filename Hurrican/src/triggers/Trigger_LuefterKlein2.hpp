#ifndef _TRIGGER_LUEFTERKLEIN2_HPP_
#define _TRIGGER_LUEFTERKLEIN2_HPP_

#include "GegnerClass.hpp"
#include "enemies/Gegner_Stuff.hpp"

class GegnerLuefterKlein2 : public GegnerClass {
  public:
    GegnerLuefterKlein2(int Wert1,
                        int Wert2,  // Konstruktor
                        bool Light);
    void GegnerExplode();  // Gegner explodiert
    void DoKI();           // Gegner individuell mit seiner
    // eigenen kleinen KI bewegen
    void DoDraw();
};

#endif
