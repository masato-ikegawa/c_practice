#include <stdio.h>
#include <math.h>
#define K 50
#define E1 0.00001
#define E2 0.00001

double x[K],fx[K],Fx[K];

void init(){
	double x_0;
	printf("初期値：");
	scanf("%lf",&x_0);
	x[0] = x_0;
	printf("Initialx_0=%lf\n",x[0] );
}

double nonlinear_func(double x){
	double f_x = pow(x,3) - 3*pow(x,2)*sin(x) -1;
	return f_x;
}

double nonlinear_func_diff(double x){
	double F_x  = 3*pow(x,2)*(1-cos(x)) - 6*x*sin(x);
	return F_x;
}

void newton(double X){
	int i;
	for(i=0;i<K;i++){
		fx[i] = nonlinear_func(x[i]);
		Fx[i] = nonlinear_func_diff(x[i]);
		if(!(fabs(Fx[i]) >= E1)){
			printf("もう１度初期値を設定:Fx[%d]=%lf\n",i,Fx[i]);
			init();
			newton(x[0]);
		}
		x[i+1] = x[i] - (fx[i]/Fx[i]);
		printf("Iteration:%d,x[%d]:%lf\n",i+1,i+1,x[i+1]);
		if(fabs(nonlinear_func(x[i+1])) <= E2){
			printf("result:%lf,Iteration number:%d\n",x[i+1],i+1);
			break;
		} 
		if(i>=1){
			if(fabs(x[i+1] - x[i]) >= fabs(x[i] - x[i-1])){
				printf("もう１度初期値を設定\n");
				init();
				newton(x[0]);
				break;
			}
		}
	}

	if(i == K-1){
		printf("もう１度初期値を設定\n");
		init();
		newton(x[0]);
	}

}

int main (void){

	
	init();

	newton(x[0]);


	return 0;
}
