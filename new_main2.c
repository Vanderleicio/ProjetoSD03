
typedef struct {
    int screen_x, screen_y;
    int char_x, char_y;
} Tela;

typedef struct {
    int pos_x;
    int pos_y;
    int dir; //Não sei o que seria isso, então vou criar o vel embaixo (att: Vanderleicio)
    int vel[2];
    unsigned color;
} Bola;

typedef struct {
    // (pos_x1, pos_y1)
    int pos_x1;    // Ponto inferior esquerdo
    int pos_y1;    // Ponto inferior esquerdo

    // (pos_x2, pos_y2)
    int pos_x2;    // Ponto superior direito
    int pos_y2;    // Ponto superior direito

    //
    int existe;
    int width;
    int height;
    unsigned color;
} Bloco;


typedef struct {
    int pos_x1;    // Extremidade esquerda da barra
    int pos_x2;    // Extremidade direita da barra
    int width;     // Comprimento da barra (embora possa ser obtido com um simples x2-x1)
    int pos_y;     // Posição em Y onde a barra vai estar (é um valor inalterável)
    unsigned color;// 0xFFE0;
} Barra;


//
void erase_pos_Bola_previous(){
    //video_box(aux_x1-1,aux_y1-1,aux_x2-1,aux_y2-1,0);

}

//
void erase_pos_Bloco(){

}
// ====================================== BLOCO DE FUNÇÕES PARA AS CONFIGURAÇÕES INICIAIS ==========================================//
/* Constrói as bordas da tela
    Param:
        - tela -> objeto que representa os tamanhos da tela
        - color -> cor a ser usada para fazer as linhas das bordas
*/
void build_cage(Tela *tela, unsigned color){
    //linha horizontal em cima de (0,0) para (319,0)
	video_line(0, 0, (*tela).screen_x - 1, 0, color);
	//fazer linha lateral esquerda de (0,0) para (0,239)
	video_line(0,0, 0, (*tela).screen_y - 1, color);
	// fazer linha lateral direita de (319,0) para (319, 239)
	video_line((*tela).screen_x - 1, 0, (*tela).screen_x - 1, (*tela).screen_y - 1, color);
	//
	//video_show();
}


// ========================================================== BLOCO DE FUNÇÕES PARA A BARRA ==========================================//
/* Apaga pinta de preto a posição da barra passada
    Param:
        - pos_x1 -> Posição da extremidade esquerda da barra
        - pos_x2 -> Posição da extremidade direita da barra
  
*/
void erase_pos_Barra_previous(int pos_x1, int pos_x2){
    video_line(pos_x1, 235, pos_x2, 235, 0);
}


/* Atualiza a posição da barra, no buffer, com base na leitura de acelerômetro
    Param:
        - barra -> Objeto Barra
*/
void update_pos_Barra(Barra *barra, Tela *tela){
    int ac_ready, ac_tap, ac_dtap, x_val, y_val, z_val, mgper;
    // Apaga a posição anterior antes de atualizar
    erase_pos_Barra_previous((*barra).pos_x1, (*barra).pos_x2);

    // Realiza a leitura do acelerômetro
    accel_read(&ac_ready, &ac_tap, &ac_dtap, &x_val, &y_val, &z_val, &mgper);
    // Faz a calibragem para não reduzir o valor e utilizar diretamente como movimento da barra
	x_val = x_val/5;

    // Trecho de código para fazer a saturação e não deixar a bola passar das extremidades
	if ((*barra).pos_x2 + x_val >= (*tela).screen_x - 2){ // Para a bola não passar do lado direito da tela
		(*barra).pos_x2 = (*tela).screen_x - 2;
		(*barra).pos_x1 = ((*tela).screen_x - 2) - ((*barra).width);// 
	}
	else if ((*barra).pos_x1 + x_val <= 2){ // Para a bola não passa do lado esquerdo da tela
		(*barra).pos_x1 = 2;
		(*barra).pos_x2 = (*barra).pos_x1 + (*barra).width;
	}
	else{// Estando dentro do intervalo de x permitido para locomoção
		(*barra).pos_x1 = (*barra).pos_x1 + x_val;
		(*barra).pos_x2 = (*barra).pos_x2 + x_val;
	}
    // Escreve no buffer
    video_line((*barra).pos_x1, (*barra).pos_y, (*barra).pos_x2, (*barra).pos_y, (*barra).color);
}

void acelBolaBarra(Barra *bar, Bola *ball) {
    if (((*ball).pos_x >= (*bar).pos_x1) && ((*ball).pos_x <= (*bar).pos_x1 + 12)){
        (*ball).vel[0] = -1;
        (*ball).vel[1] = -1;
    } else if (((*ball).pos_x >= (*bar).pos_x1 + 13) && ((*ball).pos_x <= (*bar).pos_x1 + 16)){
        (*ball).vel[0] = 0;
        (*ball).vel[1] = -1;
    } else if (((*ball).pos_x >= (*bar).pos_x1 + 17) && ((*ball).pos_x <= (*bar).pos_x2)){
        (*ball).vel[0] = 1;
        (*ball).vel[1] = -1;
    }
}

void acelBolaBloco(Bloco *brick, Bola *ball) {
    if (((*ball).pos_y-1) == (*brick).pos_y1){
        //Batida inferior
        (*ball).vel[1] = 1; 
    } else if((*ball).pos_y == (*brick).pos_y2){
        //Batida superior
        (*ball).vel[1] = -1;
    } else if((*ball).pos_x == (*brick).pos_x2){
        //Batida na direita
        (*ball).vel[0] = 1;
    }else if(((*ball).pos_x + 1) == (*brick).pos_x1){
        (*ball).vel[0] = -1;
    }
}

// Atualiza a posição da bola
void update_pos_Bola(){

}


//Verifica se a bola bateu em alguma coisa 
void control_colision(){

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){
    Tela tela;//Ok
    Bola bola;
    Barra barra;//Ok
    Bloco vetor[24]; //6 blocos por linha; 4 linhas

    unsigned int video_color[] = {video_WHITE, video_YELLOW, video_RED,
    video_GREEN, video_BLUE, video_CYAN, video_MAGENTA, video_GREY,
    video_PINK, video_ORANGE};
    
    // Inicializando a tela vga
    video_open();
    // Obtendo params da tela
    video_read(&tela.screen_x, &tela.screen_y, &tela.char_x, &tela.char_y);
    // Limpo qualquer coisa que possa ter na tela
    video_erase();
    video_clear ( ); // clear current VGA Back buffer
    video_show ( ); // swap Front/Back to display the cleared buffer
    video_clear ( ); // clear the VGA Back buffer, where we will draw lines

    // Inicializando o Acelerômetro
    accel_open();
    accel_init ();
    accel_calibrate ();
    accel_format (1, 8);

    // Colocando os objetos nas posições iniciais
    build_cage(&tela, video_color[1]);// Constrói a gaiola
    // =========== Constrói a barra
    barra.width = 30;
    barra.pos_y = 235;
    barra.color = video_color[0];
    barra.pos_x1 = ((tela.screen_x -2)/2) - (barra.width/2);
    barra.pos_x2 = barra.pos_x1 + barra.width;
    // =========== Constrói a bola
    bola.color = video_color[0];
    bola.dir = 12;
    bola.vel[0] = 0;
    bola.vel[1] = 1;
    bola.pos_x = rand() % (tela.screen_x-2);// Gera a bola em uma posição aleatória no eixo x
    bola.pos_y =  tela.screen_y - 100;// Posição fixa em y 
    // Constrói os blocos
    int x1, y1, x2, y2;
    int numBlocos = 24;
    int x1_in = 1;
    int y1_in = 20;
    int auxx1 = 0;
    int auxy1 = 0;
    int passo_x = 53; //Tamanho X do bloco
    int passo_y = 20; //Tamanho Y do bloco

    for (int i = 0; i < numBlocos; i++){
        x1 = x1_in + (passo_x * auxx1++);
        y1 = y1_in + (passo_y * auxy1);
        x2 = x1 + (passo_x - 1);
        y2 = y1 - (passo_y - 1);

        if (auxx1 == 5){
            auxx1 = 0;
            auxy1++;
        }

        vetor[i].pos_x1 = x1;
        vetor[i].pos_y1 = y1;
        vetor[i].pos_x2 = x2;
        vetor[i].pos_y2 = y2;
        vetor[i].existe = 1;
        vetor[i].color = video_color[0];
    }

    int limite_x1= 0;
    int limite_y1= 239;
    int limite_x2= 319;
    int limite_y2= 0;

    video_show();// Exibe


    // Loop principal do jogo

    while (1){
        moverBola(bola);
        
        //Checar choque com as laterais
        if (bola.pos_x <= limite_x1){
            bola.vel[0] = 1;
        } else if((bola.pos_x + 1) >= limite_x2){
            bola.vel[0] = -1;
        } else if((bola.pos_y - 1) >= limite_y2){
            bola.vel[1] = 1;
        } else if(bola.pos_y >= limite_y1){
            //PERDEU F
        } 

        for (int i = 0; i < numBlocos; i++){
            if(vetor[i].existe){

                if (((bola.pos_x >= vetor[i].pos_x1 && bola.pos_x <= vetor[i].pos_x2) && 
                (bola.pos_y >= vetor[i].pos_y1 && bola.pos_y <= vetor[i].pos_y2)) ||
                (((bola.pos_x + 1) >= vetor[i].pos_x1 && (bola.pos_x + 1) <= vetor[i].pos_x2) && 
                ((bola.pos_y - 1) >= vetor[i].pos_y1 && (bola.pos_y - 1) <= vetor[i].pos_y2))){
                    //Houve colisão com o bloco
                    vetor[i].existe = 0; //Bloco deixa de existir
                    acelBolaBloco(&vetor[i], &bola); //Bola muda de direção
                }
            }
        }

        if (bola.pos_x >= barra.pos_x1 && bola.pos_x <= barra.pos_x2 && bola.pos_y == barra.pos_y){
            acelBolaBarra(&barra, &bola);
        }
    }

    return 0;
}
