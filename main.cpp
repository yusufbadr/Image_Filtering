
/* Program Name : Image Filtering on Greyscale bitmap (.bmp) (256*256)
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
unsigned char image[SIZE][SIZE];
unsigned char image2[SIZE][SIZE];

// load the image file requested by the user
void loadImage(){
    char imageFilename[100];
    cout << "Please enter the source image file name (without the extension):\n";
    cin.getline(imageFilename, 100);
    strcat(imageFilename, ".bmp");
    readGSBMP(imageFilename, image);

}
// save the new image according to the target filename entered by user
void saveImage(){
    char targetFilename[100];
    cout << "Please enter the target file name:\n";
    cin.getline(targetFilename, 100);
    strcat(targetFilename, ".bmp");
    writeGSBMP(targetFilename, image);
}

// filter 1: converts the source image into a black and white image
void GSToBlackAndWhite(){
    // calculating average number of greyscale intensity in all pixels
    int sumOfGreyscale = 0;
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            sumOfGreyscale += image[i][j];
        }
    }
    int averageGreyscale = sumOfGreyscale / (SIZE * SIZE);

    /* converting each pixel to black or white depending on the comparison of
     * the pixels greyscale intensity number to the average greyscale intensity number */
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            if (image[i][j] >= averageGreyscale){
                image[i][j] = 255;
            } else {
                image[i][j] = 0;
            }
        }
    }
}


void flipVertical(){
    for (int col = 0; col < SIZE; col++){ // for every column
        int y = 127; int z = 128; // y and z here correspond to the row numbers
        for (int a = 0; a < SIZE / 2; a++){ // 128 iterations
            unsigned char tempPixel = image[y][col];
            image [y][col] = image[z][col];
            image [z][col] = tempPixel;
            y--; z++;
        }
    }
}

void flipHorizontal(){
    for (int row = 0; row < SIZE; row++){ // for every row
        int y = 127; int z = 128; // y and z here correspond to the column numbers
        for (int a = 0; a < SIZE / 2; a++){ // 128 iterations
            unsigned char tempPixel = image[row][y];
            image [row][y] = image[row][z];
            image [row][z] = tempPixel;
            y--; z++;
        }
    }
}

// filter 4 flip image horizontally or vertically
void flipImage(){
    string flipType;
    cout << "Please chose (v)ertical or (h)orizontal flip:\n";
    getline(cin, flipType); // defensive not done yet // assume input either V v H h only

    if (flipType == "v" || flipType == "V"){
        flipVertical();
    } else {
        flipHorizontal();
    }
}



void leftMirror(){
    for (int row = 0; row < SIZE; row++){ // for every row
        int y = 127; int z = 128; // y and z here correspond to the column numbers
        for (int a = 0; a < SIZE / 2; a++){ // 128 iterations
            image [row][z] = image[row][y];
            y--; z++;
        }
    }
}

void rightMirror(){
    for (int row = 0; row < SIZE; row++){ // for every row
        int y = 127; int z = 128; // y and z here correspond to the column numbers
        for (int a = 0; a < SIZE / 2; a++){ // 128 iterations
            image [row][y] = image[row][z];
            y--; z++;
        }
    }
}

void upperMirror(){
    for (int col = 0; col < SIZE; col++){ // for every column
        int y = 127; int z = 128; // y and z here correspond to the row numbers
        for (int a = 0; a < SIZE / 2; a++){ // 128 iterations
            image [z][col] = image[y][col];
            y--; z++;
        }
    }
}

void lowerMirror(){
    for (int col = 0; col < SIZE; col++){ // for every column
        int y = 127; int z = 128; // y and z here correspond to the row numbers
        for (int a = 0; a < SIZE / 2; a++){ // 128 iterations
            image [y][col] = image[z][col];
            y--; z++;
        }
    }
}

// detect image edges done using Sobel operator for edge detection
void detectImageEdges(){

    // assigning all pixels of image2 to white
    for (int row = 0; row < SIZE; row++){ // for every row
        for (int col = 0; col < SIZE; col++){ // for every column
            image2[row][col] = 255;
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
                    Gx += image[row3x3][col3x3] * xDir[row3x3 - (imageRow - 1)][col3x3 - (imageCol-1)];
                    Gy += image[row3x3][col3x3] * yDir[row3x3 - (imageRow - 1)][col3x3 - (imageCol-1)];
                }
            }
            int gradient = sqrt(pow(Gx, 2) + pow(Gy, 2));
            if (gradient >= 255){
                image2[imageRow][imageCol] = 0;

            }else {
                image2[imageRow][imageCol] = 255 - gradient;
            }
        }
    }

    // copying contents of image2 to image
    for (int imageRow = 0; imageRow < SIZE; imageRow++){ // for every image row
        for (int imageCol = 0; imageCol < SIZE; imageCol++){ // for every image column
            image[imageRow][imageCol] = image2[imageRow][imageCol];
        }
    }
}

int main() {
    loadImage();
    cout << "\n\n(1) Greyscale to Black and White\n"
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
            GSToBlackAndWhite();
        case '2':
            flipImage();
        case '3':
            leftMirror();
        case '4':
            rightMirror();
        case '5':
            upperMirror();
        case '6':
            lowerMirror();
        case '7':
            detectImageEdges();
    }
    saveImage();
    return 0;
}
