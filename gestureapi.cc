/**
 * @file gestureapi.cc
 * @brief This file contains method definitions for the GestureInput class
 * defined in the file gestureapi.hpp
 * @author IMU API team
 * 
 * This file is to be compiled with the rest of the application source code
 * to properly integrate the gesture recognition API provided by the class
 * GestureInput.
 */

#define __GESTURE_ERROR_CODES__
#include "gestureapi.hpp"

namespace gestureapi{
    /**
     * @brief This initializes the class to receive gesture information
     * 
     * This initializes the communication link between the application and the
     * gesture recognition service currently running in the system. This
     * operation will fail if another application is currently using the service
     * as applications get an exclusive access to it. All other functions will
     * fail if this has not returned true.
     * 
     * @return true if the link is established and false if otherwise.
     */
    bool GestureInput::begin(){
        char buffer[100];
        DWORD longValue = PIPE_READMODE_MESSAGE;
        inputPipe = CreateFile(TEXT("\\\\.\\pipe\\wxImuApi"),
            GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if(inputPipe == INVALID_HANDLE_VALUE){
            errorCode = ERROR_PIPE_OPEN_FAILED;
            return false;
        }

        if(!SetNamedPipeHandleState(inputPipe, &longValue, NULL, NULL)){
            CloseHandle(inputPipe);
            errorCode = ERROR_PIPE_MESSAGEMODE_FAILED;
            return false;
        }

        if(!WriteFile(inputPipe, (void*) "imuapi1.0", 10, &longValue, NULL)){
            CloseHandle(inputPipe);
            errorCode = ERROR_PIPE_TRANSACTION_FAILED;
            return false;
        }

        if(!ReadFile(inputPipe, buffer, sizeof(buffer), &longValue, NULL)){
            CloseHandle(inputPipe);
            errorCode = ERROR_PIPE_TRANSACTION_FAILED;
            return false;
        }

        if(strcmp(buffer, "imuapi1.0")){
            !WriteFile(inputPipe, (void*) "apiexit", 8, &longValue, NULL);
            CloseHandle(inputPipe);
            errorCode = ERROR_API_RESPONSE_MISMATCH;
        }
        isActive = true;
        return true;
    }

    /**
     * @brief Terminates the link to the gesture recognition server to allow
     * other applications to communicate with it.
     * 
     * This closes the connection safely to remove the exclusive lock the
     * application has over the server and allow any other application to
     * link with it. It is good practice to call this function as it is not
     * always guaranteed that this will close automatically, thus leaving the
     * system unable to service other gesture API calls.
     */
    void GestureInput::end(){
        DWORD longValue;
        if(isActive){
            WriteFile(inputPipe, (void*) "apiexit", 8, &longValue, NULL);
            CloseHandle(inputPipe);
            isActive = false;
        }
    }

    /**
     * @brief This function queries for the any gesture input received.
     * 
     * This function requires that the method begin() had been successfully
     * executed before receiving the current gesture recognized by the service.
     * This function returns a positive number corresponding to the gesture
     * and clears the input buffer of the service. Gestures detected are
     * available on the buffer for a maximum of 500ms before it is superseded
     * by a new gesture detected.
     * 
     * @return -1 for an internal error or a number corresponding
     * to an input value. (0 for no gesture detected)
     */
    int GestureInput::input(){
        DWORD longValue;
        char buffer[100];
        if(!isActive){
            errorCode = ERROR_CLIENT_NOT_INITIALIZED;
            return -1;
        }
        if(!WriteFile(inputPipe, (void*) "apidetect", 10, &longValue, NULL)){
            CloseHandle(inputPipe);
            errorCode = ERROR_PIPE_TRANSACTION_FAILED;
            isActive = false;
            return -1;
        }
        if(!ReadFile(inputPipe, buffer, sizeof(buffer), &longValue, NULL)){
            CloseHandle(inputPipe);
            errorCode = ERROR_PIPE_TRANSACTION_FAILED;
            isActive = false;
            return -1;
        }
        if(longValue != 1){
            CloseHandle(inputPipe);
            errorCode = ERROR_API_RESPONSE_MISMATCH;
            isActive = false;
            return -1;
        }

        return buffer[0];
    }

    GestureInput::~GestureInput(){
        DWORD longValue;
        if(isActive){
            WriteFile(inputPipe, (void*) "apiexit", 8, &longValue, NULL);
            CloseHandle(inputPipe);
            isActive = false;
        }
    }

    bool GestureInput::noGesture(int in){
        return in == 0;
    }

    bool GestureInput::isLeft(int in){
        return in & 0x8;
    }

    bool GestureInput::isRight(int in){
        return in & 0x4;
    }

    bool GestureInput::isUp(int in){
        return in & 0x20;
    }

    bool GestureInput::isDown(int in){
        return in & 0x10;
    }

    bool GestureInput::isForward(int in){
        return in & 0x2;
    }

    bool GestureInput::isBackward(int in){
        return in & 0x1;
    }

    bool GestureInput::isClockwise(int in){
        return in & 0x40;
    }

    bool GestureInput::isCounterClockwise(int in){
        return in & 0x80;
    }
}