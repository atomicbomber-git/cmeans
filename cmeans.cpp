#include <iostream>
#include <cstdio>
#include <cmath>
#include <random>
#include <cstdlib>

int n_data;
int n_features;
int n_clusters;
double fuzziness;
int n_iterations;
int DATA_MAX_VAL = 1000;

FILE* data_start;
FILE* membership_start;
FILE* membership_end;

double** data;
double** centroids;
double** membership_degrees;

void initialize()
{
    std::srand(time(NULL));

    data_start = fopen("data.txt", "w");
    membership_start = fopen("begin.txt", "w");
    membership_end = fopen("end.txt", "w");

    data = new double*[n_data];
    for (int i = 0; i < n_data; ++i) {
        data[i] = new double[n_features];

        for (int j = 0; j < n_features; ++j) {
            data[i][j] = std::rand() % (DATA_MAX_VAL + 1);
        }
    }

    centroids = new double*[n_clusters];
    for (int i = 0; i < n_clusters; ++i) {
        centroids[i] = new double[n_features];
        
        for (int j = 0; j < n_features; ++j) {
            centroids[i][j] = 0.0f;
        }
    }

    membership_degrees = new double*[n_data];
    for (int i = 0; i < n_data; ++i) {
        membership_degrees[i] = new double[n_clusters];

        for (int j = 0; j < n_clusters; ++j) {
            membership_degrees[i][j] = (double) (std::rand() % (DATA_MAX_VAL + 1)) / (double) DATA_MAX_VAL;
        }
    }
}

void compute_centroids()
{
    for (int i = 0; i < n_clusters; ++i) {
        for (int j = 0; j < n_features; ++j) {

            double total_den = 0.0;
            double total_num = 0.0;
            for (int k = 0; k < n_data; ++k) {
                total_den += (pow(membership_degrees[k][i], fuzziness) * data[k][j]);
                total_num += pow(membership_degrees[k][i], fuzziness);
            }

            centroids[i][j] = total_den / total_num;

        }
    }
}

double euclidean_distance(double* vec_a, double* vec_b, int len)
{
    double total = 0.0;
    for (int i = 0; i < len; ++i) {
        total += pow(vec_b[i] - vec_a[i], 2);
    }

    return sqrt(total);
}

void update_membership_degrees()
{
    for (int i = 0; i < n_data; ++i) {
        for (int j = 0; j < n_clusters; ++j) {
            
            double total = 0.0;
            
            for (int k = 0; k < n_clusters; ++k) {
                total += pow((euclidean_distance(data[i], centroids[j], n_features) / euclidean_distance(data[i], centroids[k], n_features)), 2.0 / (fuzziness - 1.0));
            }

            membership_degrees[i][j] = 1.0 / total;
        }
    }
}

void print_data(FILE* fileptr)
{
    for (int i = 0; i < n_data; ++i) {
        for (int j = 0; j < n_features; ++j) {
            fprintf(fileptr, "%.18f ", data[i][j]);
        }
        fprintf(fileptr, "\n");
    }
}

void print_membership_degrees(FILE* fileptr)
{
    for (int i = 0; i < n_data; ++i) {
        for (int j = 0; j < n_clusters; ++j) {
            fprintf(fileptr, "%.18f ", membership_degrees[i][j]);
        }
        fprintf(fileptr, "\n");
    }
}

int main(int argc, const char** argv)
{

    n_data = atoi(argv[1]);
    n_features = atoi(argv[2]);
    n_clusters = atoi(argv[3]);
    fuzziness = atoi(argv[4]);
    n_iterations = atoi(argv[5]);

    initialize();
    print_data(data_start);

    print_membership_degrees(membership_start);
    for (int i = 0; i < n_iterations; ++i) {
        compute_centroids();
        update_membership_degrees();
    }
    print_membership_degrees(membership_end);
}