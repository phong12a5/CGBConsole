#ifndef QGOOGLEAUTHENTICATOR_GLOBAL_H
#define QGOOGLEAUTHENTICATOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QGOOGLEAUTHENTICATOR_LIBRARY)
#  define QGOOGLEAUTHENTICATORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QGOOGLEAUTHENTICATORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QGOOGLEAUTHENTICATOR_GLOBAL_H
