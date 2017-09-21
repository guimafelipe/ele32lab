#include <map>
#include <cstdio>
#include <iostream>
#include <utility>
#include <cmath>
#include <cstring>

using namespace std;

map<char, double> dic;
int contLetras = 0;
map<char, map<char, double> > matriz;

//Função para adicionar um novo caractere, com frequência 0, no mapa
void addDicionario( char c){
        dic.insert(make_pair(c, 0));
}

//Função que aumenta no dicionário o valor relacionado ao número de vezes
//que um caractere apareceu
void contarLetra( char c){
    if(dic.find(c) == dic.end()){
        addDicionario( c);
    }
    dic[c] += 1;
}

//Função que processa os caracteres de uma string fazendo a contagem
void processarLetra( char* str){
    for(int i = 0; i < strlen(str); i++){
        contarLetra(str[i]);
    }
}

int main(){
    FILE* texto;
    char linha[250], nomedotexto[15], ctxt, c;
    dic.clear();
    printf("Diga o idioma a ser lido (P, E, F ou D):"); //Fazendo a escolha do idioma do texto
    scanf(" %c", &ctxt);
    switch(ctxt){
        case 'D':
        strcpy(nomedotexto, "deutsch.txt");
        break;
        case 'E':
        strcpy(nomedotexto, "english.txt");
        break;
        case 'P':
        strcpy(nomedotexto, "portugues.txt");
        break;
        case 'F':
        strcpy(nomedotexto, "francais.txt");
        break;
        default :
        break;
    }
    
    texto = fopen(nomedotexto, "r");

    map<char, double>::iterator it;
    
    printf("comecando operacao:\n");
    //Beginning of the operation
    while(fscanf(texto, "%c", &c) > 0){
        contarLetra(c);
        contLetras++; //Faz a contagem das letras
    }
    for(it = dic.begin(); it != dic.end();it++){
        it->second /= contLetras;
    }
    
    //Beginning of the calculation of the H(x)
    float hdex = 0;
    
    for(it = dic.begin(); it != dic.end();it++){
        hdex -= it->second * log2(it->second);
    }
    
    printf("H(X) = %f\n", hdex);
    
    fclose(texto);
    
    matriz.clear();
    
    //Começo do cálculo de H(X|Y)S
    for(it = dic.begin(); it != dic.end(); it++){
        texto = fopen(nomedotexto, "r");
        char chant;
        fscanf(texto, " %c", &chant);
        int counter = 0;
        while(fscanf(texto, "%c", &c) > 0){
            
            if(chant == it->first){
                counter++;
                if(matriz.find(chant) == matriz.end()){
                    matriz[chant][c] = 1;
                    
                }
                else if (matriz[chant].find(c) == matriz[chant].end()){
                    matriz[chant][c] = 1;
                    
                } else {
                    matriz[chant][c]++;
                    
                }
            }
            chant = c;
        }
        //transformar em probabilidades:
        map<char, double>::iterator itaux;
        
        for(itaux = (matriz[it->first]).begin(); itaux != (matriz[it->first]).end(); itaux++){
            (itaux->second) /= counter;
        }
        
        fclose(texto);
    }
    
    map<char, map<char, double> >::iterator it1;
    
    double bagulhoTotal = 0;
    
    for(it1 = matriz.begin(); it1 != matriz.end(); it1++){
        double bagulho = 0;
        for(it = (it1->second).begin(); it != (it1->second).end(); it++){
            bagulho -= it->second*log2(it->second);
        }
        bagulho *= dic[(it1->first)];
        bagulhoTotal += bagulho;
    }
    
    printf("H(X|Y) = %f\n", bagulhoTotal);
    
    return 0;
}
