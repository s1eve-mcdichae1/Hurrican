#ifndef _BOSS_WUXESPINNEN_HPP_
#define _BOSS_WUXESPINNEN_HPP_

#include "GegnerClass.hpp"
#include "enemies/Gegner_Stuff.hpp"

class GegnerWuxeSpinnen : public GegnerClass
{
private:
    float	ActionCounter;

public:

    GegnerWuxeSpinnen (int Wert1,	int Wert2,			// Konstruktor
                       bool		  Light);
    void GegnerExplode (void);							// Gegner explodiert
    void DoKI		   (void);							// Gegner individuell mit seiner 														// eigenen kleinen KI bewegen
};

#endif
