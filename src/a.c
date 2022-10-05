#include<gl_utils.h>
#include<math.h>

#define WIDTH 250
#define HEIGHT 250
#define HW 125
#define HH 125

float p[2];
int r = 90;
float trig[2][360];
#define SIN(a) trig[0][a]
#define COS(a) trig[1][a]

#define GS 10
int walls[GS][GS] = {
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
#define CW (WIDTH/GS)
#define CH (HEIGHT/GS)

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
void ray(int a)
{
    if(a < 0) a += 360;
    if(a > 359) a -= 360;
    float end[2];
    end[0] = p[0] + COS(a) * 1000.0;
    end[1] = p[1] + SIN(a) * 1000.0;
    t_ray[0] = p[0] + 1;
    t_ray[1] = p[1] + sqrt( 1 + (end[1]*end[1])/(end[0]*end[0]) );
}

void ray2(int a)
{
    if(a < 0) a += 360;
    if(a > 359) a -= 360;
    t_ray[0] = p[0] + COS(a) * 300.0;
    t_ray[1] = p[1] + SIN(a) * 300.0;
}

void draw2D()
{
    for(int x = 0; x < GS; x++)
    {
        for(int y = 0; y < GS; y++)
        {
            box(x*CW, y*CH, CW, CH, walls[GS-1-y][x]);
            box(x*CW, y*CH, 1, CH, 8);
            box(x*CW, y*CH, CH, 1, 8);
        }
    }

    int fov = 1;
    for(int i = 0; i < fov; i++)
    {
        ray(r+i-(fov>>1));
        line(p[0]*CW, p[1]*CH, t_ray[0]*CW, t_ray[1]*CH, 3);
        ray2(r+i-(fov>>1));
        line(p[0]*CW, p[1]*CH, t_ray[0]*CW, t_ray[1]*CH, 9);
    }
    pixel(p[0]*CW, p[1]*CH, 4);
}

int loop()
{
    float speed = 0.05;
    if(getKey(GLFW_KEY_W)) { p[0] += COS(r)*speed; p[1] += SIN(r)*speed; }
    if(getKey(GLFW_KEY_S)) { p[0] -= COS(r)*speed; p[1] -= SIN(r)*speed; }
    if(getKey(GLFW_KEY_D)) { p[0] += SIN(r)*speed; p[1] -= COS(r)*speed; }
    if(getKey(GLFW_KEY_A)) { p[0] -= SIN(r)*speed; p[1] += COS(r)*speed; }
    if(getKey(GLFW_KEY_RIGHT)) { r-=5; if(r < 0) r += 360; }
    if(getKey(GLFW_KEY_LEFT)) { r+=5; if(r > 359) r -= 360; }

    draw2D();
    return 0;
}

int main(int argc, char** argv)
{
    createWindow("Window", WIDTH<<1, HEIGHT<<1);
    if(!window) return 1;
    glPointSize(2.0);
    
    p[0] = p[1] = GS>>1;
    for(int i = 0; i < 360; i++)
    {
        trig[0][i] = sin(i*M_PI/180);
        trig[1][i] = cos(i*M_PI/180);
    }

    start(loop);
    return 0;
}
