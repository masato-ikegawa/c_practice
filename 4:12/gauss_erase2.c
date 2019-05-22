#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define EPSIRON 0.0005 // 丸め誤差対策


double **a,*x,**c;
//double *b


//3行の行列式
void det(int count){
	if(count == 3){
		double Det = 0;
		for(int i=0;i<count;i++){
			Det = Det + a[i][0] * (a[(i+1)%3][1] * a[(i+2)%3][2] - a[(i+2)%3][1] * a[(i+1)%3][2]);
		}
		printf("det = %lf\n",Det );
	}
}

//swap関数
void swap(double *a,double *b)
{
	double tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;

	return;

}

void pivot(int p, int count){
	// pivot変換用関数
	int i,iMax;
	double maxValue; // 最大の変数を格納

	iMax = p;
	maxValue = a[p][p];

    // どの係数が一番大きいかを確認
    for(i=p+1;i<count;i++){
    	if(fabs(a[i][p]) > fabs(maxValue)){
    		iMax = i;
    		maxValue = a[i][p];
    	};

    };
    
    // 一番大きい係数だった行と入れ替える
    if(p != iMax){
    	for(int k=0;k<count;k++){
    		swap(&a[p][k],&a[iMax][k]);
    		swap(&c[p][k],&c[iMax][k]);
    	};
    };
}

//xを求めるときに使うΣ
double sigma(int i,int count){
	double result = 0;
	for(int k=i;k<count-1;k++){
		result = result + a[i][k+1] * x[k+1];
	}
	return result;
};

int main(void)
{
	int i,j,k,n,rankCount=0;
	int count,countData = 0;
	FILE *fp;
	char fname[50]; // ファイル名の取得
	//int buf_num[256];
	int C;
	// データ格納のための配列


    // ファイル名の入力・ファイルオープン（scanf, fopenを上手く使う）
    // コマンドラインからファイル名を取得しても良い
	scanf("%s",fname);

	//格納されてない時
	if ((fp = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "%s\n", "error: can't read file.");
        return EXIT_FAILURE;
    }
	// ファイル名が変数fnameに格納されている場合

	//行数カウント
    while((C = fgetc(fp)) != EOF) {
    	if(C == '\n') countData++;
    }

    //fgetc使うとなぜかfscanfをしても0しか入らないので一旦fcloseしてopenし直す、ここは本当にわからなかった
    /*fclose(fp);

    if ((fp = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "%s\n", "error: can't read file.");
        return EXIT_FAILURE;
    }*/

    //!!!ここでrewind()を使えば良いことがわかった(修正点①)
    rewind(fp);


    printf("countData = %d\n",countData );

    //動的にメモリを確保
    a = malloc(sizeof(double) * countData);
    for(i=0;i<countData;i++){
		a[i] = malloc(sizeof(double) * countData);
	}

	c = malloc(sizeof(double) * countData);
    for(i=0;i<countData;i++){
		c[i] = malloc(sizeof(double) * countData);
	}

	

    //b = (double *)malloc(sizeof(double) * countData);
    x = (double *)malloc(sizeof(double) * countData);



    // ファイル内のデータを取得する
    // fscanf等を利用する．

	for(i=0;i<countData;i++){
		for(n=0;n<countData;n++){
			fscanf(fp,"%lf",&a[i][n]);
		}
	}

	for(i=0;i<countData;i++){
		for(j=0;j<countData;j++){
			if (i==j)
			{
				c[i][j] = 1;
			}
			else c[i][j] = 0;
		}
	}

	/*fscanfが正しく機能しているか確認(N=4)
	for ( i = 0; i < countData; i++ ) {
        printf("%.2f, %.2f, %.2f, %.2f, %.2f\n", a[i][0], a[i][1], a[i][2],a[i][3], b[i]);
    }*/

    
	// 前進消去
	count = countData;

	det(count);
	
	for(i=0;i<count;i++){
        // まずピボット操作が必要かどうかを確認する
        
		if((fabs(a[i][i])< EPSIRON) || a[i][i] == 0){
			// pivot操作
			pivot(i,count); 

			printf("%d\n", i);
			for(int m=0;m<count;m++){
				for(int n=0;n<count;n++){
					printf("a[%d][%d] = %lf, ", m, n, a[m][n]);
				}
			putchar('\n');
			}
		};

		printf("-------------------------------\n");

		for(j=count-1;j>=0;j--){
			c[i][j] = c[i][j] / a[i][i];
		}
		
		for(j=count-1;j>=i;j--){
			a[i][j] = a[i][j] / a[i][i];
		}
		
        // 前進消去
        for(k=0;k<count;k++){

        	if(k == i){continue;} 

        	for(j=count-1;j>=0;j--){
        		c[k][j] = c[k][j] - c[i][j] * a[k][i];
        	}
        	//前から計算するとおかしくなってしまうので後ろから
        	//b[k] = b[k] - b[i] * a[k][i];
        	for(j=count-1;j>=i;j--){
        		a[k][j] = a[k][j] - a[i][j] * a[k][i];
        		//debug
        		/*
        		for (int m= 0; m < countData; m++ ) {
        		printf("%.2f, %.2f, %.2f, %.2f, %.2f\n", a[m][0], a[m][1], a[m][2],a[m][3], b[m]);
   				 	}
   				 printf("-------------------------------------------\n");
   				 */
        	}
        }

        /*
	    for (int m= 0; m < countData; m++ ) {
	        printf("%.2f, %.2f, %.2f ,%.2f\n", a[m][0], a[m][1], a[m][2],a[m][3]);
	    }
	    for (int m= 0; m < countData; m++ ) {
	        printf("%.2f, %.2f, %.2f,%.2f\n", c[m][0], c[m][1], c[m][2],c[m][3]);
	    }
	    printf("---------------------\n");
		*/
	    for(int m=0;m<count;m++){	
	    	rankCount = 0;
	    	for(j=0;j<count;j++){
	    		//printf("a[%d][%d] = %.20lf\n",m,j,a[m][j]);
	    		if(fabs(a[m][j]) < EPSIRON){
	    			rankCount++;
	    			//printf("rankCount:a[%d][%d]:%d\n",m,j,rankCount );
	    		};
	    	}
	    	if(rankCount == count){
	    	printf("No Inverse Matrix\n");
	    	return 0;
	    	}

	    }
    

	}

    // 後退代入
    // 今度は大きい値から引いていくことに注意

    /*for(i=count-1;i>=0;i--){
    	x[i] = (b[i] - sigma(i,count) ) / a[i][i];
    };
    */

   /* printf("確かめ%lf\n",x[0]*2 + x[1]*(-4) + x[2]*6);
    printf("確かめ%lf\n",x[0]*(-1) + x[1]*3 + x[2]*(-5));
    printf("確かめ%lf\n",x[0]*1 + x[1]*2 + x[2]*3);
	*/


	// 結果の確認
	/*
	for(i=0;i<count;i++){
		for(j=0;j<count;j++){
			printf("a[%d][%d] = %lf, ", i, j, a[i][j]);
		}
		putchar('\n');
	}
	*/

	printf("逆行列\n");
	for(i=0;i<count;i++){
		for(j=0;j<count;j++){
			printf("c[%d][%d] =%lf ",i,j,c[i][j]);
		}
		putchar('\n');
	}
	/*
	for(i=0;i<count;i++)
		printf("x_%d = %lf, ", i+1, x[i]);
	printf("\n");
	*/

	
	fclose(fp);

//修正点②
	free(a);
	//free(b);
	//free(x);
	free(c);
	return 0;

}






