#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int compara(const void *e1, const void *e2)
{
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

void leOrdenaEscreveFecha(Endereco *e, long tamanho, FILE *arq, FILE *f){
    fread(e, sizeof(Endereco), tamanho, f);
    qsort(e, tamanho, sizeof(Endereco), compara);
    fwrite(e, sizeof(Endereco), tamanho, arq);
    fclose(arq);
}

void intercala(Endereco ea, Endereco eb, FILE *arqA, FILE *arqB, FILE *arqRes){
    fread(&ea, sizeof(Endereco), 1, arqA);
    fread(&eb, sizeof(Endereco), 1, arqB);

    while(!feof(arqA) && !feof(arqB))
    {
        if(compara(&ea, &eb) < 0)
        {
            fwrite(&ea, sizeof(Endereco), 1, arqRes);
            fread(&ea, sizeof(Endereco), 1, arqA);
        }
        else
        {
            fwrite(&eb, sizeof(Endereco), 1, arqRes);
            fread(&eb, sizeof(Endereco), 1, arqB);
        }
    }

    while(!feof(arqA))
    {
        fwrite(&ea, sizeof(Endereco), 1, arqRes);
        fread(&ea, sizeof(Endereco), 1, arqA);		
    }

    while(!feof(arqB))
    {
        fwrite(&eb, sizeof(Endereco), 1, arqRes);
        fread(&eb, sizeof(Endereco), 1, arqB);		
    }

    fclose(arqA);
    fclose(arqB);
    fclose(arqRes);
}

int main(){
    FILE *f, *saida;
	Endereco *e;
	long posicao, qtd;
    int MAX_DIVISAO = 16;
    int QNTD_MAX_ARQ = 2*MAX_DIVISAO - 1;
    long TAM_MAX;

	f = fopen("cep.dat","r");

	fseek(f,0,SEEK_END);
	posicao = ftell(f); //Em Bytes
	qtd = posicao/sizeof(Endereco); //Em número de registros
    TAM_MAX = qtd/MAX_DIVISAO; //Num max de registros lidos por vez

	e = (Endereco*) malloc(TAM_MAX*sizeof(Endereco));
	
    rewind(f);

    int count = 0;
    char nomeArq[15];
    //First round
    for(long i = 0; i < qtd; i += TAM_MAX+1){
        snprintf(nomeArq, sizeof(char) * 15, "arq_%i.dat", count);
        FILE *arq = fopen(nomeArq, "w");

        leOrdenaEscreveFecha(e, TAM_MAX, arq, f);

        count += 1;
    }
    fclose(f);
    
    QNTD_MAX_ARQ -= count;

    int ultimoArq = 0;
    while(QNTD_MAX_ARQ >= 1){    
        int j = 0;
        int numArqGerados = 0;
        char nomeArqRes[15];
        char nomeArqA[15];
        char nomeArqB[15];
        while(j <= QNTD_MAX_ARQ){
            int arqAtual = ultimoArq + j;
            int proxArq = arqAtual + 1;

            snprintf(nomeArqA, sizeof(char) * 15, "arq_%i.dat", arqAtual);  
            snprintf(nomeArqB, sizeof(char) * 15, "arq_%i.dat", proxArq);
            snprintf(nomeArqRes, sizeof(char) * 15, "arq_%i.dat", count);
            printf("Le %d e %d escreve em %d\t\n", arqAtual, proxArq, count);
            FILE *arqA = fopen(nomeArqA, "r");
            FILE *arqB = fopen(nomeArqB, "r");
            FILE *arqRes = fopen(nomeArqRes, "w");
            Endereco ea, eb;

            intercala(ea, eb, arqA, arqB, arqRes);

            remove(nomeArqA);
            remove(nomeArqB);

            j += 2;
            count++;
            numArqGerados++;
        }
        ultimoArq = count-numArqGerados;
        QNTD_MAX_ARQ -= numArqGerados;
    }


    free(e);

}