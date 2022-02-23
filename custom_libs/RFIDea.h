#ifndef __RFIDEA__
#define __RFIDEA__

#include "ReaderCount.h"

class State {
    public:
        virtual void toggleEditorMode();
};

class EditorMode : public State {
    public:
        EditorMode(RFIDea &_rfidea);

        void toggleEditorMode();

    private:
        RFIDea rfidea;
};

class Default : public State {
    public:
        Default(RFIDea &_rfidea);

        void toggleEditorMode();

    private:
        RFIDea rfidea;
};

class RFIDea {
    public:
        RFIDea();

        void setState(State newState);

        void toggleEditorMode();

        void initialise();

    private:
        State currentState;
};

#endif