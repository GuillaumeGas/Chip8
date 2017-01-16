#include <iostream>

class SDLException {
public:
    SDLException (const char * str) : msg (str) {}
    virtual std::string toString () const {
	return msg;
    }
    std::string msg;
};

class SDLInitException : public SDLException {
public:
    SDLInitException (const char * str) : SDLException (str) {}
    std::string toString () const {
	return "SDL Init : " + msg;
    }
};

class ScreenInitException : public SDLException {
public:
    ScreenInitException (const char * str) : SDLException (str) {}
    std::string toString () const {
	return "SDL SetVideoMode : " + msg;
    }
};

class PixelInitException : SDLException {
public:
    PixelInitException (const char * str) : SDLException (str) {}
    std::string toString () const {
	return "SDL CreateRGBSurface : " + msg;
    }
};
