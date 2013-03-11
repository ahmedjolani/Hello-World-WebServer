/* Headers to be sent */

#define STATUS "HTTP/1.1 200 OK\r\n"
#define CONTENT_TYPE "Content-Type: text/html; charset=UTF-8\r\n"
#define DATE "Date: Wed, 30 May 2012 21:19:23 GMT\r\n"
#define SERVER "Server: Jolani/1.0\r\n"
#define HEADERS STATUS CONTENT_TYPE DATE SERVER "\r\n"

/* Content */

#define DOCTYPE "<!doctype html>"
#define HTML "<html>"
#define HEAD "<head>"
#define BODY "<body>"
#define EHTML "</html>"
#define EHEAD "</head>"
#define EBODY "</body>"
#define SCRIPT "<script type='text/javascript'>"
#define ESCRIPT "</script>"
#define P "<p>"
#define EP "</p>"
#define DIV "<div>"
#define EDIV "</div>"
#define CONTENT DOCTYPE HTML HEAD EHEAD BODY
#define ECONTENT EBODY EHTML
