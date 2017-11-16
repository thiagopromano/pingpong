#pragma region Bibliotecas e Variáveis Globais

#include "lanhouse.h"
#define PI 3.1415

GameState *game;
int raquete_h = 100;
int raquete_w = 10;
int screen_w = 1280;
int screen_h = 720;
int playerVelocity = 70;
int playerDirection = 0;
int distancePlayer = 10;
int ballRadius = 7;
int keys[2] = {0};
int initialVelocity = 25;
int server;
#pragma endregion

#pragma region Funcoes Glut
void displayText(float x, float y, const char *string)
{
	int j = strlen(string);

	glRasterPos2f(x, y);
	for (int i = 0; i < j; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void desenhaRetangulo(int x1, int y1, int x2, int y2, float r, float g, float b, int preenchido)
{

	glColor3f(r + .2, g + 0.2, b + 0.2);
	if (preenchido)
	{
		glBegin(GL_QUADS);
		glVertex2f(x1, y1);
		glVertex2f(x2, y1);
		glVertex2f(x2, y2);
		glVertex2f(x1, y2);
		glEnd();
	}
	glColor3f(r, g, b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();
}

void desenhaCirculo(int radius, float x, float y)
{
	glBegin(GL_POLYGON);
	for (double i = 0; i < 10 * PI; i += PI / 6) //<-- Change this Value
		glVertex3f((cos(i) * radius) + x, (sin(i) * radius) + y, 0.0);
	glEnd();
}

void display(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fundo Branco
	glClear(GL_COLOR_BUFFER_BIT);		  // Limpa o Buffer de cor
	char buffer[4];
	glColor3f(1.0f, 1.0f, 1.0f);
	sprintf(buffer, "%d", game->p1.pontos);
	displayText(screen_w / 2 - 25, 5, buffer);
	sprintf(buffer, "%d", game->p2.pontos);
	displayText(screen_w / 2 + 10, 5, buffer);

	//Desenha as raquetes
	desenhaRetangulo(distancePlayer, game->p1.posY, distancePlayer + raquete_w, game->p1.posY + raquete_h, 1.0f, 1.0f, 1.0f, 1);
	desenhaRetangulo(screen_w - distancePlayer -raquete_w, game->p2.posY, screen_w - distancePlayer, game->p2.posY+raquete_h,1.0f,1.0f,1.0f,1);

	//Desenha a divisao da tela
	desenhaRetangulo((screen_w / 2) - 1, 0, (screen_w / 2) + 1, screen_h, 1.0f, 1.0f, 1.0f, 1);

	//Desenha a bolinha
	desenhaCirculo(ballRadius, game->posX, game->posY);

	if(!game->conectado)
	{
		desenhaRetangulo(screen_w/2 - 120,(screen_h/2) - 34 ,screen_w/2 + 130,(screen_h/2) + 24, 1, 0, 0, 1);
		glColor3f(1.0f, 1.0f, 1.0f);
		displayText(screen_w/2 - 100,(screen_h/2) - 12, "Aguardando conexao");
	}

	glFlush();
}

void keyPressed(unsigned char key, int x, int y)
{
	printf("%d\n", key);
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 119:
		keys[0] = 1;
		playerDirection = 1;
		break;
	case 115:
		keys[1] = 1;
		playerDirection = -1;
		break;
	}
}

void keyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 119:
		keys[0] = 0;
		if (keys[1])
			playerDirection = -1;
		else
			playerDirection = 0;
		break;
	case 115:
		keys[1] = 0;
		if (keys[0])
			playerDirection = 1;
		else
			playerDirection = 0;
		break;
	}
}

void *ThreadProc(void *lpv)
{
	glutInitWindowSize(screen_w, screen_h);		 // Determina o tamanho da janela gráfica
	glutInitWindowPosition(100, 100);			 // Determina qual será a posição inicial da Janela
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Define o modo de operação da Janela Gráfica
	glutCreateWindow("Janela de Testes");		 // Cria uma janela com as configurações anteriores e com um determinado título
	glutFullScreen();
	glOrtho(0, 1280, 0, 720, -1, 1); // Define o espaço de desenho em uma matrix ortográfica
	glutDisplayFunc(display);		 // Define a função que atualizará a tela
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(keyPressed); // Define a função que tratará teclas de teclado
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(display); // Inicia a função display
	glutMainLoop();		   // Inicia o laço de desenho em tela
	pthread_exit(NULL);
}

#pragma endregion

struct timespec clockAnterior;

void UpdateGame()
{	
	struct timespec atual;
	clock_gettime(CLOCK_REALTIME, &atual);
	double delta = (atual.tv_sec - clockAnterior.tv_sec) + (atual.tv_nsec - clockAnterior.tv_nsec) / (double)1000000000;
	clockAnterior = atual;
	// if(!game->conectado)
	// 	return;
		

	if (server){
		game->p1.posY += playerDirection * delta * playerVelocity;
		if(game->p1.posY + raquete_h > screen_h)
			game->p1.posY = screen_h - raquete_h;
		if(game->p1.posY < 0)
			game->p1.posY = 0;

	}
	else{
		game->p2.posY += playerDirection * delta * playerVelocity;
		if(game->p2.posY + raquete_h > screen_h)
			game->p2.posY = screen_h - raquete_h;
		if(game->p2.posY < 0)
			game->p2.posY = 0;
	}

	game->posX = game->posX + game->velX * delta;
	game->posY = game->posY + game->velY * delta;
	if (game->velX > 0)
		game->velX += 5.5*delta;
	else
		game->velX -= 5.5*delta;	
	if (game->velY > 0)
		game->velY += 5.5*delta;
	else
		game->velY -= 5.5*delta;
	//rebate parede
	if (game->posY < ballRadius || game->posY > screen_h - ballRadius)
	{
		game->velY *= -1;
	}

	//perdeu
	if (game->posX < 0)
	{
		game->p2.pontos++;
		game->posX = screen_w/2;
		game->posY = screen_h/2;
		game->velX = initialVelocity;
		game->velY = initialVelocity;
	}
	if (game->posX > screen_w)
	{
		game->p1.pontos++;
		game->posX = screen_w/2;
		game->posY = screen_h/2;
		game->velX = - initialVelocity;
		game->velY = - initialVelocity;
	}

	//Colisao do player 1
	if ( ( game->posX - ballRadius < distancePlayer + raquete_w ) && 
		( game->posY - ballRadius  > game->p1.posY && game->posY + ballRadius < game->p1.posY + raquete_h ) ){
			game->velX *=-1;
			game->posX = distancePlayer + raquete_w + ballRadius;
		}

	//Colisao do player 2
	if( (game->posX + ballRadius > screen_w - distancePlayer - raquete_w) &&
		( game->posY - ballRadius > game->p2.posY && game->posY + ballRadius < game-> p2.posY + raquete_h ) ){
			game->velX *=-1;
			game->posX = screen_w - distancePlayer -raquete_w - ballRadius;
		}
}

int main(int argc, char *argv[])
{
	game = malloc(sizeof(GameState));
	game->posX = screen_w / 2 + 350;
	game->posY = screen_h / 2;
	game->velX = -50;
	game->velY = -50;

	game->p1.pontos = game->p2.pontos = 0;
	game->p1.posY = screen_h / 2 - raquete_h / 2;
	game->p2.posY = screen_h / 2 - raquete_h / 2;
	game->conectado = 0;
	pthread_t thread[2]; // Handle do Console
	int H_Thread = 0;

	printf("Voce deseja servir o jogo ou so conectar a um outro servidor? (1 para servir, 0 para ser cliente)\n");
	scanf("%d", &server);

	if (server)
	{
		pthread_t thread;

		pthread_create(&thread,
					   NULL,
					   servidor,
					   game);
	}
	else
	{
		printf("Digite o endereco do servidor\n");
		scanf("%s", enderecoServidor);

		pthread_t thread;

		pthread_create(&thread,
					   NULL,
					   cliente,
					   game);
	}

	glutInit(&argc, argv); // Inicia GLUT
	usleep(1000000);

	H_Thread = pthread_create(&thread[0], // pthread_t
							  NULL,		  // Atributos da Thread
							  ThreadProc, // Função que a Thread executará
							  NULL);	  // Parâmetro da função

#ifdef Verbose
	if (H_Thread != -1)
		printf("Thread GLUT Iniciada						[ 0K ] \n");
	else
		printf("Thread GLUT Iniciada						[ Não 0K ] \n");
#endif

	clock_gettime(CLOCK_REALTIME, &clockAnterior);
	while (1)
	{
		UpdateGame();
		usleep(10000);
	}
}
