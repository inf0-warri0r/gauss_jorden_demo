#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
*structure containing a step of the calculation.
*/
typedef struct step{
	char *s;
	struct step *next;
	struct step *back;
} step;

double **m;

step *start = NULL;
step *last  = NULL;
step *curnt = NULL;


/*
*allocating memory for the metrix
*/
void alloc_m(int r){
	m = (double **)malloc(sizeof(double *) * r);
	int l = 0;
	for(l = 0; l < r; l++){
		m[l] = (double *)malloc(sizeof(double) * (r + 1));
	}
}
/*
*convert a step into a char buffer.
*/
void record_step(char *buf, int r){
	int i, j;
	strcpy(buf, "");
	for(i = 0; i < r; i++){
		strcat(buf, "|");
		for(j = 0; j < r + 1; j++){
			char s[20];
			sprintf(s, "%f,", m[i][j]);
			char tmp[20 - strlen(s)];
			memset(tmp, ' ', 20 - strlen(s));
			if(j == r)
				strcat(buf, "|");
			strcat(buf, s);
			
		}
		strcat(buf, "|\n");
	}
}
/*
*swap a raw in a metrix.
*/
void swap(double *a, double *b, int c){
	float tmp[c];
	int j;
	for(j = 0; j < c; j++) tmp[j] = a[j];
	for(j = 0; j < c; j++) a[j] = b[j];
	for(j = 0; j < c; j++) b[j] = tmp[j];
}
/*
*add a step into the queue.
*/
void add(int num_r){
	char *buf = (char *)malloc(num_r*(num_r + 1) * 22);
	record_step(buf, num_r);
	step *tmp = (step *)malloc(sizeof(step));
	tmp -> s = buf;
	tmp -> next = NULL;
	if(start == NULL){
		start = tmp;
		last  = tmp;	
		tmp -> back = NULL;
	}else{
		last -> next = tmp;
		tmp -> back = last;
		last = last -> next;	
	}
}
/*
*reset the queue.
*/
void empty_queues(){
	start = NULL;
	last  = NULL;
	curnt = NULL;
}
/*
*actuale algorithum to solve the metrix
*/
int solve(int num_c, int num_r){
	int k = 0;
	empty_queues();
	add(num_r);
		
	for(k = 0; k < num_r ; k++){
		////////////////////////////pivoting/////////////////////////////
		int i = 0;
		int max = k;
		for(i = k; i < num_r ; i++){
			if(m[i][k] > m[max][k]){
				max = i;			
			}	
		}
		if(max != k){
			swap((double *)m[k], (double *)m[max], num_c);
			add(num_r);
		}
		//////////////////////////replating raws//////////////////////////
		for(i = k; i < num_r  && k > 0; i++){
			int j;
			double mul = m[i][k - 1];
			for(j = k - 1; j < num_c; j++){
				m[i][j] += -1.0 * mul * m[k - 1][j];
				add(num_r);
			}			
		}
		int j;
		double dv = m[k][k];
		if(dv == 0.0) return 0;

		///////////////////////////devide raw/////////////////////////////
		if(dv != 1.0){
			for(j = 0; j < num_c; j++){			
				if(m[k][j] != 0){
					m[k][j] = (double) m[k][j] / dv;
					add(num_r);	
				}
			}
		}
	}
	
	int i, j;
	//////////////////////////replating raws//////////////////////////
	for(k = num_r - 1; k > 0; k--){
		for(i = 1; i < k + 1 ; i++){
			double mul = m[k - i][k];
			for(j = k; j < num_c; j++)
				m[k - i][j] += -1.0 * mul * m[k][j];
			add(num_r);		
		}
	}
	add(num_r);	
	return 1;
}
/*
*validate input
*/
int validate(char *n){
	int i;
	for(i = 0; i < strlen(n); i++){
			if(n[i] != '0' 
			&& n[i] != '1' 
			&& n[i] != '2'
			&& n[i] != '3'
			&& n[i] != '4' 
			&& n[i] != '5'
			&& n[i] != '6'
			&& n[i] != '7' 
			&& n[i] != '8'
			&& n[i] != '9'
			&& n[i] != '.'
			&& n[i] != ' '
			&& n[i] != '-'
			&& n[i] != '+'){
			
				return 0;
			}		
	}
	return 1;
}
/*
*count raws or columns
*/
int arg_count(char *s, char c){
	int i = 1;
	int j = 0; 
 	
	while(i < strlen(s)){		
		j++;
		while((s[i] != c) && i < strlen(s)) i++;
		while(s[i] == c && i < strlen(s)) i++;
	}
	return j;
}
/*
*split a char buffer.
*/
int splitter(char *com, char **args, char *dl){
	args[0] = strtok(com, dl);
	int i;	
	for(i = 0; args[i] != NULL; i++){
		args[i + 1] = strtok(NULL, dl);	
	}
	return i;
}
/*
*travle forword in the steps queue.
*/
char *get_start(){
	if(start != NULL) {
		if(curnt == NULL || curnt -> next == NULL) curnt = start;
		else curnt = curnt -> next;
		return curnt -> s;
	}else return NULL;
}
/*
*travle backword in the steps queue.
*/
char *get_back(){
	if(curnt != NULL && curnt -> back != NULL) {
		curnt = curnt -> back;
		return curnt -> s;
	}else return NULL;
}
