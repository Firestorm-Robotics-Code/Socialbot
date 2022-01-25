// Web views

#include "urlMap.hpp"
#define ADMIN_CODE "july_18_2007"

#define AUTH_DRIVETRAIN_ARCADE     1 // Arcade control
#define AUTH_DRIVETRAIN_MECANUM    2 // Mecanum control
#define AUTH_DRIVETRAIN_SPEEDLIMIT 4 // Speed limiter control

#define AUTH_DRIVE                 7 // Can entirely control drive

#define AUTH_MACROS                8 // Macros for control, like rotate and move or go to and grab etc
#define AUTH_MODE                 16 // Ability to change between teleop, autonomous, test, and disabled.
#define AUTH_LIFT                 32 // Ability to control lift, whether pneumatics or electic cylinder.

#define AUTH_USER_MANAGEMENT      64 // Ability to allocate and manage user ids.

#define AUTH_ADMIN                127 // Can do everything.


struct ClientState{
    std::string name;
    uint16_t perms;
};

namespace site{
    void index(HTTPARGS){
        ron -> content = "<!DOCTYPE html>\r\n"
                         "<!-- Hello, intrepid inspect-elementer. You really shouldn't poke around here, it's veeery scary. -->"
                         "<html>"
                         "<head>"
                         "<title>Socialbot Web Console (if this is Becky, tell Tyler to rename this and explain that he is an idiot)</title>"
                         "<link rel=\"stylesheet\" href=\"/main.css\">"
                         "</head>"
                         "<body>"
                         "<p>Please enter your access code.<br>If you need an access code, ask Tyler; he has the admin code and can help you.<br>Warning: some browsers will disable their connections after a minute or so of inactivity. This means you will lose all permissions and will have to return to this page and re-enter your access code.</p><input id = \"name\" type = \"text\"></input><button onclick = \"sendCode()\" id=\"send\">Done!</button>"
                         "<script type=\"text/javascript\">"
                         "function sendCode(){"
                         "window.location.replace(\"/user/\" + document.getElementById('name').value)"
                         "}"
                         "</script>"
                         "</body>"
                         "</html>";
        ron -> contentType = "text/html";
        ron -> status = 200;
    }
    void main_css(HTTPARGS){
        ron -> contentType = "text/css";
        ron -> content = "body {background-image: repeating-linear-gradient(45deg, orange 0px, orange 40px, red 40px, red 80px);}";
        ron -> status = 200;
    }
    void user(HTTPARGS){
        ron -> status = 200;
        std::string code = req -> url.substr(6);
        if (code == ADMIN_CODE){
            ron -> contentType = "text/html";
            ron -> content = "<!DOCTYPE html><html><head></head><body>Client state has been updated; you are now an administrator. <a href = '/admin'>Administration console</a></body></html>";
            ((ClientState*)client -> state) -> perms = AUTH_ADMIN;
        }
    }
    void admin(HTTPARGS){
        if (((ClientState*)client -> state) -> perms == AUTH_ADMIN){
            ron -> contentType = "text/html";
            ron -> status = 200;
            ron -> content = "<!DOCTYPE html><html>Who cares.</html>";
        }
        else{
            ron -> status = 401;
            ron -> contentType = "text/plain";
            ron -> content = "Error 401: You cannot access this endpoint because Tyler has outwitted you yet again ;)";
        }
    }
    void error_404(HTTPARGS){
        ron -> contentType = "text/html";
        ron -> status = 404;
        ron -> content = "<!DOCTYPE html>\r\n"
                         "<!-- Hello, intrepid inspect-elementer. You really shouldn't poke around here, it's veeery scary. -->"
                         "<html>"
                         "<head>"
                         "<title>Socialbot Web Console (if this is Becky, tell Tyler to rename this and explain that he is an idiot)</title>"
                         "<link rel=\"stylesheet\" href=\"/main.css\">"
                         "</head>"
                         "<body>"
                         "<p>404: that doesn't exist! Try the back button, it probably can help.</p>"
                         "</body>"
                         "</html>";
    }
    UrlMap urlMap;
    void beginSite(){
        urlMap.push(new mapping {"/", false, true, 0, index});
        urlMap.push(new mapping {"/main.css", false, false, 0, main_css});
        urlMap.push(new mapping {"/user/", true, false, 0, user});
        urlMap.push(new mapping {"/admin", true, true, 0, admin});
    }
    void respond(HTTPARGS){
        ron -> keepAlive = true;
        if ((ClientState*)client -> state == nullptr){
            client -> state = new ClientState{"", 0};
        }
        urlMap(HTTPPASSARGS);
    }
    void disconnect(Client *client){

    }
}
