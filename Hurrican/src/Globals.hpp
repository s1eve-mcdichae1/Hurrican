// Datei : Globals.hpp

// --------------------------------------------------------------------------------------
//
// Header Datei mit global gültigen Makros, Defines usw
//
// (c) 2002 Jörg M. Winterstein
//
// --------------------------------------------------------------------------------------

#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_

// --------------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------------

#include "Mathematics.hpp"
#include "SDL_port.hpp"
#include "Texts.hpp"

// --------------------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------------------

//----- Spielzustände

enum class GameStateEnum {
  CRACKTRO,
  VORSPANN,
  INTRO,
  MAINMENU,
  GAMELOOP,
  OUTTRO
};

// Tasten Abfragen (simple Methode)
#define SimpleKeydown(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// --------------------------------------------------------------------------------------
// Definitionen für den Spielablauf
// --------------------------------------------------------------------------------------

constexpr int MAX_ENERGY = 100;             // Maximalwert für die Spielerenergie
constexpr int MAX_ARMOUR = 200;             // Maximalwert für das Rad
constexpr int DIAMONDS_FOR_EXTRALIFE = 50;  // Diamanten die man für 200 Zeiteinheiten

#define RARFILENAME "Hurrican.dat"
#define RARFILEPASSWORD "+/#(%,,/"

#if defined(__AROS__)
#  define TEMP_FILE_PREFIX "T:"
#else
#  define TEMP_FILE_PREFIX
#endif

// --------------------------------------------------------------------------------------
// Externals
// --------------------------------------------------------------------------------------

extern bool Sprache;      // true == deutsch / false == englisch (nur für Menugrafiken)
extern bool GameRunning;  // Spiel läuft :-)
#ifndef NDEBUG
extern bool DebugMode;  // Debug Mode ein/aus
#endif
extern bool FlameThrower;
extern bool HasCheated;
extern RECT_struct srcrect, destrect;

#endif
