#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

inline double get_k(double x0, double y0, double x1, double y1)
{
    if (x0 == x1)
        return (y1 - y0) / (1e-20);
    return (y1 - y0) / (x1 - x0);
}

inline double get_d(double x0, double y0, double x1, double y1)
{
    return sqrt((y1 - y0) * (y1 - y0) +  (x1 - x0) *  (x1 - x0));
}

inline double get_y(double delta_x, double k)
{
    return delta_x * k;
}

void get_dxy(double delta, double k, double *dx, double *dy)
{
    double delta_x = sqrt((delta *delta) / (k * k + 1.0f));
//    printf("%lf %lf %lf %lf %lf\n", delta *delta,  (k * k + 1.0f), (delta *delta) / (k * k + 1.0f), delta_x, delta);
    if (delta < 0.0f)
        *dx = -delta_x;
    else
        *dx = delta_x;
    *dy = k * (*dx);
}

int gen_line(double x0, double y0, double x1, double y1)
{
    double speed = 1.8;
    double speed_mps = speed * 1000.0 / 3600.0;
    double unit = speed_mps / 30.0;
    double delta_m = unit * 2.5;

    int i, c;
    double x, y, d, k, dx, dy;

    d = get_d(x0,y0,x1,y1);
    k = get_k(x0,y0,x1,y1);
    c = (int)(d / delta_m);

    //printf("d=%lf k=%lf c=%d\n", d, k, c);

    x = x0;
    y = y0;
    if (x1 < x0)
        delta_m = -delta_m;

    get_dxy(delta_m, k, &dx, &dy);
    //printf("delta_m=%lf dx=%lf, dy=%lf", delta_m, dx, dy);

    for (i=0; i<c; i++) {
        printf("%0.6f,%0.6f,0.00,%0.6f \n", x, y, speed_mps);
        x += dx;
        y += dy;
    }

    return 0;
}

struct point {
    double x;
    double y;
};

struct point point_array_[] = {
#if 0
    {0, 0},
    {105.17f, 0.00f},
    {105.17f, 127.45f},
    {0.00f, 125.53f},
    {0, 0},
    {45.0f, 0},
#endif
#if 1
//test 1
    {0, 0},
    {7.9248f, 0.0},
    {7.9248f, -4.572f},
    {0.00f, -4.572f},
    {0.00f, 0.00f},
    {4.50f, 0.00f},
#endif
/* lab
    {0, 0},
    {9.144f, 0.0},
    {9.144f, -6.5532f},
    {0.00f, -6.5532f},
    {0.00f, 0.00f},
    {5.00f, 0.00f},
    //{7.45f, 3.20f},
*/
/* demo 1.0
    {0, 0},
    {5.00, 0},
    {7.45f, 3.05f},
    {16.37f, 3.05f},
    //{7.45f, 3.20f},
    //{16.37f, 3.20f},
*/
/* demo 1.2 
    {0, 0},
    {19.2024f, 0},
    {19.5072f, 19.5072f},
    {14.7828f, 21.9456f},
    {9.23544f, 9.84504f},
*/

};

void generate_waypoints(void *array, int count)
{
    struct point * point_array = (struct point *)array;
    int i;
    for (i=0; i<count-1; i++) {
        gen_line(point_array[i].x, point_array[i].y, point_array[i+1].x, point_array[i+1].y);
    }
}

#ifndef USE_GPX
int main(int argc, char** argv)
{
    struct point * point_array = NULL;
    int line = 0;
//    printf("%d argc \n", argc);
    if (argc > 1) {
        char *fname = argv[1];
        FILE *fp = fopen(fname, "r");
        if (!fp)
            return -1;
        char buff[256] = {0};
        double x = 0.0f;
        double y = 0.0f;

        while (fgets(buff, 256, fp)) {
            line++;
        }
        fseek(fp, 0, SEEK_SET);
        point_array = (struct point *)malloc(line*sizeof(struct point));
        int i = 0;
        while (fgets(buff, 256, fp)) {
            sscanf(buff, "%lf,%lf", &x, &y);
            point_array[i].x = x;
            point_array[i].y = y;
            i++;
        }
        fclose(fp);
    }
    else {
        point_array = point_array_;
        line = sizeof (point_array_) / sizeof(struct point);
    }

    printf("0.00,0.00,0.00\n");

#if 0
    int i;
    int count = sizeof (point_array) / sizeof(struct point);
    for (i = 0; i<count-1; i++) 
        gen_line(point_array[i].x, point_array[i].y, point_array[i+1].x, point_array[i+1].y);
#endif

    int count = line;
    generate_waypoints(&point_array[0], count);

    printf("200,0.00,0.00,0.00\n");
    printf("200,0.00,0.00,0.00\n");

    if (argc > 1)
        free(point_array);
    return 0;
}
#endif
