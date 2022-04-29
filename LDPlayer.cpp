#include "LDCommand.h"
#include "LDPlayer.h"

#include <QByteArray>
#include <QCryptographicHash>
#include <QTextCodec>
#include "ScreenReferences.h"

#define INVALID_POINT QPoint(-1,-1)
#define LOG qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")<<"ID:"<<profile.id<<__FUNCTION__<<__LINE__

using namespace screen_references;

int LDPlayer::started = 0;

double LDPlayer::value = 0;

QString modelBase = "QXN1c3xBbGNhdGVsIFBpeGkgNCAoNSkqQXN1c3xBc3VzIFJPRyBQaG9uZSpBc3VzfEFzdXMgWmVuZm9uZSAyIExhc2VyKkFzdXN8QXN1cyBaZW5Gb25lIDMqQXN1c3xBc3VzIFplbmZvbmUgMypBc3VzfEFzdXMgWmVuZm9uZSAzIERlbHV4ZSA1LjUqQXN1c3xBc3VzIFplbmZvbmUgMyBMYXNlcipBc3VzfEFzdXMgWmVuZm9uZSAzIE1heCpBc3VzfEFzdXMgWmVuZm9uZSAzIFVsdHJhKkFzdXN8QXN1cyBaZW5mb25lIDMgWm9vbSpBc3VzfEFzdXMgWmVuZm9uZSAzcyBNYXgqQXN1c3xBc3VzIFplbkZvbmUgNCAoWkU1NTRLTCkqQXN1c3xBc3VzIFplbkZvbmUgNCBNYXgqQXN1c3xBc3VzIFplbkZvbmUgNCBNYXggUHJvIChaQzU1NEtMKSpBc3VzfEFzdXMgWmVuRm9uZSA0IFBybypBc3VzfEFzdXMgWmVuRm9uZSA0IFNlbGZpZSAoWkQ1NTNLTCkqQXN1c3xBc3VzIFplbkZvbmUgNCBTZWxmaWUgUHJvIChaRDU1MktMKSpBc3VzfEFzdXMgWmVuZm9uZSA1KkFzdXN8QXN1cyBaZW5mb25lIDUgTGl0ZSpBc3VzfEFzdXMgWmVuZm9uZSA1USpBc3VzfEFzdXMgWmVuZm9uZSA1WipBc3VzfEFzdXMgWmVuZm9uZSBBUipBc3VzfEFzdXMgWmVuZm9uZSBHbypBc3VzfEFzdXMgWmVuZm9uZSBHbyAoWkI1NTJLTCkqQXN1c3xBc3VzIFplbkZvbmUgTGl2ZSAoTDEpIFpBNTUwS0wqQXN1c3xBc3VzIFplbmZvbmUgTGl2ZSAoWkI1MDFLTCkqQXN1c3xBc3VzIFplbmZvbmUgTWF4KkFzdXN8QXN1cyBaZW5mb25lIE1heCAoTTEpIFpCNTU1S0wqQXN1c3xBc3VzIFplbmZvbmUgTWF4IFBsdXMgKE0xKSBaQjU3MFRMKkFzdXN8QXN1cyBaZW5mb25lIE1heCBQcm8gKE0xKSBaQjYwMUtMKkFzdXN8QXN1cyBaZW5mb25lIE1heCBQcm8gTTEqQXN1c3xBc3VzIFplbkZvbmUgTWF4IFBybyBNMSpBc3VzfEFzdXMgWmVuZm9uZSBWIFY1MjBLTCpBc3VzfEFzdXMgWmVuUGFkIDNzIDEwKkFzdXN8QXN1cyBaZW5QYWQgM3MgOC4wKkFzdXN8QXN1cyBaZW5QYWQgWjEwKkFzdXN8QXN1cyBaZW5wYWQgWjhzIChaVDU4MktMKSpBc3VzfEJsdWJvbyBENipBc3VzfEJRIEFxdWFyaXMgTTUqQXN1c3xEb29nZWUgWDUgTWF4KkFzdXN8RWxlcGhvbmUgVSBQcm8qQXN1c3xFc3NlbnRpYWwgUGhvbmUgUEgtMSpBc3VzfEdlbmVyYWwgTW9iaWxlIEdNIDUqR29vZ2xlfEdvb2dsZSBOZXh1cyAxMCpHb29nbGV8R29vZ2xlIE5leHVzIDQqR29vZ2xlfEdvb2dsZSBOZXh1cyA1Kkdvb2dsZXxHb29nbGUgTmV4dXMgNipHb29nbGV8R29vZ2xlIE5leHVzIDZQKkdvb2dsZXxHb29nbGUgUGl4ZWwqR29vZ2xlfEdvb2dsZSBQaXhlbCAyKkdvb2dsZXxHb29nbGUgUGl4ZWwgMiBYTCpHb29nbGV8R29vZ2xlIFBpeGVsIEMqR29vZ2xlfEdvb2dsZSBQaXhlbCBYTCpIVEN8SFRDIDEwKkhUQ3xIVEMgMTAgRXZvKkhUQ3xIVEMgMTAgTGlmZXN0eWxlKkhUQ3xIVEMgRGVzaXJlIDEwIExpZmVzdHlsZSpIVEN8SFRDIERlc2lyZSAxMCBQcm8qSFRDfEhUQyBEZXNpcmUgMTIqSFRDfEhUQyBEZXNpcmUgMTIrKkhUQ3xIVEMgT25lIE04KkhUQ3xIVEMgT25lIE05KkhUQ3xIVEMgVSBQbGF5KkhUQ3xIVEMgVSBVbHRyYSpIVEN8SFRDIFUxMSpIVEN8SFRDIFUxMSBFeWVzKkhUQ3xIVEMgVTExIExpZmUqSFRDfEhUQyBVMTErKkhUQ3xIVEMgVTEyIGxpZmUqSFRDfEhUQyBVMTIrKkh1YXdlaXxIdWF3ZWkgR1IzICgyMDE3KSpIdWF3ZWl8SHVhd2VpIEhvbm9yIDEwKkh1YXdlaXxIdWF3ZWkgSG9ub3IgNlgqSHVhd2VpfEh1YXdlaSBIb25vciA3QSpIdWF3ZWl8SHVhd2VpIEhvbm9yIDdzKkh1YXdlaXxIdWF3ZWkgSG9ub3IgN1gqSHVhd2VpfEh1YXdlaSBIb25vciA4IExpdGUqSHVhd2VpfEh1YXdlaSBIb25vciA4IFBybypIdWF3ZWl8SHVhd2VpIEhvbm9yIDkqSHVhd2VpfEh1YXdlaSBIb25vciA5IExpdGUqSHVhd2VpfEh1YXdlaSBIb25vciA5TiAoOWkpKkh1YXdlaXxIdWF3ZWkgSG9ub3IgTm90ZSAxMCpIdWF3ZWl8SHVhd2VpIEhvbm9yIFBsYXkqSHVhd2VpfEh1YXdlaSBIb25vciBWaWV3IDEwKkh1YXdlaXxIdWF3ZWkgTWF0ZSAxMCpIdWF3ZWl8SHVhd2VpIE1hdGUgMTAgTGl0ZSpIdWF3ZWl8SHVhd2VpIE1hdGUgMTAgUHJvKkh1YXdlaXxIdWF3ZWkgTWF0ZSAyMCBMaXRlKkh1YXdlaXxIdWF3ZWkgTWF0ZSA4Kkh1YXdlaXxIdWF3ZWkgTWF0ZSA5Kkh1YXdlaXxIdWF3ZWkgTWF0ZSA5IFBvcnNjaGUgRGVzaWduKkh1YXdlaXxIdWF3ZWkgTWF0ZSA5IFBybypIdWF3ZWl8SHVhd2VpIE5vdmEgMipIdWF3ZWl8SHVhd2VpIE5vdmEgMiBQbHVzKkh1YXdlaXxIdWF3ZWkgTm92YSAyaSpIdWF3ZWl8SHVhd2VpIG5vdmEgMypIdWF3ZWl8SHVhd2VpIE5vdmEgM2UqSHVhd2VpfEh1YXdlaSBub3ZhIDNpKkh1YXdlaXxIdWF3ZWkgTm92YSBMaXRlKkh1YXdlaXxIdWF3ZWkgUCBzbWFydCpIdWF3ZWl8SHVhd2VpIFAgU21hcnQrKkh1YXdlaXxIdWF3ZWkgUDEwKkh1YXdlaXxIdWF3ZWkgUDEwIExpdGUqSHVhd2VpfEh1YXdlaSBQMTAgUGx1cypIdWF3ZWl8SHVhd2VpIFAyMCpIdWF3ZWl8SHVhd2VpIFAyMCBMaXRlKkh1YXdlaXxIdWF3ZWkgUDIwIFBybypIdWF3ZWl8SHVhd2VpIFA4IExpdGUgKDIwMTcpKkh1YXdlaXxIdWF3ZWkgUDggTGl0ZSAyMDE3Kkh1YXdlaXxIdWF3ZWkgUDkgTGl0ZSpIdWF3ZWl8SHVhd2VpIFA5IExpdGUgKDIwMTcpKkh1YXdlaXxIdWF3ZWkgUDkgTGl0ZSAyMDE3Kkh1YXdlaXxIdWF3ZWkgWTMgKDIwMTgpKkh1YXdlaXxIdWF3ZWkgWTUgUHJpbWUgKDIwMTgpKkh1YXdlaXxIdWF3ZWkgWTYgKDIwMTgpKkh1YXdlaXxIdWF3ZWkgWTcgKDIwMTgpKkh1YXdlaXxIdWF3ZWkgWTcgUHJpbWUqSHVhd2VpfEh1YXdlaSBZNyBQcmltZSAyMDE4Kkh1YXdlaXxIdWF3ZWkgWTcgUHJvICgyMDE4KSpIdWF3ZWl8SVVOSSBVMipMZUVjb3xMZUVjbyBMZSAxcypMZUVjb3xMZUVjbyBMZSAyKkxlRWNvfExlRWNvIExlIE1heCAyKkxlRWNvfExlRWNvIExlIFBybyAzKkxlbm92b3xMZW5vdm8gQTUqTGVub3ZvfExlbm92byBBNjAwMCpMZW5vdm98TGVub3ZvIEE2MDAwIFBsdXMqTGVub3ZvfExlbm92byBBNjYwMCBQbHVzKkxlbm92b3xMZW5vdm8gSzMyMHQqTGVub3ZvfExlbm92byBLNSpMZW5vdm98TGVub3ZvIEs1IE5vdGUgKDIwMTgpKkxlbm92b3xMZW5vdm8gSzUgcGxheSpMZW5vdm98TGVub3ZvIEs2Kkxlbm92b3xMZW5vdm8gSzYgTm90ZSpMZW5vdm98TGVub3ZvIEs2IFBvd2VyKkxlbm92b3xMZW5vdm8gSzgqTGVub3ZvfExlbm92byBLOCBOb3RlKkxlbm92b3xMZW5vdm8gSzggUGx1cypMZW5vdm98TGVub3ZvIFAyKkxlbm92b3xMZW5vdm8gUzUqTGVub3ZvfExlbm92byBaNSpMZW5vdm98TGVub3ZvIFp1ayBFZGdlKkxlbm92b3xMZW5vdm8gWnVrIFoxKkxlbm92b3xMZW5vdm8gWnVrIFoyKkxlbm92b3xMZW5vdm8gWlVLIFoyIChQbHVzKSpMZW5vdm98TGVub3ZvIFp1ayBaMiBQcm8qTEd8TEcgQXJpc3RvIDIqTEd8TEcgRzIqTEd8TEcgRzMqTEd8TEcgRzUqTEd8TEcgRzYqTEd8TEcgRzcgRml0KkxHfExHIEc3IE9uZSpMR3xMRyBHNyBUaGluUSpMR3xMRyBHNyBUaGluUSBQbHVzKkxHfExHIEsxMCAyMDE4KkxHfExHIEsxMSBQbHVzKkxHfExHIEszMCpMR3xMRyBLOCAyMDE4KkxHfExHIE5leHVzIDVYKkxHfExHIFBhZCBJViA4LjAqTEd8TEcgUSBTdHlsdXMqTEd8TEcgUTYqTEd8TEcgUTcqTEd8TEcgUTgqTEd8TEcgVjEwKkxHfExHIFYyMCpMR3xMRyBWMzAqTEd8TEcgVjMwUyBUaGluUSpMR3xMRyBWMzUgVGhpblEqTEd8TEcgWCBWZW50dXJlKkxHfExHIFpvbmUgNCpNb3Rvcm9sYXxNb3RvIEMqTW90b3JvbGF8TW90byBFIDIwMTUqTW90b3JvbGF8TW90byBFNCpNb3Rvcm9sYXxNb3RvIEU0IFBsdXMqTW90b3JvbGF8TW90byBFNSpNb3Rvcm9sYXxNb3RvIEU1IFBsYXkqTW90b3JvbGF8TW90byBFNSBQbGF5IEdvKk1vdG9yb2xhfE1vdG8gRTUgUGx1cypNb3Rvcm9sYXxNb3RvIEcgMjAxMypNb3Rvcm9sYXxNb3RvIEcgMjAxNCpNb3Rvcm9sYXxNb3RvIEcgMjAxNSpNb3Rvcm9sYXxNb3RvIEc0Kk1vdG9yb2xhfE1vdG8gRzQgUGx1cypNb3Rvcm9sYXxNb3RvIEc1Kk1vdG9yb2xhfE1vdG8gRzUgUGx1cypNb3Rvcm9sYXxNb3RvIEc1UypNb3Rvcm9sYXxNb3RvIEc1UyBQbHVzKk1vdG9yb2xhfE1vdG8gRzYqTW90b3JvbGF8TW90byBHNiBQbGF5Kk1vdG9yb2xhfE1vdG8gRzYgUGx1cypNb3Rvcm9sYXxNb3RvIE0qTW90b3JvbGF8TW90byBYIFB1cmUqTW90b3JvbGF8TW90byBYNCpNb3Rvcm9sYXxNb3RvIFoqTW90b3JvbGF8TW90byBaIEZvcmNlKk1vdG9yb2xhfE1vdG8gWiBQbGF5Kk1vdG9yb2xhfE1vdG8gWjIgRm9yY2UqTW90b3JvbGF8TW90byBaMiBQbGF5Kk1vdG9yb2xhfE1vdG8gWjMqTW90b3JvbGF8TW90byBaMyBQbGF5Kk1vdG9yb2xhfE1vdG9yb2xhIE1vdG8gRTQqTW90b3JvbGF8TW90b3JvbGEgTW90byBHNiBQbHVzKk1vdG9yb2xhfE1vdG9yb2xhIE1vdG8gWCBQbGF5Kk1vdG9yb2xhfE1vdG9yb2xhIE9uZSBQb3dlcipNb3Rvcm9sYXxNb3Rvcm9sYSBQMzAqTW90b3JvbGF8TmV4dXMgNVgqTW90b3JvbGF8TmV4dXMgNlAqTW90b3JvbGF8TmV4dXMgUGxheWVyKk5va2lhfE5va2lhIDEqTm9raWF8Tm9raWEgMipOb2tpYXxOb2tpYSAyLjEqTm9raWF8Tm9raWEgMypOb2tpYXxOb2tpYSAzLjEqTm9raWF8Tm9raWEgNSpOb2tpYXxOb2tpYSA1LjEqTm9raWF8Tm9raWEgNS4xIFBsdXMqTm9raWF8Tm9raWEgNipOb2tpYXxOb2tpYSA2LjEqTm9raWF8Tm9raWEgNi4xIFBsdXMqTm9raWF8Tm9raWEgNypOb2tpYXxOb2tpYSA3IFBsdXMqTm9raWF8Tm9raWEgNy4xKk5va2lhfE5va2lhIDgqTm9raWF8Tm9raWEgOCBTaXJvY2NvKk5va2lhfE5va2lhIFg1Kk5va2lhfE5va2lhIFg2Kk5va2lhfE51YmlhIFoxNypPbmVQbHVzfE9uZVBsdXMgMipPbmVQbHVzfE9uZVBsdXMgMypPbmVQbHVzfE9uZVBsdXMgM1QqT25lUGx1c3xPbmVQbHVzIDUqT25lUGx1c3xPbmVQbHVzIDUvNVQqT25lUGx1c3xPbmVQbHVzIDVUKk9uZVBsdXN8T25lUGx1cyBPbmUqT25lUGx1c3xPbmVQbHVzIFgqUmVkbWl8UmVkbWkgM3MvM3gvUHJpbWUqUmVkbWl8UmVkbWkgNCBQcmltZSpSZWRtaXxSZWRtaSA0WCpSZWRtaXxSZWRtaSA1IFBsdXMqUmVkbWl8UmVkbWkgTm90ZSA1KlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgQTMgKDIwMTcpKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgQTUgKDIwMTYpKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgQTUgKDIwMTcpKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgQTYgMjAxOCpTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IEE2KyAyMDE4KlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgQTcgKDIwMTYpKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgQTcgKDIwMTcpKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgQTggKDIwMTYpKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgQTggKDIwMTcpKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgQTggMjAxOCpTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IEE4IFN0YXIqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBBOCsgMjAxOCpTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IEE5IFBybypTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IEE5IFN0YXIqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBBbHBoYSpTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IEM3IFBybypTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IEM5IFBybypTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IEdyYW5kIFByaW1lKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgSipTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IEoyIENvcmUqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBKMiBQcm8gKDIwMTgpKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgSjMgKDIwMTgpKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgSjQgKDIwMTgpKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgSjUqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBKNSAoMjAxNykqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBKNiAoMjAxOCkqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBKNyAoMjAxOCkqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBKNyBEdW8qU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBKNyBNYXggKDIwMTcpKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgSjcgUHJpbWUqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBKNyBQcmltZSAyKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgSjcgUHJvICgyMDE3KSpTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IEo3IFYqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBKOCAoMjAxOCkqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBNZWdhIDYuMypTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IE5vdGUgMypTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IE5vdGUgOCpTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IE5vdGUgOSpTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IE5vdGUgRkUqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBPbjYqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBTMipTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IFM0KlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgUzQgTWluaSpTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IFM1IFtrbHRlXSpTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IFM2KlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgUzcqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBTNyBBY3RpdmUqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBTNyBFZGdlKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgUzgqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBTOCBBY3RpdmUqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBTOCBQbHVzKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgUzkqU2Ftc3VuZ3xTYW1zdW5nIEdhbGF4eSBTOSBQbHVzKlNhbXN1bmd8U2Ftc3VuZyBHYWxheHkgVGFiIEEgMTAuNSpTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IFRhYiBBIDkuNypTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IFRhYiBFIDkuNipTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IFRhYiBTMypTYW1zdW5nfFNhbXN1bmcgR2FsYXh5IFRhYiBTNCAxMC41KlNvbnl8U29ueSBYcGVyaWEgRTUqU29ueXxTb255IFhwZXJpYSBMMSpTb255fFNvbnkgWHBlcmlhIFIxIChQbHVzKSpTb255fFNvbnkgWHBlcmlhIFRvdWNoKlNvbnl8U29ueSBYcGVyaWEgWCpTb255fFNvbnkgWHBlcmlhIFggQ29tcGFjdCpTb255fFNvbnkgWHBlcmlhIFggUGVyZm9ybWFuY2UqU29ueXxTb255IFhwZXJpYSBYQSpTb255fFNvbnkgWHBlcmlhIFhBIFVsdHJhKlNvbnl8U29ueSBYcGVyaWEgWEExKlNvbnl8U29ueSBYcGVyaWEgWEExIFBsdXMqU29ueXxTb255IFhwZXJpYSBYQTEgVWx0cmEqU29ueXxTb255IFhwZXJpYSBYQTIqU29ueXxTb255IFhwZXJpYSBYQTIgUGx1cypTb255fFNvbnkgWHBlcmlhIFhBMiBVbHRyYSpTb255fFNvbnkgWHBlcmlhIFhaKlNvbnl8U29ueSBYcGVyaWEgWFogUHJlbWl1bSpTb255fFNvbnkgWHBlcmlhIFhaMSpTb255fFNvbnkgWHBlcmlhIFhaMSBDb21wYWN0KlNvbnl8U29ueSBYcGVyaWEgWFoyKlNvbnl8U29ueSBYcGVyaWEgWFoyIENvbXBhY3QqU29ueXxTb255IFhwZXJpYSBYWjIgUHJlbWl1bSpTb255fFNvbnkgWHBlcmlhIFhacypTb255fFNvbnkgWHBlcmlhIFoxKlNvbnl8U29ueSBYcGVyaWEgWjUqU29ueXxTb255IFhwZXJpYSBaNSBQcmVtaXVtKlNvbnl8U3ByaW50IEdhbGF4eSBUYWIgRSA4LjAqVml2b3x2aXZvIE5FWCpWaXZvfHZpdm8gTkVYIEEqVml2b3x2aXZvIE5FWCBTKlZpdm98dml2byBWMTEqVml2b3x2aXZvIFYxMSBQcm8qVml2b3x2aXZvIFYxMWkqVml2b3x2aXZvIFY3KlZpdm98dml2byBWNyBQbHVzKlZpdm98dml2byBWOSpWaXZvfHZpdm8gVjkgWW91dGgqVml2b3x2aXZvIFgyMCpWaXZvfHZpdm8gWDIwIFBsdXMqVml2b3x2aXZvIFgyMCBVRCpWaXZvfHZpdm8gWDIxKlZpdm98dml2byBYMjEgVUQqVml2b3x2aXZvIFgyMWkqVml2b3x2aXZvIFgyMypWaXZvfFZpdm8gWDkqVml2b3xWaXZvIFg5IFBsdXMqVml2b3xWaXZvIFg5cypWaXZvfFZpdm8gWDlzIFBsdXMqVml2b3x2aXZvIFk1M2kqVml2b3x2aXZvIFk3MSpWaXZvfHZpdm8gWTcxaSpWaXZvfHZpdm8gWTgxKlZpdm98dml2byBZODMqVml2b3x2aXZvIFk4MyBQcm8qVml2b3x2aXZvIFk5NypWaXZvfHZpdm8gWjEqVml2b3x2aXZvIFoxaSpWaXZvfFdpbGV5Zm94IFN3aWZ0KlhpYW9taXxYaWFvbWkgTWkgMypYaWFvbWl8WGlhb21pIE1pIDQqWGlhb21pfFhpYW9taSBNaSA1KlhpYW9taXxYaWFvbWkgTWkgNipYaWFvbWl8WGlhb21pIE1pIEExKlhpYW9taXxYaWFvbWkgTWkgQTIgTGl0ZSpYaWFvbWl8WGlhb21pIE1pIE1heCpYaWFvbWl8WGlhb21pIFBvY28gRjEqWGlhb21pfFhpYW9taSBSZWRtaSAyKlhpYW9taXxYaWFvbWkgUmVkbWkgNCBQcmltZSpYaWFvbWl8WGlhb21pIFJlZG1pIDRYKlhpYW9taXxYaWFvbWkgUmVkbWkgTm90ZSAzKlhpYW9taXxYaWFvbWkgUmVkbWkgTm90ZSA0KlhpYW9taXxYaWFvbWkgUmVkbWkgTm90ZSA1KlhpYW9taXxYaWFvbWkgUmVkbWkgTm90ZSA1IFBsdXMqWGlhb21pfFhpYW9taSBSZWRtaSBOb3RlIDUgUHJvKllVfFl1IEFjZSpZVXxZdSBZdW5pY29ybipZVXxZVSBZdW5pcXVlKllVfFl1IFl1bmlxdWUgMipZVXxZVSBZdXBob3JpYSpZVXxZVSBZdXJla2EqWVV8WVUgWXVyZWthIEJsYWNrKllVfFl1IFl1cmVrYSBOb3RlKllVfFl1IFl1cmVrYSBTKlpURXxaVEUgQXhvbiA3KlpURXxaVEUgQXhvbiA3IE1pbmkqWlRFfFpURSBBeG9uIDdzKlpURXxaVEUgQXhvbiA5IFBybypaVEV8WlRFIEF4b24gRWxpdGUqWlRFfFpURSBBeG9uIE1pbmkqWlRFfFpURSBBeG9uIFBybypaVEV8WlRFIEJsYWRlIEEzKlpURXxaVEUgQmxhZGUgQTYqWlRFfFpURSBCbGFkZSBWNypaVEV8WlRFIEJsYWRlIFY4KlpURXxaVEUgQmxhZGUgVjkqWlRFfFpURSBCbGFkZSBWOSBNaW5pKlpURXxaVEUgTWF2ZW4gMipaVEV8WlRFIE1heCBYTCpaVEV8WlRFIE51YmlhIFoxNypaVEV8WlRFIFRlbXBvIEdvKnhpYW9taXx4aWFvbWkgNipnb29nbGV8Z29vZ2xlIFBpeGVsIDIqeGlhb21pfHhpYW9taSA4Kmh1YXdlaXxodWF3ZWkgSG9ub3IgVjkqdml2b3x2aXZvIFg5IFBsdXMqb3Bwb3xvcHBvIHIxMSpvcHBvfG9wcG8gcjExIHBsdXMqb3Bwb3xvcHBvIFIxMSBQbHVzKm9wcG98b3BwbyBSMTcgUHJvKm1laXp1fG1laXp1IFBSTyA3IFBsdXMqbWVpenV8bWVpenUgUFJPIDYgUGx1cyp4aWFvbWl8eGlhb21pIG1peCp4aWFvbWl8bWkgMw==";
QString phonePrefix = "<<1205|+1251|+1659|+1256|+1334|+1907|+1520|+1928|+1480|+1602|+1623|+1501|+1479|+1870|+1341|+1442|+1628|+1657|+1669|+1747|+1752|+1764|+1951|+1209|+1559|+1408|+1831|+1510|+1213|+1310|+1424|+1323|+1562|+1707|+1369|+1627|+1530|+1714|+1949|+1626|+1909|+1916|+1760|+1619|+1858|+1935|+1818|+1415|+1925|+1661|+1805|+1650|+1211|+1720|+1970|+1303|+1719|+1203|+1475|+1860|+1959|+1302|+1411|+1202|+1911|+1239|+1386|+1689|+1754|+1941|+1954|+1561|+1407|+1727|+1352|+1904|+1850|+1786|+1863|+1305|+1321|+1813|+1470|+1478|+1770|+1678|+1404|+1706|+1912|+1229|+1710|+1671|+1808|+1208|+1312|+1773|+1630|+1847|+1708|+1815|+1224|+1331|+1464|+1872|+1217|+1618|+1309|+1260|+1317|+1219|+1765|+1812|+1563|+1641|+1515|+1319|+1712|+1876|+1620|+1785|+1913|+1316|+1270|+1859|+1606|+1502|+1225|+1337|+1985|+1504|+1318|+1207|+1227|+1240|+1443|+1667|+1410|+1301|+1339|+1351|+1774|+1781|+1857|+1978|+1508|+1617|+1413|+1231|+1269|+1989|+1734|+1517|+1313|+1810|+1248|+1278|+1586|+1679|+1947|+1906|+1616|+1320|+1612|+1763|+1952|+1218|+1507|+1651|+1228|+1601|+1557|+1573|+1636|+1660|+1975|+1314|+1816|+1417|+1664|+1406|+1402|+1308|+1775|+1702|+1506|+1603|+1551|+1848|+1862|+1732|+1908|+1201|+1973|+1609|+1856|+1505|+1575|+1585|+1845|+1917|+1516|+1212|+1646|+1315|+1518|+1347|+1718|+1607|+1914|+1631|+1716|+1252|+1336|+1828|+1910|+1980|+1984|+1919|+1704|+1701|+1283|+1380|+1567|+1216|+1614|+1937|+1330|+1234|+1440|+1419|+1740|+1513|+1580|+1918|+1405|+1541|+1971|+1445|+1610|+1835|+1878|+1484|+1717|+1570|+1412|+1215|+1267|+1814|+1724|+1787|+1939|+1401|+1306|+1803|+1843|+1864|+1605|+1731|+1865|+1931|+1423|+1615|+1901|+1325|+1361|+1430|+1432|+1469|+1682|+1737|+1979|+1214|+1972|+1254|+1940|+1713|+1281|+1832|+1956|+1817|+1806|+1903|+1210|+1830|+1409|+1936|+1512|+1915|+1340|+1385|+1435|+1801|+1802|+1276|+1434|+1540|+1571|+1757|+1703|+1804|+1509|+1206|+1425|+1253|+1360|+1564|+1304|+1262|+1920|+1414|+1715|+1608|+1307|+1867|+1866|+1456|+111|+1880|+1881|+1882|+1500|+1611|+1311|+1200|+1300|+1400|+1700|+1711|+1811|+1800|+1877|+1888|+8486|+8496|+8497|+8498|+8432|+8433|+8434|+8435|+8436|+8437|+8438|+8439|+8488|+8491|+8494|+8483|+8484|+8485|+8481|+8482|+8489|+8490|+8493|+8470|+8479|+8477|+8476|+8478|+8492|+8456|+8458|+8499|+8459";


LDPlayer::LDPlayer(int dncsId)
{
    this->ldhelper = new LDPlayerHelper();
    this->profile = ldhelper->getProfileByDnsId(dncsId);
    LOG<<profile.id<<profile.name<<profile.address;

    m_screens = QList<DefinedScreen*>();

    qRegisterMetaType<QProcess::ProcessState>("QProcess::ProcessState");


}

LDPlayer::~LDPlayer()
{
    if(ldhelper != nullptr){
        delete  ldhelper;
    }
}

//start
void LDPlayer::start()
{
//    LOG<<": start function";
    QString ldPlayerPath = ConfigHelper::GetLDPlayerLibsPath();

    QStringList agruments;
    agruments<<"launch"<<"--index"<<QString::number(profile.id);
    ldhelper->ldConsoleCommand(agruments,true);
//    LOG<<": process end";

    qDebug()<<"Number started : "<<started++;
}

//void LDPlayer::modifyResolution(DeviceInfo &deviceInfo)
//{
//    QStringList agrs;
//    agrs<<"modify"<<"--index"<<QString::number(profile.id);
//    agrs<<"--resolution"<<deviceInfo.resolution();
//    agrs<<"--cpu"<<QString::number(deviceInfo.cpuCount());
//    agrs<<"--memory"<<QString::number(deviceInfo.memory());
//    agrs<<generateRandomProp(deviceInfo);
//    ldhelper->ldConsoleCommand(agrs,true);
//    agrs.clear();
//}

void LDPlayer::disableGPS()
{
    ldhelper->adbShellCommand(profile.address,QString("shell settings put secure location_providers_allowed -gps").split(" "),true);
}

void LDPlayer::enableAccessibility(QString packageName, QString serviceName)
{
//    LOG;
    bool isEnabled = false;
    do{
        QString rs = command( "shell settings get secure enabled_accessibility_services" );
        if(rs.contains(packageName)){
            isEnabled = true;
        }else {
            Utility::waitForMiliseconds(500);
            command( "shell settings put secure enabled_accessibility_services "+packageName+"/"+serviceName);
        }
        Utility::waitForMiliseconds(500);
    }while (!isEnabled);
}

void LDPlayer::disableAccessibilityService()
{
//    LOG;
    bool isEnabled = false;
    do{
        QString rs = command( "shell settings get secure enabled_accessibility_services" );
        if(rs.contains("1")){
            isEnabled = true;
        }else {
            Utility::waitForMiliseconds(500);
            command( "shell settings put secure enabled_accessibility_services 1" );
        }
        Utility::waitForMiliseconds(500);
    }while (!isEnabled);
}

void LDPlayer::refreshScreenElement()
{
//    LOG;
    QString rs = LDCommand::getElements(profile.id); /*command({"shell","am","broadcast","-a","com.cgb.support.SCREEN_ELEMENT_ACTION","com.cgb.support"})*/;
//    LOG<<rs;
    QRegExp rg("(\\{.{0,3}elements.+:.+\\})");
//    printf("%s",rs.toUtf8().data());
//    Utility::write(Utility::getExecutedPath()+"/crlog.txt",rs);
    if(rg.indexIn(rs) != -1){
        QJsonDocument doc = QJsonDocument::fromJson(rg.cap(1).toUtf8());
//        qDebug()<<rg.cap(1);
        if(!doc.isNull()
             && !doc.isEmpty()){
            QString encodedElement = doc.object()["elements"].toString();

            QString decoded = QString::fromStdString(QByteArray::fromBase64(encodedElement.toLocal8Bit()).toStdString());
//            qDebug()<<"decode: "<<decoded;
            Utility::write(Utility::getExecutedPath()+"/screen.txt",decoded,true);
            QRegExp rg2("(\\[.+\\])");
            if(rg2.indexIn(decoded) != -1){
                QString screen = rg2.cap(1);
                QJsonDocument doc = QJsonDocument::fromJson(screen.toUtf8());
                if(!doc.isNull()
                        && !doc.isEmpty()){
                    QJsonObject rt;
                    QJsonArray screenArr;

                    ScreenInfo *info = ScreenInfo::fromJson(doc.array());
                    for(int i=0;i<m_screens.size();i++){
                        if(m_screens[i]->acceptScreen(*info)){
                            screenArr.append(m_screens[i]->screenId());
                        }
                    }
                    info->deleteLater();

                    if(screenArr.size() > 0){
                        rt.insert("screenId",screenArr);
                        rt.insert("elements",doc.array());
//                        Utility::writeTo(ConfigHelper::getScreenLogPath(),QString(QJsonDocument(rt).toJson(QJsonDocument::Compact))+"\n",true);
                        m_currentScreen = rt;
                        return;
                    }

                    screenArr.append(SCREEN_UNKNOWN);
                    rt.insert("screenId",screenArr);
                    rt.insert("elements",doc.array());
//                    LOG<<"rt"<<QJsonDocument(rt).toJson(QJsonDocument::Compact);
                    Utility::writeTo(ConfigHelper::getScreenLogPath(),QString(QJsonDocument(rt).toJson(QJsonDocument::Compact))+"\n",true);
                    m_currentScreen = rt;
                    return;
                }
            }
        }

    }

    m_currentScreen = QJsonObject();
}

QPoint LDPlayer::findPosition(QJsonArray screen, QString key, PropertyType type)
{
    for(int i=0;i<screen.size();i++){
        if(screen[i].toObject()[propertyToString(type)].toString() == key){
            int x = screen[i].toObject()["x"].toInt();
            int y = screen[i].toObject()["y"].toInt();
            if(x>=0
//                && x<=320
                && y>=0
                /*&& y<=480*/){
                return QPoint(x,y);
            }
        }
    }

    return INVALID_POINT;
}

bool LDPlayer::restoreLDPlayer(QString filePath)
{
    if(!QFileInfo(filePath).exists()){
        LOG<<"Backup file not found";
        return false;
    }
    return ldhelper->ldConsoleCommand({"restore","--index",QString::number(profile.id),"--file",filePath},true);
}

bool LDPlayer::fakeDevice()
{
    int retry = 0;
    while(retry++<=3){
        closeApp("com.facebook.katana");
    //    Utility::waitForMiliseconds(500);
        clearData("com.facebook.katana");
    //    Utility::waitForMiliseconds(500);
        command( "shell pm grant com.facebook.katana android.permission.WRITE_EXTERNAL_STORAGE" );
    //    Utility::waitForMiliseconds(500);
        command( "shell pm grant com.facebook.katana android.permission.READ_EXTERNAL_STORAGE");
    //    Utility::waitForMiliseconds(500);
        openPackage("com.facebook.katana");
        Utility::waitForMiliseconds(1000);
        inputKeyEvent("KEYCODE_HOME");
    //    Utility::waitForMiliseconds(1000);
        QString rs = command("shell cat /sdcard/data/device.json");
         if(rs.contains("buildProp")
                 && rs.contains("other")){
             break;
         }else{
             openPackage("com.cgb.myxposed");
             Utility::waitForMiliseconds(1000);
             inputKeyEvent("KEYCODE_HOME");
         }
    }

    if(retry==4){
        LOG<<"fake device false";
        return false;
    }

    return true;

}

QString LDPlayer::getXposedData()
{
    return command("shell cat /sdcard/data/device.json");
}

void LDPlayer::restoreDeviceInfo(QJsonObject xposedData)
{
    QString temp = QFileInfo(ConfigHelper::getPackagePath()).dir().path();
    Utility::write(temp+"/devices/LD"+QString::number(profile.id)+"/device.json",QJsonDocument(xposedData).toJson(QJsonDocument::Compact));
    LOG<<command( "push "+temp+"/devices/LD"+QString::number(profile.id)+"/device.json /sdcard/data");
}

QString LDPlayer::getManufacturner()
{
//    if(profile->getManufactor() != ""){
//        return profile->getManufactor();
//    }
//    QString rs = ldhelper->adbShellCommand2(profile.address,QStringList()<<"shell"<<"getprop"<<"ro.product.manufacturer");
//    LOG<<rs;
//    if(rs.size() > 3){
//        profile->setManufactor(rs.remove('\n').remove('\r').toUpper());
//    }
    return "";
}

QString LDPlayer::getModel()
{
//    if(profile->getModel() != ""){
//        return profile->getModel();
//    }
////    LOG<<profile.address;
//    QString rs = ldhelper->adbShellCommand2(profile.address,QStringList()<<"shell"<<"getprop"<<"ro.product.model");
////    LOG<<rs;
//    if(rs.size() > 3){
//        profile->setModel(rs.remove('\n').remove('\r'));
//    }
    return "";
}

bool LDPlayer::isRunning()
{
    QString runninglist = ldhelper->ldConsoleCommand2(QStringList()<<"runninglist");
    QStringList listName = runninglist.split("\r\n");
    foreach(QString str,listName){
        if(str == profile.id){
            return true;
        }
    }
    return false;
}

bool LDPlayer::isAdbDeviceAvailable()
{
    if(!profile.address.contains("unknown")){
        LOG<<profile.address;
        return true;
    }
    profile.address = ldhelper->getAdbAdress(profile.id);
//    LOG<<profile.address;
    return false;
}

bool LDPlayer::refreshAdbAddress()
{
    profile.address = ldhelper->getAdbAdress(profile.id);
//    LOG<<profile.address;
    ldhelper->refreshServer();
    int retry = 20;
    while(retry-->0 && !isAdbDeviceAvailable()){
        Utility::waitForMiliseconds(1000);
    }

    return retry>0;
}

bool LDPlayer::isWindowAvailable()
{
    if(profile.bindHWND > 0){
        return true;
    }
    QList<LDPlayerProfile> list = ldhelper->getListProfile();

    LDPlayerProfile load;
    for(int i=0;i<list.size();i++){
        if(list[i].id == profile.id){
            load = list[i];
            break;
        }
    }

    if(load.id != -1
            && load.topHWND != 0){
        profile = load;
        return true;
    }
    return false;
}

void LDPlayer::setupProxy()
{
    int port = Utility::random()%7+4001;
    QString ip = "222.252.23.179";
//    int port = 0;
//    QString ip = "";
    LDCommand::ld_adb_command(profile.id,"shell settings put global http_proxy "+ip+":"+QString::number(port));
}

void LDPlayer::clearProxy()
{
    LDCommand::ld_adb_command(profile.id,"shell settings  put global http_proxy :0");
}

QString LDPlayer::getCurrentProxy()
{
//    Address addr;
    QString proxy = LDCommand::ld_adb_command(profile.id,"shell settings get global http_proxy");
    if(proxy != "null"
            && proxy.contains(":")
            && proxy.contains(".")){
        return proxy;
    }

    return "";
}


void LDPlayer::openDeepLink(QString deepLink)
{
    ldhelper->adbShellCommand(profile.address,{"shell","am","start","-W","-a","android.intent.action.VIEW","-d",
                              QString("\"%1\"").arg(deepLink)},true);
}

//login
bool LDPlayer::openPackage(QString packageName, bool isCleanUp)
{
    QStringList agruments;
    if(isCleanUp){
        //remove cached
        clearData(packageName);
        Utility::waitForMiliseconds(1000);
    }
    //login
    agruments.clear();
    agruments<<"runapp"<<"--index"<<QString::number(profile.id)<<"--packagename"<<packageName;
    return ldhelper->ldConsoleCommand(agruments,true);
}

bool LDPlayer::isInstallApp(QString packageName)
{
    QStringList agruments;
    agruments<<"shell"<<"pm"<<"list"<<"packages";
    QString rs = ldhelper->adbShellCommand2(profile.address,agruments);
    if(rs.contains(packageName)){
        return true;
    }else if(rs.contains("daemon not running")){
        ldhelper->refreshServer();
    }
    return false;
}

bool LDPlayer::installApk(QString apkPath)
{
    QStringList agruments;
    agruments<<"install"<<apkPath;
    LOG<<agruments;
    LOG<<profile.address;
    QString rs = ldhelper->adbShellCommand2(profile.address,agruments);
    if(rs.contains("Success")){
        return true;
    }
    return false;
}

bool LDPlayer::uninstallPackage(QString packageName)
{
    return false;
}

bool LDPlayer::isAppRunning(QString packageName)
{
    QStringList agruments;
    agruments<<"shell"<<"pidof"<<packageName;
    QString out = LDCommand::ld_adb_command(profile.id,"shell pidof "+packageName);
    LOG<<": com.facebook.katana pid = "<<out;
    if(out.length() == 0){
        return false;
    }
    bool ok = false;
    out.toInt(&ok);
    return ok;
}

void LDPlayer::closeApp(QString packageName)
{
    LDCommand::killApp(profile.id,packageName);
}

void LDPlayer::clearData(QString packageName)
{
    //remove cached
    LDCommand::ld_adb_command(profile.id,"shell pm clear "+packageName);
}

bool LDPlayer::isContains(QString key, PropertyType type)
{
    if(m_currentScreen.contains("elements")){
        return findPosition(m_currentScreen["elements"].toArray(),key,type) != INVALID_POINT;
    }

    return false;
}

QPoint LDPlayer::getPositionOf(QString key, PropertyType type)
{
    if(m_currentScreen.contains("elements")){
        return findPosition(m_currentScreen["elements"].toArray(),key,type);
    }


    return INVALID_POINT;
}

bool LDPlayer::clickOn(QString key, PropertyType type)
{
    QPoint pos = getPositionOf(key,type);
    if(pos != INVALID_POINT){
        tapOn(pos);
        return true;
    }

    return false;
}



QJsonArray LDPlayer::getScreenId()
{
    if(m_screens.size() == 0){
       loadScreen();
       LOG<<"Screen defined count:"<<m_screens.size();
    }else{
        if(QDateTime::currentDateTime().toSecsSinceEpoch()%/*300*/5==0){
//            loadScreen();
        }
    }

    refreshScreenElement();
    if(m_currentScreen.contains("screenId")){
        LOG<<m_currentScreen["screenId"];
        return m_currentScreen["screenId"].toArray();
    }

    return QJsonArray();
}

bool LDPlayer::isNeedToClean()
{
    double dataSize = ldhelper->dataSizeOf(profile.id);
    if(dataSize >= 3.5){
        return true;
    }
    return false;
}

double LDPlayer::getDataFolderSize()
{
    return ldhelper->dataSizeOf(profile.id);
}

QString LDPlayer::propertyToString(PropertyType type)
{
    switch (type) {
    case CLASS_NAME:
        return "className";
    case TEXT:
        return "text";
    case CONTENT_DESCRIPTION:
        return "contentDescription";
    }

    return "";
}

void LDPlayer::setupKeyboard(bool isUnicode)
{
    LOG;
    if(isUnicode){
        ldhelper->adbShellCommand(profile.address,QStringList()<<"shell"<<"ime"<<"set"<<"com.android.adbkeyboard/.AdbIME",true);
    }else{
        ldhelper->adbShellCommand(profile.address,QStringList()<<"shell"<<"ime"<<"set"<<"com.android.inputmethod.pinyin/.InputService",true);
    }

}

void LDPlayer::tapOn(double x, double y)
{
    LDCommand::ld_adb_command(profile.id,QString("shell input tap %1 %2").arg(x).arg(y));
}

void LDPlayer::tapOn(QPoint point)
{
    tapOn(point.x(),point.y());
}

void LDPlayer::doubleTapOn(double x, double y)
{
    tapOn(x,y);
    tapOn(x,y);
}

void LDPlayer::inputText(QString content, bool isUnicode, bool isSlow)
{
    LOG<<": "<<content;
    if(isUnicode){
        LDCommand::ld_adb_command(profile.id,"shell ime set com.android.adbkeyboard/.AdbIME");
    }else{
        LDCommand::ld_adb_command(profile.id,"shell ime set com.android.inputmethod.pinyin/.InputService");
    }

    if(isSlow){
        for(int i=0;i<content.size();i++){
    //        Utility::waitForMiliseconds(50);
            if(isUnicode){
                LDCommand::ld_adb_command(profile.id,"shell am broadcast -a ADB_INPUT_B64 --es msg '"+content.mid(i,1).toUtf8().toBase64()+"'");
            }else{
                LDCommand::ld_adb_command(profile.id,"shell input text '"+content.mid(i,1)+"'");
            }
        }
    }else{
        if(isUnicode){
            LDCommand::ld_adb_command(profile.id,"shell am broadcast -a ADB_INPUT_B64 --es msg '"+content.toUtf8().toBase64()+"'");
        }else{
            LDCommand::ld_adb_command(profile.id,"shell input text \'"+content+"\'");
        }
    }
}

void LDPlayer::inputKeyEvent(QString key)
{
    ldhelper->adbShellCommand(profile.address,{"shell","input","keyevent",key.toUpper()},true);
}

void LDPlayer::clearText(bool isClearAll)
{
    LOG;
    QStringList arguments;
    if(isClearAll){
        arguments<<"shell"<<"am"<<"broadcast"<<"-a"<<"ADB_CLEAR_TEXT";
    }else {
        arguments<<"shell"<<"input"<<"keyevent"<<"KEYCODE_DEL";
    }
    ldhelper->adbShellCommand(profile.address,arguments,true);
}

void LDPlayer::swipeUp(bool isLong)
{
    ldhelper->adbShellCommand(profile.address,QStringList()<<"shell"<<"input"<<"swipe"
                              <<QString::number(160)<<QString::number(isLong?450:370)//from
                              <<QString::number(160)<<QString::number(290)//to
                              <<QString::number(100),true);//duration
}

void LDPlayer::swipDown(bool isLong)
{
    ldhelper->adbShellCommand(profile.address,QStringList()<<"shell"<<"input"<<"swipe"
                              <<QString::number(160)<<QString::number(290)//from
                              <<QString::number(160)<<QString::number(isLong?450:370)//to
                              <<QString::number(100),true);//duration
}

QString LDPlayer::backupFacebook(QString uuid,QString destinationPath)
{
    LOG<<uuid;
    int retry=0;
    QString fileName = "uuid_"+uuid+".tar";
    while(!ldhelper->adbShellCommand2(profile.address,{"shell","ls","/data/data"}).contains(fileName)
          && retry++<=3){
//        Utility::waitForMiliseconds(500);
        ldhelper->adbShellCommand2(profile.address,{"shell","cd","/data/data;","tar","-cf","uuid_"+uuid+".tar","com.facebook.katana/app_light_prefs"},120000);
//        Utility::waitForMiliseconds(500);
    }
    if(retry==4){
        return "";
    }

    retry=0;
    while(!QFileInfo(destinationPath+"/"+fileName).exists()
          && retry++<=3){
//        Utility::waitForMiliseconds(500);
        ldhelper->adbShellCommand2(profile.address,{"pull","/data/data/uuid_"+uuid+".tar",destinationPath},120000);
//        Utility::waitForMiliseconds(500);
    }
    if(retry==4){
        return "";
    }

    ldhelper->adbShellCommand(profile.address,{"shell","rm","-rf","/data/data/"+fileName},true);

    return destinationPath+"/"+fileName;
}

bool LDPlayer::restoreFacebook(QString sourcePath)
{
    LOG<<sourcePath;
    closeApp("com.facebook.katana");
//    Utility::waitForMiliseconds(1000);
    LDCommand::ld_adb_command(profile.id,"shell pm clear com.facebook.katana");
//    Utility::waitForMiliseconds(1000);

    QFileInfo info(sourcePath);
    QString fileName = info.fileName();
    int retry=0;
    while(!LDCommand::ld_adb_command(profile.id,"shell ls /data/data").contains(fileName)
          && retry++<=3){
//        Utility::waitForMiliseconds(500);
        LDCommand::ld_adb_command(profile.id,"push "+sourcePath+" /data/data",120000);
//        Utility::waitForMiliseconds(500);
    }
    if(retry==4){
        return false;
    }
    LDCommand::ld_adb_command(profile.id,"shell cd /data/data; tar -xf "+fileName,120000);
//    Utility::waitForMiliseconds(500);
    LDCommand::ld_adb_command(profile.id,"shell rm -rf /data/data/"+fileName);
//    Utility::waitForMiliseconds(500);
    command("shell pm grant com.facebook.katana android.permission.WRITE_EXTERNAL_STORAGE");
//    Utility::waitForMiliseconds(500);
    command("shell pm grant com.facebook.katana android.permission.READ_EXTERNAL_STORAGE");
//    Utility::waitForMiliseconds(500);
    return true;
}

void LDPlayer::stop()
{
    LOG;
//    ldhelper->ldConsoleCommand(QStringList()<<"quit"<<"--index"<<QString::number(profile.id),true);

//    profile->setAddress("");
//    profile->setBindHWND(0);
//    profile->setTopHWND(0);
//    profile->setStarted(false);
//    profile->setPID(0);
//    profile->setVboxPID(0);

}

LDPlayerProfile LDPlayer::getProfile()
{
    return this->profile;
}

QString LDPlayer::command(QString agruments)
{
    return LDCommand::ld_adb_command(profile.id,agruments,180000);
}

LDPlayerHelper* LDPlayer::getHelper()
{
    return ldhelper;
}

//slots



//private
void LDPlayer::loadScreen()
{
    LOG;
    QString screenDefinePath = Utility::getExecutedPath()+"/screendefine.json";
    if(!QFileInfo(screenDefinePath).exists()){
        return;
    }
    QString content = Utility::read(screenDefinePath,true);
//    LOG<<content;
    QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());
    if(!doc.isNull()
            && !doc.isEmpty()){
        for(int i=0;i<m_screens.size();i++){
            if(m_screens[i] != nullptr){
                delete m_screens[i];
            }
        }
        m_screens.clear();
        for(int i=0;i<doc.array().size();i++){
            m_screens.append(DefinedScreen::fromJson(doc.array()[i].toObject()));
        }
    }
}

//QStringList LDPlayer::generateRandomProp(DeviceInfo &deviceInfo)
//{
//    LOG;
//    QStringList modelList = QString(QByteArray::fromBase64(modelBase.toUtf8())).split("*");
//    if(modelList.size() <= 0){
//        return QStringList();
//    }

//    QString model = modelList[Utility::random()%modelList.size()];
//    QString manufacturer = model.split("|")[0];
//    QString modelName = model.split("|")[1];
//    QString imei = "86516602" + Utility::getRandomNumberArray(7);
//    QString imsi = "46000" + Utility::getRandomNumberArray(10);
//    QString simserial = "898600" + Utility::getRandomNumberArray(14);
//    QString rdString = Utility::getRandomString(32);
//    QByteArray arr = QCryptographicHash::hash(rdString.toUtf8(),QCryptographicHash::Md5);
//    QString androidId = arr.toHex().mid(Utility::random()%17,16);
//    QStringList phonePrefixArr = phonePrefix.split("|");
//    QString phoneNumber = phonePrefixArr[Utility::random()%phonePrefixArr.size()] + Utility::getRandomNumberArray(7);
//    QString mac = "00DB" + QCryptographicHash::hash(Utility::getRandomString(32).toUtf8(),QCryptographicHash::Md5).toHex().mid(Utility::random()%9,8);
//    if(deviceInfo.modelName() != ""){
//        modelName = deviceInfo.modelName();
//        manufacturer = deviceInfo.manufacturer();
//        imei = deviceInfo.imei();
//        imsi = deviceInfo.imsi();
//        simserial = deviceInfo.simserial();
//        androidId = deviceInfo.androidId();
//        phoneNumber = deviceInfo.phoneNumber();
//        mac = deviceInfo.mac();
//    }else{
//        deviceInfo.setModelName(modelName);
//        deviceInfo.setManufacturer(manufacturer);
//        deviceInfo.setImei(imei);
//        deviceInfo.setImsi(imsi);
//        deviceInfo.setSimserial(simserial);
//        deviceInfo.setAndroidId(androidId);
//        deviceInfo.setPhoneNumber(phoneNumber);
//        deviceInfo.setMac(mac);
//    }

//    QStringList arguments;
//    arguments<<"--imei" << imei << "--model" << modelName << "--manufacturer" << manufacturer
//            << "--pnumber" << phoneNumber << "--imsi" << imsi << "--simserial" << simserial << "--androidid" << androidId << "--mac" << mac.toUpper();

//    return arguments;
//}


