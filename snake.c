#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* ============================================================
   CONFIG
   ============================================================ */
#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 400

/* ============================================================
   GLOBALS
   ============================================================ */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* backbuffer = NULL;
static uint32_t* backbuffer_memory = NULL;
static int running = 1;

/* ============================================================
   RENDERER
   ============================================================ */
void clear_screen(uint32_t color)
{
    for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++)
        backbuffer_memory[i] = color;
}

void draw_rect(int x, int y, int w, int h, uint32_t color)
{
    for (int py = 0; py < h; py++)
    {
        int sy = y + py;
        if (sy < 0 || sy >= WINDOW_HEIGHT) continue;

        for (int px = 0; px < w; px++)
        {
            int sx = x + px;
            if (sx < 0 || sx >= WINDOW_WIDTH) continue;

            backbuffer_memory[sy * WINDOW_WIDTH + sx] = color;
        }
    }
}

/* ============================================================
   BACKBUFFER
   ============================================================ */
int init_backbuffer(void)
{
    backbuffer_memory = (uint32_t*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));
    if (!backbuffer_memory)
    {
        printf("Failed to allocate backbuffer memory\n");
        return 0;
    }

    backbuffer = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    if (!backbuffer)
    {
        printf("Failed to create texture: %s\n", SDL_GetError());
        free(backbuffer_memory);
        return 0;
    }

    return 1;
}

void flip_backbuffer(void)
{
    SDL_UpdateTexture(backbuffer, NULL, backbuffer_memory, WINDOW_WIDTH * sizeof(uint32_t));
    SDL_RenderCopy(renderer, backbuffer, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void cleanup(void)
{
    if (backbuffer_memory)
        free(backbuffer_memory);
    if (backbuffer)
        SDL_DestroyTexture(backbuffer);
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

/* ============================================================
   MAIN LOOP
   ============================================================ */
int main(int argc, char* argv[])
{
    /* Initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    /* Create window */
    window = SDL_CreateWindow(
        "Snake",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    /* Create renderer */
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        cleanup();
        return 1;
    }

    /* Initialize backbuffer */
    if (!init_backbuffer())
    {
        cleanup();
        return 1;
    }

    /* ============================================================
       YOUR GAME VARIABLES GO HERE
       ============================================================ */


    int snake_x = 200;
    int snake_y = 200;
    int direction = 1;   // 1 - UP    2 - RIGHT    3 - DOWN    4 - LEFT

    int berry_x = 300;
    int berry_y = 300;

    int snakebody_x[400]={snake_x+20, snake_x+40, snake_x+60};
    int snakebody_y[400]={snake_y, snake_y, snake_y};

    int segments=2;
    int turn=0;
    int pause=0;

    


    SDL_Event event;
    const uint8_t* keyboard_state;

    /* ============================================================
       MAIN GAME LOOP
       ============================================================ */
    while (running)
    {
        /* Handle events */
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = 0;
                }
            }
        }

        /* Get keyboard state */
        keyboard_state = SDL_GetKeyboardState(NULL);

        /* ===== INPUT HANDLING ===== */
        if ((keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDL_SCANCODE_UP] || keyboard_state[SDL_SCANCODE_I]) && direction!=3)
        {
            direction=1;
        }
        if ((keyboard_state[SDL_SCANCODE_S] || keyboard_state[SDL_SCANCODE_DOWN] || keyboard_state[SDL_SCANCODE_K]) && direction!=1)
        {
            direction=3;
        }
        if ((keyboard_state[SDL_SCANCODE_D] || keyboard_state[SDL_SCANCODE_RIGHT] || keyboard_state[SDL_SCANCODE_L]) && direction!=4)
        {
            direction=2;
        }
        if ((keyboard_state[SDL_SCANCODE_A] || keyboard_state[SDL_SCANCODE_LEFT] || keyboard_state[SDL_SCANCODE_J]) && direction!=2)
        {
            direction=4;
        }
        
        
        if (keyboard_state[SDL_SCANCODE_SPACE])
        {
          if(pause==0){pause=1;}
          else{pause=0;}
        }
        
        if(pause==0){

          /* Boundary checking */
          if(direction==1){snake_y=snake_y-20;}
          if(direction==3){snake_y=snake_y+20;}
          if(direction==2){snake_x=snake_x+20;}
          if(direction==4){snake_x=snake_x-20;}

          if(snake_x>380){snake_x=0;}
          if(snake_y>380){snake_y=0;}
          if(snake_x<0){snake_x=380;}
          if(snake_y<0){snake_y=380;}

          if(snake_x == berry_x && snake_y == berry_y){
              berry_x = ((rand() % 19) + 1) * 20;
              berry_y = ((rand() % 19) + 1) * 20;
              segments++;
              snakebody_x[segments]=999;
          }
          for(int i = 0; i <= segments; i++){
              if(snake_x == snakebody_x[i] && snake_y == snakebody_y[i]){
                  
                  // GAME OVER

                  snake_x = 200;
                  snake_y = 200;
                  direction = 1;   // 1 - UP    2 - RIGHT    3 - DOWN    4 - LEFT

                  berry_x = 300;
                  berry_y = 300;

                  for(int i = 0; i <= segments; i++)
                  {
                      snakebody_x[i] = 0; snakebody_y[i] = 0;
                  }
                  snakebody_x[0] = snake_x + 20;  snakebody_y[0] = snake_y;
                  snakebody_x[1] = snake_x + 40;  snakebody_y[1] = snake_y;
                  snakebody_x[2] = snake_x + 60;  snakebody_y[2] = snake_y;

                  segments=2;
                  turn=0;
                  break;

              }
              
          }

          snakebody_x[turn] = snake_x;
          snakebody_y[turn] = snake_y;
          turn++;

          if (turn > segments){turn=0;}
        }

        /* ===== RENDERING ===== */
        clear_screen(0xFF87CEEB); /* Light blue background */
        
        /* Draw player as green rectangle */
        draw_rect(snake_x, snake_y, 20, 20, 0xFF00FF00);
        for(int i = 0; i <= segments; i++){
            draw_rect(snakebody_x[i], snakebody_y[i], 20, 20, 0xFF00FF00);
        }
        draw_rect(berry_x, berry_y, 20, 20, 0xFFFF0000);
        
        /* Flip the backbuffer to the screen */
        flip_backbuffer();

        /* Frame timing (optional) */
        SDL_Delay(100); /* ~60 FPS */
    }

    /* Cleanup and exit */
    cleanup();
    return 0;
}
