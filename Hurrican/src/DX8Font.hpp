// Datei : DX8Font.hpp

// --------------------------------------------------------------------------------------
//
// Font Klasse
// zum Anzeigen von Schriften
//
// (c) 2002 Jörg M. Winterstein
//
// --------------------------------------------------------------------------------------

#ifndef _DX8FONT_HPP_
#define _DX8FONT_HPP_

// --------------------------------------------------------------------------------------
// Include Dateien
// --------------------------------------------------------------------------------------

#include "DX8Sprite.hpp"

// --------------------------------------------------------------------------------------
// Klassendeklaration
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// Font Klasse
// --------------------------------------------------------------------------------------

class DirectGraphicsFont {
  private:
    DirectGraphicsSprite *mTexture;  // Textur mit Font-Grafikdaten
    int mXChars;                     // Anzahl Zeichen in X-Richtung

    // DKS - Character widths are now hard-coded arrays we merely point to:
    // char					mCharLength[256];				// Länge der Zeichen in Pixel
    uint8_t *mCharLength;  // Länge der Zeichen in Pixel
    int mXCharSize;        // X-Grösse eines Zeichens
    int mYCharSize;        // Y-Grösse eines Zeichens

    // DKS - Added support for font scaling
    int mScaleFactor;  // Font will be scaled this much (default is 1, no scaling)

  public:
    DirectGraphicsFont();              // Konstruktor (leer)
    ~DirectGraphicsFont();             // Textur freigeben
    bool LoadFont(const char *Filename,    // Laden des Bildes "Filename"
                  int xts,                 // Textur-Grösse x
                  int yts,                 // Textur-Grösse y
                  int xCharsize,           // x-Grösse eines Zeichens
                  int yCharsize,           // y-Grösse eines Zeichens
                  int xChars,              // Anzahl der Zeichen pro Zeile
                  int yChars,              // Anzahl der Zeichen pro Spalte
                  uint8_t *width_values);  // DKS - Array of character widths to use
                                           // (Added this so that we can use compressed
                                           //  font textures and not need to read PNGs
                                           //  directly)
    bool DrawText(float x,
                  float y,  // Text an xPos,yPos mit Farbe
                  const char Text[],
                  D3DCOLOR Color);  // Color ausgeben
    bool DrawValue(float x,
                   float y,  // Zahlenwert an xPos,yPos mit Farbe
                   float Value,
                   D3DCOLOR Color);  // Color ausgeben
    void DrawTextRightAlign(float x,
                            float y,  // Text an xPos,yPos mit Farbe
                            const char Text[],
                            D3DCOLOR Color,
                            int Spacing = 1);  // Color ausgeben right aligned ausgeben
    void DrawTextCenterAlign(float x,
                             float y,  // Text an xPos,yPos mit Farbe
                             const char Text[],
                             D3DCOLOR Color,
                             int Spacing = 1);  // Color ausgeben right aligned ausgeben
    bool DrawDemoChar(float x,
                      float y,  // Text an xPos,yPos mit Farbe
                      const char Text,
                      D3DCOLOR Color);  // Color ausgeben
    bool DrawDemoText(float x,
                      float y,  // Text an xPos,yPos mit Farbe
                      const char Text[],
                      D3DCOLOR Color);  // Color ausgeben
    bool DrawText(float x,
                  float y,  // Text an xPos,yPos mit Farbe
                  const char Text[],
                  D3DCOLOR Color,
                  int Spacing);                        // Color ausgeben
    int DemoStringLength(const char Text[]);           // Länge eines Strings in Pixeln zurückliefern
    int StringLength(const char Text[]);               // Länge eines Strings in Pixeln zurückliefern
    int StringLength(const char Text[], int Spacing);  // Länge eines Strings in Pixeln zurückliefern
    void ShowFPS();                                // FPS Werte anzeigen

    // DKS - New functions added to facilitate scaled fonts, primarily the "default" font on 320x240 devices
    int GetYCharSize() const;
    int GetXCharSize() const;
    int GetScaleFactor() const;
    void SetScaleFactor(int scale_factor);

    uint8_t GetCharLength(int i) const { return mCharLength[i]; }
};

// --------------------------------------------------------------------------------------
// Externals
// --------------------------------------------------------------------------------------

extern DirectGraphicsFont *pMenuFont;
extern DirectGraphicsFont *pDefaultFont;
extern DirectGraphicsFont *pFont;
// DKS - Added these three hard-coded lists of character widths for the fonts:
extern uint8_t demofont_charwidths[256];
extern uint8_t menufont_charwidths[256];
extern uint8_t smallfont_charwidths[256];

#endif
