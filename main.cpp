#include "tools.h"

int main() {
    init();

    while (window.isOpen()) {
        Update();
        DrawAll();
        EventHandling();
    }

    return 0;
}