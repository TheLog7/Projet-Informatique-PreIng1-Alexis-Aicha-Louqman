#ifndef LIBGAMERGR_H
    #define LIBGAMERGR_H


    //------------------------------
    // INCLUDES
    //------------------------------
    #include <stdio.h>
    #include <stdlib.h>
    #include <locale.h>
    #include <curses.h>
    #include <string.h>
    #include "keys.h"

    //------------------------------
    // CONSTANTS
    //------------------------------

    
    //------------------------------
    // ENUMS, STRUCTURES AND CALLBACKS
    //------------------------------
    // Key code
    typedef long KeyCode;

    typedef enum {
        IMMO,
        JOUEUR1,
        JOUEUR2,
        JOUEUR3,
        VIDE,
    } Pion;

    typedef enum {
        MENU,
        CHOIX_PLATEAU,
        PLATEAU,
        WIN,
    } Page;

    typedef struct {
        int abscisse;
        int ordonne;
        Pion type;
    } Case;

    typedef enum {
        NONE,
        DIXxSIX,
        DIXxHUIT,
        HUITxSIX,
        HUITxHUIT,
    } Format;

    typedef struct{
        Case* plateau;
        int nb_Pions;
        int nb_Tours;
        int nb_Joueurs;
        Page page_Actuelle;
        char* toPrint;
        Format format;
        int arrow;
    } UserData;

    // Event structure
    typedef struct {
        KeyCode  code;
    } Event;            
    // Canvas Structure
    typedef struct {
        int width;      // number of characters along X-axis
        int height;     // number of characters along Y-axis
    } Screen;       
    // User Callback Functions
    typedef void (*InitCallback  )(UserData* pUSerData, Screen* pScreen);
    typedef void (*EventCallback )(UserData* pUserData, Screen* pScreen, Event* pEvt);
    typedef int  (*UpdateCallback)(UserData* pUserData, Screen* pScreen, unsigned long deltaTime);
    typedef void (*DrawCallback  )(UserData* pUSerData, Screen* pScreen);
    typedef void (*FinishCallback)(UserData* pUserData);
    // Callback structure
    typedef struct{
        InitCallback   cbInit;        
        EventCallback  cbEvent;        
        UpdateCallback cbUpdate;
        DrawCallback   cbDraw;
        FinishCallback cbFinish;        
    } Callbacks;    
    // Full game structure
    typedef struct{
        Screen*    pScreen;
        void*      pUserData;
        Callbacks* pUserCallbacks; 
        int        displayFPS;       
    } GameData;
    
    //------------------------------
    // ERRORS
    //------------------------------           
    void rageQuit(const char* fname,
              const int   lineno,
              const char* fxname,
              const int   errorCode,
              const char* format, ...);
    #define RAGE_QUIT(errCode, format, ...) rageQuit(__FILE__, __LINE__, __func__, errCode, format, ##__VA_ARGS__)
    
    //------------------------------
    // DEBUG
    //------------------------------           
    void debug(const char* format, ...);

    //------------------------------
    // GAME FUNCTIONS
    //------------------------------           
    GameData* createGame(int nbCharX, int nbCharY, void* pUserData, Callbacks* pCb, int fps);
    void gameLoop(GameData* pGame);

    //------------------------------
    // DRAW
    //------------------------------    
    void setColor    (unsigned char id, unsigned char r, unsigned char g, unsigned char b);
    void setColorPair(unsigned char id, unsigned char txtId, unsigned char backId);
    void drawText    (Screen* pScr, int x, int y, char* pText   , int clrId);
    void drawLine    (Screen* pScr, int x, int y, int w, char ch, int clrId);
    void drawCol     (Screen* pScr, int x, int y, int w, char ch, int clrId);
    void drawMenu    (UserData* pUserData, Screen* pScreen);
    void drawChoix_plateau(UserData* pUserData, Screen* pScreen);
    void drawPlateau (UserData* pUserData, Screen* pScreen);
    void init_plateau(UserData* pUserData);

#endif

