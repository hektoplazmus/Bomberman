#ifndef SRC_CERRORHANDLER_HPP
#define SRC_CERRORHANDLER_HPP
#include <unistd.h>

//using  namespace std;

/**
 * Error detecting class which allows print error message on the screen.
 */
class ErrorMessage {

public:
    /**
     * Creates ErrorMessage with text msg
     * @param [in] msg text to be displayed
     */
    ErrorMessage(const std::string & msg):text(msg) {}
    /**
     * Draw error window on the screen and prints error message
     */
    void Print() const {
        WINDOW * error_win = newwin(5, 50, 5, 5);
        box(error_win, 0, 0);
        mvwprintw(error_win,0,0,"An error occured...");
        mvwprintw(error_win,2,2,text.c_str());
        wrefresh(error_win);
        sleep(2);
        delwin(error_win);
    }
private:
    /**text to be displayed*/
    std::string text;
};

#endif //SRC_CERRORHANDLER_HPP
