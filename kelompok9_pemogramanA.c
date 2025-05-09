#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ROWS     120
#define BASE_YEAR    1960

double interp_linear(int x, int x1, double y1, int x2, double y2) {
    return y1 + (y2 - y1) * (double)(x - x1) / (double)(x2 - x1);
}

void polynomial_regression_deg3(double x[], double y[], int n, double coef[4]) {
    double X[7]={0}, Y[4]={0}, A[4][5];
    for(int i=0;i<n;i++){
        double xi=1;
        for(int j=0;j<7;j++){ X[j]+=xi; xi*=x[i]; }
        xi=1;
        for(int j=0;j<4;j++){ Y[j]+=y[i]*xi; xi*=x[i]; };
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++) A[i][j]=X[i+j];
        A[i][4]=Y[i];
    }
    for(int i=0;i<4;i++){
        for(int k=i+1;k<4;k++){
            double t=A[k][i]/A[i][i];
            for(int j=i;j<5;j++) A[k][j]-=t*A[i][j];
        }
    }
    for(int i=3;i>=0;i--){
        coef[i]=A[i][4];
        for(int j=i+1;j<4;j++) coef[i]-=A[i][j]*coef[j];
        coef[i]/=A[i][i];
    }
}

double eval_poly3(double coef[4], double x){
    return coef[0] + coef[1]*x + coef[2]*x*x + coef[3]*x*x*x;
}

void linear_regression(double x[], double y[], int n, double *a0, double *a1){
    double sx=0, sy=0, sxy=0, sxx=0;
    for(int i=0;i<n;i++){
        sx+=x[i]; sy+=y[i];
        sxy+=x[i]*y[i]; sxx+=x[i]*x[i];
    }
    *a1=(n*sxy - sx*sy)/(n*sxx - sx*sx);
    *a0=(sy - (*a1)*sx)/n;
}

int find_index(int arr[], int n, int target){
    for(int i=0;i<n;i++) if(arr[i]==target) return i;
    return -1;
}

int main(){
    FILE *f=fopen("Data Tugas Pemrograman A.csv","r");
    int years[MAX_ROWS];
    double net[MAX_ROWS], pop[MAX_ROWS];
    int n=0;
    char buf[256];
    fgets(buf,sizeof(buf),f);
    while(fgets(buf,sizeof(buf),f)){
        int y; double i,p;
        if(sscanf(buf,"%d,%lf,%lf",&y,&i,&p)==3){
            years[n]=y; net[n]=i; pop[n]=p/1e6; n++;
        }
    }
    fclose(f);

    int miss[4]={2005,2006,2015,2016};
    for(int k=0;k<4;k++){
        int yr=miss[k];
        int lo=(k<2?2004:2014), hi=(k<2?2007:2017);
        int i1=find_index(years,n,lo), i2=find_index(years,n,hi);
        net[n]=interp_linear(yr,years[i1],net[i1],years[i2],net[i2]);
        pop[n]=interp_linear(yr,years[i1],pop[i1],years[i2],pop[i2]);
        years[n++]=yr;
    }

    for(int i=0;i<n-1;i++) for(int j=0;j<n-1-i;j++){
        if(years[j]>years[j+1]){
            int ty=years[j]; years[j]=years[j+1]; years[j+1]=ty;
            double tn=net[j]; net[j]=net[j+1]; net[j+1]=tn;
            double tp=pop[j]; pop[j]=pop[j+1]; pop[j+1]=tp;
        }
    }

    double x_pop[MAX_ROWS], y_pop[MAX_ROWS], x_net[MAX_ROWS], y_net[MAX_ROWS];
    int np=0, nn=0;
    for(int i=0;i<n;i++){
        double x=years[i]-BASE_YEAR;
        if(!(years[i]==2005||years[i]==2006||years[i]==2015||years[i]==2016)){
            x_pop[np]=x; y_pop[np++]=pop[i];
            x_net[nn]=x; y_net[nn++]=net[i];
        }
    }

    double coef_pop[4], a0_net, a1_net;
    polynomial_regression_deg3(x_pop,y_pop,np,coef_pop);
    linear_regression(x_net,y_net,nn,&a0_net,&a1_net);

    int targets[]={2005,2006,2015,2016,2030,2035};
    for(int t=0;t<6;t++){
        int yr=targets[t];
        double x=yr-BASE_YEAR;
        printf("Tahun %d:\n",yr);
        if(t<4){
            int idx=find_index(years,n,yr);
            printf("  Populasi Indonesia    = %.4f juta\n",pop[idx]);
            printf("  Persentase Internet   = %.4f %%\n\n",net[idx]);
        } else if(yr==2030){
            double pp=eval_poly3(coef_pop,x);
            printf("  Populasi Indonesia    = %.4f juta\n\n",pp);
        } else {
            double ii=a0_net + a1_net*x;
            if(ii<0) ii=0; if(ii>100) ii=100;
            printf("  Persentase Internet   = %.4f %%\n\n",ii);
        }
    }

    printf("Persamaan Polinomial Populasi (deg3):\n");
    printf("y = %.6fx^3 + %.6fx^2 + %.6fx + %.6f\n\n",
           coef_pop[3],coef_pop[2],coef_pop[1],coef_pop[0]);
    printf("Persamaan Linear Internet:\n");
    printf("y = %.6fx + %.6f\n",a1_net,a0_net);


    FILE *out = fopen("output_linear.dat", "w");
    fprintf(out, "#Tahun\tY_Asli\tY_Internet_Regresi\tY_Populasi_Regresi\tY_Interpolasi\n");
    for(int i=0; i<n; i++) {
        double x = years[i] - BASE_YEAR;

        double y_reg_net = a0_net + a1_net * x;
        double y_reg_pop = eval_poly3(coef_pop, x);

        int is_interp = (years[i] == 2005 || years[i] == 2006 || years[i] == 2015 || years[i] == 2016);
        double y_interp = is_interp ? net[i] : NAN;

        fprintf(out, "%d\t%.2f\t%.2f\t%.2f\t%.2f\n", 
            years[i], net[i], y_reg_net, y_reg_pop, y_interp);
    }
    fclose(out);

    return 0;
}