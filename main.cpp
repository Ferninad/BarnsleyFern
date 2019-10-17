#include "common.h"
#include "cmath"

bool Init();
void CleanUp();
void Run();
double ScaleNum(double n, double minN, double maxN, double min, double max);
void BarnsleyFern();
void Draw();

SDL_Window *window;
SDL_GLContext glContext;
SDL_Surface *gScreenSurface = nullptr;
SDL_Renderer *renderer = nullptr;

int screenWidth = 500;
int screenHeight = 500;
double x = 0;
double y = 0;

bool Init()
{
    if (SDL_Init(SDL_INIT_NOPARACHUTE & SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        //Specify OpenGL Version (4.2)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_Log("SDL Initialised");
    }

    //Create Window Instance
    window = SDL_CreateWindow(
        "Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,   
        SDL_WINDOW_OPENGL);

    //Check that the window was succesfully created
    if (window == NULL)
    {
        //Print error, if null
        printf("Could not create window: %s\n", SDL_GetError());
        return false;
    }
    else
        gScreenSurface = SDL_GetWindowSurface(window);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_Log("Window Successful Generated");

    //Map OpenGL Context to Window
    glContext = SDL_GL_CreateContext(window);

    return true;
}

int main()
{
    //Error Checking/Initialisation
    if (!Init())
    {
        printf("Failed to Initialize");
        return -1;
    }

    // Clear buffer with black background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Swap Render Buffers
    SDL_GL_SwapWindow(window);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Run();

    CleanUp();
    return 0;
}

void CleanUp()
{
    //Free up resources
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Run()
{
    bool gameLoop = true;
    srand(time(NULL));

    while (gameLoop)
    {   
        Draw();
        SDL_RenderPresent(renderer);
        
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                gameLoop = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    gameLoop = false;
                    break;
                default:
                    break;
                }
            }

            if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym)
                {
                default:
                    break;
                }
            }
        }
    }
}

void BarnsleyFern() {
    double px = ScaleNum(x, -2.1820, 2.6558, 0, screenWidth);
    double py = ScaleNum(y, 0, 9.9983, screenHeight, 0);
    SDL_Rect pos;
    pos.x = px;
    pos.y = py;
    pos.w = 1;
    pos.h = 1;
    SDL_RenderFillRect(renderer, &pos);

    double nextX;
    double nextY;
    double r = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);

    if (r < 0.01) {
        nextX = 0;
        nextY = 0.16 * y;
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    } else if (r < 0.86) {
        nextX = 0.85 * x + 0.04 * y;
        nextY = -0.04 * x + 0.85 * y + 1.60;
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    } else if (r < 0.93) {
        nextX = 0.20 * x + -0.26 * y;
        nextY = 0.23 * x + 0.22 * y + 1.60;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else {
        nextX = -0.15 * x + 0.28 * y;
        nextY = 0.26 * x + 0.24 * y + 0.44;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }

    x = nextX;
    y = nextY;
}

void Draw() {
    for (int i = 0; i < 1000; i++) {
        BarnsleyFern();
    }
}

double ScaleNum(double n, double minN, double maxN, double min, double max){
    return (((n - minN) / (maxN - minN)) * (max - min)) + min;
}