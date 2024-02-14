#include <stdio.h>

// x1 e y1 é o canto inferior esquerdo
// x2 e y2 é o oposto
struct bola {
    int x1;
    int y1;
    int x2;
    int y2;
    int vel[2];
    int cor;   
};

// x1 e y1 é o canto inferior esquerdo
// x2 e y2 é o oposto
// Tamanho de cada bloco 53px x 20px
struct bloco{
    int x1;
    int y1;
    int x2;
    int y2;
    int cor;
    int existe;
    void (*acelBolaBloco)(struct bloco, struct bola);
};

//x1 é o começo e x2 o fim da barra
struct barra{
    //Zonas da barra: Esquerda (0, 12); Centro (13, 16); Direita (17, 29)
    int x1;
    int x2;
    int y;
    int cor;
    void (*acelBolaBarra)(struct barra, struct bola);
};

// Função de soma
void acelBolaBloco(struct bloco brick, struct bola ball) {
    if (ball.y2 == brick.y1){
        //Batida inferior
        ball.vel[1] = 1; 
    } else if(ball.y1 == brick.y2){
        //Batida superior
        ball.vel[1] = -1;
    } else if(ball.x1 == brick.x2){
        //Batida na direita
        ball.vel[0] = 1;
    }else if(ball.x2 == brick.x1){
        ball.vel[0] = -1;
    }
}

// Função de subtração
void acelBolaBarra(struct barra bar, struct bola ball) {
    if ((ball.x1 >= bar.x1) && (ball.x1 <= bar.x1 + 12)){
        ball.vel[0] = -1;
        ball.vel[1] = -1;
    } else if ((ball.x1 >= bar.x1 + 13) && (ball.x1 <= bar.x1 + 16)){
        ball.vel[0] = 0;
        ball.vel[1] = -1;
    } else if ((ball.x1 >= bar.x1 + 17) && (ball.x1 <= bar.x2)){
        ball.vel[0] = 1;
        ball.vel[1] = -1;
    }
}

void moverBola(struct bola ball){
    ball.x1 = ball.x1 + ball.vel[0];
    ball.x2 = ball.x2 + ball.vel[0];
    ball.y1 = ball.y1 + ball.vel[1];
    ball.y2 = ball.y2 + ball.vel[1];
}

int main() {
    //Inicializando os blocos, serão 6 por linha, 4 linhas.
    int perdeu = 0;
    int x1, y1, x2, y2;
    int numBlocos = 24;
    int x1_in = 1;
    int y1_in = 20;
    int auxx1 = 0;
    int auxy1 = 0;
    int passo_x = 53; //Tamanho X do bloco
    int passo_y = 20; //Tamanho Y do bloco
    struct barra barra;
    struct bloco blocos[numBlocos];
    
    for (int i = 0; i < numBlocos; i++){
        x1 = x1_in + (passo_x * auxx1++);
        y1 = y1_in + (passo_y * auxy1);
        x2 = x1 + (passo_x - 1);
        y2 = y1 - (passo_y - 1);

        if (auxx1 == 5){
            auxx1 = 0;
            auxy1++;
        }

        blocos[i].x1 = x1;
        blocos[i].y1 = y1;
        blocos[i].x2 = x2;
        blocos[i].y2 = y2;
        blocos[i].existe = 1;
        blocos[i].cor = 1;
    }

    //Fim da inicialização dos blocos

    //Inicialização da bola

    struct bola bola;
    bola.x1 = 40;
    bola.x2 = 41;
    bola.y1 = 20;
    bola.y2 = 21;
    bola.vel[0] = 0;
    bola.vel[1] = 1;

    //Fim da inicialização da bola

    //Inicialização da barra


    //Fim da inicialização da barra

    //Inicialização dos limites
    int limite_x1= 0;
    int limite_y1= 239;
    int limite_x2= 319;
    int limite_y2= 0;

    while (1){
        moverBola(bola);
        
        //Checar choque com as laterais
        if (bola.x1 <= limite_x1){
            bola.vel[0] = 1;
        } else if(bola.x2 >= limite_x2){
            bola.vel[0] = -1;
        } else if(bola.y2 >= limite_y2){
            bola.vel[1] = 1;
        } else if(bola.y1 >= limite_y1){
            perdeu = 1;
        } 

        for (int i = 0; i < numBlocos; i++){
            if(blocos[i].existe){

                if (((bola.x1 >= blocos[i].x1 && bola.x1 <= blocos[i].x2) && 
                (bola.y1 >= blocos[i].y1 && bola.y1 <= blocos[i].y2)) ||
                ((bola.x2 >= blocos[i].x1 && bola.x2 <= blocos[i].x2) && 
                (bola.y2 >= blocos[i].y1 && bola.y2 <= blocos[i].y2))){
                    //Houve colisão com o bloco
                    blocos[i].existe = 0; //Bloco deixa de existir
                    blocos[i].acelBolaBloco(blocos[i], bola); //Bola muda de direção
                }
            }
        }

        if (bola.x1 >= barra.x1 && bola.x1 <= barra.x2 && bola.y1 == barra.y){
            barra.acelBolaBarra(barra, bola);
        }
    }

    printf("Hello to, world!\n");
    return 0;
}
