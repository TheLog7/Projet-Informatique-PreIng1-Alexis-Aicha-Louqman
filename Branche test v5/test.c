// Include the lib header file
#include "inc/libGameRGR2.h"


//--------------------------------------------------------------
// Userdata for the game
// This part is in another file (.h) lib.h
// Fill your own data here like :
// - character position
// - character life points or stats
// - 2d board game matrix
// - score value
// - main file directory 
// - ...    
//--------------------------------------------------------------


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
    pUserData->arrow = 1;
    pUserData->format = NONE;
    pUserData->player_state = TOUR_JOUEUR1;
}


//-------------------------------------------------------------
// EVENT 
//-------------------------------------------------------------
// This function is called once every time you press a key or
// several times when you keep a key pressed a long time.
//
// - pUserData : your game data pointer (to be casted)-m64
// - pScreen   : pointer containing the number of characters of the screen (width/height)
// - pEvt      : pointer to the event structure : it contains only one field (pEvt->code)
//               the code value can be compared to the 'KeyRGR' enum values from "keys.h"
//-------------------------------------------------------------
void event(UserData* pUserData, Screen* pScreen, Event* pEvt){

    switch(pUserData->page_Actuelle){

        case MENU :
            arrowSelectMenu(pUserData, pScreen, pEvt);
            break;

        case CHOIX_PLATEAU : 
            arrowSelectChoixPlateau(pUserData, pScreen, pEvt);
            break;

        case CHOIX_NB_JOUEURS :
            arrowSelectChoixNbJoueur(pUserData, pScreen, pEvt);
            break;
        
        case PLATEAU :
            arrowSelectPlateau(pUserData, pScreen, pEvt); // placeSpawn à l interieur
            break;
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
    //equality(pUserData, pScreen);
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

    // dessin du cadre
    drawLine(pScreen, 13, 4, 188, '_' ,0);
    drawCol(pScreen, 13, 5, 59, '|' ,0);
    drawCol(pScreen, 200, 5, 59, '|' ,0);
    drawLine(pScreen, 13, 64, 188, '_' ,0);
    
    // affichage de la touche de sortie
    drawText(pScreen, 20, 50, "ESC pour sortir du jeu", 0);

    switch(pUserData->page_Actuelle){

        case MENU :
            drawMenu(pUserData, pScreen);
            break;
        case CHOIX_PLATEAU :
            drawChoix_plateau(pUserData, pScreen);
            break;
        case CHOIX_NB_JOUEURS :
            drawChoixNbJoueurs(pUserData, pScreen);
            break;
        case PLATEAU :
            drawPlateau(pUserData, pScreen);
            break;
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

