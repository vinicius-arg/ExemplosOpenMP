/* Arquivo:  
 *    pth_lembrete.c
 *
 * Propósito:
 *    Implementar um gerenciador de lembretes de medicamentos usando 
 *    openMP. Cada thread fica responsável por lembrar o usuário de
 *    um único medicamento.
 *
 * Input:
 *    nenhum
 * Output:
 *    Mensagens de cada thread lembrando o usuário de tomar o medicamento.  
 *
 * Compile:  gcc -g -Wall -fopenmp -o omp_lembrete omp_lembrete.c
 * Usage:    ./omp_lembrete 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

typedef struct Medicacao{
   char nome[50];
   int intervalo;
   int total;
} Medicacao;

/* OPENMP GERA CÓDIGO QUE CRIA THREADS POR #PRAGMA */

void Lembrete(Medicacao* med);  /* Thread function */
void Constructor_Medicacao(Medicacao* m, char* nome, int intervalo, int total);

int main(int argc, char* argv[]) {
    int thread_count = 4;
    
    char nomes[][20] = { "Parecetamol", "Dorflex", "Cataflan", "B12" };
    int intervalos[] = { 8, 6, 12, 24 };
    int total[] = {10, 12, 8, 7};
    
    printf("Gerenciador de lembretes iniciado!!\n");
    printf("====================================\n");
    
    /* Define um vetor para thread_count medicações */
    Medicacao *m = (Medicacao*)malloc(sizeof(Medicacao) * thread_count);
    
    for (int i = 0; i < thread_count; i++) 
    {
        Constructor_Medicacao(&m[i], nomes[i], intervalos[i], total[i]);
    }
    
    #pragma omp parallel for num_threads(thread_count)
        for (int i = 0; i < thread_count; i++) {
            Lembrete(&m[i]);
        }
    
    /* 
        PARÂMETROS DE OUTROS #PRAGMA SÃO ACEITOS INDEPENDENTE DA DIRETRIZ 
        Exemplo: Pode-se usar num_threads em parallel sections.
    */
    
    printf("=====================================\n");
    printf("Gerenciador de lembretes finalizado!!\n");
    return 0;
}  /* main */

void Lembrete(Medicacao* med) {
   int i;
   time_t t;   
   
   for  (i = 1; i <= med->total; i++){
      time(&t);
      printf("Tomar %s %d/%d ## %s", med->nome, i, med->total, ctime(&t));
      sleep(med->intervalo);
   }
   
   printf("FIM: %s\n", med->nome);
   
   free(med);
}  /* Lembrete */

void Constructor_Medicacao(Medicacao* m, char* nome, int intervalo, int total) {
    strcpy(m->nome, nome);
    m->intervalo = intervalo;
    m->total = total;
}