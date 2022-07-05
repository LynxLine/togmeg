
#ifndef PROXYDETECTION_H
#define PROXYDETECTION_H

#include <QString>

class ProxyDetection {
public:
    static void init();
    static bool detectProxy( QString url, QString * host, int * port);
}; 

#endif // PROXYDETECTION_H

