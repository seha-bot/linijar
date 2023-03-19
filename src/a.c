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

void box(int x, int y, int w, int h, u_int32_t c)
{
    x = clamp(x, 0, WIDTH);
    y = clamp(y, 0, HEIGHT);
    w = clamp(w, 0, WIDTH);
    h = clamp(h, 0, HEIGHT);
    for(int i = x; i < x+w; i++)
    {
        for(int j = y; j < y+h; j++)
        {
            pixel(i, j, c);
        }
    }
}

float t_ray[2];
float t_delta;
int t_side;
float ray(int a)
{
    if(a < 0) a += 360;
    if(a > 359) a -= 360;

    float rayDirX = COS(a);
    float rayDirY = SIN(a);

    float dX = sqrt( 1 + (rayDirY*rayDirY) / (rayDirX*rayDirX) );
    float dY = sqrt( 1 + (rayDirX*rayDirX) / (rayDirY*rayDirY) );

    int mapX = p[0];
    int mapY = p[1];
    int stepX, stepY;

    if(rayDirX < 0)
    {
        stepX = -1;
        t_ray[0] = (p[0] - mapX) * dX;
    }
    else
    {
        stepX = 1;
        t_ray[0] = (mapX + 1.0f - p[0]) * dX;
    }
    if(rayDirY < 0)
    {
        stepY = -1;
        t_ray[1] = (p[1] - mapY) * dY;
    }
    else
    {
        stepY = 1;
        t_ray[1] = (mapY + 1.0f - p[1]) * dY;
    }

    while(1)
    {
        if(t_ray[0] < t_ray[1])
        {
            mapX += stepX;
            t_side = 0;
            t_ray[0] += dX;
        }
        else
        {
            mapY += stepY;
            t_side = 1;
            t_ray[1] += dY;
        }
        if(mapX < 0 || mapY < 0 || mapX >= GS || mapY >= GS) break;
        if(walls[GS-1-mapY][mapX]) break;
    }

    t_delta = t_side ? (t_ray[1]-dY) : (t_ray[0]-dX);
    t_ray[0] = p[0] + rayDirX * t_delta;
    t_ray[1] = p[1] + rayDirY * t_delta;
}

void draw2D()
{
    for(int x = 0; x < GS; x++)
    {
        for(int y = 0; y < GS; y++)
        {
            box(x*CW, y*CH, CW, CH, walls[GS-1-y][x] ? 0xFFFFFFFF : 0);
            box(x*CW, y*CH, 1, CH, 0xAAAA00);
            box(x*CW, y*CH, CH, 1, 0xAAAA00);
        }
    }

    int fov = 60;
    for(int i = 0; i < fov; i++)
    {
        ray(r+i-(fov>>1));
        line(p[0]*CW, p[1]*CH, t_ray[0]*CW, t_ray[1]*CH, 0xFF0000);
    }
}

void draw3D()
{
    int fov = 60;
    int c;
    for(int i = 0; i < fov; i++)
    {
        ray(r+i-(fov>>1));
        if(t_delta < 1) c = 255<<16;
        else c = (int)(255 / t_delta) << 16;
        box(WIDTH-i*(WIDTH/(float)fov), HH-((1/t_delta)*HH), (WIDTH/(float)fov)+1, (1/t_delta*2)*HH, clamp(c + (0x4F0000*t_side),0,255<<16));
    }
}

int loop()
{
    float speed = 0.05;
    if(getKey(GLFW_KEY_W)) { p[0] += COS(r)*speed; p[1] += SIN(r)*speed; }
    if(getKey(GLFW_KEY_S)) { p[0] -= COS(r)*speed; p[1] -= SIN(r)*speed; }
    if(getKey(GLFW_KEY_D)) { r-=5; if(r < 0) r += 360; }
    if(getKey(GLFW_KEY_A)) { r+=5; if(r > 359) r -= 360; }

    if(getKey(GLFW_KEY_TAB)) draw2D();
    else draw3D();
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
