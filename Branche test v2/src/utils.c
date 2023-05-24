#include <sys/time.h>
#include <string.h>

#include "libGameRGR2.h"
// Code couleur : 1 = Gris
//                2 = Rouge
//                3 = Bleu
//                4 = Jaune texte
//                5 = Jaune case

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
        printw(pWrite);
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
    switch(pUserData->format){
        case HUITxSIX :
            largeur = 8;
            hauteur = 6;
            for(int i=0; i<=hauteur; i++){
                drawLine(pScreen, 69, 11+(4*i), 7*largeur-1, '_', 0);
            }
            for(int j=0; j<=largeur; j++){
                drawCol(pScreen, 68+(7*j), 12, 24, '|', 0);
            }
            break;

        case HUITxHUIT :
            largeur = 8;
            hauteur = 8; 
            for(int i=0; i<=hauteur; i++){
                drawLine(pScreen, 69, 11+(4*i), 7*largeur-1, '_', 0);
            }
            for(int j=0; j<=largeur; j++){
                drawCol(pScreen, 68+(7*j), 12, 32, '|', 0);
            }  
            break;
        
        case DIXxSIX :
            largeur = 10;
            hauteur = 6;  
            for(int i=0; i<=hauteur; i++){
                drawLine(pScreen, 69, 11+(4*i), 7*largeur-1, '_', 0);
            }
            for(int j=0; j<=largeur; j++){
                drawCol(pScreen, 68+(7*j), 12, 24, '|', 0);
            }
            break;

        case DIXxHUIT :
            largeur = 10;
            hauteur = 8;
            for(int i=0; i<=hauteur; i++){
                drawLine(pScreen, 69, 11+(4*i), 7*largeur-1, '_', 0);
            }
            for(int j=0; j<=largeur; j++){
                drawCol(pScreen, 68+(7*j), 12, 32, '|', 0);
            }
            break;
    }



    for (int k = 0; k < largeur; k++){
        if (k + 1 == pUserData->arrow){
            drawText(pScreen, 72 + (k*7), 9, "\\/", 0);
        }
        else{
            drawText(pScreen, 72 + (k*7), 9, "  ", 0);
        }
    }
    

    for(int j=0; j<largeur; j++){
        for(int i=0; i<hauteur; i++){
            switch (((pUserData->plateau)+(case_actuelle*sizeof(Case)))->type){
                
                case IMMO :
                    drawSquare(pScreen,70+(7*j), 13+(4*i), IMMO);
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
        case VIDE:
            drawText(pScreen, x, y ,   "    ", 0);
            drawText(pScreen, x, y+1 , "    ", 0);
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
    }
}