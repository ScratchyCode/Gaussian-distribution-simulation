// Compile in gcc with option -lm
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUBSAMPLES 10

int control(void);
double input(double min, double max, int divisor);
double realRand(double min, double max);
double randGaussInit(void);
double randGauss(double mu, double sigma);
void average(double N, double *mu, double array[]);
void sigma(double N, double *mu, double *sigma, double array[]);

int main(){
    int N, i, j;
    double mu_dist, sigma_dist, *sample;
    
    // control that gnuplot is present as program to plot data
    int gnupl = control();
    if(gnupl == 1){
        printf("\nYou need gnuplot to graph the results.");
        printf("\nInstall it with: sudo apt-get install gnuplot\n\n");
        exit(2);
    }
    
    srand48(time(NULL));
    
    printf("\nEnter events's number to be simulated: ");
    N = input(10,1E12,SUBSAMPLES);
    
    printf("\nEnter the distribution's average: ");
    mu_dist = input(-1E12,1E12,1);
    
    printf("\nEnter the distribution's standard deviation: ");
    sigma_dist = input(0,1E12,1);
    
    FILE *output = fopen("gauss.dat","w");
    if(output == NULL){
        perror("\nError");
        exit(1);
    }
    
    sample = calloc(N,sizeof(double));
    if(sample == NULL){
        perror("\nError");
        exit(1);
    }
    
    // events generation
    for(i=0; i<N; i++){
        sample[i] = randGauss(mu_dist,sigma_dist);
        fprintf(output,"%lf\n",sample[i]);
    }
    fflush(output);
    
    // evaluate events's average and standard deviation
    double mu_emp, sigma_emp;
    average(N,&mu_emp,sample);
    sigma(N,&mu_emp,&sigma_emp,sample);
    
    printf("\n");
    printf("Events's average: %lf\n",mu_emp);
    printf("Events's standard deviation: %lf\n",sigma_emp);
    
    // evaluete SUBSAMPLES
    double *subsample = calloc(N/SUBSAMPLES,sizeof(double));
    if(subsample == NULL){
        perror("\nError");
        exit(1);
    }
    
    double mu_sample[SUBSAMPLES], sigma_sample[SUBSAMPLES];
    
    for(i=0; i<SUBSAMPLES; i++){
        for(j=0; j<N/SUBSAMPLES; j++){
            subsample[j] = sample[i*SUBSAMPLES+j];
        }
        average(N/SUBSAMPLES,mu_sample+i,subsample);
        sigma(N/SUBSAMPLES,mu_sample+i,sigma_sample+i,subsample);
    }
    
    double averageAverages, sigmaDev;
    average(SUBSAMPLES,&averageAverages,mu_sample);
    sigma(SUBSAMPLES,&averageAverages,&sigmaDev,mu_sample);
    
    printf("The average of averages of %d subsamples is %lf\n",SUBSAMPLES,averageAverages);
    printf("The sigma of averages of %d subsamples is: %lf\n",SUBSAMPLES,sigmaDev);
    
    fflush(output);
    fclose(output);
    
    printf("\n\t*** Printing ***\n");
    
    return 0;
}

double input(double min, double max, int divisor){
    double x;
    
    do{
        scanf("%lf",&x);
        if(!(min<x && x<max && (int)(x)%divisor==0)){
            if(divisor == 1){
                printf("\nThe number must be between %.1lf and %.1lf.\n",min,max);
            }else{
                printf("\nThe number must be between %.1lf and %.1lf and be multiple of %d\n",min,max,divisor);
            }
            printf("Try again: ");
        }
    }while(!(min<x && x<max && (int)(x)%divisor==0));
    
    return x;
}

void average(double N, double *mu, double array[]){
    int i;
    double sum = 0;
    
    for(i=0; i<N; i++){
        sum += array[i];
    }
    
    *mu = sum/N;
    
    return ;
}

void sigma(double N, double *mu, double *sigma, double array[]){
    int i;
    double sum = 0;
    
    for(i=0; i<N; i++){
        sum += pow((array[i] - *mu),2);
    }
    
    *sigma = sqrt(sum/(N-1));
    
    return ;
}

double realRand(double min, double max){
    double range = (max - min); 
    double divisore = RAND_MAX / range;
    return min + (lrand48() / divisore);
}

double randGaussInit(void){
    double g = 0;
    int i;
    
    for(i=0; i<12; i++){
        g += realRand(-0.5,0.5);
    }
    
    return g;
}

double randGauss(double mu, double sigma){
    return randGaussInit() * sigma + mu;
}

int control(void){
    char path[] = "/usr/bin/gnuplot";
    
    FILE *pf = fopen(path,"r");
    if(pf == NULL){
        fclose(pf);
        return 1;
    }else{
        fclose(pf);
        return 0;
    }
}
