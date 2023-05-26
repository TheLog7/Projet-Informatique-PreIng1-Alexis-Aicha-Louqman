#include <string.h>
#include <sys/time.h>

#include "libGameRGR2.h"
// Code couleur : 1 = Gris
//                2 = Rouge
//                3 = Bleu
//                4 = Jaune texte
//                5 = Jaune case
//                6 = Rouge texte
//                7 = Blanc

unsigned long getTimeMicros(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (tv.tv_sec*1000000) + (tv.tv_usec/1);
} 

void checkGame(GameData* pGame, int errCode){
    // Locals
    Screen*    pScr = NULL;
    void*      pDat = NULL;
    Callbacks* pCb  = NULL;
    // Check params
    if(pGame == NULL){
        RAGE_QUIT(errCode, "Game data pointer NULL !\n");
    }
    // Store pointers locally
    pScr = pGame->pScreen;
    pDat = pGame->pUserData;
    pCb  = pGame->pUserCallbacks;
    // Check internal pointers
    if(pScr == NULL){
        RAGE_QUIT(errCode+1, "Game screen pointer NULL !\n");
    }
    if(pDat == NULL){
        RAGE_QUIT(errCode+2, "Game user data pointer NULL !\n");
    }
    if(pCb == NULL){
        RAGE_QUIT(errCode+3, "Game user callback pointer NULL !\n");
    }    
}

int checkEvent(GameData* pGame){
    // Locals
    Screen*    pScr = NULL;
    void*      pDat = NULL;
    Callbacks* pCb  = NULL;
    Event      evt;
    int        ch = ERR+1;
    long       value = KEY_NONE;
    int        size  = 0;
    int        flag  = 0;
    // Check params
    checkGame(pGame, 3000);
    // Store pointers locally
    pScr = pGame->pScreen;
    pDat = pGame->pUserData;
    pCb  = pGame->pUserCallbacks;
    
    // Get all key values from the stdin
    // Some keys generate several bytes
    while( (ch=getch()) != ERR ){
        value = (value<<8) | ch;
        flag  = 0;
        size++;
        // ASCII flag
        flag |= (((size==1) && (value!=EXT_ESC)) || ((size==2) && (value!=EXT_CSI))) && (value>=0) && (value<=127);        
        // SPECIAL1 flag2
        flag |= (size==2) && ((value & 0xFF00      ) == EXT_SPEC1);        
        flag |= (size==2) && ((value & 0xFF00      ) == EXT_SPEC2);        
        // CSI flag
        flag |= (size==3) && ((value & 0xFFFF00    ) == EXT_CSI) && (value != HDR_FN1) && (value != HDR_FN2); 
        // SS3 flag
        flag |= (size==3) && ((value & 0xFFFF00    ) == EXT_SS3); 
        // FUNCTION flags
        flag |= (size==5) && ((value & 0xFFFFFF00FF) == EXT_FN1); 
        flag |= (size==5) && ((value & 0xFFFFFF00FF) == EXT_FN2); 
        flag |= (size==2) && ((value & 0xFF00      ) == EXT_FN3); 
        
        
        debug("|%3d| => [%0p]\n", ch, value); 
        if( flag ){
            debug("===>>>\n");
        } 
        
        
        // Send event to user code
        if( flag ){
            evt.code = value;
            pCb->cbEvent(pDat, pScr, &evt);            
            //value    = KEY_NONE;
            //size     = 0;
        }
    }
    
    // ESCAPE flag : exit request
    if( (size == 1) && (value == EXT_ESC) ){
        return 1;
    }
    else{
        return 0;
    }
}


//-----------------------------------------
// DRAW
//-----------------------------------------
void setColor(unsigned char id, unsigned char r, unsigned char g, unsigned char b){
    int rr = (1000*r)/255;
    int gg = (1000*g)/255;
    int bb = (1000*b)/255;
    init_color(id, rr, gg, bb);
}

void setColorPair(unsigned char id, unsigned char txtId, unsigned char backId){
    init_pair(id, txtId, backId);
}

void drawText(Screen* pScr, int x, int y, char* pText, int clrId){
    // Local buffer to copy string
    char pWrite[256] = {0};
    if(pText == NULL){
        RAGE_QUIT(50, "Text pointer NULL !\n");
    }
    if(pScr == NULL){
        RAGE_QUIT(51, "Screen pointer NULL !\n");
    }
    // TODO issue when trying to print the trailing chars of a string started on the left edge of the screen !!
    // TODO issue when printing an emoji : when reaching the right border of the screen
    if(x>=0 && y>=0 && x<pScr->width && y<pScr->height){
        move(y, x);
        attron(COLOR_PAIR(clrId));
        // limit the size of the string in order to avoid printing outside the edges
        int sz  = pScr->width - x;
        int len = strlen(pText);
        if(sz > len+1){
            sz = len;
        }
        if(len > 255){
            RAGE_QUIT(52, "pText string exceeds 255 chars !\n");            
        } 
        strcpy(pWrite, pText);
        pWrite[sz] = '\0';        
        printw("%s", pWrite);
        attroff(COLOR_PAIR(clrId));
    }
}

void drawLine(Screen* pScr, int x, int y, int w, char ch, int clrId){
    if(pScr == NULL){
        RAGE_QUIT(60, "Screen pointer NULL !\n");
    }
    move(y, x);
    attron(COLOR_PAIR(clrId));
    for(int dx=0; dx<w && (x+dx)<pScr->width; dx++){
        if(x+dx >= 0){
            mvaddch(y, x+dx, ch);    
        }
    }
    attroff(COLOR_PAIR(clrId));
}

void drawCol(Screen* pScr, int x, int y, int w, char ch, int clrId){
    if(pScr == NULL){
        RAGE_QUIT(60, "Screen pointer NULL !\n");
    }
    move(y, x);
    attron(COLOR_PAIR(clrId));
    for(int dy=0; dy<w && (y+dy)<pScr->height; dy++){
        if(y+dy >= 0){
            mvaddch(y+dy, x, ch);
        }
    }
    attroff(COLOR_PAIR(clrId));
}

void drawMenu(UserData* pUserData, Screen* pScreen){
    setColor(4,255,255,0);
    setColorPair(4,4,0);
    if(pUserData->arrow == 1){
        drawText(pScreen, 77, 25, "->", 0);
        drawText(pScreen, 77, 27, "  ", 0);
    }
    else {
        drawText(pScreen, 77, 25, "  ", 0);
        drawText(pScreen, 77, 27, "->", 0);
    }
    
    drawText(pScreen, 55, 9,    " _____                    _____     _____     _____               _____    _____            _____ "  , 4);
    drawText(pScreen, 55, 10,   "|     |   |      |   |   |         |         |     |   |\\    |   |        |                |     "  , 4);
    drawText(pScreen, 55, 11,   "|     |   |      |   |   |_____    |_____    |     |   | \\   |   |        |                |_____"  , 4);
    drawText(pScreen, 55, 12,   "|_____|   |      |   |         |         |   |_____|   |  \\  |   |        |_____                |"  , 4);
    drawText(pScreen, 55, 13,   "|         |      |   |         |         |   |     |   |   \\ |   |        |                     |"  , 4);
    drawText(pScreen, 55, 14,   "|         |______|   |    _____|    _____|   |     |   |    \\|   |_____   |_____           _____|"  , 4);
    drawText(pScreen, 80, 25, "Lancer une nouvelle partie", 0);
    drawText(pScreen, 80, 27, "Continuer votre partie"    , 0);
}

void drawChoixNbJoueurs(UserData* pUserData, Screen* pScreen){
    setColor(4,255,255,0);
    setColorPair(4,4,0);
    if(pUserData->arrow == 1){
        drawText(pScreen, 77, 25, "->", 0);
        drawText(pScreen, 77, 27, "  ", 0);
    }
    else {
        drawText(pScreen, 77, 25, "  ", 0);
        drawText(pScreen, 77, 27, "->", 0);
    }
    
    drawText(pScreen, 55, 9,    " _____                    _____     _____     _____               _____    _____            _____ "  , 4);
    drawText(pScreen, 55, 10,   "|     |   |      |   |   |         |         |     |   |\\    |   |        |                |     "  , 4);
    drawText(pScreen, 55, 11,   "|     |   |      |   |   |_____    |_____    |     |   | \\   |   |        |                |_____"  , 4);
    drawText(pScreen, 55, 12,   "|_____|   |      |   |         |         |   |_____|   |  \\  |   |        |_____                |"  , 4);
    drawText(pScreen, 55, 13,   "|         |      |   |         |         |   |     |   |   \\ |   |        |                     |"  , 4);
    drawText(pScreen, 55, 14,   "|         |______|   |    _____|    _____|   |     |   |    \\|   |_____   |_____           _____|"  , 4);
    drawText(pScreen, 80, 25, "2 joueurs",      0);
    drawText(pScreen, 80, 27, "3 joueurs"    , 0);
}

void drawChoixSensPivot(UserData* pUserData, Screen* pScreen){
    setColor(4,255,255,0);
    setColorPair(4,4,0);
    if(pUserData->arrow == 1){
        drawText(pScreen, 77, 25, "->", 0);
        drawText(pScreen, 77, 27, "  ", 0);
    }
    else {
        drawText(pScreen, 77, 25, "  ", 0);
        drawText(pScreen, 77, 27, "->", 0);
    }

    drawText(pScreen, 55, 9,    " _____                    _____     _____     _____               _____    _____            _____ "  , 4);
    drawText(pScreen, 55, 10,   "|     |   |      |   |   |         |         |     |   |\\    |   |        |                |     "  , 4);
    drawText(pScreen, 55, 11,   "|     |   |      |   |   |_____    |_____    |     |   | \\   |   |        |                |_____"  , 4);
    drawText(pScreen, 55, 12,   "|_____|   |      |   |         |         |   |_____|   |  \\  |   |        |_____                |"  , 4);
    drawText(pScreen, 55, 13,   "|         |      |   |         |         |   |     |   |   \\ |   |        |                     |"  , 4);
    drawText(pScreen, 55, 14,   "|         |______|   |    _____|    _____|   |     |   |    \\|   |_____   |_____           _____|"  , 4);
    drawText(pScreen, 80, 25, "Rotation du pivot horaire",      0);
    drawText(pScreen, 80, 27, "Rotation du pivot antihoraire"    , 0);
}

void drawChoix_plateau(UserData* pUserData, Screen* pScreen){
    setColor(4,255,255,0);
    setColorPair(4,4,0);

    drawText(pScreen, 55, 9,    " _____                    _____     _____     _____               _____    _____            _____ "  , 4);
    drawText(pScreen, 55, 10,   "|     |   |      |   |   |         |         |     |   |\\    |   |        |                |     "  , 4);
    drawText(pScreen, 55, 11,   "|     |   |      |   |   |_____    |_____    |     |   | \\   |   |        |                |_____"  , 4);
    drawText(pScreen, 55, 12,   "|_____|   |      |   |         |         |   |_____|   |  \\  |   |        |_____                |"  , 4);
    drawText(pScreen, 55, 13,   "|         |      |   |         |         |   |     |   |   \\ |   |        |                     |"  , 4);
    drawText(pScreen, 55, 14,   "|         |______|   |    _____|    _____|   |     |   |    \\|   |_____   |_____           _____|"  , 4);

    drawText(pScreen, 80, 25, "Choix du format ( largeur par hauteur )", 0);
    drawText(pScreen, 83, 27, "8x6", 0);
    drawText(pScreen, 83, 28, "8x8", 0);
    drawText(pScreen, 83, 29, "10x6", 0);
    drawText(pScreen, 83, 30, "10x8", 0);
    drawText(pScreen, 83, 31, "retour au menu", 0);

    switch(pUserData->arrow){
        case 1:
            drawText(pScreen, 80, 27, "->", 0);
            drawText(pScreen, 80, 28, "  ", 0);
            drawText(pScreen, 80, 29, "  ", 0);
            drawText(pScreen, 80, 30, "  ", 0);
            drawText(pScreen, 80, 31, "  ", 0);
            break;
        case 2:
            drawText(pScreen, 80, 27, "  ", 0);
            drawText(pScreen, 80, 28, "->", 0);
            drawText(pScreen, 80, 29, "  ", 0);
            drawText(pScreen, 80, 30, "  ", 0);
            drawText(pScreen, 80, 31, "  ", 0);
            break;
        case 3:
            drawText(pScreen, 80, 27, "  ", 0);
            drawText(pScreen, 80, 28, "  ", 0);
            drawText(pScreen, 80, 29, "->", 0);
            drawText(pScreen, 80, 30, "  ", 0);
            drawText(pScreen, 80, 31, "  ", 0);
            break;
        case 4:
            drawText(pScreen, 80, 27, "  ", 0);
            drawText(pScreen, 80, 28, "  ", 0);
            drawText(pScreen, 80, 29, "  ", 0);
            drawText(pScreen, 80, 30, "->", 0);
            drawText(pScreen, 80, 31, "  ", 0);
            break;
        case 5:
            drawText(pScreen, 80, 27, "  ", 0);
            drawText(pScreen, 80, 28, "  ", 0);
            drawText(pScreen, 80, 29, "  ", 0);
            drawText(pScreen, 80, 30, "  ", 0);
            drawText(pScreen, 80, 31, "->", 0);
            break;
    }

}

void drawPlateau(UserData* pUserData, Screen* pScreen){
    int hauteur = 0;
    int largeur = 0;
    int case_actuelle = 0;
    setColor(7,255,255,255);
    setColorPair(7, 1, 0);
    move(7, 22);
    printw("Il y a %d joueurs.", pUserData->nb_Joueurs);
    move(8, 22);
    if (pUserData->pivot == OFF){
        printw("Au tour du joueur %d.                     ", pUserData->player_state+1);
    }
    else{
        printw("C'est au tour du joueur %d apres ce pivot.", pUserData->player_state+1);
    }
    move(9, 22);
    printw("C'est le tour %d.", pUserData->nb_Tours);
    move(10, 22);
    printw("C'est un pivot de cote %d.", pUserData->nb_Pivots);

    switch(pUserData->format){
        case HUITxSIX :
            largeur = 8;
            hauteur = 6;
            for(short i=0; i<=hauteur; i++){
                drawLine(pScreen, 69, 11+(4*i), 7*largeur-1, '_', 7);
            }
            for(short j=0; j<=largeur; j++){
                drawCol(pScreen, 68+(7*j), 12, 24, '|', 7);
            }
            break;

        case HUITxHUIT :
            largeur = 8;
            hauteur = 8; 
            for(short i=0; i<=hauteur; i++){
                drawLine(pScreen, 69, 11+(4*i), 7*largeur-1, '_', 7);
            }
            for(short j=0; j<=largeur; j++){
                drawCol(pScreen, 68+(7*j), 12, 32, '|', 7);
            }  
            break;
        
        case DIXxSIX :
            largeur = 10;
            hauteur = 6;  
            for(short i=0; i<=hauteur; i++){
                drawLine(pScreen, 69, 11+(4*i), 7*largeur-1, '_', 7);
            }
            for(short j=0; j<=largeur; j++){
                drawCol(pScreen, 68+(7*j), 12, 24, '|', 7);
            }
            break;

        case DIXxHUIT :
            largeur = 10;
            hauteur = 8;
            for(short i=0; i<=hauteur; i++){
                drawLine(pScreen, 69, 11+(4*i), 7*largeur-1, '_', 7);
            }
            for(short j=0; j<=largeur; j++){
                drawCol(pScreen, 68+(7*j), 12, 32, '|', 7);
            }
            break;
        default:
            RAGE_QUIT(666, "Dimensions incompatibles X(\n");
            break;
    }

    
    
    for(short j=0; j<largeur; j++){
        for(short i=0; i<hauteur; i++){
            switch (((pUserData->plateau)+(case_actuelle*sizeof(Case)))->type){

                case IMMO :
                    drawSquare(pScreen,70+(7*j), 13+(4*i), IMMO);
                    break;

                case JOUEUR1 :
                    drawSquare(pScreen,70+(7*j), 13+(4*i), JOUEUR1);
                    break;

                case JOUEUR2 :
                    drawSquare(pScreen,70+(7*j), 13+(4*i), JOUEUR2);
                    break;

                case JOUEUR3 :
                    drawSquare(pScreen,70+(7*j), 13+(4*i), JOUEUR3);
                    break;

                case VIDE : 
                    drawSquare(pScreen,70+(7*j), 13+(4*i), VIDE);
                    break;
            }
            case_actuelle ++;
        }
    }
}

void init_plateau(UserData* pUserData){
    int hauteur = 0;
    int largeur = 0;
    int case_actuelle = 0;

    switch(pUserData->format){
        case HUITxSIX :
            largeur = 8;
            hauteur = 6;
            break;

        case HUITxHUIT :
            largeur = 8;
            hauteur = 8;   
            break;
        
        case DIXxSIX :
            largeur = 10;
            hauteur = 6;  
            break;

        case DIXxHUIT :
            largeur = 10;
            hauteur = 8;
            break;
        default:
            RAGE_QUIT(666, "Dimensions incompatibles X(\n");
            break;
    }


    for(int j=0; j<largeur; j++){
        for(int i = 0; i<hauteur; i++){
            
            ((pUserData->plateau)+(case_actuelle)*sizeof(Case))->abscisse = j;
            ((pUserData->plateau)+(case_actuelle)*sizeof(Case))->ordonne = i;

            if((i==0 && j==0) || (i==0 && j==(largeur - 1)) || (i==(hauteur - 1) && j==0) || (i==(hauteur - 1) && j==(largeur - 1))){
                ((pUserData->plateau)+(case_actuelle*sizeof(Case)))->type = IMMO;
            }       
            
            else{
                ((pUserData->plateau)+(case_actuelle*sizeof(Case)))->type = VIDE;
            }
            case_actuelle ++;    
        }
    }    
}

void drawSquare(Screen* pScreen, int x, int y, char type){
    switch(type){
        case IMMO:
            setColor(1,126,126,126);
            setColorPair(1,1,1);
            drawText(pScreen, x, y ,   "    ", 1);
            drawText(pScreen, x, y+1 , "    ", 1);
            break;

        

        case JOUEUR1:
            setColor(2,255,0,0);
            setColorPair(2,2,2);
            drawText(pScreen, x, y ,   "    ", 2);
            drawText(pScreen, x, y+1 , "    ", 2);
            break;

        case JOUEUR2:
            setColor(5,255,255,0);
            setColorPair(5,5,5);
            drawText(pScreen, x, y ,   "    ", 5);
            drawText(pScreen, x, y+1 , "    ", 5);
            break;

        case JOUEUR3:
            setColor(3,0,0,255);
            setColorPair(3,3,3);
            drawText(pScreen, x, y ,   "    ", 3);
            drawText(pScreen, x, y+1 , "    ", 3);
            break;
        case VIDE:
            drawText(pScreen, x, y ,   "    ", 0);
            drawText(pScreen, x, y+1 , "    ", 0);
            break;
    }
}

void drawPivot(UserData* pUserData, Screen* pScreen){
    setColorPair(9,2,0);
    drawLine(pScreen, 69 + (7*pUserData->xPivot), 11 + (4*pUserData->yPivot), 7*pUserData->nb_Pivots-1, '_' ,9);
    drawLine(pScreen, 69 + (7*pUserData->xPivot), 11 + (4* (pUserData->yPivot + pUserData->nb_Pivots)), 7*pUserData->nb_Pivots, '_' ,9);
    drawCol(pScreen, 68 + (7*pUserData->xPivot), 12 + (4*pUserData->yPivot), 4*pUserData->nb_Pivots, '|' ,9);
    drawCol(pScreen, 68 + (7*(pUserData->xPivot+ pUserData->nb_Pivots)), 12 + (4 * pUserData->yPivot), 4*pUserData->nb_Pivots, '|' ,9);
}

void arrow_select_Menu(UserData* pUserData, Event* pEvt){
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
        /*if(pUserData->arrow == 2){ // si entrée sur "continuer la partie"
                    clear();
                    pUserData->page_Actuelle = PLATEAU;
                    pUserData->arrow = 1;
            }   
        */
    }        
}

void arrow_select_ChoixNbJoueur(UserData* pUserData, Event* pEvt){
    if(pEvt->code == KEY_ARROW_UP || pEvt->code == KEY_ARROW_DOWN){
                if (pUserData->arrow == 1){ // mets la fleche sur "lancer une nouvelle partie"
                    pUserData->arrow = 2;
                }
                else{                       // mets la fleche sur "continuer la partie"
                    pUserData->arrow = 1;
                }
            }
    
    else if (pEvt->code == KEY_RETURN){
        if(pUserData->arrow == 1){ // si entrée sur "2joueurs"
            clear();
            pUserData->page_Actuelle = CHOIX_SENS;
            pUserData->nb_Joueurs = 2;
            pUserData->arrow = 1;
            }
        else if(pUserData->arrow == 2){ // si entrée sur "3joueurs"
            clear();
            pUserData->page_Actuelle = CHOIX_SENS;
            pUserData->nb_Joueurs = 3;
            pUserData->arrow = 1;
            }   
        
    } 
}

void arrow_select_ChoixSensPivot(UserData* pUserData, Event* pEvt){
    if(pEvt->code == KEY_ARROW_UP || pEvt->code == KEY_ARROW_DOWN){
        if (pUserData->arrow == 1){ // mets la fleche sur "lancer une nouvelle partie"
            pUserData->arrow = 2;
        }
        else{                       // mets la fleche sur "continuer la partie"
            pUserData->arrow = 1;
        }
    }

    else if (pEvt->code == KEY_RETURN){
        if(pUserData->arrow == 1){ // si entrée sur "2joueurs"
            clear();
            init_plateau(pUserData);
            pUserData->page_Actuelle = PLATEAU;
            pUserData->sensPivot = HORAIRE;
            pUserData->arrow = 1;
        }
        else if(pUserData->arrow == 2){ // si entrée sur "3joueurs"
            clear();
            init_plateau(pUserData);
            pUserData->page_Actuelle = PLATEAU;
            pUserData->sensPivot = ANTIHORAIRE;
            pUserData->arrow = 1;
        }

    }
}

void arrow_select_ChoixPlateau (UserData* pUserData, Event* pEvt){
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
                    pUserData->page_Actuelle = CHOIX_NB_JOUEURS;
                    pUserData->arrow = 1;
                }
            }
}

void arrow_select_Plateau(UserData* pUserData, Event* pEvt){
    switch(pUserData->format){
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
        default:
            RAGE_QUIT(666, "Dimensions incompatibles X(\n");
        break;
    }
    // C'est au tour du jour suivant, à mettre dans le futur dans la dernière des fonctions des case PLATEAU dans event de test.c
    switch (pUserData->player_state){
        case TOUR_JOUEUR1:
            if(pEvt->code == KEY_RETURN){
                placePawn(pUserData, JOUEUR1);
            }
            break;
        case TOUR_JOUEUR2:
            if(pEvt->code == KEY_RETURN){
                placePawn(pUserData, JOUEUR2);
            }
            break;
        case TOUR_JOUEUR3:
            if(pEvt->code == KEY_RETURN){
                placePawn(pUserData, JOUEUR3);
            }
            break;
    }

}

void pivot_select_Plateau(UserData* pUserData, Event* pEvt){
    int largeur = 0;
    int hauteur = 0;
    switch (pUserData->format){
        case HUITxSIX :
            largeur = 8;
            hauteur = 6;
            break;

        case HUITxHUIT :
            largeur = 8;
            hauteur = 8;
            break;

        case DIXxSIX :
            largeur = 10;
            hauteur = 6;
            break;

        case DIXxHUIT :
            largeur = 10;
            hauteur = 8;
            break;
        default:
            RAGE_QUIT(666, "Dimensions incompatibles X(\n");
            break;
    }
    switch (pEvt->code) {
        case KEY_ARROW_LEFT:
            if(pUserData->xPivot != 0){
                pUserData->xPivot --;
            }
            break;
        case KEY_ARROW_RIGHT:
            if(pUserData->xPivot + pUserData->nb_Pivots != largeur){
                pUserData->xPivot ++;
            }
            break;
        case KEY_ARROW_UP:
            if(pUserData->yPivot != 0){
                pUserData->yPivot --;
            }
            break;
        case KEY_ARROW_DOWN:
            if(pUserData->yPivot + pUserData->nb_Pivots != hauteur){
                pUserData->yPivot ++;
            }
            break;
        case KEY_RETURN:
            if (pUserData->pivotVide == ON){
                pivot(pUserData);
            }
            if (pUserData->pivotVide == OFF){
                pUserData->nb_Tours ++;
                pUserData->pivot = OFF;
                pUserData->xPivot = 0;
                pUserData->yPivot = 0;
                pUserData->nb_Pivots = rand()%2;
                if (pUserData->nb_Pivots == 0){
                    pUserData->nb_Pivots = 3;
                }
                else {
                    pUserData->nb_Pivots = 5;
                }
            }
            break;
    }
}

void placePawn(UserData* pUserData, char joueur){
    int largeur = 0;
    int hauteur = 0;
    int ligne = 0;
    int err = 0;
    switch (pUserData->format){
        case HUITxSIX :
            largeur = 8;
            hauteur = 6;
            break;

        case HUITxHUIT :
            largeur = 8;
            hauteur = 8;
            break;

        case DIXxSIX :
            largeur = 10;
            hauteur = 6;
            break;

        case DIXxHUIT :
            largeur = 10;
            hauteur = 8;
            break;
        default:
            RAGE_QUIT(666, "Dimensions incompatibles X(\n");
            break;
    }

    for (int i= 0; i <hauteur*largeur; i = i + hauteur){
        if((pUserData->plateau)[3*i].abscisse == (pUserData->arrow)-1){
            while(((pUserData->plateau)[3*(i + ligne)].type == VIDE) && (ligne != hauteur -1)){
                ligne++;
                }
                
            
            if(ligne == hauteur -1 && (pUserData->plateau)[3*(i + ligne)].type == VIDE){
                (pUserData->plateau)[3*(i + ligne)].type = joueur;
            }
            else if (ligne == 0){
                err = 1;
            }
            else{
                ligne--;
                (pUserData->plateau)[3*(i + ligne)].type = joueur;
            }
        }
    }
    if(err == 0){    
        switch (pUserData->player_state){
            case TOUR_JOUEUR1:
                pUserData->player_state = TOUR_JOUEUR2;
                break;
            case TOUR_JOUEUR2:
                if(pUserData->nb_Joueurs == 2){
                    pUserData->player_state = TOUR_JOUEUR1;
                }
                else if(pUserData->nb_Joueurs == 3){
                    pUserData->player_state = TOUR_JOUEUR3;
                }    
                break;
            case TOUR_JOUEUR3:
                pUserData->player_state = TOUR_JOUEUR1;
                break;
        }
        pUserData->pivot = ON;
    }
}

void drawPivotOrArrow(UserData* pUserData, Screen* pScreen){
    int largeur = 0;
    int hauteur = 0;
    
    switch (pUserData->format){
        case HUITxSIX :
            largeur = 8;
            hauteur = 6;
            break;

        case HUITxHUIT :
            largeur = 8;
            hauteur = 8;
            break;

        case DIXxSIX :
            largeur = 10;
            hauteur = 6;
            break;

        case DIXxHUIT :
            largeur = 10;
            hauteur = 8;
            break;
        default:
            RAGE_QUIT(666, "Dimensions incompatibles X(\n");
            break;
    }
    
    for(int k = 0; k <= largeur; k++){
        if (pUserData->arrow == k+1 && pUserData->pivot != ON){
            drawText(pScreen, 71 + (k*7), 9, "\\/", 0);
        }
        else{
            drawText(pScreen, 71 + (k*7), 9, "  ", 0);
        }
        if (pUserData->pivot == ON) {
            pUserData->pivot = ON;
            drawPivot(pUserData, pScreen);
        }
    }
}

void tourAntiHoraire(UserData* pUserData){
    int case_actuelle = 0;
    int* tab = NULL;
    int i = pUserData->xPivot, j = pUserData->yPivot, k = -1;

    tab = malloc((pUserData->nb_Pivots)*(pUserData->nb_Pivots)*sizeof(int));

    while((i < (pUserData->nb_Pivots + pUserData->xPivot)) && (j < (pUserData->nb_Pivots + pUserData->yPivot))){
        if (pUserData->plateau[3 * case_actuelle].abscisse == i && pUserData->plateau[3 * case_actuelle].ordonne == j){
            k ++;
            tab[k] = pUserData->plateau[3 * case_actuelle].type;
            if (j + 1 == (pUserData->nb_Pivots + pUserData->yPivot)){
                i ++;
                j = pUserData->yPivot;
            }
            else{
                j ++;
            }
        }
        case_actuelle ++;
    }

    for (int p = 0; p < (pUserData->nb_Pivots)*(pUserData->nb_Pivots); p ++){
        if (tab[p] != 4){
            pUserData->pivotVide = OFF;
        }
    }

    if(pUserData->pivotVide == OFF){
        j = pUserData->nb_Pivots + pUserData->yPivot - 1;
        i = pUserData->xPivot;
        case_actuelle = 0;
        for (int l = 0; l < (pUserData->nb_Pivots) * (pUserData->nb_Pivots); l++) {
            while (pUserData->plateau[case_actuelle].ordonne != j || pUserData->plateau[case_actuelle].abscisse != i) {
                case_actuelle++;
            }
            pUserData->plateau[case_actuelle].type = tab[l];
            if (i + 1 == (pUserData->nb_Pivots + pUserData->xPivot)) {
                i = pUserData->xPivot;
                j--;
            }
            else {
                i++;
            }
            case_actuelle = 0;
        }
        free(tab);
    }
}

void pivot(UserData* pUserData){
    if(pUserData->sensPivot == ANTIHORAIRE) {
        tourAntiHoraire(pUserData);
    }
    else{
        for(int i = 0; i < 3; i++){
            tourAntiHoraire(pUserData);
        }
    }
}

void gravity(UserData* pUserData, Screen* pScreen){
    int hauteur = 0;
    int largeur = 0;
    int case_actuelle = 0;

    switch (pUserData->format){
        case HUITxSIX :
            largeur = 8;
            hauteur = 6;
            break;

        case HUITxHUIT :
            largeur = 8;
            hauteur = 8;
            break;

        case DIXxSIX :
            largeur = 10;
            hauteur = 6;
            break;

        case DIXxHUIT :
            largeur = 10;
            hauteur = 8;
            break;

        default:
            RAGE_QUIT(666, "Dimensions incompatibles X(\n");
            break;
    }

    while(case_actuelle < largeur*hauteur){
        if(pUserData->plateau[3 * (case_actuelle + 1)].type == VIDE && pUserData->plateau[3 * case_actuelle].ordonne != hauteur-1 && pUserData->plateau[case_actuelle * 3].type != IMMO) {
            pUserData->plateau[3*(case_actuelle+1)].type = pUserData->plateau[case_actuelle*3].type;
            pUserData->plateau[case_actuelle*3].type = VIDE;
        }
        case_actuelle++;
    }
}

/*void equality(UserData* pUserData, Screen* Screen){
    int largeur = 0;
    int hauteur = 0;
    int compteur = 0;

    switch (pUserData->format){
        case HUITxSIX :
            largeur = 8;
            hauteur = 6;
            break;

        case HUITxHUIT :
            largeur = 8;
            hauteur = 8;
            break;

        case DIXxSIX :
            largeur = 10;
            hauteur = 6;
            break;

        case DIXxHUIT :
            largeur = 10;
            hauteur = 8;
            break;
    }

    for(int i=0; i< largeur*hauteur; i++){
        if((pUserData->plateau)[3*i].type != VIDE){
            compteur++;
        }
    }

    if(compteur == largeur*hauteur){
        clear();
        pUserData->page_Actuelle = EQUAL;    
    }
}*/
