#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>


// Key press surface constants
enum KeyPressSurface{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

// Init window & surface
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gHelloWorld= NULL;
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
SDL_Surface* gCurrentSurface = NULL;

SDL_Surface* loadSurface( std::string path){

    SDL_Surface* optimizedSurface = NULL;

    // Load image at an specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL ){
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else{
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        if( optimizedSurface == NULL ){
            printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        SDL_FreeSurface( loadedSurface );
    }
    return optimizedSurface;
}

// Starts up SDL and creates a window
bool init(){
    // Initialization flag
    bool sucess = true;

    // Initialize SDL
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        sucess = false;
    }
    else{
        // Create window
        gWindow = SDL_CreateWindow("First Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if( gWindow == NULL ){
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            sucess = false;
        }
        else{
            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) ){
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                sucess = false;
            } 
            else{
                 // Get window surface
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }   
        }
    }
    return sucess;
}

// Loads media
bool load_media(){
    // Loading sucess flag
    bool sucess = true;

    // Load media
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface("src/press.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT] == NULL ){
        printf("Unable to load image assets/up.bmp! SDL Error: %s\n", SDL_GetError());
        sucess = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface("src/up.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL ){
        printf("Unable to load image assets/up.bmp! SDL Error: %s\n", SDL_GetError());
        sucess = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface("src/down.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL ){
        printf("Unable to load image assets/down.bmp! SDL Error: %s\n", SDL_GetError());
        sucess = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface("src/left.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL ){
        printf("Unable to load image assets/left.bmp! SDL Error: %s\n", SDL_GetError());
        sucess = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface("src/right.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL ){
        printf("Unable to load image assets/right.bmp! SDL Error: %s\n", SDL_GetError());
        sucess = false;
    }

    return sucess;
}

// Frees media and shut down SDL
void close(){
    //Deallocate surface
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

int main (int argc, char* args[] ){
    // Start up SDL and window
    if( !init()){
        printf("Could not initialize SDL\n");
    }
    else{
        // Load media
        if( !load_media()){
            printf("Could not load media\n");
        }
        else{
            // Apply image
            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];

            // Update surface
            SDL_UpdateWindowSurface(gWindow);
            //Hack to get window to stay up
            SDL_Event e; 
            bool quit = false; 
            while( quit == false ){
                // Handle events on queue
                 while( SDL_PollEvent( &e ) != 0){
                    // User requests quit
                    if( e.type == SDL_QUIT ) {
                        quit = true;
                    }
                    else if(e.type == SDL_KEYDOWN){

                        // Select surface based on key pressed
                        switch( e.key.keysym.sym ){
                            case SDLK_UP:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                                break;
                            case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                                break;
                            case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                                break;
                            case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                                break;
                            default:
                                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                                break;
                        }
                    }
                } 
                //Apply the current image
                SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );
            
                //Update the surface
                SDL_UpdateWindowSurface( gWindow );
            }
        }
    
    }
    // Free media and quit SDL
    close();

    return 0;
}