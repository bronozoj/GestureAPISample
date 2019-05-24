/**
 * @file gestureapi.hpp
 * @brief This file is to be included for all source files that will use the
 * gesture recognition API.
 * @author IMU API team
 * 
 * This header details the contents of the gesture recognition class that
 * will be used for communicating with the gesture recognition service
 * already running.
 * 
 * @date May 13, 2019
 */

#ifndef __UCL7_GESTURE_WRAPPER__
#define __UCL7_GESTURE_WRAPPER__

#ifdef __GESTURE_ERROR_CODES__
#define ERROR_PIPE_OPEN_FAILED 1
#define ERROR_PIPE_MESSAGEMODE_FAILED 2
#define ERROR_PIPE_TRANSACTION_FAILED 3
#define ERROR_API_RESPONSE_MISMATCH 4
#define ERROR_CLIENT_NOT_INITIALIZED 5
#endif

#include <windows.h>

namespace gestureapi{
    class GestureInput{
#ifdef _WIN32
        HANDLE inputPipe;
#endif
        //std::thread gestureService;
        char buffer[150];
        bool isActive = false;
        WORD errorCode = 0;
        
        public:
        bool noGesture(int in);
        bool isLeft(int in);
        bool isRight(int in);
        bool isUp(int in);
        bool isDown(int in);
        bool isForward(int in);
        bool isBackward(int in);
        bool isClockwise(int in);
        bool isCounterClockwise(int in);
        bool begin();
        void end();
        int input();
        ~GestureInput();
    };

    /// @brief values corresponding to gesture from GestureInput.input() call
    enum gesture{
        /// no gesture recognized
        none = 0,
        /// clockwise twisting motion gesture recognized
        clockwise = 1,
        /// counterclockwise twisting motion gesture recognized
        counterclockwise = 2,
        /// upward swiping motion gesture recognized
        up = 3,
        /// downward swiping motion gesture recognized
        down = 4,
        /// left swiping motion gesture recognized
        left = 5,
        /// right swiping motion gesture recognized
        right = 6,
        /// pushing motion gesture recognized
        forward = 7,
        /// pulling motion gesture recognized
        backward = 8,
        /// combination of an upward and leftward diagonal motion recognized
        up_left = 9,
        /// combination of an upward and rightward diagonal motion recognized
        up_right = 10,
        /// combination of an downward and leftward diagonal motion recognized
        down_left = 11,
        /// combination of an downward and rightward diagonal motion recognized
        downright = 12
    };
}

#endif