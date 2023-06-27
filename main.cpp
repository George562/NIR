#include "tools.h"

int main() {
    init();

    while (window.isOpen()) {
        moveWindow();
        DrawAll();
        EventHandling();
    }

    return 0;
}