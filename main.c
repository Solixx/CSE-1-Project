#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#ifndef MAX
#define MAX 10
#endif

//Strutura para guardar os valores das analises

struct analise{
    char *file_name;
    char *tempo;
    int col;
    double median;
    double soma;
    double media;
    double max;
    double min;
    double q1;
    double q3;
    double desvio;
};


//Criacao das funcoes

void menu(void);
void leitura(FILE *ficheiro, char fic[20000], int linhas);
void tabela(FILE *ficheiro, char fic[20000], int linhas);
struct analise calcular(int colunas, int linhas, int totalcol, FILE *ficheiro, char fic[20000]);
void historico(struct analise resultado1[],int i, FILE *ficheiro, char fic[20000]);
void lercolunas(int colunas, int linhas, int totalcol, FILE *ficheiro, char fic[20000]);
void frequencia(int colunas, int linhas, int totalcol, FILE *ficheiro, char fic[20000]);
void escrever_historico(FILE  *fic_historico, char  ler_hist[20000],FILE *ficheiro, struct analise resultado1[], int i);
void pesquisa_historico( char  ler_hist[20000]);


//Funcao Main do tipo int

int main() {


    //Chamada da funcao menu

    menu();
    return 0;

}

//Funcao Remover_Espacos do tipo char

char *remover_espacos(char *c)
{

    //Criacao de variaveis
    int i = 0, j = 0;

    //Ciclo para percorrer os valores do vetor carater (c) com a posicao do i
    while (c[i])
    {

        //Se o carater do vetor c for diferente de um espaco
        if (c[i] != ' ') {

            //O vetor carater c na posicao j + 1 e igual ao valor do carater c na posicao i
            c[j++] = c[i];
        }

        //Valor do i aumenta em +1
        i++;
    }

    //Vetor carater c na posicao J vai ser igual a fim da string (\0)
    c[j] = '\0';

    //retorna o valor final da string c
    return c;
}


//Funcao Menu do tipo void

void menu(void) {

    //Criacao de variaveis tipo double
    double soma=0, median=0, media=0, q1=0, q3=0, desvio=0, max=0, min=0;

    //Criacao de variavel tipo struct analise
    struct analise resultado1[2000];

    //Criacao de variaveis tipo int
    int op, menu_counter=0, resultado_counter=0;

    char fic[20000];

    //Titulo do projeto
    printf("\n\tProjeto pratico IAPG\n\n");

    //Leitura do endereco do ficheiro escrito pelo utilizador
    printf("Insira o endereco e o nome do ficheiro\n");
    scanf("%s", &fic);

    //ciclo do while para o programa escrever o menu ate o utilizador selecionar o valor para fechar o mesmo
    do{

        //Escrita e leitura do menu
        printf("\n\t Opcoes\n");
        printf("1 -> Leitura de Dados\n");
        printf("2-> Visualizar Dados como Tabela\n");
        printf("3-> Calcular Estatisticas\n");
        printf("4-> Historico\n");
        printf("5-> Escrever Analises num Ficheiro\n");
        printf("6-> Ler Colunas\n");
        printf("7-> Pesquisa no Historico\n");
        printf("Fechar Programa (prima qualquer outra tecla)\n");

        printf("->");
        scanf("%d", &op);
        printf("\n");

        //Abertura do ficheiro tipo leitura
        FILE *ficheiro = fopen(fic, "r");

        //Se o ficheiro nao existir
        if (ficheiro == NULL) {

            //Mensagem de erro
            perror("Nao foi possivel abrir o ficheiro\n Encerrando o programa...");

            //fechar o programa
            exit(1);
        }

        //Criacao de variaveis
        char c;
        int colunas=1;
        int linhas=1;
        int totalcol=0;

        //ciclo while para leitura do ficheiro enquanto nao chega ao fim do ficheiro(EOF)
        while((c=fgetc(ficheiro)) != EOF){

            //Se o carater (c) for igual a ';' --> (Pois o ficheiro Exel diferencia as colunas por ';')
            if(c==';'){

                //Contador de colunas aumenta em +1
                colunas++;
            }

            //Se o carater (c) for igual a Enter('\n') --> Fim da primeira linha logo ja tem o numero de colunas totais
            if(c=='\n'){

                //Se o contador ede linhas for igual a 1 --> Se esta na primeira linha
                if(linhas==1){

                    //Totalcolunas vai ser igual ao numero de colunas --> So faz para a primeira linha
                    totalcol=colunas;
                }

                //Contador de linhas aumenta em +1
                linhas++;

            }

        }


        //  < Menus >

        //Se opcao(op) for igual a 1
        if (op == 1){

            //Chamada da funcao Leitura
            leitura(ficheiro, fic, linhas);

        }

        //Se opcao(op) for igual a 2
        else if(op == 2){

            //Chamada da funcao Tabela
            tabela(ficheiro, fic, linhas);

        }

        //Se opcao(op) for igual a 3
        else if(op == 3){

            //Variavel resultado1 na posicao resultado_counter vai ser igual ao valor returnado na funcao calcular
            resultado1[resultado_counter] = calcular(colunas, linhas, totalcol, ficheiro, fic);

            //Valor de resultado_counter é aumentado em +1
            resultado_counter++;

        }

        //Se opcao(op) for igual a 4
        else if(op == 4){

            //Ciclo que comeca na posicao de i = 0 ate ao numero de calculos feitos pelo utilizador o numero de calculos esta guardado na variavel resultado_counter
            for (int i = 0; i < resultado_counter; ++i) {

                //Chamada da funcao Historico
                historico(resultado1, i, ficheiro, fic);

            }

        }

        //Se opcao(op) for igual a 5
        else if(op == 5){

            //Criacao de variaveis
            char ler_hist[20000];

            //Fechar ficheiro anterior
            fclose(ficheiro);

            printf("\n\tHistorico\n");
            printf("Insira o diretorio do ficheiro onde quer guardar o historico\n");
            scanf("%s",&ler_hist);

            printf("%s", ler_hist);

            //Ciclo que comeca na posicao de i = 0 ate ao numero de calculos feitos pelo utilizador o numero de calculos esta guardado na variavel resultado_counter
            for (int i = 0; i < resultado_counter; i++) {

                //Abrir ficheiro do historico
                FILE *fic_historico= fopen(ler_hist, "a+");
                if (fic_historico == NULL) {
                    perror("Nao foi possivel abrir o ficheiro\n Encerrando o programa...");
                    exit(1);
                }

                //Chamada da funcao escrever_historico
                escrever_historico(fic_historico, ler_hist, ficheiro, resultado1, i);
            }

        }

        //Se opcao(op) for igual a 6
        else if(op == 6){

            //Chamada da funcao Lercolunas
            lercolunas(colunas, linhas, totalcol, ficheiro, fic);

        }

        else if(op == 7){

            //Criacao de variaveis
            char ler_hist[20000];
            printf("Insira o diretorio do ficheiro onde quer fazer a pesquisa do historico\n");
            scanf("%s",&ler_hist);

            pesquisa_historico(ler_hist);

        }

    //Continua o ciclo enquanto o valor da opcao escolhida (op) estiver entre 1 e 5
    } while (op <= 7 && op >= 1);

}


//Funcao Leitura do tipo void
void leitura(FILE *ficheiro, char fic[20000], int linhas){

    //Criacao de variaveis
    char line[200];

    //Abertura do ficheiro tipo leitura
    ficheiro = fopen(fic, "r");

    //Se o ficheiro nao existir
    if (ficheiro == NULL) {

        //Mensagem de erro
        perror("Nao foi possivel abrir o ficheiro\n Encerrando o programa...");

        //fechar o programa
        exit(1);
    }

    //Ciclo que comeca na posicao de i = 0 ate ao numero total de linhas do ficheiro, este numero esta guardado na variavel (linhas) e o valor de i aumenta em +1 no final de cada passagem pelo ciclo
    for(int i = 1; i<= linhas; i++){

        //Se o valor da linha do ficheiro for diferente de NULL
        if(fgets(line,sizeof(line) ,ficheiro) !=NULL){

            //Escreve o valor da linha lida do ficheiro
            printf("%-12s",line);
        }
    }

}

//Funcao Tabela do tipo void

void tabela( FILE *ficheiro, char fic[20000], int linhas){

    //Criacao de variaveis
    int menu, nlinhas, i;
    char *sp;
    char line[2000];

    printf("\n\t Tabela\n");
    printf("1-> Visualizar Tabela Toda\n");
    printf("2-> Escolher Numero de Colunas\n");

    printf("->");
    scanf("%d",&menu);
    printf("\n");

    if(menu==1){

        //Abertura do ficheiro tipo leitura
        ficheiro = fopen(fic, "r");

        //Se o ficheiro nao existir
        if (ficheiro == NULL) {

            //Mensagem de erro
            perror("Nao foi possivel abrir o ficheiro\n Encerrando o programa...");

            //fechar o programa
            exit(1);
        }

        //Ciclo que comeca na posicao de i = 1 ate ao numero total de linhas do ficheiro, este numero esta guardado na variavel (linhas) e o valor de i aumenta em +1 no final de cada passagem pelo ciclo
        for (i = 1; i <= linhas; i++){
            if(fgets(line,sizeof(line) ,ficheiro)){

                //O apontador sp recebe o valor da variavel line ate chegar a um ;
                sp = strtok(line, ";");

                //Enquanto o valor do apontador for diferente de NULL
                while (sp != NULL) {

                    //Escreve o valor de sp
                    printf("%-12s", sp);

                    //sp e igual ao proximo valor ate chegar a outro ;
                    sp = strtok(NULL, ";");
                }
                printf("\n");
            }
        }

    }
    else {

        do {
            printf("Quantas linhas da tabela pretende visualizar\n");

            printf("->");
            scanf("%d", &nlinhas);
            printf("\n");

            //Abertura do ficheiro tipo leitura
            ficheiro = fopen(fic, "r");

            //Se o ficheiro nao existir
            if (ficheiro == NULL) {

                //Mensagem de erro
                perror("Nao foi possivel abrir o ficheiro\n Encerrando o programa...");

                //fechar o programa
                exit(1);
            }

            if (nlinhas > linhas || nlinhas < 1) {

                printf("Esta linha nao existe no ficheiro %d o ficheiro tem apenas %d\n", nlinhas, linhas);

            } else {

                //Ciclo que comeca na posicao de i = 1 ate ao numero de linhas do ficheiro escolhidas pelo utilizador, este numero esta guardado na variavel (nlinhas) e o valor de i aumenta em +1 no final de cada passagem pelo ciclo
                for (i = 1; i <= nlinhas + 1; i++) {
                    if (fgets(line, sizeof(line), ficheiro)) {

                        //O apontador sp recebe o valor da variavel line ate chegar a um ;
                        sp = strtok(line, ";");

                        //Enquanto o valor do apontador for diferente de NULL
                        while (sp != NULL) {

                            //Escreve o valor de sp
                            printf("%-12s", sp);

                            //sp e igual ao proximo valor ate chegar a outro ;
                            sp = strtok(NULL, ";");
                        }
                        printf("\n");
                    }
                }
            }
        } while (nlinhas > linhas || nlinhas < 1);
    }
}

//Funcao calcular do tipo struct analise

struct analise calcular(int colunas, int linhas, int totalcol,  FILE *ficheiro, char fic[20000]){

    int menu, col, q1_size, q3_size;
    int i=0,j=0,k=0,l=0,counter=0, menu_counter=0;
    float value;
    double q1, q3, x;
    double max=0, min=0, p[1000], sd=0.0, median=0, desvio=0, numerobica=0, soma = 0, media = 0, temp=0;
    double median_nums[200];
    char line[200], col1[30], col2[30], s[2000], tempo[2000];
    char c[linhas];
    char *sp;
    bool once = false;

    struct analise resultado[2000];

    printf("\n\tMenu de Operacoes\n");
    printf("1-> Soma\n");
    printf("2-> Media e Desvio Padrao\n");
    printf("3-> Minimo e Maximo\n");
    printf("4-> Mediana, Primeiro Quartil e Terceiro Quartil\n");

    printf("->");
    scanf("%d",&menu);
    printf("\n");

    printf("Em que coluna deseja realizar a operacao\n");

    printf("->");
    scanf("%d",&col);
    printf("\n");

    ficheiro = fopen(fic, "r");
    if (ficheiro == NULL) {
        perror("Nao foi possivel abrir o ficheiro\n Encerrando o programa...");
        exit(1);
    }

    if(col > totalcol || col < 0){

        printf("Esta coluna nao existe no ficheiro %d o ficheiro tem apenas %d\n", col, totalcol);

    }
    else {

        //Ciclo para leitura das linhas do ficheiro
        for (i = 1; i <= linhas; i++) {

            //Lê as linhas do ficheiro até a linha NULL
            if ((fgets(line, sizeof(line), ficheiro)) != NULL) {

                //Ciclo para leitura das colunas do ficheiro
                for (j = 1; j <= totalcol; j++) {

                    //Se a coluna for igual a 1
                    if (j == 1) {

                        //Pointer vai ser igual ao valor da linha até ao ;
                        sp = strtok(line, ";");

                        //Copiar o valor do pointer (sp) para a string c
                        strcpy(c, sp);

                        //Precisamos de mudar as , para . pois o codigo strtod procura por . e n reconhece ,
                        //Percorrrer os carateres da string c
                        for (int k = 0; k < strlen(c); k++) {

                            //Se o carater for igual a ,
                            if (c[k] == ',') {

                                //Transforma a , em um .
                                c[k] = '.';
                            }

                        }

                        //Chama a funcao remover_espacos para remover os espacos da string c
                        remover_espacos(c);

                        //Se i diferente de 1 (para n ler o titulo)
                        if (i != 1) {

                            //igualar a variavel x ao valor de string c transformado em numero até chegar ao NULL
                            x = strtod(c, (char **) NULL);
                        }

                        //Se a coluna for maior que 1
                    } else if (j > 1) {

                        //Pointer vai ser igual ao valor da linha até ao ;
                        sp = strtok(NULL, ";");

                        //Copiar o valor do pointer (sp) para a string c
                        strcpy(c, sp);

                        //Precisamos de mudar as , para . pois o codigo strtod procura por . e n reconhece ,
                        //Percorrrer os carateres da string c
                        for (int k = 0; k < strlen(c); k++) {

                            //Se o carater for igual a ,
                            if (c[k] == ',') {

                                //Transforma a , em um .
                                c[k] = '.';
                            }

                        }

                        //Chama a funcao remover_espacos para remover os espacos da string c
                        remover_espacos(c);

                        //Se i diferente de 1 (para n ler o titulo)
                        if (i != 1) {

                            //igualar a variavel x ao valor de string c transformado em numero até chegar ao NULL
                            x = strtod(c, (char **) NULL);
                        }
                    }

                    //Se o valor da coluna atual (j) for igual ao valor da coluna que o utilizador escolheu (col)
                    if (j == col) {

                        //Soma vai ser igual a soma mais o valor de x
                        soma = soma + x;

                        //Vetor median_nums igualado a x (este vetor vai ser usado para calcular a mediana)
                        if (i != 1) {
                            median_nums[i - 1] = x;
                        }

                        //Contador do numero de numeros de uma coluna
                        //Se o tamanho da string c for igual a 1
                        if (strlen(c) == 1) {

                            //Percorrer os valores da string c
                            for (int m = 0; m < strlen(c); ++m) {

                                //Se o valor de c for igual a 0 (queremos ler o valor 0 pois o numero do ficheiro guardado numa coluna pode ser 0 e tem de ser lido mas o strlen diz q 0 é null)
                                if (c[m] == '0') {

                                    //Valor do counter é aumentado em 1
                                    counter++;
                                } else {}
                            }
                        }

                            //Se não o counter é aumentado em +1
                        else {
                            counter++;
                        }

                        //Se o valor de i for difetente de 1 (linha diferente de 1)
                        if (i != 1) {

                            //Se o maximo for menor que o valor atual da coluna x
                            if (max < x) {

                                //O novo maximo vai ser igual ao valor atual de x
                                max = x;
                            }

                            //Variavel Bool (once) para que o minimo comece com um valor da coluna para poder fazer o seu calculo (este if só vai ser feito a primeira vez)
                            if (once == false) {
                                //Minimo é igual ao maximo
                                min = max;

                                //Variavel Bool (once) vai mudar de false para true
                                once = true;
                            }

                            //Se minimo maior que o valor atual de x
                            if (min > x) {

                                //Minimo vai ser igual ao valor atual de x
                                min = x;
                            }
                        }

                    }
                }
            }
        }

        //Diminuir um valor à variavel contadora (counter) de numeros de uma coluna
        counter--;

        //media vai ser igual à soma a dividir pelo numero de numeros dessa coluna (counter)
        media = soma / counter;

        //Se a operacao selecionada pelo utilizador for 1 (soma)
        if (menu == 1) {

            //Escreve a sooma
            printf("Soma = %.3lf\t\n", soma);

            time_t now;
            time(&now);
            strcpy(tempo, ctime(&now));

            //Guardar os valores na struct analises
            resultado[menu_counter].col = col;
            resultado[menu_counter].soma = soma;
            resultado[menu_counter].media = 0;
            resultado[menu_counter].desvio = 0;
            resultado[menu_counter].max = 0;
            resultado[menu_counter].min = 0;
            resultado[menu_counter].median = 0;
            resultado[menu_counter].q1 = 0;
            resultado[menu_counter].q3 = 0;
            resultado[menu_counter].tempo = tempo;

        }

            //Se a operacao selecionada pelo utilizador for 2 (media e desvio padrao)
        else if (menu == 2) {

            //Ciclo para percorrer os valores dessa coluna
            for (i = 1; i <= counter; i++) {

                //variavel sd vai ser igual a sd mais o valor de (median_nums na proxima posicao - media) elevado a 2
                sd = sd + pow((median_nums[i + 1] - media), 2);
            }

            //Variavel numerobica vai ser igual a sd a dividir pelo valor do numero de numeros dessa coluna (counter)
            numerobica = sd / counter;

            //Desvio padra (desvio) vai ser igual à raiz do valor guardado na variavel numerobica
            desvio = sqrt(numerobica);

            //Escreve os valores das variaveis
            printf("Media = %.3lf\n", media);
            printf("Desvio Padrao = %.3lf\n", desvio);

            time_t now;
            time(&now);
            strcpy(tempo, ctime(&now));

            //Guardar os valores na struct analises
            resultado[menu_counter].col = col;
            resultado[menu_counter].soma = 0;
            resultado[menu_counter].media = media;
            resultado[menu_counter].desvio = desvio;
            resultado[menu_counter].max = 0;
            resultado[menu_counter].min = 0;
            resultado[menu_counter].median = 0;
            resultado[menu_counter].q1 = 0;
            resultado[menu_counter].q3 = 0;
            resultado[menu_counter].tempo = tempo;

        }

            //Se a operacao selecionada pelo utilizador for 3 (minimo e maximo)
        else if (menu == 3) {

            //Escreve os valores do minimo e maximo (já calculados nos fors visto acima)
            printf("Valor Maximo = %.3lf\n", max);
            printf("Valor Minimo = %.3lf\n", min);

            time_t now;
            time(&now);
            strcpy(tempo, ctime(&now));

            //Guardar os valores na struct analises
            resultado[menu_counter].col = col;
            resultado[menu_counter].soma = 0;
            resultado[menu_counter].media = 0;
            resultado[menu_counter].desvio = 0;
            resultado[menu_counter].max = max;
            resultado[menu_counter].min = min;
            resultado[menu_counter].median = 0;
            resultado[menu_counter].q1 = 0;
            resultado[menu_counter].q3 = 0;
            resultado[menu_counter].tempo = tempo;

        }


            //Se a operacao selecionada pelo utilizador for 4 (minimo e maximo)
        else if (menu == 4) {

            //Organizar os valores da coluna
            //Ciclo para percorrer todos os numeros da coluna
            for (k = 1; k <= counter; k++) {

                //Ciclo para comparar todos os proximos valores com o valor do for anterior
                for (l = 1; l <= counter - 1; l++) {

                    //Se o valor atual for maior que o proximo valor
                    if (median_nums[l] > median_nums[l + 1]) {

                        //variavel temporaria (temp) vai ser igual ao valor atual (median_nums[l])
                        temp = median_nums[l];

                        //Valor atual vai ser igual ao proximo valor
                        median_nums[l] = median_nums[l + 1];

                        //Proximo valor vai ser igual ao valor guardado na variavel temp (valor atual)
                        median_nums[l + 1] = temp;
                    }

                }
            }

            printf("\n\tValores Ordenados\n");

            //Ciclo para escrever os valores ordenados de forma crescente
            for (k = 1; k <= counter; k++) {

                printf("[%d] - %.3lf\n", k, median_nums[k]);

            }

            //Se o resto do numero de numeros desta coluna for igual a 0 (logo numero de numeros par)
            if (counter % 2 == 0) {

                //mediana (median) vai ser igual ao valor da median_nums 1 posicao a frente da metade do numero total de posicoes (median_nums[(counter / 2) + 1]) + o valor de median_nums na posicao da metade do numero de posicoes total (median_nums[counter / 2]) e isto tudo vai dividir por 2
                median = (median_nums[(counter / 2) + 1] + median_nums[counter / 2]) / 2;

                //Se não (numero de numeros impar)
            } else {

                //mediana (median) vai ser igual ao valor de median_nums 1 posicao depois do meio (median_nums[(counter / 2)+1])
                median = median_nums[(counter / 2) + 1];
            }

            //O tamanho de numeros do primeiro quartil vai ser igual ao numero de numeros da coluna a divir por 2 (counter/2)
            q1_size = counter / 2;

            //O tamanho de numeros do terceiro quartil vai ser igual ao numero de numeros da coluna a divir por 2 +1 (counter/2 +1)
            q3_size = (counter / 2) + 1;

            //Se o resto do numero de numeros desta coluna for igual a 0 (logo numero de numeros par)
            if (counter % 2 == 0) {

                //Ciclo para percorrer os numeros do primeiro quartil
                for (int m = 1; m <= q1_size; m++) {

                    //Primeiro Quartil (q1) vai ser igual ao valor da median_nums 1 posicao a frente da metade do numero total de posicoes (median_nums[(q1_size / 2) + 1]) + o valor de median_nums na posicao da metade do numero de posicoes total (median_nums[q1_size / 2]) e isto tudo vai dividir por 2
                    q1 = (median_nums[(q1_size / 2) + 1] + median_nums[q1_size / 2]) / 2;
                }

                //Ciclo para percorrer os numeros do terceiro quartil
                for (int n = q3_size; n <= counter; n++) {

                    //Terceiro Quartil (q3) vai ser igual ao valor da median_nums 1 posicao a frente da metade do numero total de posicoes (median_nums[(q3_size / 2) + 1]) mais o valor de posicoes do primeiro quartil (+ q1_size)  + o valor de median_nums na posicao da metade do numero de posicoes total (median_nums[q1_size / 2]) mais o valor total de posicoes do primeiro quartil (+ q1_size) e isto tudo vai dividir por 2
                    q3 = (median_nums[((q3_size / 2) + 1) + q1_size] + median_nums[(q3_size / 2) + q1_size]) / 2;

                }


                //Se não (numero de numeros impar)
            } else {


                //Ciclo para percorrer os numeros do primeiro quartil
                for (int m = 1; m <= q1_size; m++) {

                    //Primeiro Quartil (q1) vai ser igual ao numero da median_nums na posicao do meio de primeiro quartil (median_nums[q1_size/2]) + o numero da median_numes 1 posicao depois da posicao do meio (median_nums[(q1_size/2)+1]) e tudo a dividir por 2
                    q1 = (median_nums[q1_size / 2] + median_nums[(q1_size / 2) + 1]) / 2;
                }

                //Ciclo para percorrer os numeros do terceiro quartil
                for (int n = q3_size; n <= counter; n++) {

                    //Terceiro Quartil (q1) vai ser igual ao numero da median_nums na posicao do meio de terceiro quartil (median_nums[q1_size/2]) + o tamanho total de posicoes do terceiro quartil (+ q3_size) + o numero da median_numes 1 posicao depois da posicao do meio (median_nums[(q1_size/2)+1]) + o tamanho total de posicoes do terceiro quartil (+ q3_size) e tudo a dividir por 2
                    q3 = (median_nums[((q3_size / 2) + 1) + q3_size] + median_nums[((q3_size / 2)) + q3_size]) / 2;
                }


            }

            //Escreve os valores
            printf("q1:%.3lf\n", q1);
            printf("mediana: %.3lf\n", median);
            printf("q3:%.3lf\n", q3);

            time_t now;
            time(&now);
            strcpy(tempo, ctime(&now));

            //Guardar os valores na struct analises
            resultado[menu_counter].col = col;
            resultado[menu_counter].soma = 0;
            resultado[menu_counter].media = 0;
            resultado[menu_counter].desvio = 0;
            resultado[menu_counter].max = 0;
            resultado[menu_counter].min = 0;
            resultado[menu_counter].median = median;
            resultado[menu_counter].q1 = q1;
            resultado[menu_counter].q3 = q3;
            resultado[menu_counter].tempo = tempo;

        }

        //O contador de operacoes feitas pelo utilizador (menu_counter) é aumnetado em +1
        menu_counter++;

        //Retorna o valor do resultado (resultado[]) na posicao anterior do contador de operacoes (menu_counter-1)
        return resultado[menu_counter - 1];

    }
}

//FUncao Historico

void historico(struct analise resultado1[], int i,  FILE *ficheiro, char fic[20000]){


    //Escreve todas as informacoes da Struct analises
    printf("Coluna %d\n", resultado1[i].col);
    printf("Soma %.3lf\n", resultado1[i].soma);
    printf("Media %.3lf\n", resultado1[i].media);
    printf("Desvio Padrao %.3lf\n", resultado1[i].desvio);
    printf("Maximo %.3lf\n", resultado1[i].max);
    printf("Minimo %.3lf\n", resultado1[i].min);
    printf("Q1 %.3lf\n", resultado1[i].q1);
    printf("Mediana %.3lf\n", resultado1[i].median);
    printf("Q3 %.3lf\n", resultado1[i].q3);
    printf("%s\n", resultado1[i].tempo);

    printf("\n");


}

//Funcao Lercolunas

void lercolunas(int colunas, int linhas, int totalcol,  FILE *ficheiro, char fic[20000]){

    //Criacao de variaveis
    int i, j, menu_op, coluns=0, numero_col=0;
    char line[2000], c[linhas];
    char *sp;

    ficheiro = fopen(fic, "r");
    if (ficheiro == NULL) {
        perror("Nao foi possivel abrir o ficheiro\n Encerrando o programa...");
        exit(1);
    }

    printf("\n\tLer Colunas\n");
    printf("1-> Todas as Colunas\n");
    printf("2-> Escolher o Numero de Colunas\n");

    printf("->");
    scanf("%d",&menu_op);
    printf("\n");

    //Ler e escrever todas as colunas do ficheiro
    if(menu_op == 1){

        //Ciclo para percorrer as linhas do ficheiro
        for(i=1;i<=linhas;i++){

            //Recebe e lê as linhas do ficheiro se for diferente de NULL
            if((fgets(line, sizeof(line), ficheiro)) != NULL) {

                //Ciclo para percorrer as colunas do ficheiro
                for (j = 1; j <= totalcol; j++) {

                    //Se o numero da coluna atual (j) for igual a 1
                    if (j == 1) {

                        //Apontador sp vai ser igual ao valor da linha até ao ;
                        sp = strtok(line, ";");

                        //Copia o valor do apontador (sp) para uma string c
                        strcpy(c, sp);

                    //Se o nuemro da coluna atual (j) for maior que 1
                    } else if (j > 1) {

                        //Apontador sp vai ser igual ao proximo valor da linha até ao ;
                        sp = strtok(NULL, ";");

                        //Copia o valor do apontador (sp) para uma string c
                        strcpy(c, sp);

                    }

                    //Escreve os valores da string c e cada um desses valores vai ocupar 12 espacos (%-12s)
                    printf("%-12s",c);

                }

                printf("\n");
            }
        }

    }

        //Ler e escrever todas um numero de colunas escolhidas pelo utilizador
    else if (menu_op == 2){

        do {

            //Pergunta ao utilizar quantas linhas quer ver
            printf("Quantas Colunas quer visualizar\n");

            printf("->");
            scanf("%d", &numero_col);
            printf("\n");

            if (numero_col > totalcol || numero_col < 1) {

                printf("Esta coluna nao existe no ficheiro %d o ficheiro tem apenas %d\n", numero_col, totalcol);

            }

            else {
                //Ciclo para percorrer as linhas do ficheiro
                for (i = 1; i <= linhas; i++) {

                    //Recebe e lê as linhas do ficheiro se for diferente de NULL
                    if ((fgets(line, sizeof(line), ficheiro)) != NULL) {

                        //Ciclo para percorrer as colunas do ficheiro
                        for (j = 1; j <= numero_col; j++) {

                            //Se o numero da coluna atual (j) for igual a 1
                            if (j == 1) {

                                //Apontador sp vai ser igual ao valor da linha até ao ;
                                sp = strtok(line, ";");

                                //Copia o valor do apontador (sp) para uma string c
                                strcpy(c, sp);


                                //Se o nuemro da coluna atual (j) for maior que 1
                            } else if (j > 1) {

                                //Apontador sp vai ser igual ao proximo valor da linha até ao ;
                                sp = strtok(NULL, ";");

                                //Copia o valor do apontador (sp) para uma string c
                                strcpy(c, sp);

                            }

                            //Escreve os valores da string c e cada um desses valores vai ocupar 12 espacos (%-12s)
                            printf("%-12s", c);

                        }

                        printf("\n");
                    }
                }
            }

        } while (numero_col > totalcol || numero_col < 1);

    }

}

//Funcao Escrever_historico

void escrever_historico(FILE  *fic_historico, char  ler_hist[20000],FILE *ficheiro, struct analise resultado1[], int i){

    //Escreve no ficheiro
    fprintf(fic_historico,"Coluna %d\n", resultado1[i].col);
    fprintf(fic_historico,"Soma %.3lf\n", resultado1[i].soma);
    fprintf(fic_historico,"Media %.3lf\n", resultado1[i].media);
    fprintf(fic_historico,"Desvio Padrao %.3lf\n", resultado1[i].desvio);
    fprintf(fic_historico,"Maximo %.3lf\n", resultado1[i].max);
    fprintf(fic_historico,"Minimo %.3lf\n", resultado1[i].min);
    fprintf(fic_historico,"Q1 %.3lf\n", resultado1[i].q1);
    fprintf(fic_historico,"Mediana %.3lf\n", resultado1[i].median);
    fprintf(fic_historico,"Q3 %.3lf\n", resultado1[i].q3);
    fprintf(fic_historico,"%s\n", resultado1[i].tempo);

    fprintf(fic_historico,"\n");

    //Fechar o ficheiro
    fclose(fic_historico);

}

//Funcao Pesquisa_historico

void pesquisa_historico( char  ler_hist[20000]){

    //Criacao de variáveis
    char wrd[256], buffer[256],teste[256];
    int n, m, i, j, line;

    //Abertura do ficheiro
    FILE* fp;
    fp = fopen(ler_hist, "r");

    printf("Escreva o que quer pesquisar");
    scanf("%s",wrd);

    //Tamanha da palavra inserida
    m = strlen(wrd);

    printf("Pesquisa de \"%s\" no ficheiro \n", wrd);

    line = 0;

    //Lê as linhas do ficheiro
    while (fgets(buffer, 256, fp) != NULL) {

        i = 0;
        n = strlen(buffer);

        //Ciclo le as palavras do ficheiro e faz uma comparação com a palavra que o utilizador inserio
        while (i < n) {

            j = 0;

            //Ciclo parta comprar a palavra atual com a palavra inserida pelo utilizador
            while (i < n && j < m && buffer[i] == wrd[j]) {
                ++i, ++j;
            }


            //Se as duas palavras forem iguais
            if ((i == n || buffer[i] == ' ' || buffer[i] == '\n') && j == m) {
                printf("Linha: %d ", line);

                printf("%s",buffer);
            }

            //proxima palavra
            while (i < n && buffer[i] != ' ') {
                ++i;
            }
            ++i;

        }

        ++line;

    }

    fclose(fp);

}