#include <stdio.h>
#include <stdlib.h>

// Definição da struct para armazenar os 3 valores de cor (R, G, B)
typedef struct {
    int r, g, b;
} Pixel;

// Função para alocar dinamicamente uma matriz de pixels
Pixel **alocaMatriz(int linhas, int colunas) {
    Pixel **matriz = malloc(linhas * sizeof(Pixel*));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = malloc(colunas * sizeof(Pixel));
        for (int j = 0; j < colunas; j++){
            matriz[i][j].r = 0 ;
            matriz[i][j].g = 0 ;
            matriz[i][j].b = 0 ;
        }
    }
    return matriz;
}

// Função para liberar a memória alocada para a matriz de pixels
void liberaMatriz(Pixel **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Função para ler o arquivo e preencher a matriz de pixels
void leArquivoEPreencheMatriz(const char *nomeArquivo, Pixel ***matriz1, Pixel ***matriz2, int *linhas, int *colunas) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char formato[3];
    int valorMax;

    // Lê o formato P3
    fscanf(arquivo, "%s", formato);

    // Lê as dimensões da imagem (linhas e colunas)
    fscanf(arquivo, "%d %d", colunas, linhas);

    // Lê o valor máximo (não usado diretamente, mas pode ser útil)
    fscanf(arquivo, "%d", &valorMax);

    // Aloca dinamicamente as matrizes de pixels
    *matriz1 = alocaMatriz(*linhas, *colunas);
    *matriz2 = alocaMatriz(*linhas, *colunas);

    // Lê os dados da imagem e preenche a matriz1 de pixels
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            fscanf(arquivo, "%d %d %d", &(*matriz1)[i][j].r, &(*matriz1)[i][j].g, &(*matriz1)[i][j].b);
        }
    }

    // Lê os dados da imagem e preenche a matriz2 de pixels
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            fscanf(arquivo, "%d %d %d", &(*matriz2)[i][j].r, &(*matriz2)[i][j].g, &(*matriz2)[i][j].b);
        }
    }

    fclose(arquivo);
}

// Função para multiplicar duas matrizes de pixels
void multiplicaMatrizes(Pixel **matriz1, Pixel **matriz2, int linhas, int colunas, Pixel **resultado) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            // Realizar a multiplicação das matrizes
            for (int k = 0; k < colunas; k++) {
                resultado[i][j].r += matriz1[i][k].r * matriz2[k][j].r;
                resultado[i][j].g += matriz1[i][k].g * matriz2[k][j].g;
                resultado[i][j].b += matriz1[i][k].b * matriz2[k][j].b;
            }
        }
    }
}

void somarMatrizes(Pixel **a, Pixel **b, Pixel **c, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            c[i][j].r = a[i][j].r + b[i][j].r;
            c[i][j].g = a[i][j].g + b[i][j].g;
            c[i][j].b = a[i][j].b + b[i][j].b;
        }
    }
}

void subtrairMatrizes(Pixel **a, Pixel **b, Pixel **c, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            c[i][j].r = a[i][j].r - b[i][j].r;
            c[i][j].g = a[i][j].g - b[i][j].g;
            c[i][j].b = a[i][j].b - b[i][j].b;
        }
    }
}

void multiplicacaoStrassen( Pixel **a, Pixel **b, Pixel **c, int tamanho){
    if(tamanho == 2){
        int p1, p2, p3, p4, p5, p6, p7;
        p1 = a[0][0].r * (b[0][1].r - b[1][1].r);
        p2 = (a[0][0].r + a[0][1].r) * b[1][1].r;
        p3 = (a[1][0].r + a[1][1].r) * b[0][0].r;
        p4 = a[1][1].r * (b[1][0].r - b[0][0].r);
        p5 = (a[0][0].r + a[1][1].r) * (b[0][0].r + b[1][1].r);
        p6 = (a[0][1].r - a[1][1].r) * (b[1][0].r + b[1][1].r);
        p7 = (a[0][0].r - a[1][0].r) * (b[0][0].r + b[0][1].r);

        c[0][0].r += p5 + p4 - p2 + p6;
        c[0][1].r += p1 + p2;
        c[1][0].r += p3 + p4;
        c[1][1].r += p5 + p1 - p3 - p7;

        p1 = a[0][0].g * (b[0][1].g - b[1][1].g);
        p2 = (a[0][0].g + a[0][1].g) * b[1][1].g;
        p3 = (a[1][0].g + a[1][1].g) * b[0][0].g;
        p4 = a[1][1].g * (b[1][0].g - b[0][0].g);
        p5 = (a[0][0].g + a[1][1].g) * (b[0][0].g + b[1][1].g);
        p6 = (a[0][1].g - a[1][1].g) * (b[1][0].g + b[1][1].g);
        p7 = (a[0][0].g - a[1][0].g) * (b[0][0].g + b[0][1].g);

        c[0][0].g += p5 + p4 - p2 + p6;
        c[0][1].g += p1 + p2;
        c[1][0].g += p3 + p4;
        c[1][1].g += p5 + p1 - p3 - p7;

        p1 = a[0][0].b * (b[0][1].b - b[1][1].b);
        p2 = (a[0][0].b + a[0][1].b) * b[1][1].b;
        p3 = (a[1][0].b + a[1][1].b) * b[0][0].b;
        p4 = a[1][1].b * (b[1][0].b - b[0][0].b);
        p5 = (a[0][0].b + a[1][1].b) * (b[0][0].b + b[1][1].b);
        p6 = (a[0][1].b - a[1][1].b) * (b[1][0].b + b[1][1].b);
        p7 = (a[0][0].b - a[1][0].b) * (b[0][0].b + b[0][1].b);


        c[0][0].b += p5 + p4 - p2 + p6;
        c[0][1].b += p1 + p2;
        c[1][0].b += p3 + p4;
        c[1][1].b += p5 + p1 - p3 - p7;
    }else{
        tamanho = tamanho/2;
        
        // Aloca submatrizes de A
        Pixel **a11 = alocaMatriz(tamanho, tamanho);
        Pixel **a12 = alocaMatriz(tamanho, tamanho);
        Pixel **a21 = alocaMatriz(tamanho, tamanho);
        Pixel **a22 = alocaMatriz(tamanho, tamanho);

        // Aloca submatrizes de B
        Pixel **b11 = alocaMatriz(tamanho, tamanho);
        Pixel **b12 = alocaMatriz(tamanho, tamanho);
        Pixel **b21 = alocaMatriz(tamanho, tamanho);
        Pixel **b22 = alocaMatriz(tamanho, tamanho);

        // Aloca submatrizes de C
        Pixel **c11 = alocaMatriz(tamanho, tamanho);
        Pixel **c12 = alocaMatriz(tamanho, tamanho);
        Pixel **c21 = alocaMatriz(tamanho, tamanho);
        Pixel **c22 = alocaMatriz(tamanho, tamanho);

        // Aloca as matrizes temporadias para a multiplicação
        Pixel **m1 = alocaMatriz(tamanho, tamanho);//Matriz para a multiplicação 1
        Pixel **m2 = alocaMatriz(tamanho, tamanho);//Matriz para a multiplicação 2
        Pixel **m3 = alocaMatriz(tamanho, tamanho);//Matriz para a multiplicação 3
        Pixel **m4 = alocaMatriz(tamanho, tamanho);//Matriz para a multiplicação 4
        Pixel **m5 = alocaMatriz(tamanho, tamanho);//Matriz para a multiplicação 5
        Pixel **m6 = alocaMatriz(tamanho, tamanho);//Matriz para a multiplicação 6
        Pixel **m7 = alocaMatriz(tamanho, tamanho);//Matriz para a multiplicação 7

        Pixel **temp1 = alocaMatriz(tamanho, tamanho);
        Pixel **temp2 = alocaMatriz(tamanho, tamanho);

        // Divida as matrizes A e B em submatrizes
        for (int i = 0; i < tamanho; i++) {
            for (int j = 0; j < tamanho; j++) {
                a11[i][j] = a[i][j];
                a12[i][j] = a[i][j + tamanho];
                a21[i][j] = a[i + tamanho][j];
                a22[i][j] = a[i + tamanho][j + tamanho];

                b11[i][j] = b[i][j];
                b12[i][j] = b[i][j + tamanho];
                b21[i][j] = b[i + tamanho][j];
                b22[i][j] = b[i + tamanho][j + tamanho];
            }
        }

        // Calcula M1 = A11 * (B12 - B22)
        subtrairMatrizes(b12, b22, temp1, tamanho);
        multiplicacaoStrassen(a11, temp1, m1, tamanho);

        // Calcula M2 = (A11 + A12) * B22
        somarMatrizes(a11, a12, temp1, tamanho);
        multiplicacaoStrassen(temp1, b22, m2, tamanho);

        // Calcula M3 = (A21 + A22) * B11
        somarMatrizes(a21, a22, temp1, tamanho);
        multiplicacaoStrassen(temp1, b11, m3, tamanho);

        // Calcula M4 = A22 * (B21 - B11)
        subtrairMatrizes(b21, b11, temp1, tamanho);
        multiplicacaoStrassen(a22, temp1, m4, tamanho);

        // Calcula M5 = (A11 + A22) * (B11 + B22) 
        somarMatrizes(a11, a22, temp1, tamanho);
        somarMatrizes(b11, b22, temp2, tamanho);
        multiplicacaoStrassen(temp1, temp2, m5, tamanho);

        // Calcula M6 = (A12 - A22) * (B21 + B22)
        subtrairMatrizes(a12, a22, temp1, tamanho);
        somarMatrizes(b21, b22, temp2, tamanho);
        multiplicacaoStrassen(temp1, temp2, m6, tamanho);

        // Calcula M7 = (A11 - A21) * (B11 + B12)
        subtrairMatrizes(a11, a21, temp1, tamanho);
        somarMatrizes(b11, b12, temp2, tamanho);
        multiplicacaoStrassen(temp1, temp2, m7, tamanho);

        // Calcula C11 = M5 + M4 - M2 + M6
        somarMatrizes(m5, m4, temp1, tamanho);
        subtrairMatrizes(temp1, m2, temp2, tamanho);
        somarMatrizes(temp2, m6, c11, tamanho);

        // Calcula C12 = M1 + M2
        somarMatrizes(m1, m2, c12, tamanho);

        // Calcula C21 = M3 + M4
        somarMatrizes(m3, m4, c21, tamanho);

        // Calcula C22 = M5 + M1 - M3 - M7
        somarMatrizes(m5, m1, temp1, tamanho);
        subtrairMatrizes(temp1, m3, temp2, tamanho);
        subtrairMatrizes(temp2, m7, c22, tamanho);

        // Junta os blocos C11, C12, C21, C22 no resultado
        for (int i = 0; i < tamanho; i++) {
            for (int j = 0; j < tamanho; j++) {
                c[i][j].r += c11[i][j].r;
                c[i][j].g += c11[i][j].g;
                c[i][j].b += c11[i][j].b;
                c[i][j + tamanho].r += c12[i][j].r;
                c[i][j + tamanho].g += c12[i][j].g;
                c[i][j + tamanho].b += c12[i][j].b;
                c[i + tamanho][j].r += c21[i][j].r;
                c[i + tamanho][j].g += c21[i][j].g;
                c[i + tamanho][j].b += c21[i][j].b;
                c[i + tamanho][j + tamanho].r += c22[i][j].r;
                c[i + tamanho][j + tamanho].g += c22[i][j].g;
                c[i + tamanho][j + tamanho].b += c22[i][j].b;
            }
        }

        // Libera as submatrizes temporárias
        liberaMatriz(a11, tamanho);
        liberaMatriz(a12, tamanho);
        liberaMatriz(a21, tamanho);
        liberaMatriz(a22, tamanho);
        liberaMatriz(b11, tamanho);
        liberaMatriz(b12, tamanho);
        liberaMatriz(b21, tamanho);
        liberaMatriz(b22, tamanho);
        liberaMatriz(c11, tamanho);
        liberaMatriz(c12, tamanho);
        liberaMatriz(c21, tamanho);
        liberaMatriz(c22, tamanho);
        liberaMatriz(m1, tamanho);
        liberaMatriz(m3, tamanho);
        liberaMatriz(m3, tamanho);
        liberaMatriz(m4, tamanho);
        liberaMatriz(m5, tamanho);
        liberaMatriz(m6, tamanho);
        liberaMatriz(m7, tamanho);
        liberaMatriz(temp1, tamanho);
        liberaMatriz(temp2, tamanho);
    }
 }

int main() {
    Pixel **matriz1;
    Pixel **matriz2;
    Pixel **resultado;
    int linhas, colunas;

    leArquivoEPreencheMatriz("3.in", &matriz1, &matriz2, &linhas, &colunas);
    resultado = alocaMatriz(linhas, colunas);

     /* // Exibe a matriz de pixels
    printf("Matriz 1 de pixels (R, G, B):\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("(%d, %d, %d) ", matriz1[i][j].r, matriz1[i][j].g, matriz1[i][j].b);
        }
        printf("\n");
    }

    printf("Matriz 2 de pixels (R, G, B):\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("(%d, %d, %d) ", matriz2[i][j].r, matriz2[i][j].g, matriz2[i][j].b);
        }
        printf("\n");
    }*/

    //multiplicaMatrizes(matriz1, matriz2, linhas, colunas, resultado);
    multiplicacaoStrassen(matriz1, matriz2, resultado, linhas);
    printf("Resultado da multiplicao das matrizes (R, G, B):\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("(%d, %d, %d) ", resultado[i][j].r, resultado[i][j].g, resultado[i][j].b);
        }
        printf("\n");
    }

    // Libera a memória da matriz de pixels
    liberaMatriz(matriz1, linhas);
    liberaMatriz(matriz2, linhas);
    liberaMatriz(resultado, linhas);
    return 0;
}