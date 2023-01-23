#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>

pthread_mutex_t sum_mut, div_mut, sub_mut, mult_mut;
pthread_mutex_t sum_main_mut, div_main_mut, sub_main_mut, mult_main_mut;
pthread_t thread_sum, thread_div, thread_sub, thread_mult;
struct operators
{
    double a;
    double b;
};

void send_operators(struct operators ops);
void send_double(double x);
struct operators receive_operators();
double receive_double();

void *sum_runnable()
{
    struct operators op;
    double res;
    while (1)
    {
        pthread_mutex_lock(&sum_mut);
        printf("Start Sum Thread\n");
        op = receive_operators();
        printf("Operator 1 = %f ", op.a);
        printf("Operator 2 = %f \n", op.b);
        res = op.a + op.b;
        printf("Sum Result = %f\n", res);
        send_double(res);
        printf("End Sum Thread\n");
        pthread_mutex_unlock(&sum_main_mut);
    }
    pthread_exit(NULL);
}

void *div_runnable()
{
    struct operators op;
    double res;
    while (1)
    {
        pthread_mutex_lock(&div_mut);
        printf("Start Div Thread\n");
        op = receive_operators();
        printf("Operator 1 = %f ", op.a);
        printf("Operator 2 = %f \n", op.b);
        res = op.a / op.b;
        printf("Div Result = %f\n", res);
        send_double(res);
        printf("End Div Thread\n");
        pthread_mutex_unlock(&div_main_mut);
    }

    pthread_exit(NULL);
}

void *sub_runnable()
{
    struct operators op;
    double res;
    while (1)
    {
        pthread_mutex_lock(&sub_mut);
        printf("Start Sub Thread\n");
        op = receive_operators();
        printf("Operator 1 = %f ", op.a);
        printf("Operator 2 = %f \n", op.b);
        res = op.a - op.b;
        printf("Sub Result = %f\n", res);
        send_double(res);
        printf("End Sub Thread\n");
        pthread_mutex_unlock(&sub_main_mut);
    }

    pthread_exit(NULL);
}

void *mult_runnable()
{

    while (1)
    {
        struct operators op;
        double res;

        pthread_mutex_lock(&mult_mut);
        printf("Start Mult Thread\n");
        op = receive_operators();
        printf("Operator 1 = %f ", op.a);
        printf("Operator 2 = %f \n", op.b);
        res = op.a * op.b;
        printf("Mult Result = %f\n", res);
        send_double(res);
        printf("End Mult Thread\n");
        pthread_mutex_unlock(&mult_main_mut);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    printf("Enter three numbers\n");
    double a, b, c;
    double _x;
    double x[3];
    struct operators ops;

    pthread_mutex_init(&sum_mut, NULL);
    pthread_mutex_init(&div_mut, NULL);
    pthread_mutex_init(&sub_mut, NULL);
    pthread_mutex_init(&mult_mut, NULL);

    printf("Enter the 1st number: ");
    scanf("%lf", &a);
    printf("Enter the 2nd number: ");
    scanf("%lf", &b);
    printf("Enter the 3rd number: ");
    scanf("%lf", &c);
    x[0] = a;
    x[1] = b;
    x[2] = c;
    pthread_mutex_lock(&sum_main_mut);
    pthread_mutex_lock(&sum_mut);
    pthread_mutex_lock(&div_mut);
    pthread_mutex_lock(&div_main_mut);
    pthread_mutex_lock(&sub_mut);
    pthread_mutex_lock(&sub_main_mut);
    pthread_mutex_lock(&mult_mut);
    pthread_mutex_lock(&mult_main_mut);

    pthread_create(&thread_sum, NULL, sum_runnable, NULL);
    pthread_create(&thread_div, NULL, div_runnable, NULL);
    pthread_create(&thread_sub, NULL, sub_runnable, NULL);
    pthread_create(&thread_mult, NULL, mult_runnable, NULL);

    ops.a = a;
    ops.b = b;
    send_operators(ops);
    pthread_mutex_unlock(&sum_mut);

    pthread_mutex_lock(&sum_main_mut);

    ops.a = receive_double();
    ops.b = c;
    send_operators(ops);
    pthread_mutex_unlock(&sum_mut);
    pthread_mutex_lock(&sum_main_mut);

    ops.a = receive_double();
    ops.b = 3;
    send_operators(ops);
    pthread_mutex_unlock(&div_mut);
    pthread_mutex_lock(&div_main_mut);
    _x = receive_double();

    ops.a = x[0];
    ops.b = _x;
    send_operators(ops);
    pthread_mutex_unlock(&sub_mut);
    pthread_mutex_lock(&sub_main_mut);
    x[0] = receive_double();

    ops.a = x[1];
    send_operators(ops);
    pthread_mutex_unlock(&sub_mut);
    pthread_mutex_lock(&sub_main_mut);
    x[1] = receive_double();

    ops.a = x[2];
    send_operators(ops);
    pthread_mutex_unlock(&sub_mut);
    pthread_mutex_lock(&sub_main_mut);
    x[2] = receive_double();

    ops.a = x[0];
    ops.b = x[0];
    send_operators(ops);
    pthread_mutex_unlock(&mult_mut);
    pthread_mutex_lock(&mult_main_mut);
    x[0] = receive_double();

    ops.a = x[1];
    ops.b = x[1];
    send_operators(ops);
    pthread_mutex_unlock(&mult_mut);
    pthread_mutex_lock(&mult_main_mut);
    x[1] = receive_double();

    ops.a = x[2];
    ops.b = x[2];
    send_operators(ops);
    pthread_mutex_unlock(&mult_mut);
    pthread_mutex_lock(&mult_main_mut);
    x[2] = receive_double();

    ops.a = x[0];
    ops.b = x[1];
    send_operators(ops);
    pthread_mutex_unlock(&sum_mut);
    pthread_mutex_lock(&sum_main_mut);

    ops.a = receive_double();
    ops.b = x[2];
    send_operators(ops);
    pthread_mutex_unlock(&sum_mut);
    pthread_mutex_lock(&sum_main_mut);

    ops.a = receive_double();
    ops.b = 2;
    send_operators(ops);
    pthread_mutex_unlock(&div_mut);
    pthread_mutex_lock(&div_main_mut);
    _x = receive_double();
    printf("Result = %f\n", _x);

    pthread_mutex_destroy(&sum_mut);
    pthread_mutex_destroy(&div_mut);
    pthread_mutex_destroy(&sub_mut);
    pthread_mutex_destroy(&mult_mut);

    return 0;
}

void send_operators(struct operators ops)
{
    int f;

    f = open("./file", O_RDWR, 0);
    write(f, &ops, sizeof(struct operators));

    close(f);
}
void send_double(double x)
{
    int f;

    f = open("./file", O_RDWR, 0);
    write(f, &x, sizeof(double));
    close(f);
}

struct operators receive_operators()
{
    struct operators ops;
    int f;
    f = open("./file", O_RDWR, 0);
    read(f, &ops, sizeof(struct operators));
    close(f);
    return ops;
}

double receive_double()
{
    double op;
    int f;
    f = open("./file", O_RDWR, 0);
    read(f, &op, sizeof(double));
    close(f);
    return op;
}
