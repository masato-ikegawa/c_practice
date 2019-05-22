#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define E -0.000005

double **x,*cB,*ci,*b,*d,*k,*X;
int *r;
int width = 0,height = 0,howManyX = 0;
double result = 0;



void Malloc(void){
	x = malloc(sizeof(double) * height);
    for(int i=0;i<height;i++){
		x[i] = malloc(sizeof(double) * width);
	}

	cB = (double *)malloc(sizeof(double) * height);
	ci = (double *)malloc(sizeof(double) * width);
	b = (double *)malloc(sizeof(double) * height);
	d = (double *)malloc(sizeof(double) * width);
	k = (double *)malloc(sizeof(double) * howManyX);
	r = (int *)malloc(sizeof(int) * height);
	X = (double *)malloc(sizeof(double) * howManyX);

	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			x[i][j] = 0;
		}
	}

	 for(int i=0;i<height;i++) cB[i] = 1;
	 for(int i=0;i<height;i++) b[i] = 0;
	 for(int i=0;i<width;i++) d[i] = 0;
	 for(int i=0;i<width;i++) ci[i] = 0;
	 for(int i=0;i<height;i++) r[i] = -1;




}

void varInit(void){
	FILE *fp;
	char fname[50];
	int C;
	char c;
	

	printf("file:");
	scanf("%s",fname);

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
    width = height + howManyX*2;
    //printf("width:%d\n",width );

    rewind(fp);

    Malloc();

    for(int i=0;i<howManyX;i++){
    	fscanf(fp,"%lf",&k[i]);
    }
    

    for(int i = 0;i<howManyX;i++){
    	printf("d[%d]:%lf\n",i,d[i] );
    }

    for(int i=0;i<height;i++){
    	for(int j=0;j<howManyX;j++){
    		fscanf(fp,"%lf",&x[i][j]);
    	}
    	fscanf(fp,"%s",&c);
    	x[i][howManyX + i] = -1;
    	x[i][howManyX*2 + i] = 1;
    	fscanf(fp,"%lf",&b[i]);
    }

    for(int i=howManyX*2;i<howManyX*3;i++){
    	ci[i] = 1;
    }

    for(int i=0;i<width;i++){
    	double D=0;
    	for(int j=0;j<height;j++){
    		D = D + cB[j]*x[j][i];
    	}
    	d[i] = ci[i] - D;
    }

	/*
    for(int i=0;i<height;i++){
    	for(int j=0;j<width;j++){
    		printf("x[%d][%d]:%lf\n",i,j,x[i][j] );
    	}
    	printf("b[%d]:%lf\n",i,b[i] );
    }*/

    /*for(int i=0;i<width;i++){
    	printf("%lf\n",d[i] );
    }*/
    

    fclose(fp);
}

void varInitSecond(){

	width = width - howManyX;

	for(int j=0;j<height;j++){
		if(r[j] == 0) cB[j] = k[0];
		else if(r[j] == 1) cB[j] = k[1];
		else if(r[j] == 2) cB[j] = k[2];
		else {
			cB[j] = 0;
			r[j] = -1;
		}
	}

	for(int j=0;j<width;j++){
		if(j<3) ci[j] = k[j];
		else ci[j] = 0;
	}

	for(int i=0;i<width;i++){
    	double D=0;
    	for(int j=0;j<height;j++){
    		D = D + cB[j]*x[j][i];
    	}
    	d[i] = ci[i] - D;
    }

    result = 0;
    for(int j=0;j<height;j++){
    	result = result + cB[j]*b[j];
    }
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
	
	if(min >= E) return -1;
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
	
	//第１段階
	while((i = minus()) != -1){
		putchar('\n');
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
	//第２段階

	count = 1;
	varInitSecond();

	for(int a=0;a<height;a++){
		printf("cb[%d]:%lf\n",a,cB[a] );
	}
	printf("result:%lf\n",result );

	printf("第２段階\n");
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

	

    /*
    for(int a=0;a<height;a++){
			for(int b=0;b<width;b++){
				printf("x[%d][%d]:%lf",a,b,x[a][b] );
			}
			printf("b[%d]:%lf\n",a,b[a] );
		}
		for(int a=0;a<width;a++) printf("d[%d]:%lf",a,d[a] );

	for(int j=0;j<height;j++){
		printf("r[%d]:%d\n",j,r[j] );
	}
	*/

	for(int j=0;j<height;j++){
		//printf("r[%d]:%d\n",j,r[j]);
		if(r[j] >= howManyX){
			surplus[r[j] - howManyX] = b[j];
		}
		else if(r[j] < 0 ){
			surplus[j] = b[j];
		}
		else X[r[j]] = b[j];
	}

	result = 0;
    for(int j=0;j<height;j++){
    	result = result + X[j]*k[j];
    }


	printf("Costs = %lfyen,Intake:W1 = %lfgram,W2 = %lfgram,W3 = %lfgram\n",result,X[0],X[1],X[2]);

	printf("surplus Materials");
	for(int j=0;j<height;j++){
		printf("V%d = %lf\n",j+1,surplus[j]);
	}

	
	return 0;
}