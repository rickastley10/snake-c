#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

/* ============================================================
   CONFIG
   ============================================================ */
#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 400

/* ============================================================
   GLOBALS
   ============================================================ */
static Display* display = NULL;
static Window window;
static GC gc = NULL;
static Visual* visual = NULL;
static int screen = 0;
static XImage* ximage = NULL;
static uint32_t* backbuffer_memory = NULL;
static int running = 1;

/* Key state tracking (mimics SDL_GetKeyboardState) */
/* We map the keys we care about to indices. */
enum {
    KEY_W = 0, KEY_UP, KEY_I,
    KEY_S, KEY_DOWN, KEY_K,
    KEY_D, KEY_RIGHT, KEY_L,
    KEY_A, KEY_LEFT, KEY_J,
    KEY_SPACE,
    KEY_COUNT
};

static int keyboard_state[KEY_COUNT] = {0};

/* Map an X11 KeySym to one of our key indices, or -1 if not tracked. */
static int keysym_to_index(KeySym ks)
{
    switch (ks)
    {
        case XK_w: case XK_W: return KEY_W;
        case XK_Up:           return KEY_UP;
        case XK_i: case XK_I: return KEY_I;
        case XK_s: case XK_S: return KEY_S;
        case XK_Down:         return KEY_DOWN;
        case XK_k: case XK_K: return KEY_K;
        case XK_d: case XK_D: return KEY_D;
        case XK_Right:        return KEY_RIGHT;
        case XK_l: case XK_L: return KEY_L;
        case XK_a: case XK_A: return KEY_A;
        case XK_Left:         return KEY_LEFT;
        case XK_j: case XK_J: return KEY_J;
        case XK_space:        return KEY_SPACE;
        default:              return -1;
    }
}

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

    /* Create an XImage that wraps our backbuffer memory.
       We use 32-bit depth, and we'll use ZPixmap so each pixel
       is written directly into our buffer. */
    ximage = XCreateImage(
        display,
        visual,
        DefaultDepth(display, screen),
        ZPixmap,
        0,
        (char*)backbuffer_memory,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        32,
        0
    );

    if (!ximage)
    {
        printf("Failed to create XImage\n");
        free(backbuffer_memory);
        return 0;
    }

    return 1;
}

void flip_backbuffer(void)
{
    /* Push our backbuffer to the window */
    XPutImage(display, window, gc, ximage,
              0, 0, 0, 0,
              WINDOW_WIDTH, WINDOW_HEIGHT);
    XFlush(display);
}

void cleanup(void)
{
    /* Note: XDestroyImage frees the data pointer too, which is
       backbuffer_memory. So don't double-free. */
    if (ximage)
    {
        /* Prevent XDestroyImage from freeing our memory twice if
           we want to free manually; simplest is to let XDestroyImage
           free it and null out our pointer. */
        backbuffer_memory = NULL;
        XDestroyImage(ximage);
    }
    else if (backbuffer_memory)
    {
        free(backbuffer_memory);
    }

    if (gc)
        XFreeGC(display, gc);
    if (window)
        XDestroyWindow(display, window);
    if (display)
        XCloseDisplay(display);
}

/* Helper: current time in milliseconds */
static long long now_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000LL + tv.tv_usec / 1000LL;
}

/* ============================================================
   MAIN LOOP
   ============================================================ */
int main(int argc, char* argv[])
{
    /* Initialize X11 */
    display = XOpenDisplay(NULL);
    if (!display)
    {
        printf("Failed to open X display\n");
        return 1;
    }

    screen = DefaultScreen(display);
    visual = DefaultVisual(display, screen);

    /* Create window */
    window = XCreateSimpleWindow(
        display,
        RootWindow(display, screen),
        0, 0,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0,
        BlackPixel(display, screen),
        BlackPixel(display, screen)
    );

    if (!window)
    {
        printf("Window creation failed\n");
        XCloseDisplay(display);
        return 1;
    }

    XStoreName(display, window, "Snake");
    XSelectInput(display, window,
                 ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask);

    /* Center-ish placement */
    {
        int scr_w = DisplayWidth(display, screen);
        int scr_h = DisplayHeight(display, screen);
        XMoveWindow(display, window,
                    (scr_w - WINDOW_WIDTH) / 2,
                    (scr_h - WINDOW_HEIGHT) / 2);
    }

    XMapWindow(display, window);

    /* Create graphics context */
    gc = XCreateGC(display, window, 0, NULL);
    if (!gc)
    {
        printf("GC creation failed\n");
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

    


    XEvent event;

    /* ============================================================
       MAIN GAME LOOP
       ============================================================ */
    long long last_move_time = now_ms();
    const long long MOVE_INTERVAL_MS = 100;

    while (running)
    {
        /* Handle events */
        while (XPending(display))
        {
            XNextEvent(display, &event);

            if (event.type == ClientMessage)
            {
                /* WM_DELETE_WINDOW */
                running = 0;
            }
            else if (event.type == KeyPress)
            {
                KeySym ks = XLookupKeysym(&event.xkey, 0);
                if (ks == XK_Escape)
                {
                    running = 0;
                }
                int idx = keysym_to_index(ks);
                if (idx >= 0) keyboard_state[idx] = 1;
            }
            else if (event.type == KeyRelease)
            {
                KeySym ks = XLookupKeysym(&event.xkey, 0);
                int idx = keysym_to_index(ks);
                if (idx >= 0) keyboard_state[idx] = 0;
            }
        }

        /* ===== INPUT HANDLING ===== */
        if ((keyboard_state[KEY_W] || keyboard_state[KEY_UP] || keyboard_state[KEY_I]) && direction!=3)
        {
            direction=1;
        }
        if ((keyboard_state[KEY_S] || keyboard_state[KEY_DOWN] || keyboard_state[KEY_K]) && direction!=1)
        {
            direction=3;
        }
        if ((keyboard_state[KEY_D] || keyboard_state[KEY_RIGHT] || keyboard_state[KEY_L]) && direction!=4)
        {
            direction=2;
        }
        if ((keyboard_state[KEY_A] || keyboard_state[KEY_LEFT] || keyboard_state[KEY_J]) && direction!=2)
        {
            direction=4;
        }
        
        
        if (keyboard_state[KEY_SPACE])
        {
          if(pause==0){pause=1;}
          else{pause=0;}
          keyboard_state[KEY_SPACE] = 0; /* consume so it doesn't toggle every frame */
        }

        /* Frame timing: only advance game logic every MOVE_INTERVAL_MS */
        long long current_time = now_ms();
        if (current_time - last_move_time >= MOVE_INTERVAL_MS)
        {
            last_move_time = current_time;

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
        usleep(1000); /* small sleep to avoid busy-looping on events; game speed is governed by MOVE_INTERVAL_MS */
    }

    /* Cleanup and exit */
    cleanup();
    return 0;
}
