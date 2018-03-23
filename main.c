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


int maior(int a, int b) {
    return (a > b) ? a : b;
}

int menor(int a, int b) {
    return (a < b) ? a : b;
}

int positivo(int a) {
    return (0 > a) ? 0 : a;
}

int pixel_igual(Pixel p1, Pixel p2) {
    if (p1.r == p2.r &&
        p1.g == p2.g &&
        p1.b == p2.b)
        return 1;
    return 0;
}

int pixel_media(Pixel pixel) {
    return (pixel.r + pixel.g + pixel.b) / 3;
}

Image escala_de_cinza(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int media = pixel_media(img.pixel[i][j]);
            img.pixel[i][j] = (Pixel){media, media, media};
        }
    }

    return img;
}

Image sepia(Image img) {
    for(unsigned int x = 0; x < img.height; ++x) {
        for(unsigned int j = 0; j < img.width; ++j) {
            Pixel pixel = img.pixel[x][j];

            int p = pixel.r * .393 + pixel.g * .769 + pixel.b * .189;
            int menor_r = menor(255, p);
            img.pixel[x][j].r = menor_r;

            p = pixel.r * .349 + pixel.g * .686 + pixel.b * .168;
            menor_r = menor(255, p);
            img.pixel[x][j].g = menor_r;

            p = pixel.r * .272 + pixel.g * .534 + pixel.b * .131;
            menor_r = menor(255, p);
            img.pixel[x][j].b = menor_r;
        }
    }

    return img;
}

Image blur(Image img, int T)
{
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = menor(img.height - 1, i + T / 2);
            int menor_w = menor(img.width -1, j + T/2);
            for(int x = positivo(i - T/2); x <= menor_h; ++x) {
                for(int y = positivo(j - T/2); y <= menor_w; ++y) {
                    media.r += img.pixel[x][y].r;
                    media.g += img.pixel[x][y].g;
                    media.b += img.pixel[x][y].b;
                }
            }

            // printf("%u", media.r)
            media.r /= T * T;
            media.g /= T * T;
            media.b /= T * T;

            img.pixel[i][j] = media;
        }
    }

    return img;
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
        for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j] = img.pixel[x][y];
        }
    }

    return rotacionada;
}

Image espelhar_vertical(Image img) {
    int horizontal = 0;
    scanf("%d", &horizontal);

    int width = img.width, height = img.height;

    if (horizontal == 1)
        width /= 2;
    else
        height /= 2;

    for (int i2 = 0; i2 < height; ++i2) {
        for (int j = 0; j < width; ++j) {
            int x = i2, y = j;

            if (horizontal == 1) y = img.width - 1 - j;
            else x = img.height - 1 - i2;

            Pixel aux1 = img.pixel[i2][j];

            img.pixel[i2][j] = img.pixel[x][y];

            img.pixel[x][y] = aux1;
        }
    }

    return img;
}

Image inverter_cores(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j] = (Pixel){255 - img.pixel[i][j].r,
                                      255 - img.pixel[i][j].g,
                                      255 - img.pixel[i][j].b};
        }
    }

    return img;
}

Image cortar_imagem(Image img, int x, int y, int width, int height) {
    Image cortada;

    cortada.width = width;
    cortada.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cortada.pixel[i][j] = img.pixel[i + y][j + x];
        }
    }

    return cortada;
}


int main() {
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

    // read the number of operations to be applied
    int n_opcoes;
    scanf("%d", &n_opcoes);

    // read each opperation 'code'
    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = sepia(img);

                break;
            }
            case 3: { // Blur
                // read degree of blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                img = blur(img, tamanho);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotacionar90direita(img);
                }
                break;
            }
            case 5: { // Espelhamento
                img = espelhar_vertical(img);
                break;
            }
            case 6: { // Inversao de Cores
                img = inverter_cores(img);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cortar_imagem(img, x, y, width, height);
                break;
            }
        }

    }

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
    return 0;
}
