#include "request.h"

//start of the process 
void Request::process() {
    outFnc("Starting process request " + std::to_string(value) + "...");
}

//end of the process message
void Request::finish() {
    outFnc("Finish request " + std::to_string(value));
}
