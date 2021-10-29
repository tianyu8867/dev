#include <stdio.h>

#define _CRT_SECURE_NO_WARNINGS

struct data {
    int *data_len;
    int** data;
};

int no2g_len;
struct data* H; //水平
struct data* V; //垂直

int** run_data;

int out[20];

void set_zero(int start)
{
    for (int i = start; i < no2g_len; i++) {
        out[i] = 0;
    }
    return;
}
int set_data(int data, int start)
{
    int i;
    for (i = 0; i < data && i + start < no2g_len; i++) {
        out[i + start] = 1;
    }
    if (i == data) {
        return i + start;
    }
    return -1;
}

int dfs(int* in, int in_cnt, int n, int start)
{
    int ret, i;
    if (n == in_cnt) {
        for (int i = 0; i < no2g_len; i++) {
            printf("%d ", out[i]);
        }
        printf("\n");
        return 0;
    }
    for (i = start; i < no2g_len; i++) {
        if (n != 0 && i == start) {
            continue;
        }
        set_zero(start);

        ret = set_data(in[n], i);
        if (ret == -1) {
            break;
        }
        for (int j = in[n] + i; j <= no2g_len; j++) {
            set_zero(j);
            if (!dfs(in, in_cnt, n + 1, j)) {
                break;
            }
        }
    }
    return 0;
}

int** create_array(int len)
{
    int** a;
    a = malloc(sizeof(int *) * len);
    for (int i = 0; i < len; i++) {
        a[i] = malloc(sizeof(int) * len);
        memset(a[i], 0, sizeof(int) * len);
    }

    return a;
}
void init_data(FILE* fp, struct data* a)
{
    a->data_len = malloc(sizeof(int) * no2g_len);
    for (int i = 0; i < no2g_len; i++) {
        (void)fscanf(fp, "%d", &a->data_len[i]);
    }
    a->data = malloc(sizeof(int *) * no2g_len);
    for (int i = 0; i < no2g_len; i++) {
        a->data[i] = malloc(sizeof(int) * a->data_len[i]);
        for (int j = 0; j < a->data_len[i]; j++) {

            (void)fscanf(fp, "%d", &a->data[i][j]);
        }
    }
    return;
}

void print_data(struct data* a)
{
    for (int i = 0; i < no2g_len; i++) {
        printf("%d ", a->data_len[i]);
    }
    printf("\n");
    for (int i = 0; i < no2g_len; i++) {
        for (int j = 0; j < H->data_len[i]; j++) {

            printf("%d ", a->data[i][j]);
        }
    }
    printf("\n");

    return;
}

void print_array()
{
    for (int i = 0; i < no2g_len; i++) {
        for (int j = 0; j < no2g_len; j++) {
            if (run_data[i][j] == 0) {
                printf("%d ", run_data[i][j]);
            }
            else {
                printf("  ");
            }
        }
        printf("\n");
    }
    return;
}
int get_data_from_file(char* file_path)
{
    FILE* fp;
    char StrLine[1024];
    fp = fopen(file_path, "rt");
    if (fp == NULL) {
        printf("\nCannot open file strike any key exit!");
        return -1;
    }

    (void)fscanf(fp, "%d", &no2g_len);
    H = malloc(sizeof(struct data));
    V = malloc(sizeof(struct data));

    init_data(fp, H);

    init_data(fp, V);
    fclose(fp);

    return 0;
}

int create_one_line(int *in, int in_no2g_len)
{
    int sum = 0;
    for (int i = 0; i < in_no2g_len; i++) {
        sum += in[i];
    }
    if (sum + in_no2g_len - 1 > no2g_len) {
        printf("input error.\n");
        return -1;
    }
    dfs(in, in_no2g_len, 0, 0);
    return 0;
}

void create_data(struct data* H)
{
    printf("########\n");

    for (int i = 0; i < no2g_len; i++) {
        printf("--------\n");

        create_one_line(H->data[i], H->data_len[i]);
    }
    printf("########\n");

}
int main()
{
    if (get_data_from_file("d:\\c1.txt")) {
        return -1;
    }
    run_data = create_array(no2g_len);
    print_array();

    create_data(H);
    create_data(V);

    return 0;
}
