
/* Program Name : Image Filtering on RGB bitmap (.bmp) (256*256)
 * Program Description: Applies various filters on a given bitmap (.bmp) (256*256)
 * Author: Yusuf Badr | Student at Faculty of Computers and Artificial Intelligence, Cairo University
 * Email: yusufbadr@yahoo.com
 * LinkedIn: linkedin.com/in/yusufbadr/
 */

#include <iostream>
#include <cstring>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "bmplib.cpp"
#include <algorithm>
// note GS = grey scale
using namespace std;
unsigned char image[SIZE][SIZE][RGB];
unsigned char image2[SIZE][SIZE][RGB];
unsigned char imageGS[SIZE][SIZE];

// load the image file requested by the user
void loadImageRGB(){
    char imageFilename[100];
    cout << "Please enter the source image file name (without extension):\n";
    cin.getline(imageFilename, 100);
    strcat(imageFilename, ".bmp");
    readRGBBMP(imageFilename, image);
}
// save the new image according to the target filename entered by user
void saveImageRGB(){
    char targetFilename[100];
    cout << "Please enter the target file name:\n";
    cin.getline(targetFilename, 100);
    strcat(targetFilename, ".bmp");
    writeRGBBMP(targetFilename, image);
}



// converts the RGB source image into a grayscale(GS) image using the weighted method
void RGBToGS() {
    double GSlevel;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            GSlevel = double(0.299) * image[i][j][0] + double(0.587) * image[i][j][1] +
                      double(0.114) * image[i][j][2];
            imageGS[i][j] = GSlevel;
        }
    }
}


// filter 1 - converts the RGB source image into a black and white image
void RGBToBlackAndWhite(){
    RGBToGS();
    // calculating average number of greyscale intensity in all pixels
    int sumOfGreyscale = 0;
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            sumOfGreyscale += imageGS[i][j];
        }
    }
    int averageGreyscale = sumOfGreyscale / (SIZE * SIZE);

    /* converting each pixel to black or white depending on the comparison of
     * the pixels greyscale intensity number to the average greyscale intensity number */
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            if (imageGS[i][j] >= averageGreyscale){
                imageGS[i][j] = 255;
            } else {
                imageGS[i][j] = 0;
            }
        }
    }

    // copying contents of imageGS to RGB image

    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            for (int k = 0; k < 3; k++) {
                image[i][j][k] = imageGS[i][j];
            }
        }
    }
}

// filter 4 flip image horizontally or vertically
void flipVerticalRGB(){
    for (int col = 0; col < SIZE; col++){ // for every column
        int y = 127; int z = 128; // y and z here correspond to the row numbers
        for (int a = 0; a < SIZE / 2; a++){ // 128 iterations
            for (int k = 0; k < 3; k++){
                unsigned char tempPixel = image[y][col][k];
                image [y][col][k] = image[z][col][k];
                image [z][col][k] = tempPixel;
            }
            y--; z++;
        }
    }
}

void flipHorizontalRGB(){
    for (int row = 0; row < SIZE; row++){ // for every row
        int y = 127; int z = 128; // y and z here correspond to the column numbers
        for (int a = 0; a < SIZE / 2; a++){ // 128 iterations
            for (int k = 0; k < 3; k++) {
                unsigned char tempPixel = image[row][y][k];
                image[row][y][k] = image[row][z][k];
                image[row][z][k] = tempPixel;
            }
            y--; z++;
        }
    }
}
// filter 4 flip image horizontally or vertically
void flipImageRGB(){
    string flipType;
    cout << "Please chose (v)ertical or (h)orizontal flip:\n";
    getline(cin, flipType); // defensive not done yet // assume input either V v H h only

    if (flipType == "v" || flipType == "V"){
        flipVerticalRGB();
    } else {
        flipHorizontalRGB();
    }
}



void leftMirrorRGB(){
    for (int row = 0; row < SIZE; row++){ // for every row
        int y = 127; int z = 128; // y and z here correspond to the column numbers
        for (int a = 0; a < SIZE / 2; a++){ // 128 iterations
            for (int k = 0; k < 3; k++) {
                image[row][z][k] = image[row][y][k];
            }
            y--; z++;
        }
    }
}

void rightMirrorRGB(){
    for (int row = 0; row < SIZE; row++){ // for every row
        int y = 127; int z = 128; // y and z here correspond to the column numbers
        for (int a = 0; a < SIZE / 2; a++){ // 128 iterations
            for (int k = 0; k < 3; k++) {
                image[row][y][k] = image[row][z][k];
            }
            y--; z++;
        }
    }
}

void upperMirrorRGB(){
    for (int col = 0; col < SIZE; col++){ // for every column
        int y = 127; int z = 128; // y and z here correspond to the row numbers
        for (int a = 0; a < SIZE / 2; a++){ // 128 iterations
            for (int k = 0; k < 3; k++) {
                image[z][col][k] = image[y][col][k];
            }
            y--; z++;
        }
    }
}

void lowerMirrorRGB(){
    for (int col = 0; col < SIZE; col++){ // for every column
        int y = 127; int z = 128; // y and z here correspond to the row numbers
        for (int a = 0; a < SIZE / 2; a++){ // 128 iterations
            for (int k = 0; k < 3; k++) {
                image[y][col][k] = image[z][col][k];
            }
            y--; z++;
        }
    }
}




void detectImageEdgesRGB(){
    RGBToGS();

    // assigning all pixels of image2 to white
    for (int row = 0; row < SIZE; row++){ // for every row
        for (int col = 0; col < SIZE; col++){ // for every column
            image2[row][col][0] = 255;
            image2[row][col][1] = 255;
            image2[row][col][2] = 255;
        }
    }


    // x direction - for detecting vertical edges
    int xDir [3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
    };
    // y direction - for detecting horizontal edges
    int yDir [3][3] = {
            {-1, -2, -1},
            {0, 0, 0},
            {1, 2, 1}
    };

    // row3x3 and col3x3 correspond to the row and column numbers of a 3x3 "slice" taken from the complete photo matrix,
    // such that their values are either 0, 1 or 2.
    // imageRow and imageCol correspond to the row and column numbers of the complete photo matrix
    for (int imageRow = 1; imageRow <= 254; imageRow++){
        for (int imageCol = 1; imageCol <= 254; imageCol++){
            // for the pixel  image[imageRow][imageCol]
            int Gx = 0, Gy = 0;
            for (int row3x3 = imageRow - 1; row3x3 <= imageRow + 1; row3x3++){
                for (int col3x3 = imageCol-1; col3x3 <= imageCol + 1; col3x3++){
                    Gx += imageGS[row3x3][col3x3] * xDir[row3x3 - (imageRow - 1)][col3x3 - (imageCol-1)];
                    Gy += imageGS[row3x3][col3x3] * yDir[row3x3 - (imageRow - 1)][col3x3 - (imageCol-1)];
                }
            }
            int gradient = sqrt(pow(Gx, 2) + pow(Gy, 2));
            if (gradient >= 255){
                image2[imageRow][imageCol][0] = 0;
                image2[imageRow][imageCol][1] = 0;
                image2[imageRow][imageCol][2] = 0;

            }else {
                image2[imageRow][imageCol][0] = 255 - gradient;
                image2[imageRow][imageCol][1] = 255 - gradient;
                image2[imageRow][imageCol][2] = 255 - gradient;
            }
        }
    }

    // copying contents of image2 to image
    for (int imageRow = 0; imageRow < SIZE; imageRow++){ // for every image row
        for (int imageCol = 0; imageCol < SIZE; imageCol++){ // for every image column
            image[imageRow][imageCol][0] = image2[imageRow][imageCol][0];
            image[imageRow][imageCol][1] = image2[imageRow][imageCol][1];
            image[imageRow][imageCol][2] = image2[imageRow][imageCol][2];

        }
    }
}

int main() {
    loadImageRGB();
    cout << "\n\n(1) RGB to Black and White\n"
         << "(2) Flip Image\n"
         << "(3) Left Mirror\n"
         << "(4) Right Mirror\n"
         << "(5) Upper Miror\n"
         << "(6) Lower Mirror\n"
         << "(7) Detect Image Edges\n";
    cout << "Please enter a number to choose the required filter\n\n";

    string choice;
    getline(cin, choice);
    switch (choice[0]) {
        case '1':
            RGBToBlackAndWhite();
        case '2':
            flipImageRGB();
        case '3':
            leftMirrorRGB();
        case '4':
            rightMirrorRGB();
        case '5':
            upperMirrorRGB();
        case '6':
            lowerMirrorRGB();
        case '7':
            detectImageEdgesRGB();
    }
    saveImageRGB();
    return 0;
}
