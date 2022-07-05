
#ifdef WIN32
    #include <windows.h>
    #include <winhttp.h>
#endif

#include <QtCore>
#include <QtNetwork>

#ifdef Q_WS_WIN
    #include <qt_windows.h>
#endif

#include "ProxyDetection.h"
#include "LicenseManagerGui.h"

#define PROXY_DEBUG 0

#if PROXY_DEBUG
    #define pDebug qDebug
#else
    QByteArray ba_debug;
    #define pDebug() QDataStream(&ba_debug, QIODevice::WriteOnly)
#endif

void ProxyDetection::init()
{
    int port;
    QString host;
    bool hasProxy = ProxyDetection::detectProxy(LicenseGeneratorHost, &host, &port);
    if (hasProxy) {
        QString proxy1 = QString("HTTP_PROXY=%1:%2").arg(host).arg(port);
        _putenv(proxy1.toLatin1().data());
        QString proxy2 = QString("http_proxy=%1:%2").arg(host).arg(port);
        _putenv(proxy2.toLatin1().data());
        
        QNetworkProxy np;
        np.setPort(port);
        np.setHostName(host);
        np.setType(QNetworkProxy::HttpProxy);
        QNetworkProxy::setApplicationProxy(np);
        qDebug() << "Detected proxy:" << host << port;
    }
}

bool ProxyDetection::detectProxy(QString url, QString * host, int * port)
{
#ifdef Q_WS_WIN
    pDebug() << "ProxyDetection::detectProxy()";

    WINHTTP_CURRENT_USER_IE_PROXY_CONFIG proxyConfig;
    bool ok = WinHttpGetIEProxyConfigForCurrentUser( &proxyConfig );

    if ( !ok ) {
        DWORD error = GetLastError();
        pDebug() << "WinHttpGetIEProxyConfigForCurrentUser() fails, GetLastError():" << long(error);
        return false;
    }

    QString proxyConfigUrl = QString::fromUtf16((ushort*)proxyConfig.lpszAutoConfigUrl);
    QString proxyBypass = QString::fromUtf16((ushort*)proxyConfig.lpszProxyBypass);
    QString proxy = QString::fromUtf16((ushort*)proxyConfig.lpszProxy);
    bool autoDetect = proxyConfig.fAutoDetect;

    if (autoDetect)
        pDebug() << "Enabled proxy auto detection.";
    
    if (!proxy.isEmpty()) {
        pDebug() << "Proxy:" << proxy;

        foreach(QString proxyEntry, proxy.split(";")) {
            QStringList args = proxyEntry.split("=");
            QString proxyHostPort;
            QString protocol;

            if (args.count() < 2) {
                protocol = "http";
                proxyHostPort = args[0];
            }
            else {
                protocol = args[0];
                proxyHostPort = args[1];
            }

            if (url.startsWith(protocol + ":")) {
                if (proxyHostPort.startsWith(protocol +"://"))
                    proxyHostPort = proxyHostPort.mid( protocol.length() +3 );

                args = proxyHostPort.split(":");

                *port = 3128;
                *host = args[0];
                if ( args.count() == 2 ) {
                    *port = args[1].toInt();
                }
                
                pDebug() << "Detected proxy " << *host << *port;
                pDebug() << "For url" << url;
                return true;
            }
        }
        return false;
    }

    if (!proxyBypass.isEmpty())
        pDebug() << "Proxy bypass list:" << proxyBypass;

    if (!proxyConfigUrl.isEmpty())
        pDebug() << "Automatic configuration script:" << proxyConfigUrl;

    if ( !autoDetect && proxyConfigUrl.isEmpty() ) {
        pDebug() << "No autoDetect and proxyConfigUrl enabled.";
        return false;
    }

    WINHTTP_AUTOPROXY_OPTIONS autoProxyOptions;    
    if (autoDetect) {
        autoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_AUTO_DETECT;
        autoProxyOptions.dwAutoDetectFlags = WINHTTP_AUTO_DETECT_TYPE_DHCP | WINHTTP_AUTO_DETECT_TYPE_DNS_A;
    }
    else {
        autoProxyOptions.dwFlags = 0;
        autoProxyOptions.dwAutoDetectFlags = 0;
    }

    if (!proxyConfigUrl.isEmpty()) {
        autoProxyOptions.dwFlags |= WINHTTP_AUTOPROXY_CONFIG_URL;
        autoProxyOptions.lpszAutoConfigUrl = proxyConfig.lpszAutoConfigUrl;
    }
    else 
        autoProxyOptions.lpszAutoConfigUrl = 0L;
    
    autoProxyOptions.fAutoLogonIfChallenged = false;
    autoProxyOptions.lpvReserved = NULL;
    autoProxyOptions.dwReserved = 0;

    HINTERNET hSession = WinHttpOpen(L"Pipe-It", 
                                     WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     WINHTTP_NO_PROXY_NAME, 
                                     WINHTTP_NO_PROXY_BYPASS, 0);
    
    if (hSession == NULL) {
        DWORD error = GetLastError();
        pDebug() << "WinHttpOpen() fails, GetLastError():" << long(error);
        return false;
    }

    WINHTTP_PROXY_INFO proxyInfo;
    ok = WinHttpGetProxyForUrl( hSession, 
                                (WCHAR*)url.utf16(), 
                                &autoProxyOptions, 
                                &proxyInfo);

    if ( !ok ) {
        DWORD error = GetLastError();
        pDebug() << "WinHttpGetProxyForUrl() fails, GetLastError():" << long(error);
        WinHttpCloseHandle(hSession);
        return false;
    }

    proxyBypass = QString::fromUtf16((ushort*)proxyInfo.lpszProxyBypass);
    proxy = QString::fromUtf16((ushort*)proxyInfo.lpszProxy);
    DWORD dwAccessType = proxyInfo.dwAccessType;

    WinHttpCloseHandle(hSession);

    pDebug() << "For url: " << url;

    if ( !proxy.isNull() ) {
        pDebug() << "Proxy:" << proxy;
    }

    if ( !proxyBypass.isNull() )
        pDebug() << "Proxy for Proxy bypass list:" << proxyBypass;

    if ( dwAccessType == WINHTTP_ACCESS_TYPE_NO_PROXY ) {
        pDebug() << "Proxy Access Type: Direct connection";
        return false; // direct connection
    }
    else if ( dwAccessType == WINHTTP_ACCESS_TYPE_DEFAULT_PROXY )
        pDebug() << "Proxy Access Type: Default proxy";
    else if ( dwAccessType == WINHTTP_ACCESS_TYPE_NAMED_PROXY ) {
        pDebug() << "Proxy Access Type: Named proxy";

        if (proxy.startsWith("http://"))
            proxy = proxy.mid(7);

        QStringList args = proxy.split(":");
        *host = args[0];
        *port = 3128;

        if ( args.count() == 2 ) {
            *port = args[1].toInt();
        }
        
        pDebug() << "Detected proxy " << *host << *port;
        pDebug() << "For url" << url;
        return true;
    }

    pDebug() << "";

    if ( proxy.isNull() ) return false; // direct connection
    if ( proxy == url ) return false; // direct connection
#endif

    return false;
}

