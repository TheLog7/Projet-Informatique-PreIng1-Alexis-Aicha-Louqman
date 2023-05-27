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
    #include <time.h>
    #include "keys.h"

    //------------------------------
    // CONSTANTS
    //------------------------------
    
    #define TOUR_JOUEUR1 0
    #define TOUR_JOUEUR2 1
    #define TOUR_JOUEUR3 2

    #define ON 1
    #define OFF 0

    #define ANTIHORAIRE 1
    #define HORAIRE 0

    #define IMMO 0
    #define JOUEUR1 1
    #define JOUEUR2 2
    #define JOUEUR3 3
    #define VIDE 4
    
    //------------------------------
    // ENUMS, STRUCTURES AND CALLBACKS
    //------------------------------
    // Key code
    typedef long KeyCode;

    typedef enum {
        NONE,
        DIXxSIX,
        DIXxHUIT,
        HUITxSIX,
        HUITxHUIT,
    } Format;

    typedef enum {
        MENU,
        CHOIX_PLATEAU,
        CHOIX_NB_JOUEURS,
        PLATEAU,
        EQUAL,
        WIN,
        CHOIX_SENS,
    } Page;

    typedef struct {
        char abscisse;
        char ordonne;
        char type;
    } Case;

    //--------------------------------------------------------------
// Userdata for the game
// This part SHOULD BE in another file (.h)
// Fill your own data here like :
// - character position
// - character life points or stats
// - 2d board game matrix
// - score value
// - main file directory 
// - ...    
//--------------------------------------------------------------

    typedef struct{
        Case* plateau;
        short nb_Pions;
        short nb_Tours;
        Page page_Actuelle;
        Format format;
        short arrow;
        short allPions;
        short player_state;
        short xPivot;
        short yPivot;
        short nb_Joueurs;
        short nb_Pivots;
        short pivot;
        short sensPivot;
        char pivotVide;
        short win;
        FILE* sauvegarde;
    } UserData;

    // Event structure
    typedef struct {
        KeyCode  code;
    } Event;  

    // Canvas Structure
    typedef struct {
        short width;      // number of characters along X-axis
        short height;     // number of characters along Y-axis
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
    void rageQuit(const char* fName,
              const int   lineno,
              const char* fxName,
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
    void drawChoixNbJoueurs(UserData* pUserData, Screen* pScreen);
    void drawPlateau (UserData* pUserData, Screen* pScreen);
    void drawChoixSensPivot(UserData* pUserData, Screen* pScreen);

void init_plateau(UserData* pUserData);

    void drawSquare  (Screen* pScreen, int x, int y, char type);
    void drawPivot(UserData* pUserData, Screen* pScreen);

    void arrow_select_Menu(UserData* pUserData, Event* pEvt);
    void arrow_select_ChoixPlateau (UserData* pUserData, Event* pEvt);
    void arrow_select_ChoixNbJoueur(UserData* pUserData, Event* pEvt);
    void arrow_select_Plateau (UserData* pUserData, Event* pEvt);
    void pivot_select_Plateau(UserData* pUserData, Event* pEvt, Screen* pScreen);
    void arrow_select_ChoixSensPivot(UserData* pUserData, Event* pEvt);

    int testHaut(UserData* pUserData, Screen* pScreen, int x, int y, int type, int nb);
    int testBas(UserData* pUserData, Screen* pScreen, int x, int y, int type, int nb);
    int testDroite(UserData* pUserData, Screen* pScreen, int x, int y, int type, int nb);
    int testGauche(UserData* pUserData, Screen* pScreen, int x, int y, int type, int nb);
    int testHautGauche(UserData* pUserData, Screen* pScreen, int x, int y, int type, int nb);
    int testBasGauche(UserData* pUserData, Screen* pScreen, int x, int y, int type, int nb);
    int testHautDroite(UserData* pUserData, Screen* pScreen, int x, int y, int type, int nb);
    int testBasDroite(UserData* pUserData, Screen* pScreen, int x, int y, int type, int nb);

    void placePawn(UserData* pUserData, char joueur);
    void drawPivotOrArrow(UserData* pUserData, Screen* pScreen);
    void tourAntiHoraire(UserData* pUserData);
    void pivot(UserData* pUserData, Screen* pScreen);
    void gravity(UserData* pUserData);
    //void equality(UserData* pUserData, Screen* Screen);
    void winCond(UserData* pUserData, Screen* pScreen);
    void restaureSave(UserData* pUserData);

#endif

