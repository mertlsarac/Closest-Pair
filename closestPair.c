#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char FILE_PATH[] = "Input.txt";

void swap(double *, double *);
double divide(double **, int, int);
double closestPoint(double **, int, int);
double checkSpecialCase(double **, int, double);
double distance(double, double, double, double);

//for sorting
void quickSort(double **, int, int, int);
int partition(double **, int, int, int);

//void printArr(int n, double **);

int main() {
    int i, j;

    FILE *filePointer = fopen(FILE_PATH, "r");
    if(filePointer == NULL) {
        printf("Terminated due to the file problem.\n");
        return 0;
    }

    //get the number of points from the file
    int n = 0;
    while(fscanf(filePointer, "%lf %lf\n") != EOF) {
        n++;
    }

    //initialize the points array
    double **points;
    points = (double **) malloc(sizeof(double *) * n);
    for(i = 0; i < n; i++) {
        points[i] = (double *) malloc(sizeof(double) * 2);
    }

    i = 0; //for tracking the index of the file
    fseek(filePointer, 0, SEEK_SET);
    //get the points from the file
    while(fscanf(filePointer, "%lf %lf\n", &x, &y) != EOF) {
        points[i][0] = x;
        points[i][1] = y;
        i++;
    }

    //sort the array
    quickSort(points, 0, n - 1, 0);


    double d;
    d = divide(points, 0, n - 1);

    printf("Minimum distance: %lf\n", d);
    return 0;
}

double divide(double **arr, int l, int r) {
    //if the size of the array smaller than 3, find the min dist using brute force
    if(r - l < 3) {
        return closestPoint(arr, l, r);
    }

    //find the medium value
    int m = l + (r - l) / 2;    //you can use m = (l + r) / 2 but it could be overflow

    double dL, dR, d;
    dL = divide(arr, l, m);
    dR = divide(arr, m + 1, r);

    if(dL < dR)
        d = dL;
    d =  dR;

    //this variable hold the the index of array whose difference are lower than d
    int i = m, j = m;

    //to keep the lower bound of the array which keeps the points closer than d
    while(i >= l && (arr[m][0] - arr[i][0]) < d) {
        i--;
    }

    //to keep the upper bound of the array which keeps the points closer than d
    while(j <= r && (arr[j][0] - arr[m][0] ) < d) {
        j++;
    }

    i++;
    j--;

    //init the array to check the special case
    double **arr2 = (double **) malloc(sizeof(double*) * (j - i + 1));

    int k;
    for(k = 0; k < j - i + 1; k++) {
        arr2[k] = (double *) malloc(sizeof(double) * 2);
    }

    int n = j - i + 1;
    k = 0;

    while(i <= j) {
        arr2[k][0] = arr[i][0];
        arr2[k][1] = arr[i][1];
        i++;
        k++;
    }

    d = checkSpecialCase(arr2, n, d);

    return d;
}

double closestPoint(double **arr, int l, int r) {
    int i, j;
    double d, dist;
    d = distance(arr[0][0], arr[1][0], arr[0][1], arr[1][1]);
    for(i = l; i < r; i++) {
        for(j = i + 1; j < r + 1; j++) {
            double x1 = arr[i][0];
            double x2 = arr[j][0];
            double y1 = arr[i][1];
            double y2 = arr[j][1];
            dist = distance(x1, x2, y1, y2);
            if(dist < d) {
                d = dist;
            }
        }
    }
    return d;
}

//this function calculate the distance between the points in special case
//and return the lowest difference
double checkSpecialCase(double **arr, int size, double minDist) {
    quickSort(arr, 0, size - 1, 1);
    int i, j;
    double d = minDist, currentDist;
    for(i = 0; i < size; i++) {
        for(j = i + 1; j < size && (arr[j][1] - arr[i][1] < minDist); j++) {
            currentDist = distance(arr[j][0], arr[i][0], arr[j][1], arr[i][1]);
            if(currentDist < d) {
                d = currentDist;
            }
        }
    }
    return d;
}

void swap(double *x, double *y) {
    double tmp = *x;
    *x = *y;
    *y = tmp;
}

//for sorting
void quickSort(double **arr, int l, int r, int index) {
    if(l < r) {
        int pivot = partition(arr, l, r, index);
        quickSort(arr, l, pivot - 1, index);
        quickSort(arr, pivot + 1, r, index);
    }
}

int partition(double **arr, int l, int r, int index) {

    int i = (l - 1);
    int j;
    double pivot = arr[r][index];
    for(j = l; j < r; j++) {
        if(arr[j][index] < pivot) {//get the number of points from the file
            i++;
            swap(&arr[i][0], &arr[j][0]);
            swap(&arr[i][1], &arr[j][1]);
        }
    }

    swap(&arr[i + 1][0], &arr[r][0]);
    swap(&arr[i + 1][1], &arr[r][1]);

    return i + 1;
}

double distance(double x1, double x2, double y1, double y2) {
    double dist = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return dist;
}
