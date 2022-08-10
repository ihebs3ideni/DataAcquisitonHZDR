#include "Examples/Tomography_Controlled_Continuous_Steel_Casting.h"
int main() {
    //TODO FINISH README AND RUN.BAT
    auto t = std::thread(tomography_controlled_continuous_steel_casting);
    t.join();//this waits for UI thread to be closed
    return 0;
}


