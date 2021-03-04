#include <stdio.h>
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

void busca(char cep[8], FILE *f){
    long inicio, fim, meio;
    int cmp;
    Endereco res;

    inicio = 0;
    fseek(f, 0, SEEK_END);
    fim = (ftell(f)/sizeof(Endereco)) - 1;
    meio = (inicio+fim/2);
    
    int count;
    while(inicio <= fim){
        fseek(f, meio * sizeof(Endereco), SEEK_SET);
        fread(&res, sizeof(Endereco), 1, f);
        
        cmp = strncmp(cep, res.cep, 8);
        
        if(cmp == 0){
            printf("CEP encontrado:\n %.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",res.logradouro,res.bairro,res.cidade,res.uf,res.sigla,res.cep);
            break;
        }
        else if(cmp < 0){
            fim = meio - 1;
            meio = (fim+inicio)/2;
            printf("Inicio: %ld, meio: %ld, fim: %ld\n", inicio, meio, fim);
            fseek(f, meio*sizeof(Endereco), SEEK_SET);
        }
        else{
            inicio = meio + 1;
            meio = (fim+inicio)/2;
            printf("Inicio: %ld, meio: %ld, fim: %ld\n", inicio, meio, fim);
            fseek(f, meio*sizeof(Endereco), inicio*sizeof(Endereco));
        }

        count++;
    }

    if(cmp != 0){
        printf("CEP não encontrado\n");
    }
    printf("%d\n", count);
}

int main(int argc, char** argv){
    FILE *f;
	Endereco e;
	f = fopen("cep_ordenado.dat","r");

    busca(argv[1], f);

	fclose(f);
    return 0;
}