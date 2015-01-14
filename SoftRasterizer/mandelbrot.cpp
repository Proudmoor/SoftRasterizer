#include <iostream>
#include <complex>
#include <SDL2/SDL.h>

const int 					MAX_FPS 	= 60;
int 						MAX_ITER 	= 100,
    						WIDTH		= 640,
    						HEIGHT		= 480,
    						Ox			= WIDTH/2 - 1,
    						Oy 			= HEIGHT/2 - 1,
    						X_MAX 		= WIDTH - 1,
    						Y_MAX 		= HEIGHT - 1;

double 						scale 		= 240.0,
							x_center	= -0.67,
							y_center	= 0.0;

const std::complex<double> 	i_unit		(0, 1);
bool    					is_grid 	= true; 

void grid(SDL_Renderer* r)
{
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);

    // Draw OX and OY axes if they are visible
    if (std::abs(x_center) < 0.5 * WIDTH / scale && 
    	std::abs(y_center) < 0.5 * HEIGHT / scale)
    {
    	SDL_SetRenderDrawColor(r, 255, 255, 255, 100);
    	// OY axis
    	SDL_RenderDrawLine(r, (int) (1.0 * Ox - scale * x_center), 0, (int) (1.0 * Ox - scale * x_center), Y_MAX);
    	// OX axis
    	SDL_RenderDrawLine(r, 0, (int) (1.0 * Oy - scale * y_center), X_MAX, (int) (1.0 * Oy - scale * y_center));
    }
}

void mandelbrot(SDL_Renderer* r)
{
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            bool                    use_p = true;
            
            std::complex<double>    p = (((x - Ox) / scale) + x_center) + (((y - Oy) / scale) + y_center) * i_unit,
            						z0 = 0.0,
                                    z1;
            double                  distance;
            int                     iterations = 0;

            for (; iterations < MAX_ITER; iterations++)
            {
                z1  = z0 * z0 + p;

                if (z1.real() * z1.real() + z1.imag() * z1.imag() >= 4.0)
                {
                    use_p = false;
                    break;
                }
                else
                    z0  = z1;
            }

            if (use_p)
            {
                SDL_SetRenderDrawColor(r, 200, 0, 0, 255);
            }
            else
            {
               	double nsmooth = iterations + 1 - std::log( std::log( std::abs(z1) ) ) / std::log(2.0);
               	if (iterations < MAX_ITER / 2)
                	SDL_SetRenderDrawColor(	r, 
                						(int) (255 * 2.0 * iterations / MAX_ITER), 
                						40,
                						(int) (130 * (MAX_ITER - 2.0 * iterations) / MAX_ITER), 
                						255);
                else 
                	SDL_SetRenderDrawColor(	r, 
                						150, (int) (255 * 0.5 * iterations / MAX_ITER), 
                						(int) (20 * (MAX_ITER - 0.5 * iterations) / MAX_ITER), 
                						255);
            }
            SDL_RenderDrawPoint(r, x, y);
        }
    }
    if (is_grid)
        grid(r);
    
    SDL_RenderPresent(r);
    SDL_RenderClear(r);
}

int main(int argc, char * arg[])
{
    // init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "ERROR SDL_Init" << std::endl;

        return -1;
    }
    // create a window
    SDL_Window * window; 
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Zbiór Mandelbrota");

    bool    running 	= true,
            is_rendered = false;
    int 	poz 		= 300;
    SDL_Event input;

    while (running) {
        Uint32 start = SDL_GetTicks();

        SDL_PollEvent(&input);

        if (input.key.keysym.sym == SDLK_ESCAPE || input.key.keysym.sym == SDLK_q || input.type == SDL_QUIT)
            running = false;
        else if (input.type == SDL_KEYDOWN) {
            std::cout << (char)input.key.keysym.sym << std::endl;

            if (input.key.keysym.sym == SDLK_w)
                scale += 100;
            if (input.key.keysym.sym == SDLK_s)
                scale -= 100;

            if (input.key.keysym.sym == SDLK_LEFT)
                x_center -= 0.1 * WIDTH / scale;
            if (input.key.keysym.sym == SDLK_RIGHT)
                x_center += 0.1 * WIDTH / scale;
            if (input.key.keysym.sym == SDLK_UP)
                y_center -= 0.1 * HEIGHT / scale;
            if (input.key.keysym.sym == SDLK_DOWN)
                y_center += 0.1 * HEIGHT / scale;

            if (input.key.keysym.sym == SDLK_t)
                is_grid = !is_grid;

            if (input.key.keysym.sym == SDLK_e)
            {
            	x_center 	= -0.67;
            	y_center 	= 0.0;
            	scale 		= 240;
            }
            else if (input.key.keysym.sym == SDLK_r)
            {
            	x_center 	= 0.3;
            	y_center 	= 0.0;
            	scale 		= 10 * WIDTH;
            }
            else if (input.key.keysym.sym == SDLK_y)
            {
            	x_center	= -0.75;
            	y_center	= 0.1;
            	scale 		= 10 * WIDTH;
            }

            is_rendered = false;
        }
        if (!is_rendered)
        {
            mandelbrot(renderer);
            is_rendered = true;
        }
  
        if(1000/MAX_FPS > SDL_GetTicks()-start) {
            SDL_Delay(1000/MAX_FPS-(SDL_GetTicks()-start));
        }     
    }

    // clean up
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}
