#pragma region Bibliotecas e Variáveis Globais
	#include "lanhouse.h"
	#define PI 3.1415

	// ---------- Declarações de Bibliotecas ----------
    /*
    *	Biblioteca para implementação de threads em C para linux
    *	Você precisará utilizar a diretriz -lpthread ao compilar
    */
	#include <pthread.h>

    /*
    *	Biblioteca só está disponível após a instalação dos pacotes de desenvolvedor
    *	apt-get install freeglut3-dev
    *	Você precisará utilizar as diretrizes -lGLU -lGL -lglut ao compilar
    */
	#include <GL/glut.h>
	
    // para funcoes do gpio
// 	#include "mylib.h"


// ------------- DefiniÃ§Ãµes -------------

// ---------- Declarações de Variáveis ----------

	GameState* game;
    int raquete_w = 100;	
    int raquete_h = 10;	
	int screen_w = 1280;
	int screen_h = 720;
	float velocidadeBolinha = 1;
	
#pragma endregion

#pragma region Funcoes Glut
	void displayText( float x, float y, const char *string ) {
		int j = strlen( string );
	 
		glRasterPos2f( x, y );
		for( int i = 0; i < j; i++ ) {
			glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, string[i] );
		}
	}
	
	void desenhaRetangulo( int x1, int y1, int x2, int y2, float r, float g, float b, int preenchido) {

		glColor3f(r+.2,g+0.2,b+0.2);
		if (preenchido)
		{
			glBegin(GL_QUADS);
				glVertex2f(x1, y1);
				glVertex2f(x2, y1);
				glVertex2f(x2, y2);
				glVertex2f(x1, y2);
			glEnd();
		}
		glColor3f(r,g,b);
		glBegin(GL_LINE_LOOP);
				glVertex2f(x1, y1);
				glVertex2f(x2, y1);
				glVertex2f(x2, y2);
				glVertex2f(x1, y2);
		glEnd();
	}
	
	void desenhaCirculo(int radius, float x, float y){
	    glBegin(GL_POLYGON);
		for(double i = 0; i < 10 * PI; i += PI / 6) //<-- Change this Value
 					glVertex3f((cos(i) * radius) + x, (sin(i) * radius) + y, 0.0);
		glEnd();
	}

	/*
	*	Função Display
	*	Função que guarda tudo o que será desenhado na atualização da tela
	*/
	void display(void) 
	{
		/* 	
		*	Limpa o Buffer de Cor
		* 	void glClearColor(	GLfloat red,
 								GLfloat green,
 								GLfloat blue,
 								GLfloat alpha);
		*/
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// Fundo Branco
		glClear(GL_COLOR_BUFFER_BIT);			// Limpa o Buffer de cor
		char buffer[4];
		glColor3f(1.0f,1.0f,1.0f);
		sprintf(buffer,"%d",game->p1.pontos);
		displayText(5,screen_h/2 + 10, buffer);
		sprintf(buffer,"%d",game->p2.pontos);
        displayText(5,screen_h/2 - 25, buffer);

        //Desenha as raquetes    
        desenhaRetangulo(game->p1.posX,screen_h-10,game->p1.posX+raquete_w,screen_h-10-raquete_h,1.0f,1.0f,1.0f,1);
        desenhaRetangulo(game->p2.posX,10,game->p2.posX+raquete_w,10+raquete_h,1.0f,1.0f,1.0f,1);
        
        //Desenha a divisao da tela        
        desenhaRetangulo(0,(screen_h/2)-1,screen_w,(screen_h/2)+1,1.0f,1.0f,1.0f,1);
        
        desenhaCirculo(7,game->posX,game->posY);
		/* 	Limpa o Buffer
		* 	GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT ou GL_STENCIL_BUFFER_BIT
		*/
			
		glFlush();
	}

	void keyPressed(unsigned char key, int x, int y) {
	    
	    switch(key){
	        case 27:
    	        exit(0);
	        break;
	        
	    }
	    
	    
		/*
			Função capaz de tratar ações de teclado
			key -> tecla pressionada
		*/
	}

	void MouseFunc(int button, int state, int x, int y){
		/*
			Função capaz de tratar ações de mouse
			button -> tecla pressionada
			state  -> tecla pressionada ou não
			x e y  -> posição do ponteiro
		*/
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
			}
			else
			{
			}
		}

		if (button == GLUT_RIGHT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
			}
		}
		
	}
	void * ThreadProc(void* lpv)
	{	
		glutInitWindowSize(screen_w, screen_h);					// Determina o tamanho da janela gráfica
		glutInitWindowPosition(100, 100);				// Determina qual será a posição inicial da Janela
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	// Define o modo de operação da Janela Gráfica
		glutCreateWindow("Janela de Testes");			// Cria uma janela com as configurações anteriores e com um determinado título
		glutFullScreen();
		glOrtho(0, 1280, 0, 720, -1, 1);					// Define o espaço de desenho em uma matrix ortográfica
		glutDisplayFunc(display);						// Define a função que atualizará a tela
		glutKeyboardFunc(keyPressed);					// Define a função que tratará teclas de teclado
		glutMouseFunc(MouseFunc);						// Define a função que tratatá o uso do mouse
		glutIdleFunc(display);							// Inicia a função display
		
		glutMainLoop();									// Inicia o laço de desenho em tela

		pthread_exit(NULL);
	}

#pragma endregion
struct timespec clockAnterior;
void UpdateGame(){
    struct timespec atual;
    clock_gettime(CLOCK_REALTIME, &atual);
    double delta = (atual.tv_sec - clockAnterior.tv_sec) + (atual.tv_nsec - clockAnterior.tv_nsec)/1000000000L;
    printf("%lf\n", delta);
    clockAnterior=atual;
    
    game->posX = game->posX + velocidadeBolinha*delta;
    game->posY = game->posY + velocidadeBolinha*delta;
    


}

int main(int argc, char* argv[]){
	#pragma region Declarações Locais		
        game = malloc(sizeof(GameState));
        game->posX = screen_w/2;
        game->posY = screen_h/2;
        game->directionX = 1;
        game->directionY = 1;
        
        game->p1.pontos = game->p2.pontos = 0;
        game->p1.posX = screen_w/2 - raquete_w/2;
        game->p2.posX = screen_w/2 - raquete_w/2;
        game->conectado = 0;
		// ---------- Declarações de Thread ----------
		pthread_t thread[2];						// Handle do Console
		int H_Thread = 0;

	#pragma endregion
    
	int server;
	printf("Voce deseja servir o jogo ou so conectar a um outro servidor? (1 para servir, 0 para ser cliente)\n");
	scanf("%d", &server);

	if (server)
	{
		pthread_t thread;
		
	
		pthread_create( thread,
			NULL,
			servidor,
			game);
	}

    #pragma region Inicia Thread GLUT
		glutInit(&argc, argv);						// Inicia GLUT
		usleep(1000000);
		
		H_Thread = pthread_create(&thread[0],		// pthread_t
			NULL,									// Atributos da Thread
			ThreadProc,								// Função que a Thread executará
			NULL);									// Parâmetro da função

		#ifdef Verbose
			if (H_Thread != -1)
				printf("Thread GLUT Iniciada						[ 0K ] \n");
			else
				printf("Thread GLUT Iniciada						[ Não 0K ] \n");
		#endif
	#pragma endregion
	
    clock_gettime(CLOCK_REALTIME, &clockAnterior);
	while(1){
	    UpdateGame();
	    usleep(500);
	}
}