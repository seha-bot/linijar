#include<gl_utils.h>
#include<math.h>

#define WIDTH 250
#define HEIGHT 250
#define HW 125
#define HH 125

float p[2];
int r = 270;
float trig[2][360];
#define SIN(a) trig[0][a]
#define COS(a) trig[1][a]

#define GW 10
#define GH 10
int walls[GW][GH] = {
    1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,1,
    1,0,0,1,0,0,1,0,0,1,
    1,0,0,0,0,0,0,0,0,1,
    1,0,1,0,0,0,0,1,0,1,
    1,0,1,1,1,1,1,1,0,1,
    1,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,
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

float t_ray[2];
char col = 0;
void ray(int a, int c)
{
    if(!c)
    {
        // t_ray[0] = p[0]+HEIGHT;
        // t_ray[1] = p[1]+HEIGHT;
        return;
    }
    if(a < 0) a += 360;
    if(a > 359) a -= 360;
    if(a < 180) col = 5;
    else col = 8;
    t_ray[0] += COS(a);
    t_ray[1] += SIN(a);
    int x = GW / (WIDTH / t_ray[0]);
    int y = GH / (HEIGHT / t_ray[1]);
    if(x < 0 || y < 0 || x >= GW || y >= GH) return;
    if(!walls[GH-1-y][x]) ray(a, c - 1);
}

void draw2D()
{
    int xp = GW / (WIDTH / p[0]);
    int yp = GH / (HEIGHT / p[1]);
    for(int x = 0; x < GW; x++)
    {
        for(int y = 0; y < GH; y++)
        {
            box(x*(WIDTH/GW), y*(HEIGHT/GH), (WIDTH/GW), (HEIGHT/GH), (x == xp && y == yp) ? 2 : 0);
        }
    }

    for(int x = 0; x < GW; x++)
    {
        for(int y = 0; y < GH; y++)
        {
            if(x == xp && y == yp) continue;
            box(x*(WIDTH/GW), y*(HEIGHT/GH), (WIDTH/GW), (HEIGHT/GH), walls[GH-1-y][x]);
        }
    }

    int fov = 60;
    for(int i = 0; i < fov; i++)
    {
        t_ray[0] = p[0];
        t_ray[1] = p[1];
        ray(r+i-(fov>>1), 300);
        line(p[0], p[1], t_ray[0], t_ray[1], col);
    }
    pixel(p[0], p[1], 2);
}

void draw3D()
{
    int fov = 60;
    for(int i = 0; i < fov; i++)
    {
        t_ray[0] = p[0];
        t_ray[1] = p[1];
        ray(r+i-(fov>>1), 300);
        t_ray[0] -= p[0];
        t_ray[1] -= p[1];
        float ray = 1/Q_rsqrt(t_ray[0]*t_ray[0]+t_ray[1]*t_ray[1]);
        // ray = min(ray*3, HW);
        // ray = ray/2;
        box(WIDTH-i*(WIDTH/fov), ray, (WIDTH/fov), HEIGHT-ray*2, col);
    }
}

int loop()
{
    if(getKey(GLFW_KEY_W)) { p[0] += COS(r); p[1] += SIN(r); }
    if(getKey(GLFW_KEY_S)) { p[0] -= COS(r); p[1] -= SIN(r); }
    if(getKey(GLFW_KEY_D)) { p[0] += SIN(r); p[1] -= COS(r); }
    if(getKey(GLFW_KEY_A)) { p[0] -= SIN(r); p[1] += COS(r); }
    if(getKey(GLFW_KEY_RIGHT)) { r-=5; if(r < 0) r += 360; }
    if(getKey(GLFW_KEY_LEFT)) { r+=5; if(r > 359) r -= 360; }

    // if(getKey(GLFW_KEY_TAB)) draw2D();
    // else draw3D();
    draw2D();
    return 0;
}

int main(int argc, char** argv)
{
    createWindow("Window", WIDTH<<1, HEIGHT<<1);
    if(!window) return 1;
    glPointSize(2.0);
    
    p[0] = p[1] = HH + 10;
    for(int i = 0; i < 360; i++)
    {
        trig[0][i] = sin(i*M_PI/180);
        trig[1][i] = cos(i*M_PI/180);
    }

    start(loop);
    return 0;
}
