#include <iostream>
#include <stdlib.h>
#include <fstream>                          
#include <vector>                           
#include <sstream>                          

using namespace std;

void aluno(){
	cout<<"\n\t----------------------------------------------------------";
	cout<<"\n\tSISTEMAS OPERACIONAIS - SO (2019.1)";
	cout<<"\n\tALUNO: Helter Yordan";
	cout<<"\n\n\t\t>>>> Trabalho de Implementacao 02 <<<<\n";
	cout<<"\n\n\t    	Desenvolver um simulador de paginacao\n";
	cout<<"\t----------------------------------------------------------\n\n"<<endl;	
}

int main()
{
   //MANIPULAÇÃO DE ARQUIVO --------------------------
   ifstream arquivo;                       
   stringstream output;                     
   //-------------------------------------------------
   
   //FLAGs --------
   int flag=0;                              //verifica se a pagina esta ou não no quadro
   int cheio=0;                             //avisa quando o quadro esta cheio
   int hit = 0;                             //flag binaria
	//-------------

   int j;
   int i = 0;
   int k;
   int m;

   int faltaPagina_fifo = 0;                //contador do FIFO
   int faltaPagina_otm = 0;					//contador do OTM
   int faltaPagina_lru = 0;					//contador do LRU
   int pag = 0;                             //auxiliar para passar do arqruivo para o vetor
   int tam_quadro;                          //primeira posicão do arquivo é armazanada (tamanho do quadro)
   int vitima;                              //pagina que vai sair
	
   vector<int> paginas;                     //criando os vetores que vamos utilizar todos com 0 elementos iniciais
   vector<int> quadros;
   vector<int> seq_pag_acess;
   vector<int> fifo;                        
   vector<int> otm;
   vector<int> lru;

   int tam;                                 //armazenamos o tamanho do vetor
   string nome_arq;							//coletando nome do arquivo de entrada informado pelo usuario
   
   //---- MEUS DADOS ---
   aluno();
   //-------------------

//--------------------- LER ARQUIVO E SALVANDO NO VETOR ----------------------------------

	cout << "\n\tInforme o nome do arquivo de entrada: ";
	cin >> nome_arq;

	arquivo.open(nome_arq);                //abrindo arquivo

    while(!arquivo.eof()){                  //vai lendo linha a linha até acabar o arquivo
        arquivo >> pag;
        paginas.push_back(pag);             //acrescenta ao final do vetor de paginas o valor da pagina
        i++;                                //incrementando para vermos o tamanho
    }

    arquivo.close();                        //fechando arquivo

    for ( j=0; j < i ; j++ ){

        if ( j == 0){
            tam_quadro = paginas[j];        //pegando a posição 0 e salvando no tamanho
            quadros.resize(tam_quadro);     //alterando o tamanho do quadro
        }
        else
            seq_pag_acess.push_back(paginas[j]);
    }

    i--;                                    //filtrando a primeira posição
    tam = i;                                //coletando os acessos aos quadros

    fifo = seq_pag_acess;                   //salvando a sequencia de acesso para cada algoritmo
    otm = seq_pag_acess;
    lru = seq_pag_acess;

//------------------------------------------------------------------------------------------


//-------------------------------- FIFO ----------------------------------------------------
    i = 0;                                  //elemento atual
    j = 0;                                  //proximo elemento no quadro
    k = 0;                                  //contador FIFO

    vector<int>::iterator v = fifo.begin();

    while( v != fifo.end() ){               //while até o termino do vetor FIFO

        for( m = 0; m < tam_quadro ; m++){  //verificando a existencia da pagina no quadro

          if( fifo[k] != quadros[m]){
                flag++;                     //Scaso não esteja incrementa o contador

          }else if( (fifo[k] == 0) && (cheio < tam_quadro) ){       //tratando o caso de uma pagina com 0 esta tentando entrar no quadro
                flag++;                                             
          }
        }

        if( (flag == tam_quadro) && (cheio < tam_quadro) ){         //executando até encher o quadro
            quadros[j] = fifo[k];                                   //inserindo a pagina no quadro
            faltaPagina_fifo++;
            cheio++;

        }else if( (flag == tam_quadro) && (cheio >= tam_quadro) ){  //verifica quando esta cheio o quadro e executa
            quadros[vitima] = fifo[k];                              //quadro "vitima" vai receber a pagina da vez
            faltaPagina_fifo++;
            vitima++;
            cheio++;
        }

        if( cheio == tam_quadro ){          //se o quadro encheu escolhemos a primeira pagina a entrar no quadro
            vitima = 0;
        }

        if( vitima == tam_quadro ) {        //chegando ao final do quadro aponta vitima para o começo
            vitima = 0;
        }

        j = (i+1) % tam_quadro;             //apontando para o proximo elemento de forma ciclica

        if( i == (tam_quadro-1) ){
            i=0;                            //ao chegar no final aponta par ao começo

        }else
            i++;                            

        v++;                                //contador do WHILR
        k++;                                //contador do FIFO
        flag = 0;                           //zerando a FLAG
   }

//-----------------------------------------------------------------------------------------


//--------------------------------- OTM ---------------------------------------------------
   for ( m = 0 ; m < tam_quadro ; m++){      //adicionando flag aos elementos do quadro
        quadros[m] = 93;
    }

    int aFrente[tam_quadro];                 //vetor para calcular a distancia para frente

    for( i = 0; i < tam; i++ ){              //analisando ate o tamanho do vetor de OTM
        hit = 0;

        for( j = 0; j < tam_quadro ; j++ ){  //varrendo o quadro a procura da pagina

            if( quadros[j] == otm[i] ){      //a pagina estando no quadro a gente adiciona na flag
                hit = 1;                     //caso a pagina já esteja no quadro pulamos para a proxima
                break;                       
            }
        }

        if( hit == 0 ){                      //tratando a falta de paginas

            for( j = 0; j < tam_quadro; j++ ){  //analisando quadro a quadro
                int pg = quadros[j];            //cada elemento do quadro com OTM
                int found = 0;

                for( k = i; k < tam; k++ ){

                    if( pg == otm[k] ){         //caso a pagina esteja no quadro salva a distancia no vetor afrente comparando com o vetor que quer entrar
                        aFrente[j] = k;         
                        found = 1;              //seta a flag ENCONTRAR
                        break;                  //sai do for quando encontrar o elemento que esta no quadro e no OTM salvando a distancia a frente
                                                
                    }else
                        found=0;                
                }
                if( !found )
                    aFrente[j] = 93;
            }

            int max = -93;                     
            int index;

            for( j = 0; j < tam_quadro; j++ ){  //inserindo no quadro um de cada vez
                                                //comparando e achando o vetor referente no quadro mais longe de onde quer entrar
                if( aFrente[j] > max ){
                    max = aFrente[j];           //verificando a maior distancia
                    index = j;
                }
            }

            quadros[index] = otm[i];            //inserindo elemtno no quadro
            faltaPagina_otm++;
        }
    }
//------------------------------------------------------------------------------------------


//------------------------------------- LRU ------------------------------------------------
    for ( m=0 ; m < tam_quadro ; m++){          //adicionando flag aos elementos do quadro
        quadros[m] = 93;
    }

    int aAtras[tam_quadro];

    for( i = 0; i < tam; i++ ){					//verificando até o tamanho do vetor
        hit = 0;

        for( j = 0; j < tam_quadro; j++){		//verifica se a pagina esta no quadro

            if( quadros[j] == lru[i] ){			//vai analisando pagina por pagina e verificando no quadro
                hit = 1;						
                break;							
            }
        }

        if( hit == 0 ){							//tratando a falta de paginas

            for( j = 0; j < tam_quadro; j++ ){	//analisando quadro a quadro
                int pg = quadros[j];			//cada elemento do quadro com LRU
                int found = 0;

                for( k = i-1; k >= 0; k--){

                    if( pg == lru[k] ){			//verifica se a pagina esta no quadro
                        aAtras[j] = k;			//salvando distancia do vetor atras comparando com vetor que vai entrar
                        found = 1;				
                        break;					//sai do for assim que encontra o elemento no quadro e no LRU
                        						
                    }else
                        found=0;				
                }

                if( !found )
                    aAtras[j] = -93;
            }

            int min = 93;						
            int index;

            for( j = 0; j < tam_quadro; j++ ){	//inserindo no quadro um de cada vez
            									//comparando e achando o vetor referente no quaro mais longe de onde quer entrar
                if( aAtras[j] < min ){
                    min = aAtras[j];			//procurando no vetor o de maior distancia
                    index = j;
                }
            }

            quadros[index] = lru[i];			//inserindo elemento no quadro
            faltaPagina_lru++;
        }
    }
//--------------------------------------------------------------------------------------------

//-------------------------- PRINTANDO RESULTADOS NA TELA --------------------------------

    cout << endl;

    cout   << "FIFO" << " " << faltaPagina_fifo << endl;
    output << "FIFO" << " " << faltaPagina_fifo << endl;
    cout   << "OTM"  << " " << faltaPagina_otm  << endl;
    output << "OTM"  << " " << faltaPagina_otm  << endl;
    cout   << "LRU"  << " " << faltaPagina_lru  << endl;
    output << "LRU"  << " " << faltaPagina_lru  << endl;

    cout << endl;

    //SALVA OUTPUT NO .TXT
    ofstream saida("saida.txt", ofstream::out);
    saida << output.str();

    return 0;
}
//----------------------------------------------------------------
