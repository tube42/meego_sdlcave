#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "SDL_gfxPrimitives.h"


#define ACCEL 0.4
#define NL 100
#define NC 20

#define IC(i) (i+NC) % NC
#define IL(i) (i+NL) % NL

double obtenir_meilleur_score()
{
#ifndef WIN
	FILE *sc;
	double score;
	char l_score[50];

        sprintf(l_score, "~/.sdlcave_score");
	sc = fopen(l_score, "r");
	if(sc == NULL)
	{
		printf("Cannot open %s.\n", l_score);
		score = 0;
	} 
	else 
	{
		fscanf(sc, "%lf", &score);
		fclose(sc);
	}
	return score;
#else
	return 0;
#endif
}

void maj_meilleur_score(float score)
{
#ifndef WIN
	FILE *sc;
	double a_score;
	char l_score[50];
    
	sprintf(l_score, "~/.sdlcave_score");

	a_score = obtenir_meilleur_score();

	if(score > a_score)
	{
		sc = fopen(l_score, "w");
		if(sc == NULL) 
		{
			printf("Error while trying to write to %s\n", l_score);
			return;
		}
		fprintf(sc, "%lf", score);
		fclose(sc);
	} 
#else
#endif
}

#undef main

int main(int argc, char **argv)
{
	double ligne[NL];
	double allure_cave[NC][2];
	double epaisseur_cave;
	double vz;
	double vx;
	double yo;
	double accel;
	double a_score;
	int ticks, wticks;
	unsigned char *police;
	FILE *police_f;

	double score;
	int pos_ligne;
	int pos_cave;
	double x, lx, xt, dt;
	int i, i_image, i_niveau, niveau, j, ym, xm, yM, xM, couleur;

	double echelle;
	char m_score[200];
	signed char mode;

	SDL_Surface *ecran;
	SDL_Event ev;

    srand(time(NULL));
#ifdef WIN32    
        police_f = fopen("10x20.fnt", "rb");    
#else
        police_f = fopen("/opt/sdlcave/10x20.fnt", "rb");    
#endif
	if(police_f != NULL)
	{
		police = malloc(10240);
		fread(police, 1, 10240, police_f);
		gfxPrimitivesSetFont(police, 10, 20);
                printf("LOADED font: %x\n", police);

	} else printf("Error opening font file. Will be using the default 8x8 font.\n");


	pos_ligne = 0;
	pos_cave = NC-1;
	x = 0;
	epaisseur_cave = 0.5;
	accel = ACCEL;
	lx = 0;
	vz = 0;
	vx = 0.5;
	i_image = 0;
	i_niveau = 0;
	niveau = 1;
	mode = 0;
	dt = 0.01;
	score = 0;
	a_score = obtenir_meilleur_score();

        printf("INIT...\n"); //DEBUG
	SDL_Init(SDL_INIT_VIDEO);
    
        printf("SET MODE...\n"); //DEBUG    
#ifdef WIN32
	ecran = SDL_SetVideoMode(850, 480, 0, SDL_HWSURFACE);    
#else
	ecran = SDL_SetVideoMode(0, 0, 16, SDL_HWSURFACE | SDL_FULLSCREEN);    
#endif
        printf("SET MODE done...\n"); //DEBUG

	SDL_ShowCursor(0);
	echelle = ecran->h;

	allure_cave[0][0] = 0;
	allure_cave[0][1] = 0;

	for(i=1; i<NC; i++)
	{
		allure_cave[i][0] = allure_cave[i-1][0]+0.2;
		allure_cave[i][1] = 0.3*(double)rand()/RAND_MAX;
	}



	ticks = SDL_GetTicks();
	wticks = SDL_GetTicks() + 20;

        printf("GAME LOOP...\n"); //DEBUG
	while(1)
	{


		if(x>lx)
		{
			lx += + 0.2;
			allure_cave[pos_cave][0] = allure_cave[IC(pos_cave-1)][0]+0.2;
			allure_cave[pos_cave][1] = 0.3*(double)rand()/RAND_MAX;
			pos_cave = IC(pos_cave + 1);
		}

		if(mode == 1)
		{
			vz += accel*dt;
			ligne[pos_ligne] = ligne[IL(pos_ligne-1)]+vz*dt;
			score += vx*vx*dt*epaisseur_cave/epaisseur_cave*100;
			epaisseur_cave *= 0.9999;
	//		vx *= 1.0001;
			dt *= 1.0001; // Better : makes the entire game faster, so it is still "possible" to survive at high speeds
		}
		x += vx*dt;
		ticks = SDL_GetTicks();
		if(i_image == 3)
			if(ticks < wticks)
			{

				if(mode == 1)
				{
					xt = 0.4;
					for(i=pos_ligne; i>pos_ligne-NL+1; i--) 
					{
						couleur = 100+300*(dt/0.01-1);
						if(couleur > 255) couleur = 255;
	
						for(j=-1; j<=1; j++)
							lineRGBA(ecran, xt*echelle, echelle*ligne[IL(i)]+j, (xt-vx*dt)*echelle, echelle*ligne[IL(i-1)]+j, couleur, 255-couleur, 255-couleur, 255); // Ligne de trois pixels de large
						xt -= vx*dt;
					}
				}
		
				couleur = 150*(0.5/epaisseur_cave-1);
				if(couleur > 255) couleur = 255;
				for(i=pos_cave+1; i<pos_cave + NC; i++)
				{
					if(allure_cave[IC(i-1)][0]>allure_cave[IC(i)][0]) continue;
					yo = 0.5-epaisseur_cave/2-0.15;

					if((yo+allure_cave[IC(i-1)][1]) > (allure_cave[IC(i)][1]+yo)) 
					{
						ym = echelle*(allure_cave[IC(i)][1]+yo);
						yM = echelle*(yo+allure_cave[IC(i-1)][1]+epaisseur_cave);
						xm = echelle*(allure_cave[IC(i-1)][0]-x);
						xM = echelle*(allure_cave[IC(i)][0]-x);
					} 
					else
					{
						ym = echelle*(yo+allure_cave[IC(i-1)][1]);
						yM = echelle*(allure_cave[IC(i)][1]+yo+epaisseur_cave);
						xm = echelle*(allure_cave[IC(i)][0]-x);
						xM = echelle*(allure_cave[IC(i-1)][0]-x);
					}

					filledTrigonRGBA(ecran, echelle*(allure_cave[IC(i-1)][0]-x), echelle*(yo+allure_cave[IC(i-1)][1]), xm, ym, echelle*(allure_cave[IC(i)][0]-x), echelle*(allure_cave[IC(i)][1]+yo), 139-couleur*139/255, 69+couleur*186/255, 19+couleur*236/255, 255);
					boxRGBA(ecran, echelle*(allure_cave[IC(i-1)][0]-x), 0, echelle*(allure_cave[IC(i)][0]-x), ym, 139-couleur*139/255, 69+couleur*186/255, 19+couleur*236/255, 255);
					
					filledTrigonRGBA(ecran, echelle*(allure_cave[IC(i-1)][0]-x), echelle*(allure_cave[IC(i-1)][1]+yo+epaisseur_cave), xM, yM, echelle*(allure_cave[IC(i)][0]-x), echelle*(allure_cave[IC(i)][1]+yo+epaisseur_cave), 139-couleur*139/255, 69+couleur*186/255, 19+couleur*236/255, 255);
					boxRGBA(ecran, echelle*(allure_cave[IC(i-1)][0]-x), echelle, echelle*(allure_cave[IC(i)][0]-x), yM, 139-couleur*139/255, 69+couleur*186/255, 19+couleur*236/255, 255);
				}
				i_image = 0;
				sprintf(m_score, "Score : %.0f (H: %.0f) - Width : %.2f - Speed : %.2f", score, a_score, epaisseur_cave, 1e2*dt);
				stringRGBA(ecran, 5, 5, m_score, 255, 255, 255, 255);
				if(mode == 2)
				{
					sprintf(m_score, "Game over. Your score is : %.0f.", score);
					stringRGBA(ecran, ecran->w/2-150, ecran->h/8, m_score, 255, 0, 0, 255);
					if(score > a_score) stringRGBA(ecran, ecran->w/2-53, ecran->h/8+22, "High score !", 0, 0, 255, 255);
					boxRGBA(ecran, 3*ecran->w/8, 3*ecran->h/8, ecran->w/2, 5*ecran->h/8, 100, 255, 100, 255);
					boxRGBA(ecran, ecran->w/2, 3*ecran->h/8, 5*ecran->w/8, 5*ecran->h/8, 255, 100, 100, 255);
					stringRGBA(ecran, 7*ecran->w/16-50, ecran->h/2-10, "Play again", 0, 0, 0, 255);
					stringRGBA(ecran, 9*ecran->w/16-20, ecran->h/2-10, "Quit", 0, 0, 0, 255);
				}
				if(mode == 0) 
				{
					stringRGBA(ecran, ecran->w/2-125, ecran->h/4+25, "Touch the screen to start.", 0, 255, 0, 255);
					stringRGBA(ecran, ecran->w/2-32, ecran->h/4-10, "SDLCave", 0, 0, 255, 255);
				}
				SDL_Flip(ecran);
				SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

				ticks = SDL_GetTicks();
				if(ticks < wticks) SDL_Delay(wticks-ticks);
				wticks = SDL_GetTicks()+20;
			}
		i_image++;

		for(i=0; i<NC; i++)
		{
			if(allure_cave[IC(i)][0] < x+0.4 && allure_cave[IC(i+1)][0] > x+0.4)
			{
				yo = 0.5-0.15-epaisseur_cave/2 + 
					allure_cave[IC(i)][1] + 
					(allure_cave[IC(i+1)][1]-allure_cave[IC(i)][1]) / 
					(allure_cave[IC(i+1)][0]-allure_cave[IC(i)][0]) * 
					(x+0.4-allure_cave[IC(i)][0]);
				if((ligne[pos_ligne] < yo || ligne[pos_ligne] > yo+epaisseur_cave) && mode == 1) {mode = 2; maj_meilleur_score(score); vx = 0; SDL_ShowCursor(1); SDL_WarpMouse(0, 0);}
				break;
			}
		}

                while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_MOUSEBUTTONDOWN: switch(mode)
							  {
								  case 0: mode = -1; break;
								  case 1: accel = -3*ACCEL; break;
							} break;
				case SDL_MOUSEBUTTONUP: switch(mode)
							{
								case 2: 
									SDL_GetMouseState(&xm, &ym); 
									if(xm > 3*ecran->w/8 && xm <= ecran->w/2 && ym >= 3*ecran->h/8 && ym <= 5*ecran->h/8) mode = -1; 
									if(xm > ecran->w/2 && xm <= 5*ecran->w/8 && ym >= 3*ecran->h/8 && ym <= 5*ecran->h/8) {SDL_Quit(); exit(0);}; 
								case 1: accel = ACCEL; break;
							} break;
				case SDL_KEYDOWN:
						if(mode == 2) switch(ev.key.keysym.sym)
						{
							case SDLK_q: SDL_Quit(); exit(0); break;
                                                case SDLK_r:  mode = -1;
                                                    default: ;
						}
                            break;

                        case SDL_QUIT: SDL_Quit(); exit(0);
                        default:
                            ;
			}
		}

		if(mode == 1)
		{
			pos_ligne++;
			if(pos_ligne >= NL) pos_ligne -= NL;
		}
		
		if(mode == -1)
		{
			for(i=0; i<NL; i++) ligne[i] = 0.5;
			a_score = (score > a_score ? score: a_score); 
			score = 0;
			pos_ligne = 0;
			vx = 0.5;
			dt = 0.01;
			epaisseur_cave = 0.5;
			mode = 1;
			vz = 0;
			SDL_ShowCursor(0);
		}



		if(x >= 10)
		{
			for(i=0; i<NC; i++)
				allure_cave[i][0] -= 10;
			x -= 10;
			lx -= 10;
		}
		


	}

	return 0;
}



