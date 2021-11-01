#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct data {
    int *data_len;
    int **data;
    int *num;
    char ***out_data;
};

int no2g_len;
struct data* H; //水平
struct data* V; //垂直

int answer_H[32];
int answer_V[32];

char out[64];
int dfs2_cnt = 0;

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

void print_arr(char *a, int len)
{
    for (int i = 0; i < len; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return;
}

int dfs(int* in, int in_cnt, int n, int start, struct data* H, int line_cnt)
{
    int ret, i;
    if (n == in_cnt) {
        H->num[line_cnt] += 1;
        H->out_data[line_cnt] = realloc(H->out_data[line_cnt], sizeof(char *) * H->num[line_cnt]);

        H->out_data[line_cnt][H->num[line_cnt] - 1] = malloc(sizeof(char) * no2g_len);

        memcpy(H->out_data[line_cnt][H->num[line_cnt] - 1], out,sizeof(char) *  no2g_len);
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
            if (!dfs(in, in_cnt, n + 1, j, H, line_cnt)) {
                break;
            }
        }
    }
    return 0;
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

struct data *create_VH()
{
    struct data *H = malloc(sizeof(struct data));
    memset(H, 0, sizeof(struct data));

    H->num = malloc(sizeof(int) * no2g_len);
    memset(H->num, 0, sizeof(sizeof(int) * no2g_len));

    H->out_data = malloc(sizeof(char **) * no2g_len);
    memset(H->out_data, 0, sizeof(sizeof(char **) * no2g_len));


    return H;
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
    H = create_VH();
    V = create_VH();

    init_data(fp, H);
    init_data(fp, V);

    fclose(fp);

    return 0;
}

int create_one_line(int *in, int in_no2g_len, struct data* H, int line_cnt)
{
    int sum = 0;
    for (int i = 0; i < in_no2g_len; i++) {
        sum += in[i];
    }
    if (sum + in_no2g_len - 1 > no2g_len) {
        return -1;
    }
    dfs(in, in_no2g_len, 0, 0, H, line_cnt);
    return 0;
}

void create_data(struct data* H)
{
    for (int i = 0; i < no2g_len; i++) {
        if(create_one_line(H->data[i], H->data_len[i], H, i) == -1){
            printf("input error.%d\n", i);
        }
    }

    return;
}

int check_answer(int n){
    for(int i=0;i<=n;i++){
        //  H的第i行第j字符，比V的第j列第
        if(H->out_data[n][answer_H[n]][i] != V->out_data[i][answer_V[i]][n] ||
            V->out_data[n][answer_V[n]][i] != H->out_data[i][answer_H[i]][n] ){
            return -1;
        }
    }
  
    return 0;
}

void print_answer(struct data* a)
{
    int *answer;
    if(a == V){
        answer = answer_V;
    } else if(a == H){
        answer = answer_H;
    } else {
        return;
    }
    for(int i=0;i<no2g_len;i++){
        print_arr(a->out_data[i][answer[i]], no2g_len);
    }

    return;
}

int dfs2(int n)
{
    int ret = -1;
    dfs2_cnt++;

    if(n == no2g_len) {
        printf("success. dfs2_cnt = %d\n", dfs2_cnt);

        return 0;
    }
    for(int i = 0; i < H->num[n]; i++) {
        for(int j=0; j< V->num[n];j++){
            answer_H[n] = i;
            answer_V[n] = j;

            if(check_answer(n) == -1) {
                continue;
            }

            ret = dfs2(n+1);
            if(ret == 0){
                goto end;
            }
        }
    }
end:
    return ret;
}

int main(int argc, char* argv[])
{
    if (get_data_from_file(argv[1])) {
        return -1;
    }

    create_data(H);
    create_data(V);

    if(dfs2(0) == 0){
        print_answer(V);
    }

    return 0;
}
