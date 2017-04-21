// Example of using the GeographicLib::UTMUPS class

#include <iostream>
#include <exception>
#include <string>
#include <iomanip>
#include <GeographicLib/UTMUPS.hpp>

using namespace std;
using namespace GeographicLib;

struct wgs84_point {
    double lat;
	double lon;
};

struct wgs84_point wgs84_point_array[] = {
    {37.4102215, -121.9492966},
    {37.4100649, -121.9494361},
    {37.4098138, -121.9490249},
    {37.4098639, -121.9489407},
    {37.4101878, -121.9491914},
    {37.4101546, -121.9493582},
};

struct utm_point {
    double x;
    double y;
};

double PI=3.1415926f;
double X, Y, X0=0, Y0;
double lat, lon;
int zone;
bool northp;

double get_degree(double x, double y)
{
    double rad = atan2(x, y);
    return rad * (180 / PI);
}

void rotate(double &x, double &y, double degree)
{
    double distance = sqrt(x*x + y*y);
    double orig_degree = get_degree(x, y);
    orig_degree -= degree;
    x = distance * sin(orig_degree * PI/180);
    y = distance * cos(orig_degree * PI/180);
}

extern void generate_waypoints(void *array, int count);

int main() {
    int len = sizeof(wgs84_point_array)/sizeof(struct wgs84_point);
    struct utm_point *new_array = new struct utm_point[len];
    int i;

#ifdef PRINT_GPX
    cout << "Lat, Lon:" << endl;
    for (i=0; i<len; i++) {
        cout << fixed << setprecision(7) << "(" << wgs84_point_array[i].lat << ", " << wgs84_point_array[i].lon << ")\n";
    }
    cout << endl;
#endif

    for (i=0 ; i<len; i++) {
        lat = wgs84_point_array[i].lat;
        lon = wgs84_point_array[i].lon;
        UTMUPS::Forward(lat, lon, zone, northp, X, Y);
        string zonestr = UTMUPS::EncodeZone(zone, northp);
        new_array[i].x = X;
        new_array[i].y = Y;
    }

#if 0
    for (i=1; i<len; i++) {
        double x =  new_array[i].x -  new_array[i-1].x;
        double y =  new_array[i].y -  new_array[i-1].y;
        cout <<  fixed << setprecision(6) << "distance: " << sqrt( x*x + y*y) << endl;
    }
#endif

    double x0 = new_array[0].x;
    double y0 = new_array[0].y;
    for (i=0 ; i<len; i++) {
        new_array[i].x -= x0;
        new_array[i].y -= y0;
        //cout << fixed << setprecision(6) << new_array[i].x << " " << new_array[i].y << " " << get_degree(new_array[i].x, new_array[i].y) << "\n";
    }

    double degree1 = get_degree(new_array[1].x, new_array[1].y);
    
    for (i=0; i<len; i++) {
        double x = new_array[i].x;
        double y = new_array[i].y;
        rotate(x, y, degree1);
        new_array[i].x = x;
        new_array[i].y = y;
    }

#if 0
    for (i=0 ; i<len; i++) {
        cout << fixed << setprecision(6) << new_array[i].x << " " << new_array[i].y << " " << get_degree(new_array[i].x, new_array[i].y) << "\n";
    }

    for (i=1; i<len; i++) {
        double x =  new_array[i].x -  new_array[i-1].x;
        double y =  new_array[i].y -  new_array[i-1].y;
        cout <<  fixed << setprecision(6) << "distance1: " << sqrt( x*x + y*y) << endl;
    }
#endif

    for (i=0; i<len; i++) {
        double t = new_array[i].x;
        new_array[i].x = new_array[i].y;
        new_array[i].y = -t;
        if (new_array[i].x <= 0.0000f && new_array[i].x >= -1e-20)
            new_array[i].x = 0.00f;
        if (new_array[i].y <= 0.0000f && new_array[i].y >= -1e-20)
            new_array[i].y = 0.00f;
    }

#ifdef PRINT_GPX
    for (i=0; i<len; i++) {
        cout << fixed << setprecision(6) << "(" <<new_array[i].x << ", " << new_array[i].y << ")\n";
    }
    cout << endl;
    cout << "X, Y:" << endl;
    cout << "Distance:" << endl;
    for (i=1; i<len; i++) {
        double x =  new_array[i].x -  new_array[i-1].x;
        double y =  new_array[i].y -  new_array[i-1].y;
        cout <<  fixed << setprecision(6) << sqrt( x*x + y*y) << endl;
    }
    cout << endl;
#endif

#ifndef PRINT_GPX
    cout <<"0.00,0.00,0.00" << endl;

//    generate_waypoints(&new_array[0], len);
    for (i=0; i<len; i++) {
        cout << "(" << new_array[i].x << ", " << new_array[i].y << ")," << endl;
    }
    cout << "200,0.00,0.00,0.00" << endl;
    cout << "200,0.00,0.00,0.00" << endl;

#endif
    delete []new_array;
    return 0;
}
