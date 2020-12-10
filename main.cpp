#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <list>

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

using namespace std;

void gotoxy(int x, int y){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;

    SetConsoleCursorPosition(hCon, dwPos);
}

void ocultarCursor(){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 25;
    cci.bVisible = FALSE;

    SetConsoleCursorInfo(hCon, &cci);
}

void pintar_limites(){
    for(int i = 2; i < 78; i++){
        gotoxy(i, 3); printf("%c",205);
        gotoxy(i, 33); printf("%c",205);
    }
    for(int i = 4; i < 33; i++){
        gotoxy(2, i); printf("%c",186);
        gotoxy(77, i); printf("%c",186);
    }
    gotoxy(2, 3); printf("%c", 201);
    gotoxy(2, 33); printf("%c", 200);
    gotoxy(77, 3); printf("%c",187);
    gotoxy(77, 33); printf("%c",188);

    gotoxy(2, 34); printf("V. 1.2.0");
}

class NAVE {
    int x, y;
    int hp;
    int vida;
public:
    NAVE(int _x, int _y, int _hp, int _vida): x(_x), y(_y), hp(_hp), vida(_vida){}
    int getX(){ return x; }
    int getY(){ return y; }
    int getVida(){ return vida; }
    void damage(){ hp--; }
    void pintar();
    void borrar();
    void mover();
    void pintar_hp();
    void destruir();
};

void NAVE::pintar_hp(){
    gotoxy(57, 2); printf("Vidas x %d",vida);
    gotoxy(68, 2); printf("Salud");
    gotoxy(73,2); printf("     ");
    for(int i=0; i < hp; i++){
        gotoxy(74+i,2); printf("%c",3);
    }
}

void NAVE::pintar(){
    gotoxy(x, y); printf("  %c",30);
    gotoxy(x, y+1); printf(" %c%c%c",40,207,41);
    gotoxy(x,y+2); printf("%c%c %c%c", 30, 190, 190, 30);
}

void NAVE::borrar(){
    gotoxy(x, y); printf("       ");
    gotoxy(x, y+1); printf("       ");
    gotoxy(x,y+2); printf("       ");
}

void NAVE::mover(){
    if(kbhit()){
            char tecla = getch();
            borrar();
            if(tecla == UP && y >  4) y--;
            if(tecla == LEFT && x > 3) x--;
            if(tecla == DOWN && y+3 < 33 ) y++;
            if(tecla == RIGHT && x+6 < 76 ) x++;
            if(tecla == 'e') hp--;
            pintar();
            pintar_hp();
        }
}

void NAVE::destruir(){
    if(hp == 0){
        borrar();
        gotoxy(x,y); printf("   **   ");
        gotoxy(x,y+1); printf("  ****  ");
        gotoxy(x,y+2); printf("   **   ");
        Sleep(200);
        borrar();
        gotoxy(x,y); printf(" * ** *");
        gotoxy(x,y+1); printf("  ****  ");
        gotoxy(x,y+2); printf(" * ** *");
        Sleep(200);
        borrar();

        vida--;
        hp = 3;
        pintar_hp();
        pintar();
    }
}

class bala{
    int x, y;
public:
    int getX(){ return x; }
    int getY(){ return y; }
    bala(int _x,int _y): x(_x), y(_y){}
    void mover();
    bool fuera();
};

bool bala::fuera(){
    if (y == 4) return true;
    return false;
}

void bala::mover(){
    gotoxy(x,y); printf(" ");
    y--;
    gotoxy(x,y); printf("%c",179);
}

class astro{
    int x, y;
public:
    astro(int _x, int _y): x(_x), y(_y){}
    void pintar();
    void mover();
    int getX(){ return x; };
    int getY(){ return y; };
    void colision(class NAVE &N);
};

void astro::pintar(){
    gotoxy(x,y); printf("%c",184);
}
void astro::mover(){
    gotoxy(x,y); printf(" ");
    y++;
    if(y > 32){
        x = rand()%73 + 3;
        y = 4;
    }
    pintar();
}
void astro::colision(class NAVE &N){
    if(x >= N.getX() && x < N.getX()+6 && y >= N.getY() && y <= N.getY()+2){
        N.damage();
        N.borrar();
        N.pintar();
        N.pintar_hp();
        x = rand()%76 + 4;
        y = 4;
    }
}

int main(){
    ocultarCursor();
    pintar_limites();
    bool game = false, start = true, gameOver = true;
    while(!game){
        gotoxy(32,1); printf("%cAstro The Game%c",179,179);
        gotoxy(3,31); printf("Start = ENTER");
        gotoxy(3,32); printf("Exit = ESC");
        if(kbhit()){
            char tecla = getch();
            if(tecla == 13){
                gotoxy(3,31); printf("             ");
                gotoxy(3,32); printf("          ");
                start = false;
                gameOver = false;
            }
            if(tecla == 27) game = true;
        }
        while(!start){
            system("cls");
            pintar_limites();

            for(int i = 0; i <= 3; i++){
                gotoxy(34, 16); printf("         ");
                gotoxy(34, 16); printf("STAR GAME");
                gotoxy(38,17); printf(" ");
                gotoxy(38,17); printf("%d", i);
                Sleep(1000);
                gotoxy(38,17); printf(" ");
                gotoxy(34, 16); printf("         ");
            }

            NAVE N(38,30,3,3);
            N.pintar();
            N.pintar_hp();

            list <bala*> B;
            list <bala*>::iterator it;

            list <astro*> A;
            list <astro*>::iterator itA;
            for(int i = 0; i < 5; i++){
                A.push_back(new astro(rand()%75+3, rand()%5 + 4));
            }

            int score = 0;
            while(!gameOver){
                gotoxy(2, 2); printf("Score %d", score);
                if(kbhit()){
                    char tecla = getch();
                    if(tecla == 32)
                        B.push_back(new bala(N.getX()+2,N.getY()-1));
                }
                for(it = B.begin(); it != B.end(); it++){
                    (*it)->mover();
                    if((*it)->fuera()){
                        gotoxy((*it)->getX(),(*it)->getY()); printf(" ");
                        delete(*it);
                        it = B.erase(it);
                    }
                }

                for(itA = A.begin(); itA != A.end(); itA++){
                    (*itA)->mover();
                    (*itA)->colision(N);
                }

                for(itA = A.begin(); itA != A.end(); itA++){
                    for(it = B.begin(); it != B.end(); it++){
                        if((*itA)->getX() == (*it)->getX() && ((*itA)->getY()+1 == (*it)->getY() || (*itA)->getY() == (*it)->getY())){
                            gotoxy((*it)->getX(),(*it)->getY()); printf(" ");
                            delete(*it);
                            it = B.erase(it);

                            A.push_back(new astro(rand()%74+3,4));
                            gotoxy((*itA)->getX(),(*itA)->getY()); printf(" ");
                            gotoxy((*itA)->getX(),(*itA)->getY()); printf("*");
                            Sleep(200);
                            gotoxy((*itA)->getX(),(*itA)->getY()); printf(" ");
                            delete(*itA);
                            itA = A.erase(itA);

                            score += 10;

                        }
                    }
                }

                if(N.getVida() == 0){
                    gotoxy(34, 16); printf("GAME OVER");
                    gameOver = true;
                    start = true;
                }

                N.destruir();
                N.mover();
                Sleep(30);
            }

        }
    }

    return 0;
}
