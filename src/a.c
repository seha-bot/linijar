#include<gl_utils.h>
#include<math.h>

#define WIDTH 250
#define HEIGHT 250
#define HW 125
#define HH 125

float p[2];
int r = 45;
float trig[2][360];

#define GW 10
#define GH 10
int walls[GW][GH] = {
    1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,1,0,0,
    0,0,0,1,1,1,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
};

void box(int x, int y, int w, int h, char c)
{
    for(int i = x; i < x+w; i++)
    {
        for(int j = y; j < y+h; j++)
        {
            pixel(i, j, c);
        }
    }
}

void draw2D()
{
    for(int x = 0; x < GW; x++)
    {
        for(int y = 0; y < GH; y++)
        {
            box(x*(WIDTH/GW), y*(HEIGHT/GH), (WIDTH/GW), (HEIGHT/GH), walls[GH-1-y][x]);
        }
    }
    pixel(p[0], p[1], 2);
}

#define SIN(a) trig[0][a]
#define COS(a) trig[1][a]
int off = 0;

int loop()
{
    if(getKey(GLFW_KEY_D)) { off=r+45; if(off>359)off-=360; p[0] += SIN(off); p[1] += COS(off); }
    if(getKey(GLFW_KEY_A)) { off=r+45; if(off>359)off-=360; p[0] -= SIN(off); p[1] -= COS(off); }
    if(getKey(GLFW_KEY_W)) { p[0] += SIN(r) - COS(r); p[1] += SIN(r) + COS(r); }
    if(getKey(GLFW_KEY_S)) { p[0] -= SIN(r) - COS(r); p[1] -= SIN(r) + COS(r); }

    if(getKey(GLFW_KEY_LEFT)) { r-=5; if(r < 0) r += 360; }
    if(getKey(GLFW_KEY_RIGHT)) { r+=5; if(r > 359) r -= 360; }

    draw2D();
    return 0;
}

int main(int argc, char** argv)
{
    createWindow("Window", WIDTH<<1, HEIGHT<<1);
    if(!window) return 1;
    glPointSize(2.0);
    
    p[0] = p[1] = HH;
    for(int i = 0; i < 360; i++)
    {
        trig[0][i] = sin(i*M_PI/180);
        trig[1][i] = cos(i*M_PI/180);
    }

    start(loop);
    return 0;
}
