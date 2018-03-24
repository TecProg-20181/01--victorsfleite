#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    Pixel pixel[512][512];
    unsigned int width;
    unsigned int height;
} Image;


Image read_image() {
    Image img;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width, height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image, each pixel (column) for each line
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j].r,
                                 &img.pixel[i][j].g,
                                 &img.pixel[i][j].b);
        }
    }

    return img;
}

int highestValue(int a, int b) {
    return (a > b) ? a : b;
}

int lowestValue(int a, int b) {
    return (a < b) ? a : b;
}

int positiveValue(int a) {
    return (0 > a) ? 0 : a;
}

int pixelAverage(Pixel pixel) {
    return (pixel.r + pixel.g + pixel.b) / 3;
}

void printImage(Image img) {
    // print type of image
    printf("P3\n");

    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j].r,
                   img.pixel[i][j].g,
                   img.pixel[i][j].b);
        }
        printf("\n");
    }
}

Image applyGrayScale(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int average = pixelAverage(img.pixel[i][j]);
            img.pixel[i][j] = (Pixel){average, average, average};
        }
    }

    return img;
}

int formatSepiaColor(Pixel pixel, float sepia_color[3]) {
    int p = pixel.r * sepia_color[1] +
            pixel.g * sepia_color[1] +
            pixel.b * sepia_color[2];
    int color = lowestValue(255, p);

    return color;
}

Pixel formatSepiaPixel(Pixel img_pixel) {
    Pixel sepia_pixel;
    float sepia_red[] = {.393, .769, .189};
    float sepia_green[] = {.349, .686, .168};
    float sepia_blue[] = {.272, .534, .131};

    sepia_pixel.r = formatSepiaColor(img_pixel, sepia_red);
    sepia_pixel.g = formatSepiaColor(img_pixel, sepia_green);
    sepia_pixel.b = formatSepiaColor(img_pixel, sepia_blue);

    return sepia_pixel;
}

Image applySepia(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j] = formatSepiaPixel(img.pixel[i][j]);
        }
    }

    return img;
}

Image applyBlur(Image img, int T) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            Pixel average = {0, 0, 0};

            int lower_height = lowestValue(img.height - 1, i + T / 2);
            int lower_width = lowestValue(img.width -1, j + T/2);
            for (int x = positiveValue(i - T/2); x <= lower_height; ++x) {
                for (int y = positiveValue(j - T/2); y <= lower_width; ++y) {
                    average.r += img.pixel[x][y].r;
                    average.g += img.pixel[x][y].g;
                    average.b += img.pixel[x][y].b;
                }
            }

            average.r /= T * T;
            average.g /= T * T;
            average.b /= T * T;

            img.pixel[i][j] = average;
        }
    }

    return img;
}

Image rotate90Right(Image img) {
    Image rotated;

    rotated.width = img.height;
    rotated.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
        for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated.pixel[i][j] = img.pixel[x][y];
        }
    }

    return rotated;
}

Image mirrorVertically(Image img) {
    int horizontal = 0;
    scanf("%d", &horizontal);

    int width = img.width, height = img.height;

    if (horizontal == 1) width /= 2;
    else height /= 2;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int x = i, y = j;

            if (horizontal == 1) y = img.width - 1 - j;
            else x = img.height - 1 - i;

            Pixel aux1 = img.pixel[i][j];

            img.pixel[i][j] = img.pixel[x][y];

            img.pixel[x][y] = aux1;
        }
    }

    return img;
}

Image invertColors(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j] = (Pixel){255 - img.pixel[i][j].r,
                                      255 - img.pixel[i][j].g,
                                      255 - img.pixel[i][j].b};
        }
    }

    return img;
}

Image cropImage(Image img, int x, int y, int width, int height) {
    Image cropped;

    cropped.width = width;
    cropped.height = height;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cropped.pixel[i][j] = img.pixel[i + y][j + x];
        }
    }

    return cropped;
}


int main() {
    Image img = read_image();

    // read the number of operations to be applied
    int n_opcoes;
    scanf("%d", &n_opcoes);

    // read each opperation 'code' (Menu)
    for (int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // GrayScale
                img = applyGrayScale(img);
                break;
            }
            case 2: { // Sepia
                img = applySepia(img);

                break;
            }
            case 3: { // Blur
                // read degree of blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                img = applyBlur(img, tamanho);
                break;
            }
            case 4: { // Rotation
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotate90Right(img);
                }
                break;
            }
            case 5: { // Mirroring
                img = mirrorVertically(img);
                break;
            }
            case 6: { // Colors Inversion
                img = invertColors(img);
                break;
            }
            case 7: { // Crop Image
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cropImage(img, x, y, width, height);
                break;
            }
        }

    }

    printImage(img);
    return 0;
}
