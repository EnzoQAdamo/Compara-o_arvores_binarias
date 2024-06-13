#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    int chave; //valor do nó
    struct no *Fesq; // Filho da esquerda
    struct no *Fdir; //Filho da direita
    struct no *pai; //pai
    int fb; // fator de balanceamento
}no;

typedef struct avl {
    struct no *sentinela; //sentinela
    int numElementos; // quantidade de elementos na arvore
}avl;

avl *criaArvore() {
    avl *arv = (avl *) malloc(sizeof(avl)); // Alocando arvore
    arv->sentinela = (no *) malloc(sizeof(no)); //Alocando sentinela
    arv->sentinela->chave = -1000;
    arv->sentinela->Fesq = NULL;
    arv->sentinela->Fdir = NULL;
    arv->sentinela->pai = NULL;
    arv->sentinela->fb = 0;
    arv->numElementos = 0;
    return arv;
}

//Função que implementa a rotação à direita
void rotacaoDir(no *noDesbal) {
    no *noEsq = noDesbal->Fesq;
    noDesbal->Fesq = noEsq->Fdir;
    if (noEsq->Fdir != NULL) {
        noEsq->Fdir->pai = noDesbal;
    }
    noEsq->pai = noDesbal->pai;
    if (noDesbal->pai == NULL) {// noDesbal era raiz
        noDesbal->pai->Fesq = noEsq;
    } else if (noDesbal == noDesbal->pai->Fesq) {
        noDesbal->pai->Fesq = noEsq;
    } else {
        noDesbal->pai->Fdir = noEsq;
    }
    noEsq->Fdir = noDesbal;
    noDesbal->pai = noEsq;

    // Atualiza os fatores de balanceamento
    noDesbal->fb = noDesbal->fb + 1 - (noEsq->fb < 0 ? noEsq->fb : 0);
    noEsq->fb = noEsq->fb + 1 + (noDesbal->fb > 0 ? noDesbal->fb : 0);
}

//Função que implementa a rotação à esquerda
void rotacaoEsq(no *noDesbal) {
    no *noDir = noDesbal->Fdir;
    noDesbal->Fdir = noDir->Fesq;
    if (noDir->Fesq != NULL) {
        noDir->Fesq->pai = noDesbal;
    }
    noDir->pai = noDesbal->pai;
    if (noDesbal->pai == NULL) { // noDesbal era raiz
        noDesbal->pai->Fesq = noDir;
    } else if (noDesbal == noDesbal->pai->Fesq) {
        noDesbal->pai->Fesq = noDir;
    } else {
        noDesbal->pai->Fdir = noDir;
    }
    noDir->Fesq = noDesbal;
    noDesbal->pai = noDir;
    
    // Atualiza os fatores de balanceamento
    noDesbal->fb = noDesbal->fb - 1 - (noDir->fb > 0 ? noDir->fb : 0);
    noDir->fb = noDir->fb - 1 + (noDesbal->fb < 0 ? noDesbal->fb : 0);
}

//Função que faz o balanceamento após inseção
void balanceamento(avl *arv, no *noDesbal) {
    if (noDesbal->fb == 2) {
        if (noDesbal->Fdir->fb >= 0) {
            rotacaoEsq(noDesbal);
        } else {
            rotacaoDir(noDesbal->Fdir);
            rotacaoEsq(noDesbal);
        }
    } else if (noDesbal->fb == -2) {
        if (noDesbal->Fesq->fb <= 0) {
            rotacaoDir(noDesbal);
        } else {
            rotacaoEsq(noDesbal->Fesq);
            rotacaoDir(noDesbal);
        }
    }
}

//Função que verifica o desbalanceamento na inserção
void atualizaFB_insercao(avl *arv, no *novoNo) {
    no *atual = novoNo;
    while (atual->pai != arv->sentinela) {
        no *pai = atual->pai;
        if (atual == pai->Fesq) {
            pai->fb--;
        } else {
            pai->fb++;
        }
        if (pai->fb == 0) {
            break;
        }
        if (pai->fb == 2 || pai->fb == -2) {
            balanceamento(arv, pai);
            break;
        }
        atual = pai;
    }
}

// Função que verifica o desbalanceamento na remoção
void atualizaFB_remocao(avl *arv, no *pai, int chaveRemovida) {
    no *atual = pai;
    while (atual != arv->sentinela) {
        no *paiAtual = atual->pai;
        if (chaveRemovida < atual->chave) {
            atual->fb++;
        } else {
            atual->fb--;
        }
        if (atual->fb == 1 || atual->fb == -1) {
            break;
        }
        if (atual->fb == 2 || atual->fb == -2) {
            balanceamento(arv, atual);
            if (paiAtual != arv->sentinela && paiAtual->fb == 0) {
                atualizaFB_remocao(arv, paiAtual->pai, atual->chave);
            }
            break;
        }
        atual = paiAtual;
    }
}

//Função que insere um elemento na árvore
//Retorna 1 se a inserção foi realizada com sucesso
//Retorna 0 se não foi possível realizar a inserção
int insereNo(avl *arv, int valor) {
    no *pai = arv->sentinela;
    no *atual = arv->sentinela->Fesq;
    while (atual != NULL) {
        pai = atual;
        if (valor < atual->chave) {
            atual = atual->Fesq;
        } else if (valor > atual->chave) {
            atual = atual->Fdir;
        } else {
            return 0; // Valor já existe na árvore
        }
    }

    no *novoNo = (no *) malloc(sizeof(no));
    novoNo->chave = valor;
    novoNo->Fesq = NULL;
    novoNo->Fdir = NULL;
    novoNo->pai = pai;
    novoNo->fb = 0;
    
    if (pai == arv->sentinela) {
        arv->sentinela->Fesq = novoNo;
    } else if (valor < pai->chave) {
        pai->Fesq = novoNo;
    } else {
        pai->Fdir = novoNo;
    }
    arv->numElementos++;
    atualizaFB_insercao(arv, novoNo);
    return 1;
}

//Função que imprime a árvore em ordem
//printf("%d - %d - %d\n",chave, pai, fb)
void imprimeOrdem(no *raiz) {
    if (raiz != NULL) {
        imprimeOrdem(raiz->Fesq);
        printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
        imprimeOrdem(raiz->Fdir);
    }
}

//Função que imprime a árvore em pré-ordem
//printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
void imprimePreOrdem(no *raiz) {
    if (raiz != NULL) {
        printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
        imprimePreOrdem(raiz->Fesq);
        imprimePreOrdem(raiz->Fdir);
    }
}

//Função que retorna a raiz da árvore
no *getRaiz(avl *arv) {
    return arv->sentinela->Fesq;
}

//Função que retorna a quantidade de elementos da árvore
int getNumElementos(avl *arv) {
    return arv->numElementos;
}

// Função que remove um elemento da árvore implementando a remoção por cópia usando o sucessor
// Retorna 1 se o elemento foi removido
// Retorna 0 se a árvore estiver vazia
// Retorna -1 caso o elemento a ser removido não esteja na árvore
int removeNo(avl *arv, int valor) {
    no *atual = arv->sentinela->Fesq;
    no *pai = arv->sentinela;
    while (atual != NULL && atual->chave != valor) {
        pai = atual;
        if (valor < atual->chave) {
            atual = atual->Fesq;
        } else {
            atual = atual->Fdir;
        }
    }
    if (atual == NULL) {
        return -1; // Elemento não encontrado
    }
    // Caso 3: O nó tem dois filhos
    if (atual->Fesq != NULL && atual->Fdir != NULL) {
        no *sucessor = atual->Fdir;
        while (sucessor->Fesq != NULL) {
            sucessor = sucessor->Fesq;
        }
        int chaveSucessor = sucessor->chave;
        removeNo(arv, chaveSucessor);
        atual->chave = chaveSucessor;
        return 1;
    }
    no *filho = (atual->Fesq != NULL) ? atual->Fesq : atual->Fdir;
    if (atual == arv->sentinela->Fesq) {
        arv->sentinela->Fesq = filho;
    } else if (atual == pai->Fesq) {
        pai->Fesq = filho;
    } else {
        pai->Fdir = filho;
    }
    if (filho != NULL) {
        filho->pai = pai;
    }
    atualizaFB_remocao(arv, pai, atual->chave);
    free(atual);
    arv->numElementos--;
    return 1;
}

//Função que lê um arquivo com números inteiros e um tipo
//Se o tipo for 1, a função deve chamar insereNo
//Se o tipo for 2, a função deve chamar removeNo
//Retorna -2 se o arquivo não puder ter sido lido
//Se o tipo for 1, insere os elementos na árvore e retorna os valores obtidos com o processamento da inserção
//  Retorna 1 se a inserção foi realizada com sucesso
//  Retorna 0 se não foi possível realizar a inserção
//Se o tipo for 2, remove os elementos da árvore e retorna os valores obtidos com o processamento da remoção
//  Retorna 1 se o elemento foi removido
//  Retorna 0 se a árvore estiver vazia
//Os valores retornados são tratados na main
int processaCarga(avl *arv, char *nomeArquivo, int tipo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        return -2; // Erro ao abrir o arquivo
    }
    int valor;
    while (fscanf(file, "%d", &valor) != EOF) {
        int res;
        if (tipo == 1) {
            res = insereNo(arv, valor);
        } else if (tipo == 2) {
            res = removeNo(arv, valor);
        }
        if (res == 0) {
            fclose(file);
            return 0; // Erro na inserção ou remoção
        }
    }
    fclose(file);
    return 1; // Sucesso
}