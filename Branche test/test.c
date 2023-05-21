// Include the lib header file
#include "inc/libGameRGR2.h"


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
/*typedef enum {
    IMMO,
    JOUEUR1,
    JOUEUR2,
    JOUEUR3,
    VIDE,
} Pion;

typedef enum {
    MENU,
    PLATEAU,
} Page;

typedef struct {
    int abscisse;
    int ordonne;
    Pion type;
} Case;

typedef struct{
    Case* plateau;
    int nb_Pions;
    int nb_Tours;
    int nb_Joueurs;
    Page page_Actuelle;
} UserData;*/


//-------------------------------------------------------------
// INIT 
//-------------------------------------------------------------
// this function will be called by the game engine
// only once at program start
//
// - pUserData : your game data pointer (to be casted)
// - pScreen   : pointer containing the number of characters of the screen (width/height)
//-------------------------------------------------------------
void init(UserData* pUserData, Screen* pScreen){
    Case* plateau = NULL;
    pUserData->plateau = plateau;
    pUserData->nb_Pions = 4;
    pUserData->nb_Joueurs = 0;
    pUserData->nb_Tours = 0;
    pUserData->page_Actuelle = MENU;
    pUserData->toPrint = "";
}


//-------------------------------------------------------------
// EVENT 
//-------------------------------------------------------------
// This function is called once every time you press a key or
// several times when you keep a key pressed a long time.
//
// - pUserData : your game data pointer (to be casted)
// - pScreen   : pointer containing the number of characters of the screen (width/height)
// - pEvt      : pointer to the event structure : it contains only one field (pEvt->code)
//               the code value can be compared to the 'KeyRGR' enum values from "keys.h"
//-------------------------------------------------------------
void event(UserData* pUserData, Screen* pScreen, Event* pEvt){
    if(pEvt->code == KEY_ARROW_UP || pEvt->code == KEY_ARROW_DOWN){
        if (strcmp(pUserData->toPrint,"")){
            pUserData->toPrint = "";
        }
        else{
            pUserData->toPrint = "EZ";
        }
    }
    
    else if (pEvt->code == KEY_RETURN){
        clear();
        pUserData->page_Actuelle = PLATEAU;
    }
}


//-------------------------------------------------------------
// UPDATE 
//-------------------------------------------------------------
// This function is called every frame (about 60 times per second)
// and is used to update your data model like :
// - increase time passing by
// - make a char blink every second
// - ...
// - anything that needs to be updated while the user does not do anything
// This function returns an integer value : 0 means this is ok and the game will continue
// any other values will make the program stop the game loop and exit.
//
// - pUserData : your game data pointer (to be casted)
// - pScreen   : pointer containing the number of characters of the screen (width/height)
// - deltaTime : number of microseconds elapsed since the previous 'update' call.
//-------------------------------------------------------------
int  update(UserData* pUserData, Screen* pScreen, unsigned long deltaTime){
    return 0;
     
}


//-------------------------------------------------------------
// DRAW 
//-------------------------------------------------------------
// This function is called every frame (about 60 times per second)
// and contains all the commands to print characters on screen
// You can use the drawText function that is provided (see examples below)
//
// - pUserData : your game data pointer (to be casted)
// - pScreen   : pointer containing the number of characters of the screen (width/height)
//------------------------------------------------------------

void draw(UserData* pUserData, Screen* pScreen){
    
    // Vérifie que l'écran est à la bonne échelle
    if (is_term_resized(1920,1080)){
        resize_term(1920,1080);
    }

    // dessin du cadre
    drawLine(pScreen, 13, 4, 188, '-' ,0);
    drawCol(pScreen, 13, 5, 59, '|' ,0);
    drawCol(pScreen, 200, 5, 59, '|' ,0);
    drawLine(pScreen, 13, 64, 188, '-' ,0);

    // si menu alors alors afficher les trucs inchangeables du menu
    if(pUserData->page_Actuelle == MENU){
        drawMenu(pUserData, pScreen);
    }
}



//-------------------------------------------------------------
// FINISH
//-------------------------------------------------------------
// This function is called only once at the end of the program
// It is mainly used to free all allocated resources
// When this function is finished the program will exit
// 
// - pUserData : your game data pointer (to be casted)
//-------------------------------------------------------------
void finish(UserData* pUserData){
    // TODO

}



//-------------------------------------------------------------
// MAIN 
//-------------------------------------------------------------
int main(int argc, char** argv){

    // Declaration of a GameData pointer
    GameData* pGame = NULL;
    // Declare your UserData structure
    UserData  data;    
    // Declare the callbacks structure
    Callbacks cb;

    // Fill the callback structure according to the previous functions
    cb.cbInit   = init;
    cb.cbEvent  = event;
    cb.cbUpdate = update;
    cb.cbDraw   = draw;
    cb.cbFinish = finish;

    // Create the game structure by calling the createGame function
    // This function returns a GameData pointer
    // - nb characters on X axis
    // - nb characters on Y axis
    // - pointer to the USerData structure
    // - pointer to the callback structure
    // - integer value either 0 or 1 to show FPS on screen
    pGame = createGame(220,65, &data, &cb, 1);
        
    // Launch the game by calling the gameLoop function (giving the pGame parameter)
    start_color();
    gameLoop(pGame);
    
    // end of program
    return 0; 
}

