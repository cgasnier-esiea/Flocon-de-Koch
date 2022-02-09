#include <stdio.h>
#include <stdlib.h>
#include "utils_SDL.h"

typedef struct sPoint{
	int m_x;
	int m_y;
}point;

/**
*	@brief	Fonction permettant de dessiner une courbe de Koch.
*
*	@param	p_affichage		Fenêtre sur laquelle on souhaite dessiner.
*	@param	p_iteration		Nombre d'itération que l'on souhaite appliquer à la courbe.
*	@param	p_a				Point de départ du segment.
*	@param	p_b				Point d'arrivée du segment.
*/
void vonKoch(SDL_Surface * p_affichage, int p_iteration, point p_a, point p_b)
{
	point p_c, p_d, p_e;

	if (p_iteration == 0) {
		
		_SDL_DrawLine(p_affichage, p_a.m_x, p_a.m_y, p_b.m_x, p_b.m_y, 255, 255, 255);
	}
	else{
		
		p_c.m_x = (p_a.m_x + (p_b.m_x - p_a.m_x) / 3);
        p_c.m_y = (p_a.m_y + (p_b.m_y - p_a.m_y) / 3);
		
		p_d.m_x = (p_a.m_x + 2*(p_b.m_x - p_a.m_x) / 3);
		p_d.m_y = (p_a.m_y + 2*(p_b.m_y - p_a.m_y) / 3);
		
		p_e.m_x = (int)((p_c.m_x + p_d.m_x)*0.5 + (p_d.m_y - p_c.m_y)*0.8660254);
        p_e.m_y = (int)((p_c.m_y + p_d.m_y)*0.5 - (p_d.m_x - p_c.m_x)*0.8660254);
		
		vonKoch(p_affichage, p_iteration - 1, p_a, p_c);
		vonKoch(p_affichage, p_iteration - 1, p_c, p_e);
		vonKoch(p_affichage, p_iteration - 1, p_e, p_d);
		vonKoch(p_affichage, p_iteration - 1, p_d, p_b);
}

}

int main(int argc, char **argv)
{
	point a, b, c ;
	SDL_Surface * window;
	int iterations = 0;
	int quit = 0;
	enum { dessin1, dessin2, dessin3, rendu } state = dessin1;
	SDL_Event e;
	Uint8 mouseButton;
	point mousePos;

	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "ERREUR - SDL_Init\n>>> %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	
	atexit(SDL_Quit);

	window = SDL_SetVideoMode(1300, 750, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

	if (window == NULL) {
		fprintf(stderr, "ERREUR - impossible de passer en : %dx%dx%d\n>>> %s\n", 640, 480, 32, SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_WM_SetCaption("VonKoch", NULL);

	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE)
					quit = 1;
				if (e.key.keysym.sym == SDLK_RIGHT)
					iterations++;
				if (e.key.keysym.sym == SDLK_LEFT)
				{
					iterations--;
					iterations = iterations >= 0 ? iterations : 0;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouseButton = SDL_GetMouseState(&mousePos.m_x, &mousePos.m_y);

				switch (state)
				{
				case dessin1:
					a.m_x = mousePos.m_x;
					a.m_y = mousePos.m_y;
					state = dessin2;
					break;
				case dessin2:
					b.m_x = mousePos.m_x;
					b.m_y = mousePos.m_y;
					state = dessin3;
					break;
				case dessin3:
					c.m_x = mousePos.m_x;
					c.m_y = mousePos.m_y;
					state = rendu;
					break;
				case rendu:
				default:
					a.m_x = mousePos.m_x;
					a.m_y = mousePos.m_y;
					state = dessin2;
					iterations = 0;
					break;
				}

				break;
			}
		}

		if (state == rendu)
		{
			
			SDL_FillRect(window, &(window->clip_rect), SDL_MapRGB(window->format, 0, 0, 0));

			vonKoch(window, iterations, a, b);
			vonKoch(window, iterations, b, c);
			vonKoch(window, iterations, c, a);

			
			SDL_UpdateRect(window, 0, 0, 0, 0);
			
			SDL_Flip(window);
		}

	}

	return EXIT_SUCCESS;
}