#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double **x,*cB,*b,*d,*k,*X;
int *r;
int width = 0,height = 0,howManyX = 0;
double result = 0;



void Malloc(void){
	x = malloc(sizeof(double) * height);
    for(int i=0;i<height;i++){
		x[i] = malloc(sizeof(double) * width);
	}

	//cB = (double *)malloc(sizeof(double) * height);
	b = (double *)malloc(sizeof(double) * height);
	d = (double *)malloc(sizeof(double) * width);
	k = (double *)malloc(sizeof(double) * width);
	r = (int *)malloc(sizeof(int) * height);
	X = (double *)malloc(sizeof(double) * howManyX);

	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			x[i][j] = 0;
		}
	}

	 //for(int i=0;i<height;i++) cB[i] = 0;
	 for(int i=0;i<height;i++) b[i] = 0;
	 for(int i=0;i<width;i++) d[i] = 0;
	 for(int i=0;i<width;i++) k[i] = 0;
	 for(int i=0;i<height;i++) r[i] = -1;




}

void varInit(void){
	FILE *fp;
	char fname[50];
	int C,max_min;
	char c;
	

	printf("file:");
	scanf("%s",fname);
	printf("max:0/min:1 :");
	scanf("%d",&max_min);

	if ((fp = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "%s\n", "error: can't read file.");
    }

    while((C = fgetc(fp)) != EOF) {
    	if(C == '\n') height++;
    }
    height = height - 1;
    //printf("height:%d\n",height );

    rewind(fp);

    while((C = fgetc(fp)) != '\n') {
    	if(C == ' ') howManyX++;
    }

    howManyX = howManyX + 1;
    
    //printf("howManyX:%d\n",howManyX );
    width = height + howManyX;
    //printf("width:%d\n",width );

    rewind(fp);

    Malloc();

    for(int i=0;i<howManyX;i++){
    	fscanf(fp,"%lf",&d[i]);
    }

    if(max_min == 0){
    	for(int i=0;i<howManyX;i++){
    		d[i] = d[i] * (-1);
    	} 
    }
	   

    for(int i = 0;i<howManyX;i++){
    	printf("d[%d]:%lf\n",i,d[i] );
    }

    for(int i=0;i<height;i++){
    	for(int j=0;j<howManyX;j++){
    		fscanf(fp,"%lf",&x[i][j]);
    	}
    	fscanf(fp,"%s",&c);
    	x[i][howManyX + i] = 1;
    	fscanf(fp,"%lf",&b[i]);
    }

	/*
    for(int i=0;i<height;i++){
    	for(int j=0;j<howManyX;j++){
    		printf("x[%d][%d]:%lf\n",i,j,x[i][j] );
    	}
    	printf("b[%d]:%lf\n",i,b[i] );
    }
    */

    fclose(fp);
}

int minus(void){
	int index;
	double min=1000000;
	for(int i=0;i<width;i++){
		if(d[i] < 0){
			if(d[i] < min){
				index = i;
				min = d[i];
			}
		}
	}
	
	if(min >= 0) return -1;
	else return index;
}

int nIndex(int i){
	int index;
	double min=1000000;
	for(int j=0;j<height;j++){
		if(x[j][i] > 0){
			if((b[j] / x[j][i]) < min){
				min = b[j] / x[j][i];
				index = j; 
			}
		}
	}
	//ピボットする行を決める
	printf("ピボット:%d行目\n",index );
	return index;
}

/*void init(void){
	x[0][0] = 2;
	x[0][1] = 3;
	x[0][2] = 1;
	x[1][0] = 6;
	x[1][1] = 4;
	x[1][3] = 1;
	x[2][0] = 2;
	x[2][1] = 1;
	x[2][4] = 1;
	b[0] = 210;
	b[1] = 380;
	b[2] = 120;
	d[0] = -4;
	d[1] = -3;
	k[0] = d[0];
	k[1] = d[1];
}*/

void division(int n, int i){
	double tmp = x[n][i];

	b[n] = b[n] / tmp;
    
		for(int m=0;m<width;m++){
			x[n][m] = x[n][m] / tmp;
		}

	

}

void delete(int n,int i){
	double tmp;
	for(int j=0;j<height;j++){

		if(j == n) continue;

		tmp = x[j][i];
		b[j] = b[j] - b[n] * tmp;
		for(int m=0;m<width;m++){
			x[j][m] = x[j][m] - x[n][m] * tmp;
		}
	}

	tmp = d[i];
	for(int j=0;j<width;j++){
		d[j] = d[j] - x[n][j] * tmp;
	}
	
	result = result - b[n] * tmp;

	/*
	for(int a=0;a<height;a++){
		for(int c=0;c<width;c++){
			printf("x[%d][%d] = %lf",a,c,x[a][c] );
		}
		printf("b[%d] = %lf\n",a,b[a] );
		putchar('\n');
	}
	//printf("iii:%lf %lf %lf\n",x[0][1],x[1][1],x[2][1] );
	for(int a=0;a<width;a++){
		printf("d[%d] = %lf",a,d[a]);
	}	
	putchar('\n');
	*/
}


int main(void){

	int i,n,count=1;

	varInit();
	//init();

	double surplus[height];
	for(int j=0;j<height;j++){
		surplus[j] = 0;
	}
	
	while((i = minus()) != -1){
		printf("ピボット%d回目\n",count );
		//ピボットする列を決める
		printf("ピボット:%d列目\n",i );
		n = nIndex(i);
		//cB[n] = k[i];
		r[n] = i;
		division(n,i);
		delete(n,i);
		count++;
		
	}


	for(int j=0;j<height;j++){
		//printf("r[%d]:%d\n",j,r[j]);
		if(r[j] > howManyX){
			surplus[r[j] - howManyX] = b[j];
		}
		else if(r[j] < 0 ){
			surplus[j] = b[j];
		}
		else X[r[j]] = b[j];
	}


	printf("Profits = %lfman-yen,Production volume:P1 = %lfton,P2 = %lfton,P3 = %lfton\n",result,X[0],X[1],X[2]);

	for(int j=0;j<height;j++){
		printf("surplus materials M%d = %lf\n",j+1,surplus[j]);
	}

	
	return 0;
}