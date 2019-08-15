/*-------------------------------------------------------------

DISCIPLINA: Sistemas Operacionais
ALUNO: Helter Yordan Alves da Costa
MATRICULA: 11406573

-----------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <vector>
#include <stdlib.h>

using namespace std;
class BRL: public numpunct<char>{
	protected:
		virtual char do_decimal_point() const {
			return ',';
		}
};

int main(){
	
	//************************************************************	CRIAÇÃO DAS VARIAVEIS ****************************************************************
	//--------------- ARQUIVOS
	ifstream doc; // ler arquivo
	stringstream saida; // Cria um obeto string para ser manipulado
	//------------------------
	
	int i = 0, j, n, z, k, x, minimo;
	int troca1, troca2, erro, tam, saida_rr, espera, soma_chegada;	
	
	//-------------- FLAGS 
	// para tratar o tempo de chegada quando diferente de "0" e ordenar o tempo de pico no SJF
	int flag1 = 0;
	int flag2 = 0;
	//--------------------
	
	//--------------- VARIAVEIS P/ TEMPO
	int tempo = 0;
	int aux1 = 0;
	int aux2 = 0;
	int contador_seg = 0;
	//----------------------------------
	
	//---------------- VARIAVEIS P/ MEDIA DOS TEMPOS
	// - FCFS -
	double media_fcfs_retorno = 0;
	double media_fcfs_resposta = 0;
	double media_fcfs_espera = 0;
	//----------
	
	// - SJF -
	double media_sjf_retorno = 0;
	double media_sjf_resposta = 0;
	double media_sjf_espera = 0;
	//----------
	
	// - RR -
	double media_rr_retorno = 0;
	double media_rr_resposta = 0;
	double media_rr_espera = 0;
	//----------
	//----------------------------------------------
	
	//--------------- VETORES DE ARMAZENAMENTO
	vector<int> tempo_pico;
	vector<int> tempo_chegada;
	vector<int> tempo_total;
	
	// - FCFS -
	vector<int> tempo_pico_fcfs;
	vector<int> tempo_chegada_fcfs;
	//----------
	
	// - SJF -
	vector<int> tempo_pico_sjf;
	vector<int> tempo_chegada_sjf;
	//----------
	
	// - RR -
	vector<int> tempo_pico_rr;
	vector<int> tempo_chegada_rr;
	//----------
	//----------------------------------------
	//************************************************************************************************************************************************


	//********************************************* SALVANDO OS DADOS DO ARQUIVO DE ENTRADA NO VETOR *************************************************
	doc.open("entrada.txt");						// Abre o arquivo
	
	while(!doc.eof()){                              // While ate acabar o arquivo
        doc >> tempo;
        tempo_total.push_back(tempo);               // Acrescente ao final de "TEMPO_TOTAL" o valor de tempo
        i++;                                            
    }
    doc.close();                                    // Fecha o arquivo

    for( j = 0; j < i; j++){                        // Preenche os vetores de tempo
         if( j%2 == 0 )
            tempo_chegada.push_back(tempo_total[j]);
        else                                            
            tempo_pico.push_back(tempo_total[j]);
    }
	
	tempo_pico_fcfs = tempo_pico;                           // Salvando os valores em vetores de apoio (FCFS)
    tempo_chegada_fcfs = tempo_chegada;						//
    tempo_pico_sjf = tempo_pico;                            // Salvando os valores em vetores de apoio (SJF)
    tempo_chegada_sjf = tempo_chegada;						//
    tempo_pico_rr = tempo_pico;                             // Salvando os valores em vetores de apoio (RR)
    tempo_chegada_rr = tempo_chegada;						//
    tam = i/2;												// Salvando tam do arquivo
    //*************************************************************************************************************************************************
    
    
    //***************************************************  ESCALONADOR -> FCFS ************************************************************************
    
	//	FCFS - aqui o primeiro que chega é o primeiro que sai, ou seja, o primeiro processo que chegar será o primeiro a ser executado
    
	// ------------------ OBTENDO O TEMPO DE RETORNO
    
	// primeiramente vamos verificar a situação de erro onde vamos utilizar da flag que foi criada para sinalizar e tratar
	
	for (j = 0; j < tam; j++){
    	if(j == 0){
    		if(tempo_chegada_fcfs[j] == 0){	// OK
    			flag1 = 0;
			}
			else{	// ERRO
				flag1 = 1;
			}
			aux1 = tempo_pico_fcfs[j] - tempo_chegada_fcfs[j];
			media_fcfs_retorno = media_fcfs_retorno + aux1;
		}
		else{
			if((tempo_pico_fcfs[j-1] != aux1) && (flag1 == 1)){ //corrigindo o erro caso o tempo de chegada não seja igual a "0"
				erro = tempo_pico_fcfs[0] - aux1;
				aux1 = aux1 + erro;
				flag1 = 0;
			}
			aux2 = aux1 + tempo_pico_fcfs[j];
			aux1 = aux1 + tempo_pico_fcfs[j];
			aux2 = aux2 - tempo_chegada_fcfs[j];
			media_fcfs_retorno = media_fcfs_retorno + aux2;
		}
	}
	media_fcfs_retorno = media_fcfs_retorno / tam; // obtendo o valor da media final de retorno
	//--------------------------------------------------------------
	    
	//---------------------------------- OBTENDO O TEMPO DE RESPOSTA
	aux2 = 0;
	for(j = 0; j < tam; j++){
		if (j == 0){
			if(tempo_chegada_fcfs[j] != 0){ // se o tempo de chegada não for "0" sera corrigido
				tempo_chegada_fcfs[j] = 0;
			}
			media_fcfs_resposta = media_fcfs_resposta + tempo_chegada_fcfs[j];
			aux1 = tempo_pico_fcfs[j];
		}
		else{
			aux2 = aux1 - tempo_chegada_fcfs[j];
			aux1 = aux1 + tempo_pico_fcfs[j];
			media_fcfs_resposta = media_fcfs_resposta + aux2;
		}		
	}
	media_fcfs_resposta = media_fcfs_resposta / tam; //obtendo a media dos tempos de resposta
	//----------------------------------------------------------------
	
	//----------------------------------- OBTENDO O TEMPO DE ESPERA
	aux2 = 0;
	for(j=0; j<tam; j++){
		if(j==0){
			if(tempo_chegada_fcfs[j] != 0){ // se o tempo de chegada não for "0" sera corrigido
				tempo_chegada_fcfs[j] = 0;
			}
			media_fcfs_espera = media_fcfs_espera + tempo_chegada_fcfs[j];
			aux1 = tempo_pico_fcfs[j];
		}
		else{
			aux2 = aux1 - tempo_chegada_fcfs[j];
			aux1 = aux1 + tempo_pico_fcfs[j];
			media_fcfs_espera = media_fcfs_espera + aux2;
		}
	}
	media_fcfs_espera = media_fcfs_espera / tam; // obtendo a media dos tempos de resposta 
	//-------------------------------------------------------------
     
    //*************************************************************************************************************************************************

    
    //***************************************************  ESCALONADOR -> SJF ************************************************************************
    
    // Seleciona o processo com MENOR tempo de execução para ser executado primeiro, para tal, precisamos ordenar nosso vetor de apoio com os tempo em ordem do menor tempo de execução para o maior.
    
    // --------------------------------- ORDENANDO O TEMPO DE CHEGADA
    
    //aqui vamos organizar o vetor com os tempos de pico e em seguida verificar o menor tempo de pico pra ir executando.
    //logo em seguida re-organizar apos a exacução do menor
    
    for (z = 0; z<(tam -1); z++){
    	minimo = z;
    	for(j = (z+1); j< tam; j++){
    		if(tempo_chegada_sjf[j] < tempo_chegada_sjf[minimo]){
    			minimo = j;
    			troca1 = tempo_chegada_sjf[minimo];
    			tempo_chegada_sjf[minimo] = tempo_chegada_sjf[z];
    			tempo_chegada_sjf[z] = troca1;
    			troca2 = tempo_pico_sjf[minimo];
    			tempo_pico_sjf[minimo] = tempo_pico_sjf[z];
    			tempo_pico_sjf[z] = troca2;
			}
			else if(tempo_chegada_sjf[j] == tempo_chegada_sjf[minimo]){
				if(tempo_pico_sjf[j] < tempo_pico_sjf[minimo]){
					minimo = j;
					troca1 = tempo_chegada_sjf[minimo];
					tempo_chegada_sjf[minimo] = tempo_chegada_sjf[z];
					tempo_chegada_sjf[z] = troca1;
					troca2 = tempo_pico_sjf[minimo];
					tempo_pico_sjf[minimo] = tempo_pico_sjf[z];
					tempo_pico_sjf[z] = troca2;
				}
			}
		}
	}    
    //----------------------------------------------------------------

    // -------------------------------------- OBTENDO TEMPO DE RETORNO
    
	//tratando erro
	for(j=0; j<tam; j++){
    	if(j==0){
    		if(tempo_chegada_sjf[j] == 0){
    			flag1 = 0;
			}
			else{
				flag1 = 1;
			}
			aux1 = tempo_pico_sjf[j] - tempo_chegada_sjf[j];
			media_sjf_retorno = media_sjf_retorno + aux1;
		}
		else {
			if((tempo_pico_sjf[j-1] != aux1) && (flag1 == 1)){
				erro = tempo_pico_sjf[0] - aux1;
				aux1 = aux1 + erro;
				flag1 = 0;
			}
			if(flag2 == 0){
				for(z = 1; z < tam; z++){
					minimo = z;
					for(n = z+1; n<tam; n++){
						if(tempo_pico_sjf[n] < tempo_pico_sjf[minimo]){
							minimo = n;
						}
					}
					troca1 = tempo_pico_sjf[minimo];
					tempo_pico_sjf[minimo] = tempo_pico_sjf[z];
					tempo_pico_sjf[z] = troca1;
					troca2 = tempo_chegada_sjf[minimo];
					tempo_chegada_sjf[minimo] = tempo_chegada_sjf[z];
					tempo_chegada_sjf[z] = troca2;
				}
				flag2 = 1;
			}
			
			contador_seg = contador_seg + tempo_pico_sjf[j-1]; //montando uma linha de tempo para comparar, caso tenha chegado a vez do programa executa, caso contrario executa o proximo na vez
			
			if(tempo_chegada_sjf[j] < contador_seg){
				aux2 = aux1 + tempo_pico_sjf[j];
				aux1 = aux1 + tempo_pico_sjf[j];
				aux2 = aux2 - tempo_chegada_sjf[j];
				media_sjf_retorno = media_sjf_retorno + aux2;
			}
			else{
				for( z=j; z < (tam-1); z++){
					minimo = z;
					for(int h = (z+1); h < tam; h++){ //vamos organizar por tempo de pico
						if(tempo_chegada_sjf[h] < tempo_chegada_sjf[minimo]){
							minimo = h;
							troca1 = tempo_chegada_sjf[minimo];
							tempo_chegada_sjf[minimo] = tempo_chegada_sjf[z];
							tempo_chegada_sjf[z] = troca1;
							troca2 = tempo_pico_sjf[minimo];
							tempo_pico_sjf[minimo] = tempo_pico_sjf[z];
							tempo_pico_sjf[z] = troca2;
						}
						else if(tempo_chegada_sjf[h] == tempo_chegada_sjf[minimo]){
							if(tempo_pico_sjf[h] < tempo_pico_sjf[minimo]){
								minimo = h;
								troca1 = tempo_chegada_sjf[minimo];
								tempo_chegada_sjf[minimo] = tempo_chegada_sjf[z];
								tempo_chegada_sjf[z] = troca1;
								troca2 = tempo_pico_sjf[minimo];
								tempo_pico_sjf[minimo] = tempo_pico_sjf[z];
								tempo_pico_sjf[z] = troca2;
							}
						}
					}
				}
				//em seguida fazemos os calculos com o novo processo que veio ocupar a posição
				aux2 = aux1 + tempo_pico_sjf[j];
				aux1 = aux1 + tempo_pico_sjf[j];
				aux2 = aux2 - tempo_chegada_sjf[j];
				media_sjf_retorno = media_sjf_retorno + aux2;
			}
		}
	}
	
	media_sjf_retorno = media_sjf_retorno / tam;
    //----------------------------------------------------------------
    
    //--------------------------------------------- OBTENDO TEMPO DE RESPOSTA
    aux2 = 0;
    for(j = 0; j < tam; j++){
    	if(j==0){
    		if(tempo_chegada_sjf[j] != 0){
    			tempo_chegada_sjf[j] = 0;
			}
			media_sjf_resposta = media_sjf_resposta + tempo_chegada_sjf[j];
			aux1 = tempo_pico_sjf[j];
		}
		else{ //vamos somando o tempo de pico
			aux2 = aux1 - tempo_chegada_sjf[j];
			aux1 = aux1 + tempo_pico_sjf[j];
			media_sjf_resposta = media_sjf_resposta + aux2;
		}
	}
	media_sjf_resposta = media_sjf_resposta / tam;
    //-----------------------------------------------------------------------
    
    //---------------------------------------------- OBTENDO TEMPO DE ESPERA
    aux2 = 0;
    for(j=0; j<tam; j++){
    	if(j==0){
    		if(tempo_chegada_sjf[j] != 0){
    			tempo_chegada_sjf[j] = 0;
			}
			media_sjf_espera = media_sjf_espera + tempo_chegada_sjf[j];
			aux1 = tempo_pico_sjf[j];
		}
		else{
			aux2 = aux1 - tempo_chegada_sjf[j];
			aux1 = aux1 + tempo_pico_sjf[j];
			media_sjf_espera = media_sjf_espera + aux2;
		}
	}
	media_sjf_espera = media_sjf_espera / tam;
	
    //----------------------------------------------------------------------
    //*************************************************************************************************************************************************
    
    //**********************************************************  ESCALONADOR -> RR  ******************************************************************
    
    // RR - aqui os processos são manipulados sem uma prioridade e sim por um intervalo de tempo que é determinado por "quantum"
    // aqui no nosso codigo vamos trabalhar com "quantum = 2"
    
    j = 0;
    k = 0;
    x = 0;
    int flag = 0;
    int tempo_duracao = 0;
    saida_rr = 0;
    espera = 0;
    double tempo_resposta_rr;
    double tempo_retorno_rr;
    double tempo_espera_rr;
    int array_fila_duracao_rr[tam] = {0};
    int array_fila_processo_rr[tam] = {0};
    int array_espera_duracao_rr[tam] = {0};
    int array_espera_processo_rr[tam] = {0};
    int array_processo[tam] = {0};
    int array_saida_rr[tam] = {0};
    int array_aux[tam] = {0};
    int array_chegada[tam] = {0};
    
    for( i = 0; i <tam; i++){
    	array_processo[i] = i;
	}
	
	while ( (x<tam) || (j>0) || (espera>0) ){
		while( (k >= tempo_chegada_rr[x]) && (x < tam)){
			array_fila_duracao_rr[j] = tempo_pico_rr[x];
			array_fila_processo_rr[j] = array_processo[x];
			x++;
			j++;
		}
		
		if (espera > 0){
			array_fila_duracao_rr[j] = array_espera_duracao_rr[espera - 1];
 			array_fila_processo_rr[j] = array_espera_processo_rr[espera - 1];
 			j++;
 			espera--;
		}
		
		if (j>0){
			array_saida_rr[saida_rr] = array_fila_processo_rr[0];
			j--;
			saida_rr++;
			
			if(array_aux[array_fila_processo_rr[0]-1] == 0){
				array_aux[array_fila_processo_rr[0]-1] = 1;
				tempo_resposta_rr = tempo_resposta_rr + k;
			}
			
			if(array_fila_duracao_rr[0] > 2){
				array_espera_duracao_rr[espera] = array_fila_duracao_rr[0] - 2;
				array_espera_processo_rr[espera] = array_fila_processo_rr[0];
				espera ++;
				k = k+2;
			}
			else{
				if(array_fila_duracao_rr[0] == 2){
					k = k+2;
				}
				if(array_fila_duracao_rr[0] < 2){
					k++;
				}
				tempo_retorno_rr = tempo_retorno_rr + k;
			}
			
			for( i=0; i <=j; i++){
				array_fila_duracao_rr[i] = array_fila_duracao_rr[i+1];
				array_fila_processo_rr[i] = array_fila_processo_rr[i+1];
			}
		}
		else{
			k ++;
		}
	}
	
	for(i = 0; i <=j; i++){
		soma_chegada = soma_chegada + array_chegada[i];
	}
	
	tempo_resposta_rr = (tempo_resposta_rr - soma_chegada)/tam ;
  	tempo_retorno_rr = (tempo_retorno_rr - soma_chegada);
  	tempo_espera_rr = (tempo_retorno_rr - tempo_duracao)/ tam;
  	tempo_retorno_rr = tempo_retorno_rr / tam;
	    
    //*************************************************************************************************************************************************
    
    //***********************************************  CRIANDO ARQUIVO E IMPRIMINDO RESULTADO  ********************************************************
    locale br(locale(), new BRL());
    cout.imbue(br);
    saida.imbue(br);
    
    cout.precision(1);
    saida.precision(1);
   
    cout << endl;
    
    cout << "FCFS" <<" "<< fixed << media_fcfs_retorno << " " << media_fcfs_resposta << " " << media_fcfs_espera << endl;
    saida << "FCFS" <<" "<< fixed << media_fcfs_retorno << " " << media_fcfs_resposta << " " << media_fcfs_espera << endl;
    cout << "SJF" << " " << fixed << media_sjf_retorno << " " << media_sjf_resposta << " " << media_sjf_espera << endl;
    saida << "SJF" << " " << fixed << media_sjf_retorno << " " << media_sjf_resposta << " " << media_sjf_espera << endl;
    cout << "RR" << " " << fixed << tempo_retorno_rr << " " << tempo_resposta_rr << " " << tempo_espera_rr << endl;
    saida << "RR" << " " << fixed << tempo_retorno_rr << " " << tempo_resposta_rr << " " << tempo_espera_rr << endl;

    cout << endl;
    
    ofstream resposta("resultado.txt", ofstream::out);
    resposta << saida.str();
    
    
    
    return 0;
    //*************************************************************************************************************************************************
    
    
    
    
    
    
    
    
    
}
