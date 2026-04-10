#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>      // Added for sprintf
#include <time.h>       // Added for time()
#include <string.h>     // Added for strlen()

/* ============================================================
   CONFIG
   ============================================================ */
#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 400

/* ============================================================
   GLOBALS
   ============================================================ */
static HBITMAP backbuffer_bitmap;
static HDC backbuffer_dc;
static void* backbuffer_memory;
static BITMAPINFO backbuffer_info;
static int running = 1; /* 1 = running, 0 = exit */

/* ============================================================
   RENDERER
   ============================================================ */
void clear_screen(uint32_t color)
{
    int i;
    uint32_t* pixel = (uint32_t*)backbuffer_memory;
    for (i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++)
        pixel[i] = color;
}

void draw_rect(int x, int y, int w, int h, uint32_t color)
{
    int px, py, sx, sy;
    for (py = 0; py < h; py++)
    {
        sy = y + py;
        if (sy < 0 || sy >= WINDOW_HEIGHT) continue;

        for (px = 0; px < w; px++)
        {
            sx = x + px;
            if (sx < 0 || sx >= WINDOW_WIDTH) continue;

            ((uint32_t*)backbuffer_memory)[sy * WINDOW_WIDTH + sx] = color;
        }
    }
}

/* ============================================================
   WINDOW
   ============================================================ */
LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CLOSE || msg == WM_DESTROY)
    {
        running = 0;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/* ============================================================
   BACKBUFFER
   ============================================================ */
void init_backbuffer(HWND hwnd)
{
    HDC window_dc;
    window_dc = GetDC(hwnd);
    backbuffer_dc = CreateCompatibleDC(window_dc);

    /* Initialize bitmap info */
    memset(&backbuffer_info, 0, sizeof(backbuffer_info));
    backbuffer_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    backbuffer_info.bmiHeader.biWidth = WINDOW_WIDTH;
    backbuffer_info.bmiHeader.biHeight = -WINDOW_HEIGHT; /* top-down */
    backbuffer_info.bmiHeader.biPlanes = 1;
    backbuffer_info.bmiHeader.biBitCount = 32;
    backbuffer_info.bmiHeader.biCompression = BI_RGB;

    backbuffer_bitmap = CreateDIBSection(
        window_dc,
        &backbuffer_info,
        DIB_RGB_COLORS,
        &backbuffer_memory,
        NULL,
        0
    );

    SelectObject(backbuffer_dc, backbuffer_bitmap);
    ReleaseDC(hwnd, window_dc);
}

/* ============================================================
   MAIN LOOP
   ============================================================ */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prev, LPSTR cmd, int show)
{
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;
    HDC window_dc;

    int rect_x, rect_y;

    /* Register window class */
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = window_proc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "snake";
    RegisterClass(&wc);

    /* Create window */
    hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        "snake",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    init_backbuffer(hwnd);

    srand(time(NULL));   // Seed once at program start

    int w = 20; int h = 20;
    /*
    400x400
    200 = center / 0
    150 = -50
    0 = -200
    400 = 200
    */
    int px=200;int py=200;
    int s1x = 999;int s1y = 999;
    int s2x = 999;int s2y = 999;
    int s3x = 999;int s3y = 999;
    int s4x = 999;int s4y = 999;
    int s5x = 999;int s5y = 999;
    int s6x = 999;int s6y = 999;
    int s7x = 999;int s7y = 999;
    int s8x = 999;int s8y = 999;
    int s9x = 999;int s9y = 999;
    int s10x = 999;int s10y = 999;
    int s11x = 999;int s11y = 999;
    int s12x = 999;int s12y = 999;
    int s13x = 999;int s13y = 999;
    int s14x = 999;int s14y = 999;
    int s15x = 999;int s15y = 999;
    int s16x = 999;int s16y = 999;
    int s17x = 999;int s17y = 999;
    int s18x = 999;int s18y = 999;
    int s19x = 999;int s19y = 999;
	int s20x = 999;int s20y = 999;
    int s21x = 999; int s21y = 999;
    int s22x = 999; int s22y = 999;
    int s23x = 999; int s23y = 999;
    int s24x = 999; int s24y = 999;
    int s25x = 999; int s25y = 999;
    int s26x = 999; int s26y = 999;
    int s27x = 999; int s27y = 999;
    int s28x = 999; int s28y = 999;
    int s29x = 999; int s29y = 999;
    int s30x = 999; int s30y = 999;
    int s31x = 999; int s31y = 999;
    int s32x = 999; int s32y = 999;
    int s33x = 999; int s33y = 999;
    int s34x = 999; int s34y = 999;
    int s35x = 999; int s35y = 999;
    int s36x = 999; int s36y = 999;
    int s37x = 999; int s37y = 999;
    int s38x = 999; int s38y = 999;
    int s39x = 999; int s39y = 999;
    int s40x = 999; int s40y = 999;

    int level=1;
    int turn=1;
    char dir[9]="up";

    int bx=(rand()%20)*20;int by=(rand()%20)*20;
    
    /* ============================================================
       MAIN LOOP
       ============================================================ */
    while (running)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // ===== PADDLE CONTROLS =====
        if (GetAsyncKeyState('W') & 0x8000) {
            strcpy(dir, "up");
        }
        if (GetAsyncKeyState('S') & 0x8000) {
            strcpy(dir, "down");
        }
        if (GetAsyncKeyState('D') & 0x8000) {
            strcpy(dir, "right");
        }
        if (GetAsyncKeyState('A') & 0x8000) {
            strcpy(dir, "left");
        }

        /* Example draw */
        clear_screen(0xFF87CEEB); /* light blue background */
        /*
        red - 0xFFFF0000
        orange - 0xFFFFA500
        black - 0xFF000000
        */
        /*x y w h colour*/
             draw_rect(px, py, w, h, 0x00ff00);
             draw_rect(s1x, s1y, w, h, 0x00ff00);
             draw_rect(s2x, s2y, w, h, 0x00ff00);
             draw_rect(s3x, s3y, w, h, 0x00ff00);
             draw_rect(s4x, s4y, w, h, 0x00ff00);
             draw_rect(s5x, s5y, w, h, 0x00ff00);
             draw_rect(s6x, s6y, w, h, 0x00ff00);
             draw_rect(s7x, s7y, w, h, 0x00ff00);
             draw_rect(s8x, s8y, w, h, 0x00ff00);
             draw_rect(s9x, s9y, w, h, 0x00ff00);
             draw_rect(s10x, s10y, w, h, 0x00ff00);
             draw_rect(s11x, s11y, w, h, 0x00ff00);
             draw_rect(s12x, s12y, w, h, 0x00ff00);
             draw_rect(s13x, s13y, w, h, 0x00ff00);
             draw_rect(s14x, s14y, w, h, 0x00ff00);
             draw_rect(s15x, s15y, w, h, 0x00ff00);
             draw_rect(s16x, s16y, w, h, 0x00ff00);
             draw_rect(s17x, s17y, w, h, 0x00ff00);
             draw_rect(s18x, s18y, w, h, 0x00ff00);
             draw_rect(s19x, s19y, w, h, 0x00ff00);
             draw_rect(s20x, s20y, w, h, 0x00ff00);
	         draw_rect(s21x, s21y, w, h, 0x00ff00);
             draw_rect(s22x, s22y, w, h, 0x00ff00);
             draw_rect(s23x, s23y, w, h, 0x00ff00);
             draw_rect(s24x, s24y, w, h, 0x00ff00);
             draw_rect(s25x, s25y, w, h, 0x00ff00);
             draw_rect(s26x, s26y, w, h, 0x00ff00);
             draw_rect(s27x, s27y, w, h, 0x00ff00);
             draw_rect(s28x, s28y, w, h, 0x00ff00);
             draw_rect(s29x, s29y, w, h, 0x00ff00);
             draw_rect(s30x, s30y, w, h, 0x00ff00);
             draw_rect(s31x, s31y, w, h, 0x00ff00);
             draw_rect(s32x, s32y, w, h, 0x00ff00);
             draw_rect(s33x, s33y, w, h, 0x00ff00);
             draw_rect(s34x, s34y, w, h, 0x00ff00);
             draw_rect(s35x, s35y, w, h, 0x00ff00);
             draw_rect(s36x, s36y, w, h, 0x00ff00);
             draw_rect(s37x, s37y, w, h, 0x00ff00);
             draw_rect(s38x, s38y, w, h, 0x00ff00);
             draw_rect(s39x, s39y, w, h, 0x00ff00);
             draw_rect(s40x, s40y, w, h, 0x00ff00);
			
			
             draw_rect(bx, by, w, h, 0xff0000);


             if(px==bx && py == by){
                level= level+1;
                bx=(rand()%20)*20;by=(rand()%20)*20;
            }


            if(level >= 1 && turn==1){
                s1x=px;
                s1y=py;
                turn = turn + 1;
            }

            else if(level >= 2 && turn==2){
                s2x=px;
                s2y=py;
                turn = turn + 1;
            }
            
            else if(level >= 3 && turn==3){
                s3x=px;
                s3y=py;
                turn = turn + 1;
            }
            
            else if(level >= 4 && turn==4){
                s4x=px;
                s4y=py;
                turn = turn + 1;
            }
            
            else if(level >= 5 && turn==5){
                s5x=px;
                s5y=py;
                turn = turn + 1;
            }
            
            else if(level >= 6 && turn==6){
                s6x=px;
                s6y=py;
                turn = turn + 1;
            }
            
            else if(level >= 7 && turn==7){
                s7x=px;
                s7y=py;
                turn = turn + 1;
            }
            
            else if(level >= 8 && turn==8){
                s8x=px;
                s8y=py;
                turn = turn + 1;
            }
            
            else if(level >= 9 && turn==9){
                s9x=px;
                s9y=py;
                turn = turn + 1;
            }
            
            else if(level >= 10 && turn==10){
                s10x=px;
                s10y=py;
                turn = turn + 1;
            }
            else if(level >= 11 && turn==11){
                s11x=px;
                s11y=py;
                turn = turn + 1;
            }

            else if(level >= 12 && turn==12){
                s12x=px;
                s12y=py;
                turn = turn + 1;
            }
            
            else if(level >= 13 && turn==13){
                s13x=px;
                s13y=py;
                turn = turn + 1;
            }
            
            else if(level >= 14 && turn==14){
                s14x=px;
                s14y=py;
                turn = turn + 1;
            }
            
            else if(level >= 15 && turn==15){
                s15x=px;
                s15y=py;
                turn = turn + 1;
            }
            
            else if(level >= 16 && turn==16){
                s16x=px;
                s16y=py;
                turn = turn + 1;
            }
            
            else if(level >= 17 && turn==17){
                s17x=px;
                s17y=py;
                turn = turn + 1;
            }
            
            else if(level >= 18 && turn==18){
                s18x=px;
                s18y=py;
                turn = turn + 1;
            }
            
            else if(level >= 19 && turn==19){
                s19x=px;
                s19y=py;
                turn = turn + 1;
            }
            
            else if(level >= 20 && turn==20){
                s20x=px;
                s20y=py;
                turn = turn + 1;
            }
			else if(level >= 21 && turn==21){
                s21x=px;
                s21y=py;
                turn = turn + 1;
            }
            else if(level >= 22 && turn==22){
                s22x=px;
                s22y=py;
                turn = turn + 1;
            }
            else if(level >= 23 && turn==23){
                s23x=px;
                s23y=py;
                turn = turn + 1;
            }
            else if(level >= 24 && turn==24){
                s24x=px;
                s24y=py;
                turn = turn + 1;
            }
            else if(level >= 25 && turn==25){
                s25x=px;
                s25y=py;
                turn = turn + 1;
            }
            else if(level >= 26 && turn==26){
                s26x=px;
                s26y=py;
                turn = turn + 1;
            }
            else if(level >= 27 && turn==27){
                s27x=px;
                s27y=py;
                turn = turn + 1;
            }
            else if(level >= 28 && turn==28){
                s28x=px;
                s28y=py;
                turn = turn + 1;
            }
            else if(level >= 29 && turn==29){
                s29x=px;
                s29y=py;
                turn = turn + 1;
            }
            else if(level >= 30 && turn==30){
                s30x=px;
                s30y=py;
                turn = turn + 1;
            }
            else if(level >= 31 && turn==31){
                s31x=px;
                s31y=py;
                turn = turn + 1;
            }
            else if(level >= 32 && turn==32){
                s32x=px;
                s32y=py;
                turn = turn + 1;
            }
            else if(level >= 33 && turn==33){
                s33x=px;
                s33y=py;
                turn = turn + 1;
            }
            else if(level >= 34 && turn==34){
                s34x=px;
                s34y=py;
                turn = turn + 1;
            }
            else if(level >= 35 && turn==35){
                s35x=px;
                s35y=py;
                turn = turn + 1;
            }
            else if(level >= 36 && turn==36){
                s36x=px;
                s36y=py;
                turn = turn + 1;
            }
            else if(level >= 37 && turn==37){
                s37x=px;
                s37y=py;
                turn = turn + 1;
            }
            else if(level >= 38 && turn==38){
                s38x=px;
                s38y=py;
                turn = turn + 1;
            }
            else if(level >= 39 && turn==39){
                s39x=px;
                s39y=py;
                turn = turn + 1;
            }
            else if(level >= 40 && turn==40){
                s40x=px;
                s40y=py;
                turn = turn + 1;
            }
            else{
                turn=1;
                s1x=px;
                s1y=py;
                turn = turn + 1;
            }
            
            
            
            if(strcmp(dir, "up")==0){py=py-h;}
            if(strcmp(dir, "down")==0){py=py+h;}
            if(strcmp(dir, "right")==0){px=px+w;}
            if(strcmp(dir, "left")==0){px=px-w;}


            if(px<0){px=400;}
            if(px>400){px=0;}
            if(py<0){py=400;}
            if(py>400){py=0;}
            
if((s1x == px && s1y == py) ||
(s2x == px && s2y == py) ||
(s3x == px && s3y == py) ||
(s4x == px && s4y == py) ||
(s5x == px && s5y == py) ||
(s6x == px && s6y == py) ||
(s7x == px && s7y == py) ||
(s8x == px && s8y == py) ||
(s9x == px && s9y == py) ||
(s10x == px && s10y == py) ||
(s11x == px && s11y == py) ||
(s12x == px && s12y == py) ||
(s13x == px && s13y == py) ||
(s14x == px && s14y == py) ||
(s15x == px && s15y == py) ||
(s16x == px && s16y == py) ||
(s17x == px && s17y == py) ||
(s18x == px && s18y == py) ||
(s19x == px && s19y == py) ||
(s20x == px && s20y == py) ||
(s21x == px && s21y == py) ||
(s22x == px && s22y == py) ||
(s23x == px && s23y == py) ||
(s24x == px && s24y == py) ||
(s25x == px && s25y == py) ||
(s26x == px && s26y == py) ||
(s27x == px && s27y == py) ||
(s28x == px && s28y == py) ||
(s29x == px && s29y == py) ||
(s30x == px && s30y == py) ||
(s31x == px && s31y == py) ||
(s32x == px && s32y == py) ||
(s33x == px && s33y == py) ||
(s34x == px && s34y == py) ||
(s35x == px && s35y == py) ||
(s36x == px && s36y == py) ||
(s37x == px && s37y == py) ||
(s38x == px && s38y == py) ||
(s39x == px && s39y == py) ||
(s40x == px && s40y == py)){

	s1x = 999;
    s1y = 999;
    
    s2x = 999;
    s2y = 999;
    
    s3x = 999;
    s3y = 999;
    
    s4x = 999;
    s4y = 999;
    
    s5x = 999;
    s5y = 999;
    
    s6x = 999;
    s6y = 999;
    
    s7x = 999;
    s7y = 999;
    
    s8x = 999;
    s8y = 999;
    
    s9x = 999;
    s9y = 999;
    
    s10x = 999;
    s10y = 999;

    s11x = 999;
    s11y = 999;
    
    s12x = 999;
    s12y = 999;
    
    s13x = 999;
    s13y = 999;
    
    s14x = 999;
    s14y = 999;
    
    s15x = 999;
    s15y = 999;
    
    s16x = 999;
    s16y = 999;
    
    s17x = 999;
    s17y = 999;
    
    s18x = 999;
    s18y = 999;
    
    s19x = 999;
    s19y = 999;
    
    s20x = 999;
    s20y = 999;
	
	s21x = 999;
    s21y = 999;
    
    s22x = 999;
    s22y = 999;
    
    s23x = 999;
    s23y = 999;
    
    s24x = 999;
    s24y = 999;
    
    s25x = 999;
    s25y = 999;
    
    s26x = 999;
    s26y = 999;
    
    s27x = 999;
    s27y = 999;
    
    s28x = 999;
    s28y = 999;
    
    s29x = 999;
    s29y = 999;
    
    s30x = 999;
    s30y = 999;

    s31x = 999;
    s31y = 999;
    
    s32x = 999;
    s32y = 999;
    
    s33x = 999;
    s33y = 999;
    
    s34x = 999;
    s34y = 999;
    
    s35x = 999;
    s35y = 999;
    
    s36x = 999;
    s36y = 999;
    
    s37x = 999;
    s37y = 999;
    
    s38x = 999;
    s38y = 999;
    
    s39x = 999;
    s39y = 999;
    
    s40x = 999;
    s40y = 999;


    px = 200;
    py = 200;


    level=1;
    turn=1;
    strcpy(dir, "up");

    bx=(rand()%20)*20;
    by=(rand()%20)*20;

}
        /* Flip backbuffer */
        window_dc = GetDC(hwnd);
        BitBlt(window_dc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, backbuffer_dc, 0, 0, SRCCOPY);
        ReleaseDC(hwnd, window_dc);

        Sleep(100); // 100ms ~ 10 FPS (changed comment to match actual value)
    }

    // Cleanup
    DeleteDC(backbuffer_dc);
    DeleteObject(backbuffer_bitmap);

    return 0;
}