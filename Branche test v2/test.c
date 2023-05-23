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

            if(pEvt->code == KEY_ARROW_UP || pEvt->code == KEY_ARROW_DOWN){
                if (pUserData->arrow == 1){ // mets la fleche sur "lancer une nouvelle partie"
                    pUserData->arrow = 2;
                }
                else{                       // mets la fleche sur "continuer la partie"
                    pUserData->arrow = 1;
                }
            }
    
            else if (pEvt->code == KEY_RETURN){
                if(pUserData->arrow == 1){ // si entrée sur "lance une nouvelle partie"
                    clear();
                    pUserData->page_Actuelle = CHOIX_PLATEAU;
                    pUserData->arrow = 1;
                }
                /*if(pUserData->arrow == 2){ // si entrée sur "lance une nouvelle partie"
                    clear();
                    pUserData->page_Actuelle = PLATEAU;
                    pUserData->arrow = 1;
                }   
                */
            }
        break;

        case CHOIX_PLATEAU : 
            if(pEvt->code == KEY_ARROW_UP){
                if(pUserData->arrow == 1){
                    pUserData->arrow = 5;
                }
                else{
                    pUserData->arrow -= 1; 
                }
            }

            else if(pEvt->code == KEY_ARROW_DOWN){
                if(pUserData->arrow == 5){
                    pUserData->arrow = 1;
                }
                else{
                    pUserData->arrow += 1; 
                }
            }

            else if (pEvt->code == KEY_RETURN){
                if(pUserData->arrow == 5){ // si entrée sur "lance une nouvelle partie"
                    clear();
                    pUserData->page_Actuelle = MENU;
                    pUserData->arrow = 1;
                }
                else{
                    switch(pUserData->arrow){
                        case 1 :
                            pUserData->format = HUITxSIX;
                            pUserData->plateau = malloc(48*sizeof(Case));
                            break;

                        case 2 :
                            pUserData->format = HUITxHUIT;
                            pUserData->plateau = malloc(64*sizeof(Case));
                            break;

                        case 3 :
                            pUserData->format = DIXxSIX;
                            pUserData->plateau = malloc(60*sizeof(Case));
                            break;

                        case 4 :
                            pUserData->format = DIXxHUIT;
                            pUserData->plateau = malloc(80*sizeof(Case));
                            break;
                    }
                    clear();
                    init_plateau(pUserData);
                    pUserData->page_Actuelle = PLATEAU;
                    pUserData->arrow = 1;
                }
            }
        break;
        
        case PLATEAU : 
            switch(pUserData->arrow){

                case HUITxSIX :
                    if(pEvt->code == KEY_ARROW_RIGHT){
                        if(pUserData->arrow == 8){
                            pUserData->arrow = 1;
                        }
                        else{
                            pUserData->arrow += 1; 
                        }
                    }

                    else if(pEvt->code == KEY_ARROW_LEFT){
                        if(pUserData->arrow == 1){
                            pUserData->arrow = 8;
                        }
                        else{
                            pUserData->arrow -= 1; 
                        }
                    }
                break;

                case HUITxHUIT :
                    if(pEvt->code == KEY_ARROW_RIGHT){
                        if(pUserData->arrow == 8){
                            pUserData->arrow = 1;
                        }
                        else{
                            pUserData->arrow += 1; 
                        }
                    }

                    else if(pEvt->code == KEY_ARROW_LEFT){
                        if(pUserData->arrow == 1){
                            pUserData->arrow = 8;
                        }
                        else{
                            pUserData->arrow -= 1; 
                        }
                    }
                break;

                case DIXxSIX :
                    if(pEvt->code == KEY_ARROW_RIGHT){
                        if(pUserData->arrow == 10){
                            pUserData->arrow = 1;
                        }
                        else{
                            pUserData->arrow += 1; 
                        }
                    }

                    else if(pEvt->code == KEY_ARROW_LEFT){
                        if(pUserData->arrow == 1){
                            pUserData->arrow = 10;
                        }
                        else{
                            pUserData->arrow -= 1; 
                        }
                    }
                break;

                case DIXxHUIT :
                    if(pEvt->code == KEY_ARROW_RIGHT){
                        if(pUserData->arrow == 10){
                            pUserData->arrow = 1;
                        }
                        else{
                            pUserData->arrow += 1; 
                        }
                    }

                    else if(pEvt->code == KEY_ARROW_LEFT){
                        if(pUserData->arrow == 1){
                            pUserData->arrow = 10;
                        }
                        else{
                            pUserData->arrow -= 1; 
                        }
                    }
                break;


            }
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
    
    // affichage de la touche de sortie
    drawText(pScreen, 20, 50, "ESC pour sortir du jeu"    , 0);

    switch(pUserData->page_Actuelle){

        case MENU :
            // si menu alors alors afficher les trucs inchangeables du menu
            drawMenu(pUserData, pScreen);
            break;
        case CHOIX_PLATEAU :
            // si menu alors alors afficher les trucs inchangeables du menu
            drawChoix_plateau(pUserData, pScreen);
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

