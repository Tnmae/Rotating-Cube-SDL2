#include<SDL2/SDL.h>
#include<iostream>
#include<math.h>

#define WIDTH 800
#define HEIGHT 600
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_GRAY 0xefefefef
#define COLOR_RED 0xeb4034
#define POINT_SIZE 1
#define COORDINATE_SYSTEM_OFFSET_X WIDTH/2
#define COORDINATE_SYSTEM_OFFSET_Y HEIGHT/2
#define SQUARE_SIDE_1 200
#define SQUARE_SIDE_2 160
#define SQUARE_SIDE_3 120
#define SQUARE_SIDE_4 80
#define SQUARE_SIDE_5 40
#define SQUARE_SIDE_6 240
#define ALPHA 0.01
#define BETA 0.01
#define GAMMA 0.02

struct Coordinates {
    double x;
    double y;
    double z;
};

// void draw_square(SDL_Surface* surface, SDL_Rect rect, unsigned int side_square) {
//     for ( double x=rect.x-side_square/2; x<=rect.x+side_square/2;x++) {
//         for (double y=rect.y-side_square/2; y<=rect.y+side_square/2; y++) {
//             if (x==rect.x-side_square/2 || x== rect.x+side_square/2 || y== rect.y-side_square/2 || y==rect.y+side_square/2) {
//                 SDL_Rect temp = { x, y, 1, 1};
//                 SDL_FillRect(surface, &temp, COLOR_WHITE);
//             }
//         }
//     } 
// };

void draw_points(SDL_Surface* surface, int x, int y, Uint32 color) {
    SDL_Rect rect= {x, y, POINT_SIZE, POINT_SIZE };
    SDL_FillRect(surface, &rect, color);
}

void draw_points_3d(SDL_Surface* surface, struct Coordinates* point, int number_of_points, Uint32 color) {
    for ( int i=0; i<number_of_points; i++) {
        double x_2d, y_2d;
        // if (event.type== SDL_MOUSEMOTION || event.motion.state== SDL_PRESSED) {
        x_2d= point[i].x + COORDINATE_SYSTEM_OFFSET_X;
        y_2d= point[i].y + COORDINATE_SYSTEM_OFFSET_Y;
        // }
        draw_points(surface, x_2d , y_2d, color);
    }
}

struct Coordinates* initialize_cube(struct Coordinates* points_of_cube, int number_of_points, int side_length) {
    // Coordinates* points= new Coordinates[number_of_points];
    //SIDE 1
    int points_per_side = number_of_points/12;
    int step_size= side_length/points_per_side;
    for ( int i=0; i<points_per_side; i++ ) {
        points_of_cube[i+0*points_per_side]= { -side_length/2+i*step_size, -side_length/2, side_length/2};
    }
    for ( int i= 0; i<points_per_side; i++) {
        points_of_cube[i+1*points_per_side]= { -side_length/2, -side_length/2+i*step_size, side_length/2};
    }
    for ( int i= 0; i<points_per_side; i++) {
        points_of_cube[i+2*points_per_side]= { -side_length/2+i*step_size, side_length/2, side_length/2};
    }
    for ( int i= 0; i<points_per_side; i++) {
        points_of_cube[i+3*points_per_side]= { side_length/2, -side_length/2+i*step_size, side_length/2};
    }
    for ( int i= 0; i<points_per_side; i++) {
        points_of_cube[i+4*points_per_side]= { -side_length/2, -side_length/2, -side_length/2+i*step_size};
    }
    for ( int i= 0; i<points_per_side; i++) {
        points_of_cube[i+5*points_per_side]= { side_length/2, -side_length/2, -side_length/2+i*step_size};
    }
    for ( int i= 0; i<points_per_side; i++) {
        points_of_cube[i+6*points_per_side]= { -side_length/2, side_length/2, -side_length/2+i*step_size};
    }
    for ( int i= 0; i<points_per_side; i++) {
        points_of_cube[i+7*points_per_side]= { side_length/2, side_length/2, -side_length/2+i*step_size};
    }
    for ( int i= 0; i<points_per_side; i++) {
        points_of_cube[i+8*points_per_side]= { -side_length/2+i*step_size, -side_length/2, -side_length/2};
    }
    for ( int i= 0; i<points_per_side; i++) {
        points_of_cube[i+9*points_per_side]= { -side_length/2, -side_length/2+i*step_size, -side_length/2}; 
    }
    for ( int i= 0; i<points_per_side; i++) {
        points_of_cube[i+10*points_per_side]={ -side_length/2+i*step_size, side_length/2, -side_length/2};
    }
    for ( int i= 0; i<points_per_side; i++) {
        points_of_cube[i+11*points_per_side]={ side_length/2, -side_length/2+i*step_size, -side_length/2};
    }
    return points_of_cube;
}

void rotate_cube(struct Coordinates* points_of_cube, int number_of_points ) {
    double rotation_matrix_y[3][3]= { { cos(BETA)*cos(GAMMA), -cos(BETA)*sin(GAMMA), sin(BETA) },
        {cos(ALPHA)*sin(GAMMA)+ sin(ALPHA)*sin(BETA)*cos(GAMMA), cos(ALPHA)*cos(GAMMA)-sin(ALPHA)*sin(BETA)*sin(GAMMA), -sin(ALPHA)*cos(BETA) },
        {sin(ALPHA)*sin(GAMMA)-cos(ALPHA)*sin(BETA)*cos(GAMMA), sin(ALPHA)*cos(GAMMA)+cos(ALPHA)*sin(BETA)*sin(GAMMA), cos(ALPHA)*cos(BETA) }
    };
    double result= 0;
    double result_arr[]= { 0, 0, 0};
    for ( int i=0; i< number_of_points; i++) {
        double points[3]= { points_of_cube[i].x, points_of_cube[i].y, points_of_cube[i].z};
        for (int j=0; j<3; j++) {
            result= 0;
            for (int k= 0; k<3; k++) {
                result= result + rotation_matrix_y[j][k]*points[k];
            }
            result_arr[j]= result;
        }
        points_of_cube[i].x= result_arr[0];
        points_of_cube[i].y= result_arr[1];
        points_of_cube[i].z= result_arr[2];
    }
}

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow( "Sample Program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Surface* surface = SDL_GetWindowSurface( window );

    SDL_Rect coord;
    coord.h=1;
    coord.w=1;
    SDL_Rect erase_rect= {0,0,900,600};

    Coordinates origin[1] = {0, 0, 0};
    // Coordinates point[1] = {-450, -300, 100};/
    unsigned int number_of_points_1= 12*SQUARE_SIDE_1/POINT_SIZE;
    unsigned int number_of_points_2= 12*SQUARE_SIDE_2/POINT_SIZE;
    unsigned int number_of_points_3= 12*SQUARE_SIDE_3/POINT_SIZE;
    unsigned int number_of_points_4= 12*SQUARE_SIDE_4/POINT_SIZE;
    unsigned int number_of_points_5= 12*SQUARE_SIDE_5/POINT_SIZE;
    unsigned int number_of_points_6= 12*SQUARE_SIDE_6/POINT_SIZE;
    Coordinates points_of_cube_1[number_of_points_1];
    Coordinates points_of_cube_2[number_of_points_2];
    Coordinates points_of_cube_3[number_of_points_3];
    Coordinates points_of_cube_4[number_of_points_4];
    Coordinates points_of_cube_5[number_of_points_5];
    Coordinates points_of_cube_6[number_of_points_6];
    initialize_cube(points_of_cube_1, number_of_points_1, SQUARE_SIDE_1);
    initialize_cube(points_of_cube_2, number_of_points_2, SQUARE_SIDE_2);
    initialize_cube(points_of_cube_3, number_of_points_3, SQUARE_SIDE_3);
    initialize_cube(points_of_cube_4, number_of_points_4, SQUARE_SIDE_4);
    initialize_cube(points_of_cube_5, number_of_points_5, SQUARE_SIDE_5);
    initialize_cube(points_of_cube_6, number_of_points_6, SQUARE_SIDE_6);

    bool status = true;

    SDL_Event event;
    int count= 0;
    while (status) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            status = false;
        }
        if (event.type == SDL_MOUSEMOTION && event.motion.state== SDL_PRESSED) {
            coord.x= event.motion.x;
            coord.y= event.motion.y;
        }
        SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
        // draw_points(surface, 200, 300, COLOR_WHITE);
        // draw_points_3d(surface, point, 1, COLOR_WHITE);
        rotate_cube(points_of_cube_1, number_of_points_1);
        rotate_cube(points_of_cube_2, number_of_points_2);
        rotate_cube(points_of_cube_3, number_of_points_3);
        rotate_cube(points_of_cube_4, number_of_points_4);
        rotate_cube(points_of_cube_5, number_of_points_5);
        rotate_cube(points_of_cube_6, number_of_points_6);
        draw_points_3d(surface, points_of_cube_1, number_of_points_1, COLOR_WHITE);
        draw_points_3d(surface, points_of_cube_2, number_of_points_2, COLOR_WHITE);
        draw_points_3d(surface, points_of_cube_3, number_of_points_3, COLOR_WHITE);
        draw_points_3d(surface, points_of_cube_4, number_of_points_4, COLOR_WHITE);
        draw_points_3d(surface, points_of_cube_5, number_of_points_5, COLOR_WHITE);
        draw_points_3d(surface, points_of_cube_6, number_of_points_6, COLOR_WHITE);
        // draw_square(surface, coord, COLOR_WHITE);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
        count++;
        std::cout<< count<< std::endl;
    }

    SDL_DestroyWindowSurface(window);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return EXIT_SUCCESS;
}