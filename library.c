#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu(void);

int main() {
    menu();
    return 0;
}

void menu(void) {

    int op;
    int fgetc(FILE *fp);
    char *fgets(char *buf, int n, FILE *fp);

    printf("Projeto pratico IAPG\n\n");
    printf("Escolha uma das seguintes opcoes:\n 1-Leitura de dados a partir de um ficheiro;\n 2-Visualizacao dos dados em formato tabela;\n 3-Calcular estatisticas(ex: soma,media,mediana,etc);\n 4-Guardar resultados das analises numa estrutura;\n 5-Guardar resultados das analises numa estrutura;\n 6-Leitura e apresentacao do historico de calculos efectuados;\n");
    scanf("%d", &op);
    FILE *the_file = fopen("C:/Users/Rui/Desktop/Projeto IAPG/ExampleCSV.csv", "r");

    if (the_file == NULL) {
        perror("Nao foi possivel abrir o ficheiro\n Encerrando o programa...");
        exit(1);
    }

    char c;
    int linhas=0;

    while((c=fgetc(the_file)) != EOF){
        if(c=='\n'){
            linhas++;
        }
    }

    // Os printfs inseridos em cima perguntam ao utilizador qual das opcoes deseja efetuar. Para que o programa execute cada funcao, sera utilizado um switch case que contem todas as opcoes acima mencionadas. Cada opcao realiza o codigo especifico
    switch (op) {
        case 1: {
            FILE *the_file = fopen("C:/Users/Rui/Desktop/Projeto IAPG/ExampleCSV.csv", "r");
            if (the_file == NULL) {
                perror("Nao foi possivel abrir o ficheiro\n Encerrando o programa...");
                exit(1);
            }
            char line[200];
            while (fgets(line,sizeof(line) ,the_file) !=NULL){
                printf("\n%s",line);
            }
        }
            break;
        case 2: {
            int menu1;
            printf("Pretende visualizar a tabela toda=1,ou apenas uma parte=2");
            scanf("%d",&menu1);

            if(menu1==1){
                FILE *the_file = fopen("C:/Users/Rui/Desktop/Projeto IAPG/ExampleCSV.csv", "r");
                if (the_file == NULL) {
                    perror("Nao foi possivel abrir o ficheiro\n Encerrando o programa...");
                    exit(1);
                }
                char line[200];
                while (fgets(line,sizeof(line) ,the_file) !=NULL){
                    printf("\n%s",line);
                }
            }
            else{
                int nlinhas,i;
                printf("Quantas linhas da tabela pretende visualizar");
                scanf("%d",&nlinhas);
                FILE *the_file = fopen("C:/Users/Rui/Desktop/Projeto IAPG/ExampleCSV.csv", "r");
                if (the_file == NULL) {
                    perror("Nao foi possivel abrir o ficheiro\n Encerrando o programa...");
                    exit(1);
                }
                char line[200];
                for(i=0;i<=nlinhas;i++){
                    if(fgets(line,sizeof(line) ,the_file) !=NULL){
                        printf("\n%s",line);
                    }
                }
            }
        }break;
        case 3: {
            int menu1;
            printf("Pretende visualizar a tabela toda=1,ou apenas uma parte=2");
            scanf("%d",&menu1);

            if(menu1==1){
                printf("Sopa\n");
            }
            else{


                FILE *the_file = fopen("C:/Users/Rui/Desktop/Projeto IAPG/ExampleCSV.csv", "r");
                if (the_file == NULL) {
                    perror("Nao foi possivel abrir o ficheiro\n Encerrando o programa...");
                    exit(1);
                }
                char line[200];
                int ages,GActg,totalGAb=0,totalAges=0;
                float GAb;
                char GAb2[30],ages2[30];
                char sex[30];

                char *sp;
                while (fgets(line,200,the_file) !=NULL){
                    sp = strtok(line,";");
                    strcpy(ages2,sp);
                    ages= atoi(sp);

                    sp = strtok(NULL, ";");
                    GActg= atoi(sp);

                    sp = strtok(NULL, ";");
                    strcpy(GAb2,sp);
                    GAb= atof(sp);
                    sp = strtok(NULL,";");
                    strcpy(sex,sp);
                    totalGAb+=GAb;
                    totalAges+=ages;
                    printf("\n%s %d  %s %s\n",ages2,GActg,GAb2,sex);
                }
                printf("Media do GA(b) %d\n",totalGAb/linhas);
                printf("Media das idades %d\n",totalAges/linhas);
            }
        }
    }
}