#pragma region Bibliotecas e Variáveis Globais
	// ---------- Declarações de Bibliotecas ----------
	#include <arpa/inet.h>
	#include <errno.h>
	#include <fcntl.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <pthread.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <sys/socket.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <unistd.h>
	#include <string.h>
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
#define PORTA 12346                         // Porta de conexÃ£o na qual os clientes se conectarÃ£o
#define MAXDATASIZE 1000 			// Tamanho mÃ¡ximo da recepÃ§Ã£o

    // ---------- Declarações de Variáveis ----------
	
	typedef struct{
	    char nome[100];
	    int pontos;
	    int posX;
	}Player;
	
	typedef struct{
	    Player p1;
	    Player p2;    
	    int posX,posY;
	}GameState;
	
#pragma endregion

#pragma region Funcoes Glut
	void displayText( float x, float y, const char *string ) {
		int j = strlen( string );
	 
		glRasterPos2f( x, y );
		for( int i = 0; i < j; i++ ) {
			glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, string[i] );
		}
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
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	// Fundo Branco
		glClear(GL_COLOR_BUFFER_BIT);			// Limpa o Buffer de cor
		
		glColor3f(0.0f,0.0f,0.0f);
		displayText(50,50,"Alana");

		/* 	Limpa o Buffer
		* 	GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT ou GL_STENCIL_BUFFER_BIT
		*/
			
		glFlush();
	}

	void keyPressed(unsigned char key, int x, int y) {
	    
	    printf("%d\n",key);
	    
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
		glutInitWindowSize(1280, 720);					// Determina o tamanho da janela gráfica
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

int main(int argc, char* argv[]){
	#pragma region Declarações Locais		
		// ---------- Declarações de Thread ----------
		pthread_t thread[2];						// Handle do Console
		int H_Thread = 0;

	#pragma endregion
    
	int server;
	printf("Voce deseja servir o jogo ou so conectar a um outro servidor? (1 para servir, 0 para ser cliente)");
	scanf("%d", &server);

	if (server)
	{
		servidor();
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
	
	while(1){}
}